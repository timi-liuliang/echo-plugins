/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceDrag_h__
#define __SIM_ForceDrag_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"
#include <UT/UT_Matrix.h>

/// A simple drag force that opposes velocities in the normal direction.
class SIM_API SIM_ForceDrag : public SIM_Force,
			      public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_V3(SIM_NAME_VELOCITY, Velocity);
    GETSET_DATA_FUNCS_V3(SIM_NAME_ANGVELOCITY, AngularVelocityDegrees);
    GETSET_DATA_FUNCS_F(SIM_NAME_FORCESCALE, ForceScale);
    GETSET_DATA_FUNCS_F(SIM_NAME_TORQUESCALE, TorqueScale);

    /// Controls whether we cancel out mass (making it a delta-v rather
    /// than delta-momentum)
    GET_DATA_FUNC_B("ignoremass", IgnoreMass);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceDrag(const SIM_DataFactory *factory);
    virtual		~SIM_ForceDrag();

    /// Override the surface force calculation. The drag force opposes
    /// velocities.
    virtual void	 getForceSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;
    virtual void	 getForceOnCircleSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &normal,
					const fpreal radius,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;

    virtual void	 getForceJacobianSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Matrix &dFdX,
					UT_Matrix &dFdV) const;

    virtual SIM_ForceResolver *getForceResolverSubclass(const SIM_Object &object) const;
    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

private:
    /// Get the Automatic DOP description for creating a drag dop.
    static const SIM_DopDescription	*getForceDragDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceDrag,
			SIM_Force,
			"Drag Force",
			getForceDragDopDescription());
};

#endif

