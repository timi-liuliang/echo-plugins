/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_CachedObject.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Base class for objects that can be cached in the RE_TextureCache and
 *	RE_BufferCache..
 */
#ifndef RE_CachedObject_h
#define RE_CachedObject_h

#include "RE_API.h"

#include <UT/UT_Assert.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_DeepString.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_StackTrace.h>
#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>

#include <iostream>
#include <stddef.h>


/// Simple class for a mutli-integer cache tag.
class RE_API RE_CacheVersion
{
public:
    RE_CacheVersion()
    {
	myVersion[0] = 0;
	myVersion[1] = 0;
	myVersion[2] = 0;
	myVersion[3] = 0;
    }
    RE_CacheVersion(const RE_CacheVersion &cv)
    {
	myVersion[0] = cv.myVersion[0];
	myVersion[1] = cv.myVersion[1];
	myVersion[2] = cv.myVersion[2];
	myVersion[3] = cv.myVersion[3];
    }
    
    RE_CacheVersion &operator=(const RE_CacheVersion &cv)
    {
	myVersion[0] = cv.myVersion[0];
	myVersion[1] = cv.myVersion[1];
	myVersion[2] = cv.myVersion[2];
	myVersion[3] = cv.myVersion[3];
	return *this;
    }
    bool operator==(const RE_CacheVersion &cv) const
    {
	return (myVersion[0] == cv.myVersion[0] &&
		myVersion[1] == cv.myVersion[1] &&
		myVersion[2] == cv.myVersion[2] &&
		myVersion[3] == cv.myVersion[3]);
    }
    bool operator!=(const RE_CacheVersion &cv) const
    {
	return (myVersion[0] != cv.myVersion[0] ||
		myVersion[1] != cv.myVersion[1] ||
		myVersion[2] != cv.myVersion[2] ||
		myVersion[3] != cv.myVersion[3]);
    }
    RE_CacheVersion &operator++()
    {
	++myVersion[0];
	if(myVersion[0] < 0)
	{
	    myVersion[0]=0;
	    ++myVersion[1];
	    if(myVersion[1] < 0)
	    {
		myVersion[1]=0;
		++myVersion[2];
		if(myVersion[2] < 0)
		{
		    myVersion[2]=0;
		    ++myVersion[3];
		}
	    }
	}
	return *this;
    }

    void setElement(int i, int64 v)
    {
	UT_ASSERT(i>=0 && i<=3);
	myVersion[i] = v;
    }
    int64 getElement(int i) const
    {
	UT_ASSERT(i>=0 && i<=3);
	return myVersion[i];
    }

    void setElementLow32(int i, int32 v)
    {
	UT_ASSERT(i>=0 && i<=3);
	myVersion[i] =
	    int64( (uint64(myVersion[i])&(uint64(0xFFFFFFFF)<<uint64(32)))
		  | uint64(v) );
    }
    void setElementHigh32(int i, int32 v)
    {
	UT_ASSERT(i>=0 && i<=3);
	myVersion[i] = int64((uint64(myVersion[i])&uint64(0xFFFFFFFF))
			     | (uint64(v)<<uint64(32)));
    }
    void clear()
    {
	myVersion[0] = 0;
	myVersion[1] = 0;
	myVersion[2] = 0;
	myVersion[3] = 0;
    }

    std::ostream &operator<<(std::ostream &os) const
    {
	os << myVersion[0] << "," << myVersion[1] << "," << myVersion[2] << ","
	   << myVersion[3];
	return os;
    }

private:
    /// I/O friends
    // @{
    friend std::ostream	&operator<<(std::ostream &os, const RE_CacheVersion &v)
    {
	os << v.myVersion[0] << "," << v.myVersion[1] << ","
	   << v.myVersion[2] << "," << v.myVersion[3];
	return os;
    }
    // @}

private:
    int64  myVersion[4];
};

// ----------------------------------------------------------------------
/// This class is bumped whenever a cached item is evicted.
class RE_API RE_CacheTag : public UT_NonCopyable
{
public:
			RE_CacheTag() : myVersion(0), myRefCount(0) {}

    void		bump()  { myVersion++; }
    void		reset() { myVersion=0; }
    int64		getVersion() const { return myVersion; }

    void		incref() { myRefCount.add(1); }
    void		decref()
			{
			    if (!myRefCount.add(-1))
				delete this;
			}
private:
    int64		myVersion;
    SYS_AtomicInt32	myRefCount;
};
typedef UT_IntrusivePtr<RE_CacheTag> RE_CacheTagHandle;

// -------------------------------------------------------------------------
// Derive from this class to stash data in a cached object.
class RE_API RE_CachedExtraData
{
public:
	     RE_CachedExtraData();

    virtual const char *className() const { return "RE_CachedExtraData"; }

    // Called when the parent object is removed from the cache.
    virtual void cacheFree() {}

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_CachedExtraData.
    /// If inclusive, include sizeof(*this), else only count
    /// other memory owned by this.
    virtual int64 getMemoryUsage(bool inclusive) const = 0;
    
    void		incref() { myRefCount++; }
    void		decref()
			{
			    myRefCount--;
			    if(myRefCount == 0)
				delete this;
			}
    int			refCount() const { return myRefCount; }
protected:
    virtual ~RE_CachedExtraData();
private:
    int myRefCount;
};
typedef UT_IntrusivePtr<RE_CachedExtraData> RE_CachedExtraDataHandle;



// ------------------------------------------------------------------------
/// Basic cached object, with version and extra data only.
class RE_API RE_CachedObjectBase 
{
public:
			RE_CachedObjectBase();
    virtual	       ~RE_CachedObjectBase();
    
    void		setVersion(RE_CacheVersion v){ myVersion = v; }
    RE_CacheVersion	getVersion() const	     { return myVersion; }

    // Add extra data to be cached along with the object (derive from the class
    // below). If 'give_own' is true, this will be deleted when the cached
    // object is, otherwise only 'cacheFree()' will be called.
    void		setExtraData(RE_CachedExtraData *data)
						         { myExtraData = data; }
    const RE_CachedExtraDataHandle &getExtraData() const { return myExtraData; }


    // Cache tags notify other interested parties that we are deleted.
    void		setCacheTag(RE_CacheTagHandle h)
			    { myCacheTagHandle = h; }
    void		resetCacheTag()
			    { myCacheTagHandle = nullptr; }
    
    void		setInCache(bool c)
			    {
				myCachedFlag = c;
				if(!c && myExtraData)
				    myExtraData->cacheFree();
			    }
    bool		isInCache() const { return myCachedFlag; }
    
    /// Returns the amount of main memory (not VRAM) owned by this object.
    virtual int64 getMemoryUsage(bool inclusive) const;

public:
    RE_CacheVersion	myVersion;
    RE_CacheTagHandle   myCacheTagHandle;
    bool		myCachedFlag;
    
    RE_CachedExtraDataHandle myExtraData;
};



// ----------------------------------------------------------------------
/// Cached object implementation for RE_TextureCache.
class RE_API RE_CachedObject : public RE_CachedObjectBase
{
public:
			RE_CachedObject();
    virtual	       ~RE_CachedObject();

    // If this object is being actively used by GL
    void		setInUse(bool inc);
    virtual bool	inUse()			    { return myUsage > 0; }
    int			getUsage() const	    { return myUsage; }

    // How many objects currently point to this object. Decrementing to zero
    // does not delete the object. Handle object will not delete this object if
    // it is still referenced by another handle.
    void		incRef()		    { myRefCount++; }
    void		decRef()		    { myRefCount--; }
    int			getRefCount() const	    { return myRefCount; }

    void		setCached(bool cached,const char *mapname);
    bool		isCached() const	    { return myCachedFlag; }
    
    const char	       *getName() const		    { return myName; }

    // Add a removal callback to this object, to be notified when this object
    // is about to be deleted from the cache.
    bool	setRemovalCallback(bool (*relCB)(void *, void *),
				   void *relObject);

    bool	clearRemovalCallback(void *relObject);

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_CachedObject.
    virtual int64 getMemoryUsage(bool inclusive) const;

private:
    int			myUsage;
    int			myRefCount;
    UT_DeepString	myName;
};


class RE_API RE_BBoxData : public RE_CachedExtraData
{
public:
	     RE_BBoxData(const UT_BoundingBox &bbox) : myBBox(bbox) {}
    virtual ~RE_BBoxData() {}

    virtual const char *className() const { return "RE_BBoxData"; }
    
    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_BBoxData.
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

    UT_BoundingBoxD myBBox;
};


inline void
RE_CachedObject::setInUse(bool inc)
{
    myUsage += inc ? 1 : -1;
#ifdef UT_DEBUG
    if(myUsage < 0)
	std::cerr << "Bad object usage " << myUsage << " " << this << "\n";
    else if(myUsage == 32767)
	std::cerr << "Apparent overflow " << myUsage << " " << this << "\n";
#endif
    UT_ASSERT_MSG(myUsage >= 0, "Underflow");
    UT_ASSERT_MSG(myUsage < 32767, "Overflow");
    if(myUsage < 0)
	myUsage = 0;
}

RE_API size_t format(char *buf, size_t bufsize, const RE_CacheVersion &v);

static inline void intrusive_ptr_add_ref(RE_CacheTag *i) { i->incref(); }
static inline void intrusive_ptr_release(RE_CacheTag *i) { i->decref(); }
static inline void intrusive_ptr_add_ref(RE_CachedExtraData *i) { i->incref(); }
static inline void intrusive_ptr_release(RE_CachedExtraData *i) { i->decref(); }

#endif
