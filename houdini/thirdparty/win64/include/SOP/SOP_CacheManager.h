/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_CacheManager.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_CacheManager__
#define __SOP_CacheManager__

#include "SOP_API.h"

#include <UT/UT_SymbolTable.h>
#include <UT/UT_ValArray.h>

class SOP_CacheData;
class SOP_Node;
class CMD_Args;

enum SOP_CacheUnloadState
{
    UNLOAD_NEVER,
    UNLOAD_FLAG,
    UNLOAD_ALWAYS
};

enum SOP_CacheMemoryState
{
    MEMORY_NEVER,
    MEMORY_ALWAYS
};

class SOP_BaseCache;
class sop_checkpoint;

/// The SOP_CacheManager tracks all active SOPs and what their current
/// memory usage is.
///
/// It can then examine the network to determine if they are cullable.
/// It can also watch memory usage to see what SOPs should be removed
/// when.
class SOP_API SOP_CacheManager
{
public:
     SOP_CacheManager();
    ~SOP_CacheManager();

    static SOP_CacheManager	*getManager();

    /// When a new SOP is created we alert the cache manager through this.
    void			 registerSOP(SOP_Node *node);

    /// When a SOP is destroyed, we alert the cache manager through this.
    void			 unregisterSOP(SOP_Node *node);

    /// Returns whether we think the given SOP is loaded in memory or not.
    bool			 isLoaded(const SOP_Node *node);

    /// When a SOP locks another SOP, this is used.
    void			 alertLockSOP(SOP_Node *locker, 
					      SOP_Node *lockee);

    /// When a SOP is unlocked, we are alerted through this.
    void			 alertUnlockSOP(SOP_Node *unlocker,
						SOP_Node *unlockee);

    /// When a SOP is recooked, we are alerted through this.
    /// These must occur in matched pairs.  The cache manager
    /// uses these to determine when all cooking is done so it
    /// can clean up any orphaned checkpoints.
    void			 alertCookStartSOP(SOP_Node *node, float time);
    void			 alertCookEndSOP(SOP_Node *node);

    /// Trigger this alert when the gdp of a node has changed without
    /// going through the normal cook or unload mechanism.  This is mostly
    /// for handling locked sops.  If we ever bring back the modeler,
    /// it's operations would also trigger this.  The cache then knows
    /// to recalculate the memory usage of the SOP.
    void			 alertGeometryChangedSOP(SOP_Node *node);

    /// When a SOP is unloaded, we are alerted through this.
    void			 alertUnloadSOP(SOP_Node *node);

    /// Whenever a SOP's data is accessed (view getCookedGeoHandle)
    /// we are alerted by this mechanism.
    void			 alertTouchedSOP(SOP_Node *node);

    /// These manage the checkpoints associated with the cache.
    /// These are epochal time periods, nodes which are older
    /// than myLastCheckpoint have not been touched recently enough
    /// so are candidates for unloading.

    /// This marks the current time as a checkpoint.  To avoid deletion,
    /// nodes must keep above the last two checkpoint.  (Last two, or the
    /// act of setting the checkpoint would implicitly delete all current
    /// nodes)
    void			 setCheckpoint(const char *mark);
    
    /// This unloads all nodes that don't reside above myLastCheckpoint.
    /// It will also adjust myLastCheckpoint to be myCurrentCheckpoint,
    /// thus allowing hasNewCheckpoint to detect if there is a new
    /// operation pending.
    void			 unloadToCheckpoint(const char *mark);

    /// Returns true if the checkpoints have changed since the last
    /// unloadToCheckpoint.
    bool			 hasNewCheckpoint(const char *mark);

    /// Unloads all nodes until the used memory drops below the given
    /// amount.  Nodes are culled from oldest to newest.  Nodes are
    /// not culled if currently in use (much like the checkpoint code)
    void			 cullToMemory(int64 amount);

    /// Culls to the maximum memory setting of the cache.
    /// Only does anything if myMemoryState is set to always.
    void			 cullToMaxMemory();

    /// Used internally to get the data.
    SOP_CacheData		*getData(int idx) const;

    /// Used as the interface to sopcache, which is installed in
    /// MOT_Command.C
    void			 parseCommand(CMD_Args &args);

    SOP_CacheUnloadState	 getUnloadState() const
				 { return myUnloadState; }
    
    /// Returns total used cache memory in bytes
    int64			 getTotalMem() const
				 { return myTotalMem; }

    /// Returns the current memory state.
    SOP_CacheMemoryState	 getMemoryState() const
				 { return myMemoryState; }

    /// Returns maximum allowed memory
    int64			 getMaxMemory() const
				 { return myMaxMemory; }

    /// Sets the maximum memory limit.  This will trigger
    /// another cull pass, so may result in memory being freed.
    void			 setMaxMemory(int64 size);

    /// Clears the cache, freeing everything that can be freed.
    /// Returns the number of nodes that have been cleared.
    int			 	 clearCache();

private:
    /// This stores the special cache data associated with each node.
    /// It is indexed by uniqueId.
    UT_ValArray<SOP_CacheData *>	 myNodeReferences;

    int64				 myTotalMem;
    int64				 myTimeStamp;
    
    int					 myNumCooks;

    int64				 myMaxMemory;
    SOP_CacheMemoryState		 myMemoryState;
    SOP_CacheUnloadState		 myUnloadState;

    /// This is a double linked list of nodes who are currently
    /// marked as checkpoint nodes.  In otherwords: They are active,
    /// want to be unloaded, and have more than one output.
    SOP_CacheData			*myCheckPointHead, *myCheckPointTail;

    /// This is a double linked list of all nodes that are loaded and
    /// hence potentially unloadable.
    SOP_CacheData			*myLoadedHead, *myLoadedTail;

    /// This is an array of recently cooked nodes that are candidates
    /// for unloading.
    UT_ValArray<SOP_CacheData *>         myCookedNodes;

    UT_SymbolMap<sop_checkpoint *>	 myCheckpoints;

    /// The global utCache which gives us unload requests and to whom
    /// we alert of any changes.
    SOP_BaseCache			*myBaseCache;
};


#endif

