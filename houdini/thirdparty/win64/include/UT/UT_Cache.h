/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Cache.h ( UT Library, C++)
 *
 * COMMENTS:
 *	An interface to a cache within Houdini. This is used so that the cache
 *    manager can monitor and manage the caches.
 *
 */
#ifndef UT_CACHE_H
#define UT_CACHE_H

#include "UT_API.h"
#include "UT_ValArray.h"


/// Common base class for various caches
class UT_API UT_Cache
{
public:
		 UT_Cache();
    virtual	~UT_Cache();

    /// clear this cache. May not entirely clear the cache if some items are in
    /// use (depending on the cache)
    void	utClearCache();

    /// required - return the english name for this cache.
    virtual const char *utGetCacheName() const = 0;
    
    /// required - return the current cache size, in bytes
    virtual int64	utGetCurrentSize() const = 0;

    /// required - free contents of the cache by @c amount bytes. Returns the
    /// amount of memory (in bytes) actually freed.
    /// This does @b not change the cache size
    virtual int64	utReduceCacheSizeBy(int64 amount) = 0;

    /// optional - override if the cache has a well defined maximum size
    virtual bool	utHasMaxSize() const { return false; }
    virtual int64       utGetMaxSize() const { return 0; }
    virtual void	utSetMaxSize(int64) { }

    /// optional - override if the cache has a well defined minimum size (this
    /// doesn't include a current min size imposed by items locked in the cache)
    virtual bool	utHasMinSize() const { return false; }
    virtual int64       utGetMinSize() const { return 0; }
    virtual void	utSetMinSize(int64) { }

    /// override in case a method needs to be run to get the most up-to-date
    /// cache information. Return true if it has changed.
    virtual bool	utUpdateCacheInfo() { return false; } 

    /// get the list of caches
    static const UT_ValArray<UT_Cache *> &utGetCacheList();

    /// callback to be called when a cache is added or deleted.
    static void  setCacheAddRemoveCB(void (*callback)(void *), void *data);

    /// this method attempts to free up 'amount' bytes from all the caches.
    static int64 utClearSpaceFromCaches(int64 amount);
};

#endif
