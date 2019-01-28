/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	Use PY_InterpreterAutoLock to automatically lock blocks of code
 *	that call into the python interpreter.  You cannot make any python API
 *	calls without having the global interpreter lock.  It's okay to have
 *	multiple levels of locks.  The HOMF_Utils functions and the methods
 *	of PY_CompiledCode already use this class, so you don't need to
 *	use this lock with those classes.
 *
 *	Note that this header avoids including the Python headers.  This
 *	helps avoid warnings that can occur when the Python headers are not
 *	the first headers included.
 */

#ifndef __PY_InterpreterAutoLock_h__
#define __PY_InterpreterAutoLock_h__

#include "PY_API.h"
#include <UT/UT_Thread.h>

class PY_API PY_InterpreterAutoLock
{
public:
    PY_InterpreterAutoLock();
    ~PY_InterpreterAutoLock();

private:
    int myGlblInterpLockState;
};


// A PY_InterpreterAutoUnlock lets you release the Python GIL, if it was
// held, to let other Python threads run while you make a time-consuming or
// blocking call.  It's ok to use an auto unlock in code that's not being
// called from the Python interpreter.  For example, HOMF code is usually
// invoked from the Python interpreter, but it may be invoked via C++ from
// other places in Houdini, and it's safe to use an auto unlock in both cases.
//
// Using this class is roughly equivalent to the Py_BEGIN_ALLOW_THREADS and
// Py_END_ALLOW_THREADS macros, except it's safe to use it when not invoked
// from Python.  It also ensures that the GIL is reacquired if an exception
// is thrown.
//
// Note that this class does not release the HOM lock; use HOM_AutoUnlock in
// combination with this class to release them both.  Be sure to create
// the HOM_AutoUnlock *after* the PY_InterpreterAutoUnlock, so that when
// the auto locks are destructed the HOM lock is reacquired before the Python
// GIL.  Otherwise, it will grab the GIL and then try to grab the HOM lock,
// and if another thread with the HOM lock tries to run Python we'll end up
// with deadlock.
//
// A sample use of this class is:
//
// {
//     PY_InterpreterAutoLock interpreter_lock;
//     ... make Python API calls ...
//
//     {
//         PY_InterpreterAutoUnlock interpreter_unlock;
//         // Other Python threads are now free to run.
//         ... make a blocking I/O call ...
//     }
//
//     ... continue to make Python API calls ...
// }
class PY_API PY_InterpreterAutoUnlock
{
public:
    PY_InterpreterAutoUnlock();
    ~PY_InterpreterAutoUnlock();

private:
    void *mySavedThreadState;
};

// Return true if we've been initialized
PY_API bool PYisPythonInitialized();

#endif
