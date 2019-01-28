/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP3D_SelectionManager.h ( OP3D Library, C++)
 *
 * COMMENTS:
 * 	An OP3D_SelectionManager stores and manages selections that need to
 * 	persist beyond the lifetime of an individual selector.  This may be
 * 	because the selection is needed to bootstrap another selector later,
 * 	or because we want the selection to be saved for when we next select
 * 	from that geometry.
 *
 * 	Each viewer has its instance of this selection manager.
 *
 */

#ifndef __OP3D_SelectionManager__
#define __OP3D_SelectionManager__

#include "OP3D_API.h"

#include <UT/UT_NonCopyable.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_Map.h>
#include <UT/UT_IntArray.h>
#include "OP3D_SelectionCache.h"

class UT_JSONWriter;
class UT_JSONParser;
class OP_Node;

class OP3D_API OP3D_SelectionManager : public UT_NonCopyable
{
public:
     OP3D_SelectionManager();

    ~OP3D_SelectionManager();

    class ComponentScopeKey;

    static ComponentScopeKey	 getComponentSelectionScope(OP_Node *node);
    static ComponentScopeKey	 getComponentSelectionScopeOfChildren(
							    OP_Node *node);
    static bool			 isObject(const ComponentScopeKey &scope);

    /// Component selectors (OP3D_InputSelector) can be divided into two
    /// categories based on the lifetime of the temporary selections they
    //  create.
    ///  1. Selectors that save their temporary selections on exit.  These
    ///     selections persist until they are consumed (either by the next
    ///     selector or by a state explicitly consuming them to perform an
    ///     operation) or explicitly cleared.
    ///  2. Selectors with completely local temporary selections.  These
    ///     selections cease to exist with the selectors that created them
    ///     and generally do not intefere with any other selections (saved
    ///     or live).
    ///
    /// Component selectors of the second type cannot interfere with any
    /// other selector, but selectors of the first type, by far the more
    /// common, require careful synchronization and storage when multiple
    /// instances are active simultaneously (each, of course, in a different
    /// viewer) or for successive instances to pass selections between them.
    /// We choose to use an independent selection manager (and hence set of
    /// caches) for each viewer, sidestepping the first issue entirely.  To
    /// address the passing of selections from one selector to the next, we
    /// implement a set of caches.
    ///
    /// The first simplifying decision we make for selectors that save their
    /// temporary selections on exit is to distinguish these saved selections
    /// by the network (abstracted to a ComponentScopeKey) at which they are
    /// made.  Selections made at /obj/A will generally not intefere with
    /// ones made at /obj/B, even if the selectors allow selecting from other
    /// objects (and connecting them with Object Merge SOPs).  This is made
    /// possible by each OP3D_SelectionCache being able to store selections
    /// for nodes external to that scope.

    OP3D_SelectionCacheHandle	 getUserSelectionCache(
						    ComponentScopeKey scope);
    OP3D_SelectionCacheHandle	 getOrCreateUserSelectionCache(
						    ComponentScopeKey scope);

    /// Return the total memory used by the user selection caches in bytes.
    int64			 getUserSelectionCacheTotalMem() const;

    /// Attempt to reduce the memory used by the user selection caches by a
    /// specified amount, returning the actual number of bytes freed.
    int64			 reduceUserSelectionCacheMem(int64 amount);

    /// Flag enabled when geometry selection is set by the select state.
    void setSopSelectionStashed(bool v);
    bool getSopSelectionStashed();

    /// Node IDs of most recent object selection.
    void setObjSelection(const UT_IntArray &nodes);
    void getObjSelection(UT_IntArray &nodes);

private:
    UT_Map<int, OP3D_SelectionCacheHandle> myUserComponentSelections;
    UT_IntArray	myObjSelection;
    bool mySelectionStashed;
};

class OP3D_API OP3D_SelectionManager::ComponentScopeKey
{
public:
    ComponentScopeKey() : myNodeId(-1)
    {
    }

    bool isValid() const { return myNodeId >= 0; }
    bool operator==(const ComponentScopeKey &src) const
					{ return myNodeId == src.myNodeId; }
    bool operator!=(const ComponentScopeKey &src) const
					{ return myNodeId != src.myNodeId; }

    bool save(UT_JSONWriter &w) const;
    bool load(UT_JSONParser &p);
private:
    ComponentScopeKey(int node_id)
	: myNodeId(node_id)
    {
    }

    int		 myNodeId;
    friend class OP3D_SelectionManager;
};

OP3D_API OP3D_SelectionManager *OP3DgetSelectionManager();

#endif // __OP3D_SelectionManager__
