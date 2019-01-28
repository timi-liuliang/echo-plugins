/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_SelectionSet.h ( GU Library, C++)
 */

#ifndef __GU_SelectionSet__
#define __GU_SelectionSet__

#include "GU_API.h"

#include <GA/GA_Types.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_SharedPtr.h>

class UT_JSONParser;
class UT_JSONWriter;

class GU_Selection;
typedef UT_SharedPtr<GU_Selection> GU_SelectionHandle;

class GU_SelectionSet;
typedef UT_SharedPtr<GU_SelectionSet> GU_SelectionSetHandle;

/// A static set of unique selection types. Once created, the set remains 
/// immutable. The class also stores the current active selection type for the 
/// set. By default, the first group type by group type order will be set as 
/// the active selection.
class GU_API GU_SelectionSet : public UT_NonCopyable
{
public:
    /// Construct a new selection set. The mask should contain at least
    /// one valid group type for selection. If the mask is invalid and doesn't
    /// contain a valid selection group type, a point selection will be
    /// chosen by default.
    GU_SelectionSet(GA_GroupMaskType sel_types);
    
    /// Create a new selection set from a subset of an existing one.
    GU_SelectionSet(GA_GroupMaskType sel_types, 
                    const GU_SelectionSet &selection_set);

    /// Returns \c true if the set contains the requested selection
    /// type.
    bool hasType(GA_GroupType sel_type) const;
    
    /// Returns the entire group mask of selection types. This is the same
    /// value that was passed into the constructor.
    GA_GroupMaskType typeMask() const;

    /// Returns the selection handle of the given selection type. If the
    /// set does not contain the given selection type, an empty
    /// selection handle is returned.
    GU_SelectionHandle selection(GA_GroupType sel_type) const;

    /// Returns the active selection type for this selection set. This
    /// is the selection type that the GUI should consider first for display
    /// and manipulation.
    GA_GroupType activeSelectionType() const;
    
    /// Returns the selection handle of the currently active selection type.
    GU_SelectionHandle activeSelection() const;

    /// Sets the active selection type for this selection set. If the wanted
    /// selection type is not available, this function returns \c false, and
    /// the active selection type remains unchanged.
    bool setActiveSelectionType(GA_GroupType sel_type);
    
    /// Save the selection set to a json stream. 
    bool save(UT_JSONWriter &w) const;
    
    /// Load the selection set from a json stream. Will recycle existing 
    /// selection objects, if possible, or create new ones if needed.
    bool load(UT_JSONParser &p);
    
private:
    GU_SelectionHandle mySelections[GA_GROUP_N];
    GA_GroupType myActiveSelection;
};

#endif // __GU_SelectionSet__
