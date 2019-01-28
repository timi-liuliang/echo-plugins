/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceVex_h__
#define __SIM_ForceVex_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// A simple drag force that opposes velocities in the normal direction.
class SIM_API SIM_ForceVex : public SIM_Force,
			      public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_S("vexscript", VexScript);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceVex(const SIM_DataFactory *factory);
    virtual		~SIM_ForceVex();

    /// Override the surface force calculation. The drag force opposes
    /// velocities.
    virtual void	 getForceSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;
    virtual bool	 getForceSetSubclass(const SIM_Object &object,
					const UT_Vector3Array &positions,
					const UT_Vector3Array &velocities,
					const UT_Vector3Array &angvelocities,
					const UT_FloatArray &masses,
					UT_Vector3Array &forces,
					UT_Vector3Array &torques) const;

    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

private:
    /// Get the Automatic DOP description for creating a drag dop.
    static const SIM_DopDescription	*getForceVexDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceVex,
			SIM_Force,
			"Vex Force",
			getForceVexDopDescription());
};

#endif

