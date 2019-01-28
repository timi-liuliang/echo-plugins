/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_COW.h ( UT Library, C++)
 *
 * COMMENTS:
 *		Defines our templated copy-on-write methods.
 */

#ifndef __UT_COW__
#define __UT_COW__

#include "UT_API.h"

#include <SYS/SYS_AtomicInt.h>

#include "UT_Assert.h"
#include "UT_SharedPtr.h"

///
/// This is a copy-on-write shared pointer to data.
/// If you create a writehandle, it will uniquify the
/// underlying data.
/// Do not pass references to handles or pointers to them around.
/// This can cause you to invalidate the tracking of copies.
/// In any case, passing by value is fast.
///

// Forward declarations for our friendship.
template <typename Data> class UT_COWReadHandle;
template <typename Data> class UT_COWTransientReadHandle;
template <typename Data> class UT_COWWriteHandle;

///
/// A plain handle is an opaque refernce to the data.  You can't
/// read or write to it with this sort of handle, but you can
/// copy and store it.  Assign a plain handle into a ReadHandle
/// or a WriteHandle to gain more privelages.
/// 
template <typename Data>
class UT_COWHandle
{
public:
    /// Default constructor creates a null pointer.  To assign the pointer:
    /// @code
    ///		UT_COWHandle<Type>  handle;
    ///		handle.steal( new Type(...) );
    /// @endcode
    UT_COWHandle()
	: myData(),
	  myWriteCount(0)
    {
    }
    UT_COWHandle(const UT_COWHandle<Data> &src)
	: myData(),
	  myWriteCount(0)
    {
        *this = src;
    }

    virtual ~UT_COWHandle()
    {
        // Locks must have expired.
        UT_ASSERT(!myWriteCount.relaxedLoad());
    }

    const UT_COWHandle<Data> &operator=(const UT_COWHandle<Data> &src)
    {
        UT_ASSERT(!myWriteCount.relaxedLoad());
        // It's fine to make a new handle of something with an existing
        // write lock, as long as we release the handle before we
        // next try to access the write lock. This condition is checked
        // in the UT_COWWriteHandle cast functions.
        myData = src.myData;

	return *this;
    }

    /// Calls the default constructor on the object.
    void        allocate()
    {
        UT_ASSERT(!myWriteCount.relaxedLoad());
        myData = UTmakeShared<Data>();
    }

    /// Makes a copy of the source data.  This is the safest
    /// approach as the caller will retain ownership.
    /// @note This uses the Data copy constructor.
    void        copy(const Data *src)
    {
        UT_ASSERT(!myWriteCount.relaxedLoad());
        myData = UTmakeShared<Data>(*src);
    }

    /// Be careful when initializing using this function.  The
    /// handle will gain ownership of the provided pointer, so
    /// the caller should cease referring to it.
    void        steal(Data *src)
    {
        UT_ASSERT(!myWriteCount.relaxedLoad());
        myData.reset(src);
    }

    /// Turns us into a null pointer, a useful way to end our use
    /// of a handle without having to play weird scope games.
    void        resetHandle()
    {
        UT_ASSERT(!myWriteCount.relaxedLoad());
        myData.reset();
    }

    /// Returns the number of shared pointer references to the same Data
    int getRefCount() const
    { return myData.use_count(); }

private:
    void        makeUnique()
    {
        // Zero pointers are already unique
        if (!myData)
            return;

        if (!myData.unique())
        {
            myData = UTmakeShared<Data>(*myData);
        }
        // We are now unique!
    }

    UT_SharedPtr<Data>     myData;

    /// Tracks the number of outstanding writecounts referring to this
    /// data.
    /// This is only required for sanity-checking.
    mutable SYS_AtomicInt32     myWriteCount;

    /// Grant our friends direct access.
    template<typename OtherData> friend class UT_COWReadHandle;
    template<typename OtherData> friend class UT_COWTransientReadHandle;
    template<typename OtherData> friend class UT_COWWriteHandle;
};

///
/// Read Handles are copies of the original handle that have
/// their internal data exposed.  You are not allowed to edit them,
/// so many read handles may be sharing the same underlying data.
///
template <typename Data>
class UT_COWReadHandle
{
public:
    UT_COWReadHandle()
    {
        // Handle's default to null so this is fine.
    }

    ~UT_COWReadHandle()
    {
        // Natural destructor suffices.
    }

    UT_COWReadHandle(const UT_COWReadHandle<Data> &src)
    {
        myHandle = src.myHandle;
    }

    UT_COWReadHandle(const UT_COWHandle<Data> &src)
    {
        *this = src;
    }

    const UT_COWReadHandle<Data> &operator=(const UT_COWReadHandle<Data> &src)
    {
        myHandle = src.myHandle;
        return *this;
    }

    const UT_COWReadHandle<Data> &operator=(const UT_COWHandle<Data> &src)
    {
        myHandle = src;
        // It's fine to make a read lock of something with an existing
        // write lock, as long as we release the read lock before we
        // next try to access the write lock. This condition is checked
        // in the UT_COWWriteHandle cast functions.
        return *this;
    }

    /// Removes references, making this a null pointer
    void resetHandle()
    {
        myHandle.resetHandle();
    }

    /// Test if this is a NULL pointer
    bool	isNull() const
    {
	return !myHandle.myData;
    }

    const Data &operator*() const
    {
        return *myHandle.myData;
    }

    const Data *operator->() const
    {
        return myHandle.myData.get();
    }
    const Data *get() const
    {
        return myHandle.myData.get();
    }

    int getRefCount() const
    { return myHandle.getRefCount(); }

private:
    UT_COWHandle<Data>            myHandle;
};

///
/// Transient Read Handles are simply accessors to the internal data of
/// the original handle.  Unlike normal read handles, transient read
/// handles don't result in a tracked reference to the underlying data,
/// and so have lower overhead, but are unsafe to use outside the scope
/// of, or after any changes to, the original handle.  Write handles will
/// not be aware of any active transient read handles, so errors in this
/// usage will not trigger assertions.  In fact, no write handles to the
/// underlying data should exist during the entire scope of a transient
/// read handle.
/// 
/// You are not allowed to edit the underlying data, so many read handles
/// may be sharing the same underlying data.
///
template <typename Data>
class UT_COWTransientReadHandle
{
public:
    UT_COWTransientReadHandle()
	: myData(NULL)
    {
    }

    ~UT_COWTransientReadHandle()
    {
        // Natural destructor suffices.
    }

    UT_COWTransientReadHandle(const UT_COWTransientReadHandle<Data> &src)
	: myData(src.myData)
    {
    }

    UT_COWTransientReadHandle(const UT_COWHandle<Data> &src)
        : myData (src.myData.get())
    {
    }

    const UT_COWTransientReadHandle<Data> &operator=(
				    const UT_COWTransientReadHandle<Data> &src)
    {
	myData = src.myData;
        return *this;
    }

    const UT_COWTransientReadHandle<Data> &operator=(
				    const UT_COWHandle<Data> &src)
    {
	// We can't verify in the UT_COWWriteHandle cast functions that
	// transient read handles are released before we next try to
	// access the write lock, so we verify that they're not being
	// created while any write lock is active.
	UT_ASSERT(!src.myWriteCount.relaxedLoad());
	myData = src.myData.get();
        return *this;
    }

    /// Removes references, making this a null pointer
    void resetHandle()
    {
	myData = 0;
    }

    /// Test if this is a NULL pointer
    bool	isNull() const
    {
	return !myData;
    }

    const Data &operator*() const
    {
        return *myData;
    }

    const Data *operator->() const
    {
        return myData;
    }
    const Data *get() const
    {
        return myData;
    }

private:
    const Data			 *myData;
};

///
/// A write handle can be thought of as a pointer to the actual
/// data.  It allows you to modify the underlying data.  To do this,
/// it will unique the plain-handle's data so no outstanding
/// readlocks will be affected by the writes.
///
/// Building read handles while a write handle is active is valid
/// so long as no dereference occurs during the lifetime of the read
/// handle.
///
template <typename Data>
class UT_COWWriteHandle
{
public:
    UT_COWWriteHandle()
    {
        myHandle = 0;
    }

    ~UT_COWWriteHandle()
    {
        unlock();
    }

    UT_COWWriteHandle(const UT_COWWriteHandle<Data> &src)
    {
        myHandle = 0;
        *this = src;
    }

    UT_COWWriteHandle(UT_COWHandle<Data> *src)
    {
        myHandle = 0;
        *this = src;
    }

    /// Passing write handles by value is fine as the users understand
    /// they all share the same underlying data, so is responsible
    /// for thread safety, etc.
    const UT_COWWriteHandle<Data> &operator=(const UT_COWWriteHandle<Data> &src)
    {
        // Track our lock counts appropriately.
        src.lock();
        unlock();
        myHandle = src.myHandle;

        return *this;
    }

    /// Converting from a handle into a write handle can only be
    /// done once.  This prevents other threads or methods from
    /// invalidating our uniqueness property.
    /// Note we up case from a *pointer* to a UT_COWHandle, not from
    /// a UT_COWHandle.  This avoids people accidentally using an
    /// temporary as the source and underlines that write handles
    /// are like pointers while read handles are like values.
    const UT_COWWriteHandle<Data> &operator=(UT_COWHandle<Data> *src)
    {
        myHandle = src;

        if (src)
        {
            // Ensure we are the only copy.
            src->makeUnique();
        }

        lock();
	return *this;
    }

    /// Turns this into a null handle to drop references.
    void resetHandle()
    {
        unlock();
        myHandle = 0;
    }

    /// Test if this is a NULL pointer
    bool	isNull() const
    {
	return !myHandle || !myHandle->myData;
    }

    Data &operator*() const
    {
        if (myHandle)
        {
            // It is possible UT_COWReadHandles were created on
            // our handle. This is fine as long as they are
            // destroyed before we do our next write access.
            UT_ASSERT(myHandle->myData.unique());
            return *(myHandle->myData);
        }
        else
	{
	    // As much of a crash as returning zero,
	    // but the compiler is happier.
            // return *(0);
            return *(myHandle->myData);
	}
    }

    Data *operator->() const
    {
        if (myHandle)
        {
            // It is possible UT_COWReadHandles were created on
            // our handle. This is fine as long as they are
            // destroyed before we do our next write access.
            UT_ASSERT(myHandle->myData.unique());
	    return &*(myHandle->myData);
        }
        else
            return (Data *)0;
    }
    Data *get() const
    {
	if (myHandle)
	{
            UT_ASSERT(myHandle->myData.unique());
	    return &*(myHandle->myData);
	}
        return nullptr;
    }

    int getRefCount() const
    { return myHandle->getRefCount(); }

private:
    void        unlock() const
    {
        if (myHandle)
        {
            // Don't track reference counts on null arrays since
            // they are safe to share.
            if (!myHandle->myData)
                return;
            UT_ASSERT(myHandle->myWriteCount.relaxedLoad());
            if (myHandle->myWriteCount.relaxedLoad())
                myHandle->myWriteCount.add(-1);
        }
    }

    void        lock() const
    {
        if (myHandle)
        {
            // Don't track reference counts on null arrays since
            // they are safe to share.
            if (!myHandle->myData)
                return;
            myHandle->myWriteCount.add(1);
        }
    }

    UT_COWHandle<Data>    *myHandle;
};

#if defined( WIN32 ) || defined( LINUX ) || defined( MBSD ) || defined(GAMEOS)
    #include "UT_COW.C"
#endif


#endif

