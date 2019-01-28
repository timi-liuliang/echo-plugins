/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ForceImpulse.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_ForceImpulse__
#define __SIM_ForceImpulse__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. This
/// implementation returns a force value that is scaled by the timestep
/// of the simulation.
class SIM_API SIM_ForceImpulse : public SIM_Force,
				 public SIM_OptionsUser
{
public:
    /// Controls the magnitude and direction of the impulse.
    GETSET_DATA_FUNCS_V3("impulse", Impulse);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceImpulse(const SIM_DataFactory *factory);
    virtual		~SIM_ForceImpulse();

    /// Calculates the impulse on a point.  This force will be scaled
    /// by the timestep & optionally by the mass.
    virtual void	 getForceSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;

    /// Calculates the impulse on a surface. 
    /// This function simply calls getForceSubclass(), ignoring the circle
    /// parameters.
    virtual void	 getForceOnCircleSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &normal,
					const fpreal radius,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;
    /// Calculates the impulse on a volume. 
    /// This function simply calls getForceSubclass(), ignoring the sphere
    /// parameters.
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
    static const SIM_DopDescription	*getForceImpulseDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceImpulse,
			SIM_Force,
			"Impulse Force",
			getForceImpulseDopDescription());
};

#endif

