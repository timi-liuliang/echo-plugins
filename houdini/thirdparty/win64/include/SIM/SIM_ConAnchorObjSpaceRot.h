/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorObjSpaceRot_h__
#define __SIM_ConAnchorObjSpaceRot_h__

#include "SIM_API.h"
#include "SIM_ConAnchorObjRotational.h"

/// A concrete constraint source: the "lookat" constraint, set to
/// track the orientation from one object to another.
class SIM_API SIM_ConAnchorObjSpaceRot : public SIM_ConAnchorObjRotational
{
public:
    /// Object-space offset.
    GETSET_DATA_FUNCS_V3(SIM_NAME_OFFSET, Offset);

    /// This value must be true to force the constraint to initialize
    /// itself from the provided world space rotation. Otherwise the
    /// constraint-specific parameters are used directly.
    GETSET_DATA_FUNCS_B(SIM_NAME_USEWORLDSPACEROTATION, UseWorldSpaceRotation);

    /// Retrieve the world-space rotation for the guide geometry.
    GETSET_DATA_FUNCS_V3(SIM_NAME_WORLDSPACEROTATION, WorldSpaceRotation);

protected:
    explicit		 SIM_ConAnchorObjSpaceRot(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjSpaceRot();

    virtual void		 initParmsFromWorldSpacePositionSubclass(
					    const UT_Vector3 &worldspacepos,
					    const SIM_Object *object);

    virtual UT_Vector3		 getGuidePositionSubclass(
					const SIM_Time &time) const;
    virtual UT_Quaternion	 getOrientationSubclass(
					const SIM_Time &time) const;
    virtual void	 initAnchorSubclass(const SIM_Object *object);

private:
    static const SIM_DopDescription	*getDopDescription();

    void initParmsFromWorldSpaceRotation(const UT_Vector3 &worldspacerot,
					 const SIM_Object *object);

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjSpaceRot,
			SIM_ConAnchorObjRotational,
			"Anchor: Object Space Rotation",
			getDopDescription());
};

#endif

