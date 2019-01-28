/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Thread.h ( UT Library, C++)
 *
 * COMMENTS:	Generic thread class.
 *	The owner of the thread can do things like:
 *
 *	killThread()	- Stop execution of thread
 *	waitThread()	- Wait until thread finishes execution
 *	suspendThread()	- Suspend execution of thread
 *	restartThread()	- Restart a stopped thread
 *	niceThread()	- Prioritize thread (0 low priority, 100 high)
 *
 *  TODO:  It might be nice to have a way to get the exit status of a thread.
 */

#ifndef __UT_Thread__
#define __UT_Thread__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Defines.h"

#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_SequentialThreadIndex.h>
#include <SYS/SYS_Types.h>

#include <stdlib.h>

#if defined(WIN32)
#   include <intrin.h>
    typedef int 	ut_thread_id_t;
#elif defined(USE_PTHREADS)
#   include <sched.h>
#   include <pthread.h>
    typedef pthread_t	ut_thread_id_t;
#else
    #error Unsupported Platform for UT_Thread
#endif

#define UT_INVALID_THREAD_ID ((ut_thread_id_t)0)

// some stack size defines
#define UT_THREAD_DEFAULT_STACK_SIZE	(8U*1024U*1024U)
#define UT_THREAD_SMALL_STACK_SIZE	(1U*1024U*1024U)

typedef void *(*UTthreadFunc)(void*);

// forward declarations
class UT_TaskScope;

class UT_API UT_Thread
{
public:
    // The destructor will wait until the thread is idle before it completes
    // If you wish to kill the thread, call killThread() first.
    virtual ~UT_Thread();

    // This enum specifies the current state for a persistent thread.  The
    // thread will typically be running or idle.  If the thread is idle, it's
    // behaviour will be determined by the SpinState.
    enum State
    {
	ThreadIdle,
	ThreadRunning
    };

    // // The thread status determines how the thread will behave once the
    // callback function is completed:
    //	  ThreadSingleRun	- The thread cannot be restarted
    //	  ThreadLowUsage	- The thread will yeild cycles while idle
    //
    enum SpinMode
    {
	ThreadSingleRun,
	ThreadLowUsage,
    };

    static UT_Thread	*allocThread(SpinMode spin_mode);

    static int		 getNumProcessors();

    /// This is only valid in debug builds
    static int		 activeThreadCount();

    /// Reset the number of threads that is used by Houdini. This will reread
    /// the HOUDINI_MAXTHREADS setting.
    /// @note There should be no active tasks when this is called.
    /// @note Only call this from the MAIN THREAD!
    static void		 resetNumProcessors();

    // getMyThreadId() is inlined for speed if we're using pthreads.
#if defined(USE_PTHREADS)
    static ut_thread_id_t getMyThreadId() { return pthread_self(); }
#else
    static ut_thread_id_t getMyThreadId();
#endif

    static ut_thread_id_t getMainThreadId();
    static int		  getMainSequentialThreadId();
    static inline int	  isMainThread()
			  {
			     return getMyThreadId() == getMainThreadId();
			  }

    /// Returns true if the current thread is a UT_Thread.
    /// Returns false if the current thread is either the main thread
    /// or a TBB thread.
    static bool           isUTThreadCurrent();

    /// Returns true iff the current thread is allowed to create more tasks.
    /// This is sometimes disabled, to avoid needing to create a UT_TaskArena
    /// for small cases that won't get much benefit from threading.
    /// This should be checked by anything using tbb::parallel_for,
    /// tbb::parallel_invoke, or anything else creating TBB tasks.
    static bool           isThreadingEnabled();

    /// This is used to disable (false) threading for the current thread,
    /// to avoid needing to create a UT_TaskArena for small cases that won't
    /// get much benefit from threading.  It returns if it was enabled before.
    /// It is also used to re-enable (true) threading for the current thread.
    static bool           setThreadingEnabled(bool will_be_enabled);

    class ThreadingDisabledScope
    {
    public:
        ThreadingDisabledScope()
            : myPreviouslyEnabled(setThreadingEnabled(false))
        {}
        ~ThreadingDisabledScope()
        {
            if (myPreviouslyEnabled)
                setThreadingEnabled(true);
        }
    private:
        const bool myPreviouslyEnabled;
    };

    // CPU pauses the task for a given number of cycles
    static inline void	  pause(uint cycles)
			  {
			      for(uint i = 0; i < cycles; i++)
#if defined(USE_PTHREADS)
				  __asm__ __volatile__("pause;");
#else
				  _mm_pause();
#endif
			  }
    // Yields the task to the scheduler.
#if defined(USE_PTHREADS)
    static inline void	  yield(bool higher_only=false)
			  {
			      if (higher_only)
			      {
				  ::sched_yield();
			      }
			      else
			      {
				  // Sleep for 100ns. That's 10,000,000 sleep 
				  // cycles a second (in case you don't have a
				  // calculator :-)
				  struct timespec ts = {0,100};
				  ::nanosleep(&ts, 0);
			      }
			  }
#else
    static void		yield(bool higher_only=false);
#endif

    /// This function has been deprecated. Use SYS_SequentialThreadIndex::get()
    /// or SYSgetSTID instead.
    static int SYS_DEPRECATED(12.5) getMySequentialThreadIndex()
			 { return SYS_SequentialThreadIndex::get(); }

    /// Configure the global number of tasks used by the system and the default
    /// stack size for threads. For the @c stacksize:
    /// - A value of -1 leaves the stack size unchanged
    /// - A value of 0 uses the stack size of the main thread
    /// - A value larger than 0 will use that specific stack size
    /// @note Only call this in the main thread when there are no tasks active.
    /// @note This function is NOT thread-safe.
    static void		 configureMaxThreads(int maxthreads = -1,
					     int stacksize  = -1);

    /// Returns true if configureMaxThreads() has been called at least once
    static bool		 isMaxThreadsConfigured();

    /// Return function pointer to terminate task scheduler that is activated
    /// by configureMaxThreads(). This function should called prior to exit()
    /// in order to avoid possible deadlocks when the process exits. Note that
    /// multiple calls to the termination function are handled by only
    /// terminating the first time. After that, no task scheduling is allowed.
    using TerminateFunc = void (*)();
    static TerminateFunc getTaskSchedulerExitCallback();

    // Start the thread running.  If the thread is not in idle state, the
    // thread will wait until it's in idle before starting. If the thread
    // doesn't exist yet, it will be created.
    virtual bool         startThread(UTthreadFunc func, void *data,
                                     int stacksize) = 0;

    // Use the global thread stack size set by configureMaxThreads()
    bool	         startThread(UTthreadFunc func, void *data);

    // This method is called when the thread function is first entered.
    // By default it does nothing but some sub-classes may need this.
    virtual void	 threadStarted();

    // This method is called when the thread function is returned from.
    // By default it sets the state to idle.
    virtual void	 threadEnded();


    // Some thread architectures have very expensive resources (i.e. sproc()
    // threads).  While these threads spin (are idle), they consume system
    // resources.  This method will let the user know whether the threads are
    // resource hogs (so that if they spin for a long time, they could
    // possibley be cleaned up).
    virtual int		 isResourceHog() const;

    // For persistent threads (which get restarted)
    virtual State 	 getState();
    virtual SpinMode 	 getSpinMode();
    virtual void	 waitForState(State desired) = 0;
    virtual void	 setSpinMode(SpinMode spin_mode);

    // Terminate the thread process
    virtual void	 killThread() = 0;

    // If it's possible to perform these tasks, the return code will be 1.  If
    // not, the return code will be 0.  Not all
    virtual int		 niceThread(int priority) = 0;
    virtual int		 suspendThread() = 0;
    virtual int		 restartThread() = 0;

    int			 isActive()
			 { return waitThread(0); }

    static void	 interval(int count, int thisSection, int numSections,
			  int &start, int &len)
		 {
		   start = (int)(thisSection*count/(fpreal)numSections+0.5F);
		   len = (int)((thisSection+1)*count/(fpreal)numSections+0.5F) -
				start;
		 }

    /// NOTE: This level doesn't own any data apart from itself.
    virtual int64	 getMemoryUsage(bool inclusive) const = 0;

protected:
    // System dependent internal functions.
    //	waitThread() returns 1 if the thread is still active (i.e. exists) and
    //	should return 0 if the thread doesn't exist.  If waitThread detects
    //	that the thread no longer exists, it should do appropriate cleanup.
    virtual int		 waitThread(int block=1) = 0;

    // Quick check to see that the thread is really active
    virtual int		 isValid();

    // This method can be used to kill an idle process.
    void		 killIdle();

    static void		*threadWrapper(void *data);

    // Internally used to change the state safely.
    virtual void	 setState(State state) = 0;

    volatile State	 myState;
    SpinMode		 mySpinMode;
    UTthreadFunc	 myCallback;
    void		*myCBData;

    const UT_TaskScope	*myTaskScope;

    UT_Thread(SpinMode spin_mode);
};

// For debugging, the following uses a single thread (i.e. is not
// multi-threaded)
class UT_API UT_NullThread : public UT_Thread
{
public:
	     UT_NullThread();
    virtual ~UT_NullThread();

    virtual bool	 startThread(UTthreadFunc func, void *data,
                                     int stacksize);
    virtual void	 killThread();
    virtual int		 waitThread(int block);
    virtual void	 waitForState(State);

    virtual int		 niceThread(int priority);
    virtual int		 suspendThread();
    virtual int		 restartThread();

    virtual int64	 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        // NOTE: We don't know how much memory Windows uses,
        //       so we can't count it.
        return mem;
    }

protected:
    virtual void	 setState(State state);
};


class UT_API UT_ThreadSet
{
public:
     UT_ThreadSet(int nthreads=-1, int null_thread_if_1_cpu = 0);
    ~UT_ThreadSet();

    void	setFunc(UTthreadFunc func)
		{
		    myFunc = func;
		}
    void	setUserData(void *user_data_array, size_t structlen)
		{
		    myUserData = user_data_array;
		    myUserDataInc = structlen;
		}
    void	setUserData(void *user_data)
		{
		    myUserData = user_data;
		    myUserDataInc = 0;
		}

    void	reuse(UT_Thread::SpinMode spin_mode);
    void	go();
    int		wait(int block=1);

    int			 getNumThreads() const { return myThreadCount; }
    UT_Thread		*getThread(int which);
    UT_Thread		*operator[](int which)
			 {
			     UT_ASSERT_P(which < myThreadCount);
			     return myThreads[which];
			 }

protected:
    int		  myThreadCount;
    UT_Thread	**myThreads;
    UTthreadFunc  myFunc;
    void	 *myUserData;
    int64	  myUserDataInc;
};

class UT_API UT_ThreadFarm
{
public:
    enum AssignmentStyle
    {
	NON_BLOCKING	= 0,	// Only assign thread if one is available
	BLOCKING	= 1,	// Block until a thread is free.
	DYNAMIC		= 2	// If no threads are availble, create a new one.
    };
    
    // similar to UT_ThreadSet, but a bit simpler. Called UT_ThreadFarm
    // because it farms out the next available thread. You also don't need to
    // match the number of data chunks to the number of threads.
    // ie.
    // farm = new UT_ThreadFarm(4);
    // while(!done) {
    //	   thread = farm->nextThread();
    //     thread->startThread(entrypoint, mydata);
    // }
    // farm->wait();
    
     UT_ThreadFarm(int nthreads=-1);
    ~UT_ThreadFarm();

    // waits for the next available thread, (or returns null if none are
    // available and block = 0). thread_index will contain the thread index
    // if you pass it a non-null pointer.
    UT_Thread	*nextThread(int *thread_index =0,
			    AssignmentStyle style = BLOCKING);

    // waits until all threads are finished (or, returns 0 if not finished and
    // block = 0).
    int		 wait(int block = 1);

    // deletes threads in the thread farm. if kill=1 the threads are killed before
    // cleanup, otherwise wait(1) is called.
    void	 cleanup(int kill = 0);

    int		 getEntries() const	{ return myThreadCount; }
    UT_Thread	*operator[](int index)
		 {
		     UT_ASSERT_P(index < myThreadCount);
		     return myThreads[index];
		 }
    
protected:
    void	 addThreads(int thread_count);
    
    int		  myThreadCount;
    UT_Thread	**myThreads;
};

// Gradual backoff when there's thread contention.
class UT_API UT_ThreadBackoff
{
public:
    UT_ThreadBackoff() : myCycles(1) {}

    static const uint	cycles_for_noop = 4;
    static const uint	cycles_for_pause = cycles_for_noop * 4;
    static const uint	cycles_for_yield_higher = cycles_for_pause * 2;
    static const uint	cycles_for_yield_all = cycles_for_yield_higher * 2;

    // Same thresholds as hboost::detail::yield(), but different behaviour
    void wait()
    {
	if (myCycles > cycles_for_yield_all)
	{
	    // Yield the thread completely, to any and all comers.
	    UT_Thread::yield(false);
	    return;
	}
	
	if (myCycles <= cycles_for_noop)
	{
	    // Noop.
	}
	else if (myCycles <= cycles_for_pause)
	{
	    UT_Thread::pause(myCycles);
	}
	else if (myCycles <= cycles_for_yield_higher)
	{
	    UT_Thread::yield(true);
	}
	myCycles += (myCycles+1)>>1;
    }

    void reset()
    {
	myCycles = 1;
    }
    
private:
    uint		myCycles;
};

// This function has been deprecated. Use SYSgetSTID instead.
static inline int SYS_DEPRECATED(12.5)
UTgetSTID()
{
    return SYS_SequentialThreadIndex::get();
}

#endif
