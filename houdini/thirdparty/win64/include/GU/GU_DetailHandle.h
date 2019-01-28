/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_DetailHandle.h ( GU Library, C++)
 *
 * COMMENTS:	A detail handle is used to maintain a reference to a 
 *		GU_Detail.  It provides a level of indirection so that
 *		the underlying GU_Detail may be deleted or changed
 *		without those interested in the GU_Detail having to tell.
 *		It also provides write/read lock tests to allow memory
 *		managers to tell what is safe to be deleted.
 */

#ifndef __GU_DetailHandle__
#define __GU_DetailHandle__

#include "GU_API.h"
#include <SYS/SYS_Math.h>
#include <UT/UT_NonCopyable.h>
#include <GA/GA_Types.h>

class GU_Detail;
class GU_DetailHandleRef;

/// A GU_DetailHandle is designed to allow arms length references to 
/// GU_Details.  The underlying GU_Detail can be deleted or swapped
/// and things which use the GU_DetailHandle will properly be notified.
/// Further, it provides access control to the GU_Detail, ensuring multiple
/// entities do not write to it at once.  This is currently used to ensure
/// deletion is not performed on active GU_Details.  Eventually it could
/// be used for multi-threaded support.
/// NOTE: The locks are not enforced.
class GU_API GU_DetailHandle
{
public:
    GU_DetailHandle();
    GU_DetailHandle(const GU_DetailHandle &handle);
    GU_DetailHandle(GU_DetailHandle &&handle);

    ~GU_DetailHandle();

    void		 clear();

    /// This method will duplicate the geometry associated with the handle,
    /// returning a new handle.
    GU_DetailHandle	 duplicateGeometry(GA_DataIdStrategy data_id_strategy = GA_DATA_ID_BUMP) const;

    GU_DetailHandle	&operator=(const GU_DetailHandle &handle);
    GU_DetailHandle	&operator=(GU_DetailHandle &&handle);

    bool		 operator==(const GU_DetailHandle &handle) const;
    bool		 operator!=(const GU_DetailHandle &handle) const;

    /// Compute a hash based on myHandle.  This is not on the contents
    /// of the gdp!
    uint		 hash() const	{ return SYSpointerHash(myHandle); }


    /// Ensures this is the only handle referring to the underlying
    /// geometry, ie, getRefCount() == 1.  A no-op if the geometry
    /// is already unique, otherwise invokes duplicateGeometry().
    void		 makeUnique(GA_DataIdStrategy data_id_strategy = GA_DATA_ID_BUMP);
    bool		 isUnique() const { return getRefCount() <= 1; }

    /// Unlocked readable & writeable pointers.
    /// If acquiring a write pointer, it is expected one will ensure uniqueness
    /// prior to acquiring it.
    GU_Detail		*gdpNC();
    const GU_Detail	*gdp() const;

    /// peekDetail does no locking.  Thus, it should be avoided unless
    /// the goal is just to test if the GU_Detail is present.  This
    /// is largely provided for supporting old code in hackish situations.
    const GU_Detail	*peekDetail() const;

    /// This will acquire a read lock on the GU_Detail.  The result is 0
    /// if no lock can be acquired, or no underlying detail exists.
    /// The returned detail should be passed to unlock().  (Technically,
    /// no locking occurs on error, but unlock is a no-op with a 0 gdp)
    const GU_Detail	*readLock();

    /// This will acquire a write lock on the GU_Detail.  The result is 0
    /// if no lock can be acquired, or no underlying detail is present.
    /// The returned detail should be passed to unlock() when finished.
    /// There can only be one active write lock at a time.  Do not destroy
    /// or modify the pointer itself and expect the unlock to update
    /// the GU_Detail *.  Instead, use one of the delete or change functions.
    /// No readlocks or write locks can be active for this to work.
    GU_Detail		*writeLock();

    /// This will unlock one layer of locking.  
    /// If the passed in gdp is null, no unlocking occurs.
    /// Otherwise, the passed in gdp is asserted to match the one
    /// which myHandle points to.
    void		 unlock(const GU_Detail *gdp);

    /// Create a new underlying reference & set it to point to the
    /// given gdp.
    /// The own flag determines if the GU_Detail will be deleted by
    /// the handle system when the underlying reference is deleted.
    void		 allocateAndSet(GU_Detail *gdp, bool own=true);

    /// Deletes the underlying gdp and sets the gdp pointer to be null.
    /// It returns true if the gdp was successfully deleted, false if not.
    /// Failure likely results from a lock being present.
    bool		 deleteGdp();

    /// Determines if anyone has a lock on this handle.
    bool		 hasActiveLock() const;

    /// Determine if this is a null handle.  This means it
    /// points to a null handle, or the handle's gdp is null.
    bool		 isNull() const;

    /// Determine if this is a valid handle (!isNull())
    bool		 isValid() const	{ return !isNull(); }

    /// Safe-bool operator to return whether the handle is valid
    SYS_SAFE_BOOL	operator bool() const { return isValid(); }

    /// Returns the number of references made to the base handle.
    int			 getRefCount() const;

    /// Returns the number of locks made to the base handle for debugging.
    int			 getLockCount() const;

    /// Preserve Requests:
    /// A preserve request is a request that people create a new
    /// GU_DetailHandle rather than editting the current one.  It
    /// is used by SOPs to determine if it is safe to do an in place
    /// cook.
    void		 addPreserveRequest();
    void		 removePreserveRequest();
    int			 getPreserveRequest() const;

    /// External references should not block unload requests at the SOP
    /// level.  Normally if any external GU_DetailHandles still refer
    /// to the handle, unloading is prevented.
    void		 addExternalReference();
    void		 removeExternalReference();
    int			 getExternalReference() const;

    /// Return the amount of memory owned by the GU_DetailHandle itself,
    /// *NOT* the detail.  This will count the memory of the GU_DetailHandleRef,
    /// which could be shared, so only the definitive "original" handle
    /// should be counted, e.g. the one on SOP_Node.
    int64		 getMemoryUsage(bool inclusive) const;

private:
    /// This changes the underlying gdp of the handle.  The old
    /// gdp is returned.  This fails if there are any locks on the gdp, and so
    /// it should never be called.
    GU_Detail		*setGdp(GU_Detail *gdp);

    GU_DetailHandleRef	*myHandle;
};

/// A GU_ConstDetailHandle uses a GU_DetailHandle to provide const-only
/// access to a GU_Detail. It uses a GU_DetailHandle member variable to
/// do all the actual work, and simply hides any functionality that is
/// unsafe or inappropriate for a const GU_Detail.
class GU_API GU_ConstDetailHandle
{
public:
    GU_ConstDetailHandle()
    { }
    GU_ConstDetailHandle(const GU_DetailHandle &handle)
    { myDetailHandle = handle; }

    const GU_ConstDetailHandle &operator=(const GU_DetailHandle &handle)
    {
	myDetailHandle = handle;
	return *this;
    }

    bool		 operator==(const GU_ConstDetailHandle &handle) const
    { return myDetailHandle == handle.myDetailHandle; }
    bool		 operator!=(const GU_ConstDetailHandle &handle) const
    { return myDetailHandle != handle.myDetailHandle; }

    uint		 hash() const	{ return myDetailHandle.hash(); }

    void                 clear()
                            { myDetailHandle.clear(); }

    /// This method casts from a GU_ConstDetailHandle to a GU_DetailHandle.
    /// Use it carefully.  Calling getWriteableCopy() is preferred.
    GU_DetailHandle castAwayConst() const
    { return myDetailHandle; }

    /// Create a writeable copy of the detail
    GU_DetailHandle	getWriteableCopy() const
    { return myDetailHandle.duplicateGeometry(); }

    /// Unlocked readable pointers.
    const GU_Detail	*gdp() const { return myDetailHandle.gdp(); }

    /// This will acquire a read lock on the GU_Detail.  The result is 0
    /// if no lock can be acquired, or no underlying detail exists.
    /// The returned detail should be passed to unlock().  (Technically,
    /// no locking occurs on error, but unlock is a no-op with a 0 gdp)
    const GU_Detail	*readLock()
    { return myDetailHandle.readLock(); }

    /// This will unlock one layer of locking.  
    /// If the passed in gdp is null, no unlocking occurs.
    /// Otherwise, the passed in gdp is asserted to match the one
    /// which myHandle points to.
    void		 unlock(const GU_Detail *gdp)
    { myDetailHandle.unlock(gdp); }

    /// Determine if this is a null handle.  This means it
    /// points to a null handle, or the handle's gdp is null.
    bool		 isNull() const
    { return myDetailHandle.isNull(); }
    /// Check if this is a valid handle.
    bool		 isValid() const
    { return myDetailHandle.isValid(); }

    /// Safe-bool operator to return whether the handle is valid
    SYS_SAFE_BOOL	operator bool() const { return isValid(); }

    /// Returns the number of references made to the base handle.
    int			 getRefCount() const
    { return myDetailHandle.getRefCount(); }

    /// Returns the number of locks made to the base handle for debugging.
    int			 getLockCount() const
    { return myDetailHandle.getLockCount(); }

    /// Preserve Requests:
    /// A preserve request is a request that people create a new
    /// GU_DetailHandle rather than editting the current one.  It
    /// is used by SOPs to determine if it is safe to do an in place
    /// cook.
    void		 addPreserveRequest()
			    { myDetailHandle.addPreserveRequest(); }
    void		 removePreserveRequest()
			    { myDetailHandle.removePreserveRequest(); }
    int			 getPreserveRequest() const
			    { return myDetailHandle.getPreserveRequest(); }

    /// Return the amount of memory owned by the GU_DetailHandle itself,
    /// *NOT* the detail.  This will count the memory of the GU_DetailHandleRef,
    /// which could be shared, so only the definitive "original" handle
    /// should be counted, e.g. the one on SOP_Node.
    int64		 getMemoryUsage(bool inclusive) const
                            { return myDetailHandle.getMemoryUsage(inclusive); }

private:
    GU_DetailHandle	 myDetailHandle;
};

/// GU_DetalHandleAutoFOOLock is a utility class to allow the easy extraction
/// of const GU_Detail * from GU_DetailHandles.  It uses the C++ scoping
/// mechanism to make the locking & unlocking transparent to the caller.
/// One can thus do:
///     GU_DetailHandleAutoFOOLock	 gdl(sop->getCookedGeo())
///     const GU_Detail			*gdp = gdl.getGdp();
/// The FOO = Read returns const GU_Details and does a read lock.
/// The FOO = Write returns non-const GU_Details and does a write lock.
///
class
GU_DetailHandleAutoReadLock : public UT_NonCopyable
{
public:
    GU_DetailHandleAutoReadLock(const GU_ConstDetailHandle &handle)
    {
	myHandle = handle;
	myGdp = myHandle.readLock();
    }
    GU_DetailHandleAutoReadLock(const GU_DetailHandle &handle)
    {
	myHandle = handle;
	myGdp = myHandle.readLock();
    }
    ~GU_DetailHandleAutoReadLock()
    {
	myHandle.unlock(myGdp);
    }
    
    bool			 isValid() const { return myGdp != 0; }
    const GU_Detail		*getGdp() const { return myGdp; }
    const GU_ConstDetailHandle	&handle() const { return myHandle; }
    const GU_Detail		*operator->() const { return myGdp; }
    const GU_Detail		&operator*() const { UT_ASSERT_P(myGdp); return *myGdp; }

    /// Safe-bool operator to return whether the handle is valid
    SYS_SAFE_BOOL	operator bool() const { return isValid(); }

private:
    GU_ConstDetailHandle	 myHandle;
    const GU_Detail		*myGdp;
};

class
GU_DetailHandleAutoWriteLock : public UT_NonCopyable
{
public:
    GU_DetailHandleAutoWriteLock(const GU_DetailHandle &handle)
    {
	myHandle = handle;
	myGdp = myHandle.writeLock();
    }
    ~GU_DetailHandleAutoWriteLock()
    {
	myHandle.unlock(myGdp);
    }
    
    bool			 isValid() const { return myGdp != 0; }
    GU_Detail			*getGdp() const { return myGdp; }
    GU_Detail			*operator->() const { return myGdp; }
    GU_Detail			&operator*() const { UT_ASSERT_P(myGdp); return *myGdp; }
    const GU_DetailHandle	&handle() const { return myHandle; }

    /// Safe-bool operator to return whether the handle is valid
    SYS_SAFE_BOOL	operator bool() const { return isValid(); }

private:
    GU_DetailHandle		 myHandle;
    GU_Detail			*myGdp;
};

#endif
