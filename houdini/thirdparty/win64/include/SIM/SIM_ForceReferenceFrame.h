/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceReferenceFrame_h__
#define __SIM_ForceReferenceFrame_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. 
/// This implementation transforms the accerations of another object
/// into forces on this object, thereby allowing you to have an object
/// use another object as a reference frame.
class SIM_API SIM_ForceReferenceFrame : public SIM_Force,
			       public SIM_OptionsUser
{
public:
    /// The object to get the new frame from.
    GET_DATA_FUNC_S("newframeobject", NewFrameName);
    /// The position data on the new frame object
    GET_DATA_FUNC_S("newframeposition", NewFramePositionName);
    // AMount to scale the effect.
    GET_DATA_FUNC_F("forcescale", ForceScale);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceReferenceFrame(const SIM_DataFactory *factory);
    virtual		~SIM_ForceReferenceFrame();

    /// Computes the acceleration of the frame object and uses
    /// that to determine fictitious forces that would generate
    /// that acceleration.  These become the returned force.
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
    static const SIM_DopDescription	*getForceReferenceFrameDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceReferenceFrame,
			SIM_Force,
			"Reference Frame Force",
			getForceReferenceFrameDopDescription());
};

#endif

