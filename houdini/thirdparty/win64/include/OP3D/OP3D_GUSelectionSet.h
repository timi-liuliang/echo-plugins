/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP3D_GUSelectionSet.h ( OP3D Library, C++)
 */

#ifndef __OP3D_GUSelectionSet__
#define __OP3D_GUSelectionSet__

#include "OP3D_API.h"

#include <GU/GU_SelectType.h>
#include <GA/GA_Types.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_SharedPtr.h>

class UT_JSONParser;
class UT_JSONWriter;

class OP3D_GUSelectionSet;
typedef UT_SharedPtr<OP3D_GUSelectionSet> OP3D_GUSelectionSetHandle;

/// A set of selections, one for each unique selection type.  The selection
/// handles may be freely assigned, unlike GU_SelectionSet.
class OP3D_API OP3D_GUSelectionSet : public UT_NonCopyable
{
public:
    /// Construct a new empty selection set.
    OP3D_GUSelectionSet();
    
    /// Create a new selection set from a subset of an existing one.
    OP3D_GUSelectionSet(GA_GroupMaskType sel_types,
			const OP3D_GUSelectionSet &selection_set,
			bool clone);

    /// Returns \c true if the set contains the requested selection
    /// type.
    bool hasType(GA_GroupType sel_type) const;
    
    /// Returns the entire group mask of selection types with non-null
    /// selections.
    GA_GroupMaskType typeMask() const;

    /// Returns the selection handle of the given selection type, which
    /// may be empty.
    GU_SelectionHandle selection(GA_GroupType sel_type) const;

    /// Set the selection handle of the given selection type.  If the
    /// selection handle points to a selection, then its selection type
    /// must match that supplied.
    void setSelection(GA_GroupType sel_type, GU_SelectionHandle selection);

    /// Save the selection set to a json stream. 
    bool save(UT_JSONWriter &w) const;
    
    /// Load the selection set from a json stream. Will recycle existing 
    /// selection objects, if possible, or create new ones if needed.
    bool load(UT_JSONParser &p);

    int64 getMemoryUsage(bool inclusive) const;
    
private:
    GU_SelectionHandle mySelections[GA_GROUP_N];
};

#endif // __GU_SelectionSet__
