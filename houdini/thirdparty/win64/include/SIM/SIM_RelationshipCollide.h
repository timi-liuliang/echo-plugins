/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RelationshipCollide_h__
#define __SIM_RelationshipCollide_h__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"
#include "SIM_OptionsUser.h"

class SIM_Collider;

/// This class defines a relationship between objects that need to perform
/// collision detection.
class SIM_API SIM_RelationshipCollide : public SIM_RelationshipData,
					public SIM_OptionsUser
{
public:
    /// The priority parameter is used when two or more collision
    /// relationships apply to the same pair of objects. In that case,
    /// the collision relationship with the higher priority is used to
    /// determine what collider to use for those objects. An example of
    /// this would be where several cloth objects are set up in a mutual
    /// collision relationship, but the user doesn't want to do any
    /// self-collision detection. In this case the mutual collision
    /// data between all the cloth objects would be given a low priority.
    /// Then each cloth object would have a high priority collision
    /// relationship with just itself, and specifying a SIM_ColliderNone
    /// as the collider.
    GETSET_DATA_FUNCS_I(SIM_NAME_PRIORITY, Priority);

    /// Gets the const SIM_Collider subdata that defines the collision
    /// detection method to use. This data is found by returning the first
    /// SIM_Collider subdata attached to this data. If no SIM_Collider
    /// subdata is attached to this relationship, the standard method
    /// of finding a proper default collider for a pair of objects is
    /// used. For information on this, see the SIM_ColliderLabel class.
    const SIM_Collider	*getCollider() const;

protected:
    explicit	 SIM_RelationshipCollide(const SIM_DataFactory *factory);
    virtual	~SIM_RelationshipCollide();

private:
    static const SIM_DopDescription	*getCollideDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RelationshipCollide,
			SIM_RelationshipData,
			"Collide Relationship",
			getCollideDopDescription());
};

#endif

