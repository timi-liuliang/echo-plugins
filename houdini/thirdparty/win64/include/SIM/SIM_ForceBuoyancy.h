/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceBuoyancy_h__
#define __SIM_ForceBuoyancy_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. This
/// implementation applies a force in a given direction from an
/// attached scalar field, treating it as a liquid applying an out-going
/// force wherever it is less than 0.
class SIM_API SIM_ForceBuoyancy : public SIM_Force,
				  public SIM_OptionsUser
{
public:
    /// Control the force attribute.
    GETSET_DATA_FUNCS_V3(SIM_NAME_FORCE, Force);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceBuoyancy(const SIM_DataFactory *factory);
    virtual		~SIM_ForceBuoyancy();

    /// Returns the buoyance force at a point - not very accurate
    /// as buoyancy is ideally defined over a volume.
    /// We end up definining it as a unit volume and just enable/disable
    /// the force according to the surface values.
    virtual void	 getForceSubclass(const SIM_Object &object,
				const UT_Vector3 &position,
				const UT_Vector3 &velocity,
				const UT_Vector3 &angvel,
				const fpreal mass,
				UT_Vector3 &force,
				UT_Vector3 &torque) const;

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
    static const SIM_DopDescription	*getForceBuoyancyDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceBuoyancy,
			SIM_Force,
			"Buoyancy Force",
			getForceBuoyancyDopDescription());
};

#endif

