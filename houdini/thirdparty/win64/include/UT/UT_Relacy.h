/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Relacy.h (UT Library, C++)
 *
 * COMMENTS:	Wrapper classes for testing with Relacy.
 *
 *		- Do NOT include this in .h files!
 *		- Only include this in the .C file of your unit tests.
 *		- Include this file *first* in your .C file so that it can
 *		  redefine everything. For any conflicting header files,
 *		  add them to the PRE_INCLUDES list below.
 *		- For all classes wrapped by this file, use the macro UT_VAR()
 *		  to access the variables of these classes.
 *
 *		For more information on the Relacy Race Detector see
 *		    http://www.1024cores.net/home/relacy-race-detector 
 *
 */

#ifndef __UT_RELACY_INCLUDED__
#define __UT_RELACY_INCLUDED__

#if !defined(UT_DEBUG_RELACY)

//
// No Relacy
//

#define UT_VAR(X)	    (X)
#define UT_VAR_T(X)	    X

#define UT_LOG_EVENT(X)	

#else


// PRE_INCLUDES
// These headers must be included before we can perform the redefinitions to
// avoid compiler errors in the files that include UT_Relacy.h.
#include "UT_Assert.h"
#include "UT_ConcurrentVector.h"
#include "UT_Debug.h"
#include "UT_IntArray.h"
#include "UT_LockUtil.h"
#include "UT_ValArray.h"
#include "UT_RecursiveTimedLock.h"
#include "UT_Array.h"
#include "UT_SmallObject.h"
#include "UT_UniquePtr.h"
#include "UT_StopWatch.h"
#include "UT_String.h"
#include "UT_SysClone.h"
#include "UT_TaskLock.h"
#include "UT_TestManager.h"
#include "UT_Thread.h"
#include "UT_ThreadSpecificValue.h"
#include "UT_ValArray.h"
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_AtomicPtr.h>
#include <SYS/SYS_BoostThread.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_StaticInit.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeTraits.h>
#include <limits>
#include <errno.h>
#include <stdio.h>


//
// Relacy
//

#define RL_MSVC_OUTPUT		    // output debug message to MSVC
#define RL_DEBUGBREAK_ON_FAILURE    // cause breakpoint on failures

#include <relacy/relacy_std.hpp>    // include Relacy

namespace UT_Relacy
{

// Import classes that we're using from Relacy into this namespace
using rl::debug_info;
using rl::debug_info_param;
using rl::atomic;
using rl::recursive_timed_mutex;

//
// Wrapper for atomic variables
//

#define SYS_MEMORY_ORDER_RELAXED    (rl::mo_relaxed)
//#define SYS_MEMORY_ORDER_CONSUME    (rl::mo_consume)
//#define SYS_MEMORY_ORDER_ACQUIRE    (rl::mo_acquire)
//#define SYS_MEMORY_ORDER_RELEASE    (rl::mo_release)
//#define SYS_MEMORY_ORDER_ACQ_REL    (rl::mo_acq_rel)
#define SYS_MEMORY_ORDER_LOAD       (rl::mo_acquire)
#define SYS_MEMORY_ORDER_STORE      (rl::mo_release)
#define SYS_MEMORY_ORDER_SEQ_CST    (rl::mo_seq_cst)

template<typename T>
class SYS_Atomic;

template<typename T>
class SYS_AtomicProxyConst
{
public:
    SYS_AtomicProxyConst(SYS_Atomic<T> const & var, debug_info_param info)
        : var_(const_cast<SYS_Atomic<T>&>(var))
        , info_(info)
    {
    }

    T load(rl::memory_order mo = rl::mo_seq_cst) const
    {
        return var_.load(mo, info_);
    }

    operator T () const
    {
        return load();
    }

protected:
    SYS_Atomic<T>& var_;
    debug_info info_;

    SYS_AtomicProxyConst& operator = (SYS_AtomicProxyConst const&);
};

template<typename T>
class SYS_AtomicProxy : public SYS_AtomicProxyConst<T>
{
public:
    SYS_AtomicProxy(SYS_Atomic<T> & var, debug_info_param info)
        : SYS_AtomicProxyConst<T>(var, info)
    {
    }

    void store(T value, rl::memory_order mo = rl::mo_seq_cst)
    {
        this->var_.store(value, mo, this->info_);
    }
    T add(T value)
    {
        return this->var_.add(value, this->info_);
    }
    T exchange(T xchg)
    {
        return this->var_.exchange(xchg, this->info_);
    }
};

template <typename T>
class SYS_Atomic : private rl::generic_atomic<T, true>
{
    typedef rl::generic_atomic<T, true> SUPER;

public:
    SYS_Atomic()
    {
    }

    SYS_Atomic(T value)
    {
	SUPER::store(value, rl::mo_relaxed, $);
    }

    void store(T val, rl::memory_order mo, rl::debug_info_param info)
    {
	SUPER::store(val, mo, info);
    }
    T load(rl::memory_order mo, rl::debug_info_param info) const
    {
	return SUPER::load(mo, info);
    }
    T add(T val, rl::debug_info_param info)
    {
	return SUPER::fetch_add(val, rl::mo_seq_cst, info) + val;
    }
    T exchange(T val, rl::debug_info_param info)
    {
	return SUPER::exchange(val, rl::mo_seq_cst, info);
    }

    SYS_AtomicProxyConst<T> operator () (debug_info_param info) const
    {
        return SYS_AtomicProxyConst<T>(*this, info);
    }

    SYS_AtomicProxy<T> operator () (debug_info_param info)
    {
        return SYS_AtomicProxy<T>(*this, info);
    }

    friend class SYS_AtomicProxy<T>;
    friend class SYS_AtomicProxyConst<T>;

    RL_NOCOPY(SYS_Atomic);
};

typedef SYS_Atomic<int32>   SYS_AtomicInt32;	// From SYS_AtomicInt.h

template <typename T>
class SYS_AtomicPtr : public SYS_Atomic<T *>	// From SYS_AtomicPtr.h
{
public:

    SYS_AtomicPtr(T *ptr = NULL)
	: SYS_Atomic(ptr)
    {
    }

};

inline int 
SYSgetSTID()
{
    return rl::ctx().current_thread()
	    + UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX;
}

class UT_RelacyObject
{
public:
    virtual	    ~UT_RelacyObject() { }
    virtual void    construct(int num_threads) = 0;
    virtual void    destruct() = 0;
};

class UT_RelacyManager
{
public:
		    UT_RelacyManager()
			: myNumThreads(0)
		    {
		    }

    void	    subscribe(UT_RelacyObject &obj)
		    {
			mySubscribers.append(&obj);
			if (myNumThreads > 0)
			    obj.construct(myNumThreads);
		    }
    void	    unsubscribe(UT_RelacyObject &obj)
		    {
			if (myNumThreads > 0)
			    obj.destruct();
			mySubscribers.findAndRemove(&obj);
		    }

    void	    construct(int num_threads)
		    {
			myNumThreads = num_threads;
			for (int i = 0; i < mySubscribers.entries(); i++)
			    mySubscribers(i)->construct(num_threads);
		    }

    void	    destruct()
		    {
			for (int i = 0; i < mySubscribers.entries(); i++)
			    mySubscribers(i)->destruct();
			myNumThreads = 0;
		    }

private:
    UT_ValArray<UT_RelacyObject *>  mySubscribers;
    int				    myNumThreads;
};
static UT_RelacyManager theRelacyManager;

template <typename T>
class UT_ThreadSpecificValue : public UT_RelacyObject
{
public:
    UT_ThreadSpecificValue()
	: myValues(NULL)
	, myNumThreads(0)
    {
	theRelacyManager.subscribe(*this);
    }
    ~UT_ThreadSpecificValue()
    {
	theRelacyManager.unsubscribe(*this);
    }

    virtual void construct(int num_threads)
    {
	myValues = new T[num_threads];
	myNumThreads = num_threads;

	// If T is an object, the default constructor will have been
	// called.  Otherwise, initialize the memory to 0.
	if (std::numeric_limits<T>::is_specialized
		|| SYS_IsPointer<T>::value)
	{
	    memset(static_cast<void *>(myValues), 0, myNumThreads*sizeof(T));
	}
    }
    virtual void destruct()
    {
	delete [] myValues;
	myValues = NULL;
	myNumThreads = 0;
    }

    T &getValueForThread(int thread_index)
    {
	thread_index -= UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX;
	UT_ASSERT(thread_index >= 0 && thread_index < myNumThreads);
	return myValues[thread_index];
    }

    const T &getValueForThread(int thread_index) const
    {
	thread_index -= UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX;
	UT_ASSERT(thread_index >= 0 && thread_index < myNumThreads);
	return myValues[thread_index];
    }

    int maxThreadsSeen() const
    {
	return myNumThreads;
    }

    class const_iterator
    {
    public:
	const_iterator()
	    : myVal(0)
	    , myI(0)
	    , myEnd(0)
	{
	}
	const_iterator(const const_iterator &copy)
	    : myVal(copy.myVal)
	    , myI(copy.myI)
	    , myEnd(copy.myEnd)
	{
	}
	const_iterator &operator=(const const_iterator &copy)
	{
	    if (this != &copy)
	    {
		myVal = copy.myVal;
		myI = copy.myI;
		myEnd = copy.myEnd;
	    }
	    return *this;
	}

	const T &get() const
	{
	    return myVal->getValueForThread(myI);
	}

	int thread() const
	{
	    return myI;
	}

	const_iterator &operator++()
	{
	    if (myI < myEnd)
		++myI;
	    return *this;
	}

	bool operator==(const const_iterator &right)
	{
	    return (myVal == right.myVal
		    && myI == right.myI
		    && myEnd == right.myEnd);
	}
	bool operator!=(const const_iterator &right)
	{
	    return !(*this == right);
	}

    protected:
	const_iterator(UT_ThreadSpecificValue<T> *value, int start)
	    : myVal(value)
	    , myI(start)
	    , myEnd(UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX
			+ value->maxThreadsSeen())
	{
	}

	UT_ThreadSpecificValue<T> *	    myVal;
	int				    myI;
	int				    myEnd;
	template <typename TY> friend class UT_ThreadSpecificValue;
    };

    /// iterator
    ///
    /// @note The iterator iterates over ALL possible thread values, thus
    /// you must be aware that the get() method will return the default value
    /// for cases where this variable was never used in a thread.
    class iterator : public const_iterator
    {
    public:
	iterator()
	    : const_iterator()
	{
	}
	iterator(const iterator &copy)
	    : const_iterator(copy)
	{
	}
	iterator &operator=(const iterator &copy)
	{
	    this->const_iterator::operator=(copy);
	    return *this;
	}


	T &get()
	{
	    return const_iterator::myVal->getValueForThread(
							const_iterator::myI);
	}

	iterator &operator++()
	{
	    if (const_iterator::myI < const_iterator::myEnd)
		++const_iterator::myI;
	    return *this;
	}

	bool operator==(const iterator &right)
	{
	    return (const_iterator::myVal == right.myVal
		    && const_iterator::myI == right.myI
		    && const_iterator::myEnd == right.myEnd);
	}
	bool operator!=(const iterator &right)
	{
	    return !(*this == right);
	}

    private:
	iterator(UT_ThreadSpecificValue<T> *value, int start)
	    : const_iterator(value, start)
	{
	}
	template <typename TY> friend class UT_ThreadSpecificValue;
    };

    /// begin() const iterator
    const_iterator begin() const
	{ return const_iterator(this,
	    UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX); }
    /// end() const iterator
    const_iterator end() const
	{ return const_iterator(this,
	    UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX + maxThreadsSeen()); }

    /// begin() iterator
    iterator begin()
	{ return iterator(this, UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX); }
    /// end() iterator
    iterator end()
	{ return iterator(this,
	    UT_Thread::UT_FIRST_SEQUENTIAL_THREAD_INDEX + maxThreadsSeen()); }

private:
    T *	myValues;
    int	myNumThreads;
};

class UT_RecursiveTimedLockProxy
{
public:
    UT_RecursiveTimedLockProxy(
	    recursive_timed_mutex &mutex,
	    debug_info_param info
	)
	: mutex_(mutex)
	, info_(info)
    {
    }

    bool timedLock(int ms)  { return mutex_.timed_lock(ms, info_); }
    bool tryLock()	    { return mutex_.try_lock(info_); }
    void lock()		    { mutex_.lock(info_); }
    void unlock()	    { mutex_.unlock(info_); }

private:
    recursive_timed_mutex & mutex_;
    debug_info		    info_;
};

class UT_RecursiveTimedLock
{
public:
    UT_RecursiveTimedLock()
    {
    }

    UT_RecursiveTimedLockProxy operator()(debug_info_param info)
    {
	return UT_RecursiveTimedLockProxy(mutex_, info);
    }

private:
    recursive_timed_mutex   mutex_;

    RL_NOCOPY(UT_RecursiveTimedLock);
};

} // namespace UT_Relacy

//
// Defines to swap in use of the wrappers
//
#define SYSgetSTID		UT_Relacy::SYSgetSTID
#define UT_RecursiveTimedLock	UT_Relacy::UT_RecursiveTimedLock
#define UT_ThreadSpecificValue	UT_Relacy::UT_ThreadSpecificValue
#define SYS_AtomicInt32		UT_Relacy::SYS_AtomicInt32
#define SYS_AtomicPtr		UT_Relacy::SYS_AtomicPtr

// Substitute UT_ASSERT() with RL_ASSERT()
#ifdef UT_ASSERT
    #undef UT_ASSERT
#endif
#define UT_ASSERT	RL_ASSERT

// Relacy specific macros
#define UT_LOG_EVENT(X)	if (rl::ctx().collecting_history()) { RL_LOG(X); }
#define UT_VAR(X)	VAR((X))
#define UT_VAR_T(X)	VAR_T(X)

#endif // UT_DEBUG_RELACY


//////////////////////////////////////////////////////////////////////////////
//
// UT_RelacyTest
//

#ifdef UT_DEBUG_RELACY

template <typename DERIVED_T, int THREAD_COUNT>
class UT_RelacyTest : public rl::test_suite<DERIVED_T, THREAD_COUNT>
{
public:
    static bool simulate(int ITER_COUNT_RELACTY)
    {
	rl::test_params params;

	params.iteration_count = ITER_COUNT_RELACY;
	params.search_type = rl::random_scheduler_type;
	params.execution_depth_limit = 4000;
	//
	//params.iteration_count = 1;
	//params.search_type = rl::fair_context_bound_scheduler_type;
	//
	//params.iteration_count = 1;
	//params.search_type = rl::fair_full_search_scheduler_type;

	return rl::simulate<DERIVED_T>(params);
    }

protected:
    void construct()
    {
	UT_Relacy::theRelacyManager.construct(THREAD_COUNT);
    }
    void destruct()
    {
	UT_Relacy::theRelacyManager.destruct();
    }

    void fail()
    {
	UT_ASSERT(!"Test failed");
    }

    // In Relacy, random timer's are already occurring so just skip
    // this step algother.
    void setSeed(unsigned int /*seed*/)
    {
    }
    void sleepRandom(unsigned int /*max_millisec*/)
    {
    }

    int getMaxThreadId(int num_threads) const
    {
	return num_threads - 1;
    }

private:
};

#else 

#include "UT_Assert.h"
#include "UT_ValArray.h"
#include "UT_Thread.h"
#include "UT_StopWatch.h"
#include "UT_String.h"
#include "UT_SysClone.h"
#include "UT_UniquePtr.h"
#include <SYS/SYS_Math.h>
#include <typeinfo>

namespace UT_Relacy
{

class ThreadPool
{
public:
    ThreadPool()
	: mySize(0)
    {
    }
    ~ThreadPool()
    {
	for (int i = 0; i < myThreads.entries(); i++)
	    delete myThreads(i);
    }

    void resize(int count)
    {
	mySize = count;

	// always grow array to maximum requested count
	for (int i = myThreads.entries(); i < count; i++)
	{
	    UT_Thread *	thr = UT_Thread::allocThread(UT_Thread::ThreadLowUsage);
	    thr->startThread(0,0);
	    myThreads.append(thr);
	}
	UT_ASSERT(count <= myThreads.entries());
    }

    UT_Thread *getThread(int i)
    {
	UT_ASSERT(i >= 0 && i < mySize);
	return myThreads(i);
    }

    void waitThreads()
    {
	for (int i = 0; i < mySize; i++)
	    myThreads(i)->waitForState(UT_Thread::ThreadIdle);
    }

    static ThreadPool *get(int count)
    {
	ThreadPool *&pool = getPoolPtr();
	
	if (!pool)
	    pool = new ThreadPool;

	pool->resize(count);
	return pool;
    }
    
    static void destroy()
    {
	ThreadPool *&pool = getPoolPtr();
	delete pool;
	pool = NULL;
    }

private:
    static ThreadPool *&getPoolPtr()
    {
	static ThreadPool *thePool = NULL;
	return thePool;
    }
    
    UT_ValArray<UT_Thread *>	myThreads;
    int				mySize;
};

template <typename T>
struct ThreadData
{
    T *	myTest;
    int	myIndex;

    static void *func(void *data)
    {
	ThreadData<T> *	me = (ThreadData<T> *)data;
	int		i = me->myIndex;

	me->myTest->mySeqThreadIndex[i] = SYSgetSTID();
	me->myTest->thread(i);
	return NULL;
    }
};

} // namespace UT_Relacy

template <typename DERIVED_T, int THREAD_COUNT>
class UT_RelacyTest
{
public:
    typedef UT_RelacyTest<DERIVED_T,THREAD_COUNT>	TestThread;


    static bool simulate(int ITER_COUNT, bool verbose)
    {
	UT_Relacy::ThreadPool *
			pool = UT_Relacy::ThreadPool::get(THREAD_COUNT);
	bool		passed = true;
	UT_UniquePtr<UT_Timer>	ts;
	UT_String	num;

	if (verbose)
	    ts.reset(new UT_Timer(typeid(DERIVED_T).name()));

	for (int iter = 0; iter < ITER_COUNT; iter++)
	{
	    DERIVED_T				test;
	    UT_Relacy::ThreadData<DERIVED_T>	data[THREAD_COUNT];

	    for (int i = 0; i < THREAD_COUNT; i++)
	    {
		data[i].myTest = &test;
		data[i].myIndex = i;
	    }

	    test.myOk = true;
	    test.before();

	    for (int i = 0; i < THREAD_COUNT; i++)
	    {
		UT_Thread * thr = pool->getThread(i);

		if (!thr->startThread(UT_Relacy::ThreadData<DERIVED_T>::func,
				      &data[i]))
		{
		    UT_ASSERT(!"Failed to start thread");
		    passed = false;
		    break;
		}
	    }
	    pool->waitThreads();

	    test.after();
	    if (!test.myOk)
		passed = false;
	}

	num.itoa(ITER_COUNT);
	if (ts)
	    ts->timeStamp("iterations", num);
	return passed;
    }

protected:
    void construct()
    {
	mySeed = (int32)(((size_t)this) & 0xFFFFFFFF);
    }
    void destruct()
    {
    }

    void fail()
    {
	UT_ASSERT(!"Test failed");
	myOk = false;
    }

    void sleepRandom(unsigned int max_millisec)
    {
	UTnap((unsigned)(SYSrandom(mySeed)*max_millisec));
    }

    int getMaxThreadId(int num_threads) const
    {
	int max_id = 0;
	int max_value = mySeqThreadIndex[max_id];

	for (int i = 1; i < num_threads; i++)
	{
	    if (mySeqThreadIndex[i] > max_value)
	    {
		max_value = mySeqThreadIndex[i];
		max_id = i;
	    }
	}

	return max_id;
    }

private:
    uint	mySeed;
    int		mySeqThreadIndex[THREAD_COUNT];
    bool	myOk;

    template <typename T> friend struct UT_Relacy::ThreadData;
};

#endif // UT_DEBUG_RELACY


#endif // __UT_RELACY_INCLUDED__
