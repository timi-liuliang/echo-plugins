/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_AttributeMerge.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_AttributeMerge__
#define __GT_AttributeMerge__

#include "GT_API.h"
#include "GT_AttributeMap.h"

/// @brief Keep track of merging of attribute maps
///
/// Attribute maps/lists can be merged to create a new attribute map.  This
/// class keeps track of which attributes fromt the two source maps get merged
/// into the merged map.
///
/// @c m1 (the first map) is given priority for duplicates.  That is, if an
/// attribute exists in both @c m1 and @c m2, only the attribute from @c m1
/// will appear in the resulting map.
class GT_API GT_AttributeMerge {
public:
    GT_AttributeMerge(const GT_AttributeMapHandle &m1,
		      const GT_AttributeMapHandle &m2);
    ~GT_AttributeMerge();

    /// Get the GT_AttributeMap representing the merged map
    const GT_AttributeMapHandle	&getMap() const	{ return myMap; }

    /// Get the destination index for the source index
    int		getDestIndex(int source_index, int source_map) const;
private:
    GT_AttributeMapHandle	 myMap;
    int				*myIndexMap;
    int				 mySize[2];
};

#endif
