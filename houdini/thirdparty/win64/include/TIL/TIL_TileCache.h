/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_TileCache.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Flat, priority based TIL_Tile cache.
 */
#ifndef TIL_TILE_CACHE_H
#define TIL_TILE_CACHE_H

#include "TIL_API.h"
#include "TIL_Defines.h"
#include "TIL_Tile.h"
#include "TIL_PriorityQueue.h"

#include <UT/UT_Cache.h>
#include <UT/UT_Map.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_StopWatch.h>
#include <UT/UT_Lock.h>
#include <SYS/SYS_Hash.h>
#include <iosfwd>

class UT_TokenString;
class til_ResourceSlush;
class TIL_Plane;
class TIL_Tile;
class TIL_TileManager;
class TIL_NodePlaneId;
class UI_Timer;

typedef bool (*TIL_IsPlaneCheckpointedCallbackType)(void *opaque_cop_node,
						    const char *plane_name,
						    float cook_time);

class TIL_API TIL_TileCache : public UT_Cache
{
public:
     TIL_TileCache(TIL_TileManager *tman);
    ~TIL_TileCache();

    // Sets the cache size and initializes if required.
    
    void	 setCacheSize(int size_in_kb, bool force_redo = false);
    void	 setInactiveCacheReduce(bool enable, int size_in_kb);

    // used for borrowing memory from the cache
    inline void	 setExternalResourceUsage(int64	 bytes_used)
		 { myExternalResourceUsage = bytes_used; }



    // Set when cooking occurs, and reset afterwards. Used when the Inactive
    // cache size is enabled.
    void	 setCookMode(bool cook);

    // Clears tiles out of the cache.
    
    void	 clear();

    // CACHE LOCKING 
    inline void	 lockCache() { TIL_LOCK(myCacheLock); }
    inline void	 unlockCache() { myCacheLock.unlock(); }


    // CACHE LOOKUP
    // Performa a cache search for a tile, with possible new tile allocation.
    // If not found, and create_flag is true, a new tile is allocated and
    // 'created' is set to true. NULL may be returned if not found and
    // create_flag is false, or the tile is blocked.
    // If the tile is returned or blocked, the token parm is deleted & zeroed.
    // Created tiles are returned write locked.
    // Cached tiles are returned read locked.

    TIL_Tile	*getCachedTile(UT_TokenString   *&token,
			       int		x,
			       int		y,
			       int		xsize,
			       int		ysize,
			       void		*node,
			       const TIL_Plane	*plane,
			       int		array_index,
			       int		component,
			       int		image_index,
			       bool		create_flag,
			       bool 	       &blocked,
			       bool	       &created,
			       TIL_Storage	storage);

    // creates an empty tile, read-only, which can be used to specify a tile
    // when one really doesn't exist and one is needed.
    TIL_Tile	*getBlackTile(UT_TokenString   *&token,
			      int		x,
			      int		y,
			      int		xsize,
			      int		ysize,
			      void		*node,
			      const TIL_Plane	*plane,
			      int		array_index,
			      int		component,
			      int		image_index);

    // This method quickly queries to see if a tile is in the cache.
    bool	 isTileInCache(const UT_TokenString	*token,
			       int			 x,
			       int			 y);

    bool	 areTilesInCache(const UT_TokenString *token, int x1,int y1,
				 int x2,int y2);
    
    // locks or unlocks a given tile with Rd or Wr access.
    inline int	 lockTile  (TIL_Tile *tile, int access, int block = 1)
			{ return privLockTile(tile, access, block, false); }
    
    inline void	 unlockTile(TIL_Tile *tile, int access)
			{ privUnlockTile(tile,access, false); }
    
    inline void	 lockTileWriteToRead(TIL_Tile *tile, int score)
			{ privLockTileWriteToRead(tile,score, false); }
    
    inline int		 isReadLocked(const TIL_Tile *t) const
				{ return t->myReadLocks; }
    inline int		 isWriteLocked(const TIL_Tile *t) const
				{ return t->myWriteLock; }

    // releases a tile (freeing it from write lock or 1 read lock)
    void	 releaseTile(TIL_Tile *tile);
    
    // frees a tile(usually due to a cooking error)
    // if write locked, deallocates.
    // if read locked, calls releaseTile().
    void	 freeTile(TIL_Tile *&tile);

    // Moves a tile from the regular cache into the list of checkpointed tiles.
    void	 moveTileFromCacheToCheckpointList(
					const UT_TokenString	*token,
					int			 xstart,
					int			 ystart,
					int			 xend,
					int			 yend,
					UT_ValArray<TIL_Tile *>	&tile_list);

    // Decrement the checkpoint reference count for a tile, moving it back
    // into the cache if the count goes to zero.
    void 	 decrementTileCheckpointCount(UT_ValArray<TIL_Tile *>&tile_list);

    // if we went over the cache size when checkpointing, this gets called once
    // the engine uncheckpoints a bunch of tiles, so we can resume trimming
    // if there's now enough non-checkpointed data to trim.
    void	 possiblyResumeTrimming();

    // removes all tiles that are belong to the passed node ptr. o(n)
    void	 removeAllWithParent(void *node);

    unsigned	 getNumCheckpointedTiles() const
				    { return myCheckpointTiles.entries(); }

    unsigned int getNumLockedTiles() const
				    { return myLockedTiles.entries(); }

    // these methods need to be called if the cook was interrupted.
    void	 uncheckpointAllTiles();
    void	 unlockAllTiles();
    
    // Returns the cache size, in bytes.
    int64	 getCacheSize() const	  { return myCacheSize; }
    int64	 getMaxCacheSize() const { return myMaxCacheSize; }
    int64	 getInactiveCacheSize() const { return myMaxInactiveSize; }
    bool	 getInactiveReduce() const { return myInactiveReduce; }

    // These methods allow you to get a free, empty tile. Used by TIL_Region
    // to build a TIL_TileList out of itself. Always call returnTile to
    // return this tile to the cache when done.
    TIL_Tile	*getTempTile();
    void	 returnTile(TIL_Tile *tile);

    static void	cacheReducePing(UI_Timer *t, void *data);

    static TIL_TileCache *getTileCache();
    
     // debug: dumps the current contents of the cache.
    void	 dumpContents(std::ostream &os);

protected:
     // the UT_Cache interface
    const char *utGetCacheName() const override { return "COP Cook Cache"; }
    int64	utGetCurrentSize() const override { return getCacheSize(); }
    bool	utHasMaxSize() const override { return true; }
    int64	utGetMaxSize() const override { return getMaxCacheSize(); }
    void	utSetMaxSize(int64 size) override; 
    int64	utReduceCacheSizeBy(int64 amount) override;
   
private:
    // *** WARNING The following must be called with the cache write locked.
    
    TIL_Tile	*getFreeTile();
    TIL_Tile	*allocateTile(TIL_DataFormat	tile_type,
			      void	       *parent,
			      TIL_Storage	storage,
			      const UT_TokenString *token,
			      til_AlignedBlock	*usemem);
    void	 deallocateTile(TIL_Tile *tile);

    bool	 returnTileFromLocked(TIL_Tile *tile);
    void	 makeRoomFor(int64 newmemsize);
    
    void	 destroy();
    void	 freeAllLocks(TIL_Tile *tile);

    int	 	 privLockTile(TIL_Tile *tile, int access, int block,
			      bool already_locked);
    void	 privUnlockTile(TIL_Tile *tile, int access,
				bool already_locked);
    void	 privLockTileWriteToRead(TIL_Tile *tile, int score,
					 bool already_locked);
    

    void	 reduceCacheSize();
    // *** WARNING END
    
private:
    class TileKey
    {
    public:
	TileKey(TIL_Tile *tile);
	TileKey(const UT_TokenString *string, int x, int y);
	
	bool operator==(const TileKey &other) const; 
	bool operator!=(const TileKey &other) const;
	
	SYS_HashType hash() const;
	
	struct Hasher
	{
	    SYS_HashType operator()(const TileKey &key) const 
	    { return key.hash(); }
	};
	
    private:
	int myOffX, myOffY;
	const UT_TokenString *myString;
    };
    
    // Tile queues.
    TIL_PriorityQueue	myCachedTiles;
    TIL_HoldingQueue	myCheckpointTiles;
    TIL_HoldingQueue	myLockedTiles;
    TIL_HoldingQueue	myFreeTiles;

    // Tile hash table.
    UT_Map<TileKey, TIL_Tile *, TileKey::Hasher> myTileTable;

    int64		myCacheSize;
    int64		myMaxCacheSize;
    int64		myMaxInactiveSize;
    int64		myExternalResourceUsage;
    int			myNumTiles;
    
    TIL_FastLock		myCacheLock;

    // Allocation saving caches
    UT_ValArray<TIL_Tile *> myTileBlocks;
    til_ResourceSlush	*my8BitSlush;
    til_ResourceSlush	*my16BitSlush;
    til_ResourceSlush	*my32BitSlush;

    TIL_TileManager	*myTileManager;
    int			 myCookMode;
    bool		 myInactiveReduce;
    int			 mySuspendCacheTrimCount;
    UT_StopWatch	 myInactiveTimer;
};



#endif
