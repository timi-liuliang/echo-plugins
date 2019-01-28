/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceFan_h__
#define __SIM_ForceFan_h__

#include "SIM_API.h"
#include <UT/UT_DMatrix4.h>
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. This
/// implementation returns a force value that is scaled by the mass of
/// the object receiving the force. No torque is applied. This SIM_Data
/// subclass also implements the SIM_Geometry interface to provide a
/// visualization of the fan and the force it will generate.
class SIM_API SIM_ForceFan : public SIM_Force,
			     public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_V3(SIM_NAME_POSITION, Position);
    GETSET_DATA_FUNCS_V3(SIM_NAME_DIRECTION, Direction);
    GETSET_DATA_FUNCS_F(SIM_NAME_CONEANGLE, ConeAngle);
    GETSET_DATA_FUNCS_F(SIM_NAME_FALLOFFANGLE, FalloffAngle);
    GETSET_DATA_FUNCS_F(SIM_NAME_FLUX, Flux);
    GETSET_DATA_FUNCS_F(SIM_NAME_FALLOFF, Falloff);
    GETSET_DATA_FUNCS_F(SIM_NAME_MAXDISTANCE, MaxDistance);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceFan(const SIM_DataFactory *factory);
    virtual		~SIM_ForceFan();

    /// Override the point force calculations. Since a fan inherently acts
    /// on a surface, we assume for points that the point is a surface with
    /// unit area.
    virtual void	 getForceSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;
    /// Override the surface force calculation. This function calls
    /// getForceSubclass() to get the force that would be applied to a
    /// unit area, then scales this value by the area of the circle that
    /// would be presented to the fan.
    virtual void	 getForceOnCircleSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &normal,
					const fpreal radius,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;

    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

    /// Override this to return a SIM_GuideStatic. The fan geometry is
    /// always the same, only the transform changes.
    virtual SIM_Guide	*createGuideObjectSubclass() const;
    /// We override this method to create the fan geometry.
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:
    /// This function massages the cone angle parameter value. It converts
    /// it to radians, divides it by two (to represent the angle from the
    /// centre of the cone to the edge), and verifies that the angle falls
    /// into a valid range.
    fpreal		 getAdjustedConeAngle() const;
    /// This function massages the falloff cone angle value. It converts
    /// it to radians, divides it by two, and ensures it fits within the
    /// overall cone angle.
    fpreal		 getAdjustedFalloffAngle(fpreal coneangle) const;

    /// Get the Automatic DOP description for creating a fan.
    static const SIM_DopDescription	*getForceFanDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceFan,
			SIM_Force,
			"Fan Force",
			getForceFanDopDescription());
};

#endif

