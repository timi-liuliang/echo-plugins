/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_LockUtil.h ( UT Library, C++)
 *
 * COMMENTS:	Templates for special types of thread locks
 */

#ifndef UT_LOCKUTILS_H
#define UT_LOCKUTILS_H

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_NonCopyable.h"
#include "UT_ValArray.h"
#include <stdio.h>


/// A simple class to acquire a lock and ensure that it is released when it
/// goes out of scope. It locks with a lightweight lock (which is the most 
/// common type used). This works with both normal and threadlock types.
/// @note This is similar to hboost::lock_guard
template <class Lock>
class UT_AutoLockType : UT_NonCopyable
{
public:
     UT_AutoLockType(Lock &lock)
	 : myLock(lock)
	{ lock.lock(); }
    
    ~UT_AutoLockType() { myLock.unlock(); }
    
private:
    Lock	&myLock;
};


/// Like UT_AutoLockType except it supports explicit control of when to lock
/// (or unlock) while providing exception safety.
/// @note This is similar to hboost::unique_lock
/// @note The same UT_LockScopeType object cannot be safely accessed from
///       multiple threads!
template <class Lock>
class UT_LockScopeType : UT_NonCopyable
{
public:
    /// Default constructor with mutex lock. Use acquire() to give it mutex.
    UT_LockScopeType()
	: myMutex(NULL)
	, myIsLocked(false)
    {
    }
    /// Construct with given mutex. Acquires it by default.
    UT_LockScopeType(Lock &mutex, bool acquire = true)
	: myMutex(&mutex)
	, myIsLocked(false)
    {
	if (acquire)
	    lock();
    }
    UT_LockScopeType(UT_LockScopeType<Lock> &&scope)
	: myMutex(scope.myMutex)
	, myIsLocked(scope.myIsLocked)
    {
	scope.myMutex = nullptr;
	scope.myIsLocked = false;
    }
    ~UT_LockScopeType()
    {
	if (myIsLocked)
	    unlock();
    }

    /// Lock the mutex
    void lock()
    {
	if (myIsLocked || !myMutex)
	{
	    UT_ASSERT(!"Can't relock");
	    return;
	}
	myMutex->lock();
	myIsLocked = true;
    }

    /// Assign a new mutex reference and lock it
    /// @note Unlocks the old mutex.
    void lock(Lock &mutex)
    {
	if (myIsLocked)
	    unlock();
	myMutex = &mutex;
	myMutex->lock();
	myIsLocked = true;
    }

    /// Try to lock the mutex, return immediately false if someone else owns it
    bool tryLock()
    {
	if (myIsLocked || !myMutex)
	{
	    UT_ASSERT(!"Can't relock");
	    return false;
	}
	myIsLocked = myMutex->tryLock();
	return myIsLocked;
    }

    /// Try to lock the mutex, return immediately false if someone else owns it.
    /// @note Unlocks the old mutex.
    bool tryLock(Lock &mutex)
    {
	if (myIsLocked)
	    unlock();
	myMutex = &mutex;
	myIsLocked = myMutex->tryLock();
	return myIsLocked;
    }

    /// Lock the mutex, return false if a dead lock was detected.
    /// @note Only works if the underlying mutex supports it.
    bool safeLock(Lock &mutex)
    {
	if (myIsLocked)
	    unlock();
	myMutex = &mutex;

	if (!mutex.safeLock())
	    return false;

	myIsLocked = true;
	return true;
    }

    /// Unlock the mutex before this object is deleted.
    void unlock()
    {
	if (!myIsLocked || !myMutex)
	{
	    UT_ASSERT(!"Can't unlock without locking first");
	    return;
	}
	myMutex->unlock();
	myIsLocked = false;
    }

private:
    Lock *  myMutex;
    bool    myIsLocked;
};


/// An empty implementation of the lock interface (for easily removing all
/// locks from an application).
class UT_NullLock : UT_NonCopyable
{
public:
    explicit UT_NullLock(int /*lockstate*/ = 0) {}

    bool	lock(int) { return true; }
    bool	safeLock() { return true; }
    void	lock() {}
    void	unlock() {}
    bool	isLocked() { return false; }

    typedef UT_LockScopeType<UT_NullLock> Scope;
};


template <class Lock>
class UT_DebugLockType : UT_NonCopyable
{
public:
     UT_DebugLockType(const char *name, int lockstate=0, bool threadlock=false)
	 : myLock(lockstate, threadlock),
	   myName(name)
     { }
     UT_DebugLockType(const UT_DebugLockType &rhs)
	 : myLock(rhs.myLock),
	   myName(rhs.myName)
     { }
    ~UT_DebugLockType()
    {
	fprintf(stderr, "Lock[%s]: %d collisions\n",
		myName, myLock.getCollisions());
    }

    bool	lock(int ms)	{ return myLock.lock(ms); }

    void	lock()		{ myLock.lock(); }
    bool	safeLock()	{ myLock.lock(); return true; }

    void	unlock()	{ myLock.unlock(); }

    bool	isLocked()	{ return myLock.isLocked(); }

    typedef UT_LockScopeType< UT_DebugLockType<Lock> > Scope;

private:
    Lock	 myLock;
    const char	*myName;
};

// A data-bound lock.  The lock excludes access to the given object only,
// while allowing other threads to lock different objects.
template <class Lock>
class UT_ObjLockType : UT_NonCopyable
{
public:
    UT_ObjLockType(int capacity = 256)
    {
	myObjs.setCapacity(capacity);
    }
    ~UT_ObjLockType()
    {
	clear();
    }

    int lock(const void *obj)
    {
	ut_ObjLockEntry	*olock;
	int		 ident;
	{
	    // Lock to access the list.  Make sure to grab the olock befrore
	    // the lock is released.
	    typename Lock::Scope	lock(myAccessLock);
	    ident = getIdentifier(obj);
	    olock = myObjs[ident];
	}
	olock->myLock.lock();
	return ident;
    }

    // Tries to obtain the lock, returns a non-negative identifier if the
    // lock was obtained - otherwise -1.
    int tryLock(const void *obj)
    {
	ut_ObjLockEntry	*olock;
	int		 ident;
	{
	    typename Lock::Scope	lock(myAccessLock);
	    ident = getIdentifier(obj);
	    olock = myObjs[ident];
	}
	if (olock->myLock.tryLock())
	    return ident;
	{
	    // Re-lock the list while releasing the object
	    typename Lock::Scope	lock(myAccessLock);
	    releaseIdentifier(obj, ident);
	}
	return -1;
    }

    void unlock(const void *obj, int ident)
    {
	typename Lock::Scope	lock(myAccessLock);
	myObjs[ident]->myLock.unlock();
	releaseIdentifier(obj, ident);
    }

private:
    void clear()
    {
	for (exint i = 0; i < myObjs.entries(); i++)
	    delete myObjs[i];
	myObjs.entries(0);
    }
    int	getIdentifier(const void *obj)
    {
	exint ident = -1;

	for (exint i = 0; i < myObjs.entries(); i++)
	{
	    if (myObjs[i]->myObj == obj)
	    {
		ident = i;
		break;
	    }
	    if (!myObjs[i]->myRefCount)
		ident = i;
	}
	if (ident < 0)
	{
	    myObjs.append(new ut_ObjLockEntry());
	    ident = myObjs.entries()-1;
	}

	// Either we found the object or there is an empty slot.
	myObjs[ident]->myObj = obj;
	myObjs[ident]->myRefCount++;
	return ident;
    }
    void releaseIdentifier(const void *obj, int ident)
    {
	UT_ASSERT(myObjs[ident]->myObj == obj);
	UT_ASSERT(myObjs[ident]->myRefCount > 0);
	myObjs[ident]->myRefCount--;
	if (!myObjs(ident)->myRefCount)
	    myObjs(ident)->myObj = nullptr;
    }

    class ut_ObjLockEntry
    {
    public:
	ut_ObjLockEntry()
	    : myObj(nullptr)
	    , myRefCount(0)
	{
	}

	Lock		 myLock;
	const void	*myObj;
	int		 myRefCount;
    };

public:
    Lock			myAccessLock;
    UT_Array<ut_ObjLockEntry *> myObjs;
};
    
template <class Lock>
class UT_AutoObjLockType : UT_NonCopyable
{
public:
     UT_AutoObjLockType(UT_ObjLockType<Lock> &lock, const void *obj)
	 : myLock(lock)
	 , myObj(obj)
	{ myId = lock.lock(myObj); }
    
    ~UT_AutoObjLockType() { myLock.unlock(myObj, myId); }
    
private:
    UT_ObjLockType<Lock>	&myLock;
    const void			*myObj;
    int				 myId;
};

#endif

