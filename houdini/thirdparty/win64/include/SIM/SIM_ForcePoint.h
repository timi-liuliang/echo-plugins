/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForcePoint_h__
#define __SIM_ForcePoint_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. This
/// implementation applies a force in a given direction from a given
/// point.  This means the torque will take into account the offset
/// from the center of mass.
class SIM_API SIM_ForcePoint : public SIM_Force,
			       public SIM_OptionsUser
{
public:
    /// Control the force attribute.
    GETSET_DATA_FUNCS_V3(SIM_NAME_FORCE, Force);
    /// Control the position attribute.
    GETSET_DATA_FUNCS_V3(SIM_NAME_POSITION, ForcePosition);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForcePoint(const SIM_DataFactory *factory);
    virtual		~SIM_ForcePoint();

    /// Returns the force and torque on a point. The returned values are
    /// the torque and force applied to an object with center of mass
    /// at poistion which receives a force at PointPosition of
    /// value ForcePoint.
    /// The getForceOnCircleSubclass() and getForceOnSphereSubclass()
    /// functions use the default implementations which scale the point
    /// force by the area or volume of the circle or sphere. This means
    /// that regardless of the size of samples used to quantize an object,
    /// the total force on the object should remain roughly constant.
    virtual void	 getForceSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;

    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

private:
    static const SIM_DopDescription	*getForcePointDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForcePoint,
			SIM_Force,
			"Point Force",
			getForcePointDopDescription());
};

#endif

