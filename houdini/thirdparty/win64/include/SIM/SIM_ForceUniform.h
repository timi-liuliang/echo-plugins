/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceUniform_h__
#define __SIM_ForceUniform_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. This
/// implementation returns a fixed value for force and torque regardless
/// of the object data passed in.
class SIM_API SIM_ForceUniform : public SIM_Force,
				 public SIM_OptionsUser
{
public:
    /// Control the force attribute.
    GETSET_DATA_FUNCS_V3(SIM_NAME_FORCE, UniformForce);
    /// Control the torque attribute.
    GETSET_DATA_FUNCS_V3(SIM_NAME_TORQUE, UniformTorque);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceUniform(const SIM_DataFactory *factory);
    virtual		~SIM_ForceUniform();

    /// Returns the force and torque on a point. The returned values are
    /// exact copies of the force and torque attributes. The
    /// getForceOnCircleSubclass() and getForceOnSphereSubclass()
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
    virtual SIM_ForceResolver *getForceResolverSubclass(const SIM_Object &object) const;

    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

private:
    static const SIM_DopDescription	*getForceUniformDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceUniform,
			SIM_Force,
			"Uniform Force",
			getForceUniformDopDescription());
};

#endif

