/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ThreadedAlgorithm.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_ThreadedAlgorithm__
#define __UT_ThreadedAlgorithm__

#include "UT_API.h"
#include "UT_Functor.h"
#include "UT_Lock.h"

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>


class UT_JobInfo;


///
/// UT_ThreadedAlgorithm takes care of all the thread administration that a
/// threaded algorithm requires.  
/// 
/// Its features are:
/// 1) it executes work in the main thread as well
/// 2) it assumes that the work can be divided into N parts and sends off 
///    exactly N calls to threadfunc() (jobs). (N depends on the # of 
///    installed processors and the maxthreads parm). 
///
/// Your callback function is called with several
/// useful parameters:
///
///   void * data    - the userdata pointer you provide to run()
///   int  jobindex  - there are N jobs, this is which job you're running
///                    (0 to N-1).
///   int  maxindex  - the total number of jobs farmed (N)
///   UT_Lock &lock  - a lock that you can use to lock other jobs out while
///                    you work on something non-threadsafe. Accepts NULL.
///
/// If your machine is a single proc, this will work exactly as a call to
/// threadfunc().
///
/// In addition to the traditional threadfunc() callback, you can pass
/// in a UT_Functor1 which takes a UT_JobInfo parameter to specify the
/// job to be performed (see below).  In this case, you may be better off
/// with the THREADED_METHOD macros detailed later.
///
class UT_API UT_ThreadedAlgorithm
{
public:
    UT_ThreadedAlgorithm(int maxthreads = -1);
    ~UT_ThreadedAlgorithm();

    /// Starts the jobs, and waits until all are finished. This can be called
    /// multiple times. The optional lock parm allows you to use your own
    /// lock; if not specified, this class's lock will be used.
    void	run(void (*threadfunc)(void *, int, int, UT_Lock &),
		    void *userdata,
		    UT_Lock *lock = 0);

    /// Starts the jobs and waits until all are finished.
    /// Since this uses a functor, you are expected to use BindFirst
    /// to hide any user data you may have
    /// While a return type is specified, it is not used.  This is because
    /// compiler bugs prohibit a Functor2 that returns null.
    void	run(UT_Functor1<int, const UT_JobInfo &> functor,
		    UT_Lock *lock = 0);

private:
    UT_Lock	*myLock;
    int		 myThreadCount;
    bool	 myIsRoot;
    static SYS_AtomicInt32 ourThreadedAlgorithmInUse;
};

///
/// Defines the work required for each invocation of *Partial
/// to complete.
///
class UT_API UT_JobInfo
{
public:
    UT_JobInfo(int jobidx, int numjobs, UT_Lock *lock, SYS_AtomicInt32 *aint = 0)
    {
	myJob = jobidx;
	myNumJobs = numjobs;
	myLock = lock;
	myTask = aint;
	myLocalTask = 0;
    }
    
    /// The number of jobs is the total number of threads
    /// that is running and job() is your thread #, starting at 0
    /// and going up to numJobs()-1
    int		 job() const	 { return myJob; }
    int		 numJobs() const { return myNumJobs; }

    /// These control a shared AtomicInt32 so you can split
    /// tasks among jobs using load balancing.  The total number
    /// is in this case up to you to detect.
    /// The JobIds will start with 0 and each call will get a unique
    /// higher number.
    /// Example:
    /// for (i = info.nextTask(); i < maxtask; i = info.nextTask())
    int32	nextTask() const;

    /// Resets the task list.  You are responsible for creating
    /// a barrier to ensure that all the other threads have
    /// hit the end of the tasks!
    /// This is *not* required for your first loop as you will
    /// already start at task 0.
    void	resetTasks() const;

    /// Given "units" work, report which units you are responsible
    /// for with this JobInfo.  The resulting interval is [start, end),
    ///  Appropriate for loop:
    /// for (info.divideWork(total, i, end); i < end; i++)
    void	 divideWork(int units, int &start, int &end) const;
    void	 divideWork(exint units, exint &start, exint &end) const;

    /// lock and unlock a lock shared by all the jobs.  This lock
    /// is special cased to a no-op in the threadless case to avoid
    /// overhead.
    void	lock() const
		{
		    if (myLock)
			myLock->lock();
		}
    void	unlock() const
		{
		    if (myLock)
			myLock->unlock();
		}

protected:
    int		 	myJob, myNumJobs;
    UT_Lock		*myLock;
    SYS_AtomicInt32	*myTask;
    mutable int		 myLocalTask;
};

class UT_API UT_AutoJobInfoLock
{
public:
     UT_AutoJobInfoLock(const UT_JobInfo &lock)
	 : myLock(lock)
	{ lock.lock(); }
    
    ~UT_AutoJobInfoLock() { myLock.unlock(); }
    
private:
    const UT_JobInfo	&myLock;
};

///
/// @file
/// This macro is equivalent to
/// @code
/// for (i = 0; i < n; i++)
/// @endcode
///
/// It will, however break it into BLOCK sized chunks for each thread,
/// allowing for load balancing
/// NB: Because it relies on the info.nextTask() starting at zero, you
/// can only loop once in this manner.
/// NB: This is a double loop, so break; does not work as you expect.
/// NB: The iterator, i, does not have to be defined.
/// An example of use:
///
/// @code
/// FOR_INFOTASKS(info, i, n, 100)
/// {
///	// Process task i.
/// }
/// @endcode
///
/// If you want opInterrupt() triggered for every block completed,
///
/// @code
/// FOR_INFOTASKS_BOSS(info, i, n, 100, boss)
/// {
///	// Process task i.
/// }
/// @endcode
///
#define FOR_INFOTASKS(info, i, n, blocksize) \
for (int task = info.nextTask() * blocksize; task < n; task = info.nextTask() * blocksize) \
    for (int i = task, lcl_final = SYSmin(task+blocksize, (int)n); i < lcl_final; i++)

#define FOR_INFOTASKS_BOSS(info, i, n, blocksize, boss) \
for (int task = info.nextTask() * blocksize; task < n && !boss->opInterrupt(); task = info.nextTask() * blocksize) \
    for (int i = task, lcl_final = SYSmin(task+blocksize, (int)n); i < lcl_final; i++)


///
/// @file
/// These macros automatically create all the wrapper functions
/// required to set up a multithreaded function call that can
/// be invoked transparently.
///
/// An example of use:
/// @code
/// class Foobar
/// {
///   ...
///   THREADED_METHOD2(Foobar, gdp->points().entries() > 100, 
///                    translate,
///		      GU_Detail *, gdp,
///		      const UT_Vector3 &, delta)
///
///   void translatePartial(GU_Detail *gdp, const UT_Vector3 &delta,
///                         const UT_JobInfo &info);
///   ...
/// }
///
/// void
/// Foobar::translatePartial(GU_Detail *gdp, const UT_Vector3 &delta,
///                          const UT_JobInfo &info)
/// {
///   int		i, n;
///   
///   for (info.divideWork(gdp->points().entries, i, n); i < n; i++)
///   {
///       gdp->points()(i).getPos() += delta;
///   }
/// }
/// @endcode
///
/// The suffix number is the number of parameters the function
/// should take - THREADED_METHOD#(). The parameters to the macro are:
///
/// @code
/// THREADED_METHOD#(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, ...);
/// @endcode
///
/// The DOMULTI is a boolean value (which may be a function returning
/// such) which determines if multithreading should be performed.
/// This lets you disable the multithreading for small batches.
/// Note it executes inside of METHOD, so has access to the parameters
/// in the CLASS. For threaded methods with parameters, you need to
/// specify both the PARMTYPE and the PARMNAME for each parameter.
///
/// The user must both prototype & implement a
/// @code
///    void functionPartial(parameterlist, const &UT_JobInfo)
/// @endcode
/// which does all the real work (see above).
///
/// Automatically created by the THREADED_METHOD macro are:
/// @code
///   void function(parameterlist)	    <- what users can invoke
///   void functionNoThread(parameterlist)   <- unthreaded version
///   int  functionInvokeParital(...)        <- marshalling function.
/// @endcode
///

#define THREADED_METHOD_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD) \
void METHOD() ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread() ISCONST \
{ \
    METHOD ## Partial(UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD1_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1) \
void METHOD(PARMTYPE1 PARMNAME1) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD2_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD3_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD4_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD5_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD6_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD7_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD8_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7, PARMTYPE8 PARMNAME8) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7, PARMTYPE8 PARMNAME8) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD9_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7, PARMTYPE8 PARMNAME8, PARMTYPE9 PARMNAME9) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, PARMNAME9); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, PARMNAME9, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7, PARMTYPE8 PARMNAME8, PARMTYPE9 PARMNAME9) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, PARMNAME9, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD10_INTERNAL(ISCONST, CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9, PARMTYPE10, PARMNAME10) \
void METHOD(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7, PARMTYPE8 PARMNAME8, PARMTYPE9 PARMNAME9, PARMTYPE10 PARMNAME10) ISCONST \
{ \
    if (!(DOMULTI)) \
    { \
	METHOD ## NoThread(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, PARMNAME9, PARMNAME10); \
	return; \
    } \
     \
    auto functor = [&](const UT_JobInfo &info) \
	{ METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, PARMNAME9, PARMNAME10, info); return 0; }; \
    UT_ThreadedAlgorithm alg; \
    alg.run(functor); \
} \
\
void METHOD ## NoThread(PARMTYPE1 PARMNAME1, PARMTYPE2 PARMNAME2, PARMTYPE3 PARMNAME3, PARMTYPE4 PARMNAME4, PARMTYPE5 PARMNAME5, PARMTYPE6 PARMNAME6, PARMTYPE7 PARMNAME7, PARMTYPE8 PARMNAME8, PARMTYPE9 PARMNAME9, PARMTYPE10 PARMNAME10) ISCONST \
{ \
    METHOD ## Partial(PARMNAME1, PARMNAME2, PARMNAME3, PARMNAME4, PARMNAME5, PARMNAME6, PARMNAME7, PARMNAME8, PARMNAME9, PARMNAME10, UT_JobInfo(0, 1, 0)); \
} \
/**/

#define THREADED_METHOD(CLASSNAME, DOMULTI, METHOD) \
    THREADED_METHOD_INTERNAL( , CLASSNAME, DOMULTI, METHOD)
#define THREADED_METHOD_CONST(CLASSNAME, DOMULTI, METHOD) \
    THREADED_METHOD_INTERNAL(const , CLASSNAME, DOMULTI, METHOD)

#define THREADED_METHOD1(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1) \
    THREADED_METHOD1_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1)
#define THREADED_METHOD1_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1) \
    THREADED_METHOD1_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1)

#define THREADED_METHOD2(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2) \
    THREADED_METHOD2_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2)
#define THREADED_METHOD2_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2) \
    THREADED_METHOD2_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2)

#define THREADED_METHOD3(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3) \
    THREADED_METHOD3_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3)
#define THREADED_METHOD3_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3) \
    THREADED_METHOD3_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3)

#define THREADED_METHOD4(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4) \
    THREADED_METHOD4_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4)
#define THREADED_METHOD4_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4) \
    THREADED_METHOD4_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4)

#define THREADED_METHOD5(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5) \
    THREADED_METHOD5_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5)
#define THREADED_METHOD5_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5) \
    THREADED_METHOD5_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5)

#define THREADED_METHOD6(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6) \
    THREADED_METHOD6_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6)
#define THREADED_METHOD6_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6) \
    THREADED_METHOD6_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6)

#define THREADED_METHOD7(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7) \
    THREADED_METHOD7_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7)
#define THREADED_METHOD7_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7) \
    THREADED_METHOD7_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7)

#define THREADED_METHOD8(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8) \
    THREADED_METHOD8_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8)
#define THREADED_METHOD8_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8) \
    THREADED_METHOD8_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8)

#define THREADED_METHOD9(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9) \
    THREADED_METHOD9_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9)
#define THREADED_METHOD9_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9) \
    THREADED_METHOD9_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9)

#define THREADED_METHOD10(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9, PARMTYPE10, PARMNAME10) \
    THREADED_METHOD10_INTERNAL( , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9, PARMTYPE10, PARMNAME10)
#define THREADED_METHOD10_CONST(CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9, PARMTYPE10, PARMNAME10) \
    THREADED_METHOD10_INTERNAL(const , CLASSNAME, DOMULTI, METHOD, PARMTYPE1, PARMNAME1, PARMTYPE2, PARMNAME2, PARMTYPE3, PARMNAME3, PARMTYPE4, PARMNAME4, PARMTYPE5, PARMNAME5, PARMTYPE6, PARMNAME6, PARMTYPE7, PARMNAME7, PARMTYPE8, PARMNAME8, PARMTYPE9, PARMNAME9, PARMTYPE10, PARMNAME10)

#endif
