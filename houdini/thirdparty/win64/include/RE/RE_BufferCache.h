/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_BufferCache.h ( RE Library, C++)
 *
 * COMMENTS:
 *	This cache contains GL buffers.
 */
#ifndef RE_BufferCache_h
#define RE_BufferCache_h

#include "RE_API.h"
#include "RE_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Cache.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ArrayStringMap.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>


class RE_API RE_BufferCache : public UT_Cache
{
    class UnusedBuffer;
    typedef UT_ArrayStringMap<RE_OGLBufferHandle> BufferPool;
    typedef UT_ArrayStringMap<UnusedBuffer *> UnusedBufferPool;

public:
    static	RE_BufferCache *getCache();

    /// Maximum cache size, in bytes
    int64	getMaxSizeB() const { return myMaxSizeB; }

    /// Set the maximum size of the cache, in bytes
    void	setMaxSizeB(int64 maxsize);
   
    /// Current cache size, in bytes
    int64	getCurSizeB() const { return myCurSizeB; }

    /// Put a buffer in the cache. Will replace any buffer with the same name.
    void	cacheBuffer(const UT_StringHolder &name,
			    const RE_OGLBufferHandle &buf);

    /// Return a cached buffer in 'bufh' and true, or false if not found.
    bool	findBuffer(const UT_StringRef &name,
			   RE_OGLBufferHandle &bufh);

    /// Move item to the used list from unused. Returns false if not found.
    bool	moveToUsed(const UT_StringRef &name);

    /// Move item to the unused list from used. Returns false if not found.
    bool	moveToUnused(const UT_StringRef &name);

    /// Remove a buffer from the cache. Returns true if it existed in the cache
    bool	removeBuffer(const UT_StringRef &name);

    /// call if a cached buffer changes size, with the change in size.
    void	updateBufferSize(int64 delta_size)
		    {
			UT_ASSERT(myCurSizeB + delta_size >= 0);
			myCurSizeB = SYSmax(0, myCurSizeB+delta_size);
		    }


    /// Explicitly prune the cache if beyond its max size. This does not
    /// happen automatically.
    void	pruneCache();

    /// Explicitly prune all unused buffer.
    void	pruneUnusedBuffers();

    /// Debug dump of the cache contents.
    void	dumpCache();
protected:
		RE_BufferCache(int64 maxsize);
	       ~RE_BufferCache();

    // UT_Cache interface.
    virtual const char *utGetCacheName() const { return "OpenGL Vertex Cache"; }
    virtual int64	utGetCurrentSize() const { return myCurSizeB; }
    virtual int64	utReduceCacheSizeBy(int64 amount);
  
    virtual bool	utHasMaxSize() const { return true; }
    virtual int64       utGetMaxSize() const { return myMaxSizeB; }
    virtual void	utSetMaxSize(int64 size) { setMaxSizeB(size); }

    virtual int64       utGetMinSize() const
			    { return myCurSizeB - myUnusedSizeB; }

private:
    void		addToUnusedBufferList(const UT_StringHolder &key,
					      RE_OGLBufferHandle &bh);

    void		removeFromUnusedBufferList(const UT_StringRef &key,
						   UnusedBuffer *buf);
    
    BufferPool	     myUsedBuffers;
    UnusedBufferPool myUnusedBuffers;
    UnusedBuffer    *myLRUBufferList;
    UnusedBuffer    *myLRUBufferTail;
    
    int64	  myMaxSizeB;
    int64	  myCurSizeB;
    int64	  myUnusedSizeB;
};

#endif
