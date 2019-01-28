/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP3D_SelectionCache.h ( OP3D Library, C++)
 *
 * COMMENTS:
 *      A cache for the various OP3D_GUSelectionSet objects that we create
 *      to represent user component selections.
 *
 * 	Generally there is one cache instance per selection scope that is
 * 	used to cache selections both for nodes in that scope (internal)
 * 	and for nodes outside of that scope (external).
 */

#ifndef __OP3D_SelectionCache__
#define __OP3D_SelectionCache__

#include "OP3D_API.h"

#include <UT/UT_NonCopyable.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_Map.h>
#include <UT/UT_Array.h>
#include <SYS/SYS_Hash.h>

class OP3D_GUSelectionSet;
typedef UT_SharedPtr<OP3D_GUSelectionSet> OP3D_GUSelectionSetHandle;

class OP3D_SelectionCache;
typedef UT_SharedPtr<OP3D_SelectionCache> OP3D_SelectionCacheHandle;

/// A cache for user component selections.
class OP3D_API OP3D_SelectionCache : public UT_NonCopyable
{
public:
    /// Construct a new, empty, component selection.
     OP3D_SelectionCache();

    ~OP3D_SelectionCache();

    void clear(bool clear_local, bool clear_external);

    /// Returns the selection set handle for the given node.  If there is no
    /// selection set for the given node, an empty handle is returned.
    OP3D_GUSelectionSetHandle	getSelectionSet(
					int node_id, int detail_index = 0,
					bool external = false);	 	

    /// Set the selection set for the given node.  This class only stores
    /// one selection set per node, so do not set both an external and a
    /// local (non-external) set on the same node.
    void			setSelectionSet(
					int node_id, int detail_index,
					OP3D_GUSelectionSetHandle selection_set,
					bool external);

    void			remove(int node_id, int detail_index);

    /// Return the total memory used by the selections stored in this cache
    /// in bytes.
    int64			getSelectionMemoryUsage() const;

    /// Attempt to reduce the memory used by a specified amount, returning the
    /// actual number of bytes freed.
    int64			reduceSelectionMemoryUsage(int64 amount);

    class Key;
    
private:
    class Entry
    {
    public:
	OP3D_GUSelectionSetHandle	 mySelectionSet;
	bool				 myExternalFlag;
    };

    UT_Map<Key, Entry>	 mySelections;
};

// Key class for the selection map.
class OP3D_API OP3D_SelectionCache::Key
{
public:
    Key(int node_id, int detail_index)
	: myNodeId(node_id), myDetailIndex(detail_index)
    {
    }

    bool operator==(const OP3D_SelectionCache::Key &key) const
    {
	return myNodeId == key.myNodeId && myDetailIndex == key.myDetailIndex;
    }

    int		 myNodeId;
    int		 myDetailIndex;
};

SYS_FORCE_INLINE size_t hash_value(const OP3D_SelectionCache::Key &key)
{
    size_t	 hash_value =  SYShash(key.myNodeId);
    SYShashCombine(hash_value, key.myDetailIndex);
    return hash_value;
}

#endif // __OP3D_SelectionCache__
