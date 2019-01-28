/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorWorldSpaceRot_h__
#define __SIM_ConAnchorWorldSpaceRot_h__

#include "SIM_API.h"
#include <UT/UT_Vector2.h>
#include "SIM_ConAnchorRotational.h"

/// A concrete constraint source: the "lookat" constraint, set to
/// track the orientation from one object to another.
class SIM_API SIM_ConAnchorWorldSpaceRot : public SIM_ConAnchorRotational
{
public:
    GETSET_DATA_FUNCS_V3(SIM_NAME_WORLDSPACEPOS, WorldSpacePosition);
    GETSET_DATA_FUNCS_V3(SIM_NAME_ANGVELOCITY, AngularVelocity);

protected:
    explicit		 SIM_ConAnchorWorldSpaceRot(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorWorldSpaceRot();

    virtual UT_Vector3		 getGuidePositionSubclass(
					const SIM_Time &time) const;
    virtual UT_Quaternion	 getOrientationSubclass(
					const SIM_Time &time) const;
    virtual UT_Vector3		 getAngularVelocitySubclass(
					const SIM_Time &time) const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorWorldSpaceRot,
			SIM_ConAnchorRotational,
			"Anchor: World Space Rotation",
			getDopDescription());
};

#endif

