/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_RWLock.h ( UT Library, C++)
 *
 * COMMENTS:
 * 	Synchronization class to support multiple readers and one writer.
 */

#ifndef __UT_RWLock_h__
#define __UT_RWLock_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_NonCopyable.h"

/// A simple class to hold a rwlock and release it when it goes out of scope.
template <typename Lock, bool READMODE>
class UT_AutoRWLockType
{
public:
     UT_AutoRWLockType(Lock &lock)
	 : myLock(lock)
     {
	 if (READMODE)
	     myLock.readLock();
	 else
	     myLock.writeLock();
     }
    
    ~UT_AutoRWLockType()
     {
	 if (READMODE)
	     myLock.readUnlock();
	 else
	     myLock.writeUnlock();
     }
    
private:
    Lock	&myLock;
};


/// Reader/Writer mutex class
class UT_API UT_RWLock : UT_NonCopyable
{
public:
     UT_RWLock();
    ~UT_RWLock();

    /// Attempt to acquire a shared read lock, returning false if the shared
    /// read lock cannot be acquired immediately.
    ///
    /// A thread holding the exclusive write lock may acquire a shared read
    /// lock as well, thereby holding both.
    bool	tryReadLock(int ntrials=1);

    /// Attempt to acquire an exclusive write lock, returning false if the
    /// exclusive write lock cannot be acquired immediately.
    ///
    /// It is an error to attempt to acquire an exclusive write lock while
    /// holding a shared read lock.
    bool	tryWriteLock(int ntrials=1);

    /// Attempt to acquire a shared read lock, blocking until the lock is
    /// acquired.
    ///
    /// A thread holding the exclusive write lock may acquire a shared read
    /// lock as well, thereby holding both.
    void	readLock();

    /// Attempt to acquire an exclusive write lock, blocking until the lock is
    /// acquired.
    ///
    /// It is an error to attempt to acquire an exclusive write lock while
    /// holding a shared read lock. If you're not sure whether you're holding
    /// the read lock or not, use @c tryWriteLock instead.
    void	writeLock();

    /// Release a shared read lock.
    void	readUnlock();

    /// Release an exclusive write lock.
    void	writeUnlock();

    typedef UT_AutoRWLockType<UT_RWLock, true>	ReadScope;
    typedef UT_AutoRWLockType<UT_RWLock, false>	WriteScope;

private:
    struct ut_RWLockData;
    
    ut_RWLockData	*myData;
};


typedef UT_AutoRWLockType<UT_RWLock, true>	UT_AutoReadLock;
typedef UT_AutoRWLockType<UT_RWLock, false>	UT_AutoWriteLock;

#endif
