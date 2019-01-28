/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_LockedRawPtr.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_LOCKEDRAWPTR_H__
#define __UT_LOCKEDRAWPTR_H__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_UniquePtr.h"
#include <SYS/SYS_Compiler.h>


/// A template class that allows auto-locking in a getter method.  This class
/// has pointer semantics and does NOT deal with ownership by design. The
/// pointer that you give it *MUST* live longer than the lifetime of this
/// class.
/// 
/// EXAMPLE:
/// @code
/// class B;
/// class A
/// {
///	/// Obtain a ptr to myB that is locked. When it goes out
///	/// of scope, it will automatically unlock.
///	UT_LockedRawPtr<B,UT_Lock> getLockedB()
///	    { return UT_LockedRawPtr<B>(myB, myBLock); }
/// private:
///	B	myB;
///	UT_Lock myBLock;
/// };
/// @endcode
template <typename T, typename LOCKABLE>
class UT_LockedRawPtr
{
public:

    typedef typename LOCKABLE::Scope	LockScopeT;
    typedef T				ElementT;

    UT_LockedRawPtr()
	: myPtr(nullptr)
	, myLockScope(nullptr)
    {
    }

    /// Construct a locked ptr instance
    UT_LockedRawPtr(T &ref, LOCKABLE &lock)
	: myPtr(&ref)
	, myLockScope(new LockScopeT(lock))
    {
    }

    /// This class is only movable
    /// @{
    UT_LockedRawPtr(UT_LockedRawPtr &&other)
	: myPtr(std::move(other.myPtr))
	, myLockScope(std::move(other.myLockScope))
    {
    }
    UT_LockedRawPtr &
    operator=(UT_LockedRawPtr &&other)
    {
	myPtr = std::move(other.myPtr);
	myLockScope = std::move(other.myLockScope);
        return *this;
    }
    /// @}

    /// Pointer semantic accessors
    /// @{
    T &	operator*() const		{ UT_ASSERT(myPtr); return *myPtr; }
    T *	operator->() const		{ UT_ASSERT(myPtr); return  myPtr; }
    SYS_SAFE_BOOL operator bool() const	{ return (myPtr != 0); }
    /// @}

private:
    UT_LockedRawPtr(const UT_LockedRawPtr &other);
    UT_LockedRawPtr &operator=(const UT_LockedRawPtr &other);
    
    T *					myPtr;
    mutable UT_UniquePtr<LockScopeT>	myLockScope;
};

#endif // __UT_LOCKEDRAWPTR_H__
