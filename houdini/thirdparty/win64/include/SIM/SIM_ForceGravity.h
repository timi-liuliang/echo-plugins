/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceGravity_h__
#define __SIM_ForceGravity_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. This
/// implementation returns a force value that is scaled by the mass of
/// the object receiving the force. No torque is applied.
class SIM_API SIM_ForceGravity : public SIM_Force,
				 public SIM_OptionsUser
{
public:
    /// Controls the magnitude and direction of the gravitational force.
    GETSET_DATA_FUNCS_V3(SIM_NAME_FORCE, Gravity);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceGravity(const SIM_DataFactory *factory);
    virtual		~SIM_ForceGravity();

    /// Calculates the force of gravtiy on a point. The force depends only
    /// on the mass of the point.
    virtual void	 getForceSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;
    virtual SIM_ForceResolver *getForceResolverSubclass(const SIM_Object &object) const;
    /// Calculates the force of gravtiy on a surface. The force depends only
    /// on the mass, so this function simply calls getForceSubclass(),
    /// ignoring the circle parameters.
    virtual void	 getForceOnCircleSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &normal,
					const fpreal radius,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;
    /// Calculates the force of gravtiy on a volume. The force depends only
    /// on the mass, so this function simply calls getForceSubclass(),
    /// ignoring the sphere parameters.
    virtual void	 getForceOnSphereSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const fpreal radius,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;

    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

private:
    static const SIM_DopDescription	*getForceGravityDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceGravity,
			SIM_Force,
			"Gravity Force",
			getForceGravityDopDescription());
};

#endif

