/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RelationshipGroup_h__
#define __SIM_RelationshipGroup_h__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"

/// This class defines a relationship between objects that form a group.
/// The SIM_Relationship parent data of a SIM_RelationshipGroup should
/// have the grouped objects in the object group, and the affector group
/// should remain empty. Even if the affector group has objects in it,
/// those objects are ignored, and are not part of the group.
class SIM_API SIM_RelationshipGroup : public SIM_RelationshipData
{
protected:
    explicit		 SIM_RelationshipGroup(const SIM_DataFactory *factory);
    virtual		~SIM_RelationshipGroup();

private:
    static const SIM_DopDescription	*getGroupDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RelationshipGroup,
			SIM_RelationshipData,
			"Group Relationship",
			getGroupDopDescription());
};

#endif

