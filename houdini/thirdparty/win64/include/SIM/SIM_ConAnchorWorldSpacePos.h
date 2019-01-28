/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorWorldSpacePos_h__
#define __SIM_ConAnchorWorldSpacePos_h__

#include "SIM_API.h"
#include "SIM_ConAnchorSpatial.h"

/// A concrete constraint source: the "nail" constraint, locked to a
/// (possibly animated) position in worldspace.
class SIM_API SIM_ConAnchorWorldSpacePos : public SIM_ConAnchorSpatial
{
public:
    GETSET_DATA_FUNCS_V3(SIM_NAME_POSITION, Position);
    GETSET_DATA_FUNCS_V3(SIM_NAME_VELOCITY, Velocity);

protected:
    explicit		 SIM_ConAnchorWorldSpacePos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorWorldSpacePos();

    virtual UT_Vector3	 getPositionSubclass(const SIM_Time &time) const;
    virtual UT_Vector3	 getVelocitySubclass(const SIM_Time &) const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorWorldSpacePos,
			SIM_ConAnchorSpatial,
			"Anchor: World Space Position",
			getDopDescription());
};

#endif
