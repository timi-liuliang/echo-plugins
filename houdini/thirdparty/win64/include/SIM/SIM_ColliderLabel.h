/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ColliderLabel_h__
#define __SIM_ColliderLabel_h__

#include "SIM_API.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

/// This class associates a string label with an object that is used to
/// help determine what SIM_Collider subclass to use when performing
/// collision detection between a pair of objects. In addition to holding
/// the collider label for an object, SIM_Colliders may be attached as
/// subdata to this data to explicitly specify a SIM_Collider to use
/// when colliding this object with another. The SIM_Collider with the
/// subdata name that matches the other object's collider label is used
/// as the collider for the two objects. For more information on determining
/// the right SIM_Collider to use for a pair of objects, see
/// SIM_Object::getCollider().
class SIM_API SIM_ColliderLabel : public SIM_Data,
				  public SIM_OptionsUser
{
public:
    /// Control the label that helps determine the proper collider to use
    /// for this object.
    GETSET_DATA_FUNCS_S(SIM_NAME_COLLIDERLABEL, ColliderLabel);

protected:
    explicit		 SIM_ColliderLabel(const SIM_DataFactory *factory);
    virtual		~SIM_ColliderLabel();

private:
    static const SIM_DopDescription	*getColliderLabelDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ColliderLabel,
			SIM_Data,
			"Collider Label",
			getColliderLabelDopDescription());
};

#endif

