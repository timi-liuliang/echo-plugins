/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Motion_h__
#define __SIM_Motion_h__

#include "SIM_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Quaternion.h>
#include <UT/UT_Matrix4.h>
#include "SIM_PositionSimple.h"

/// This is the default implementation for standard motion data.
class SIM_API SIM_Motion : public SIM_PositionSimple
{
public:
    /// Control the linear velocity attribute of this data.
    GETSET_DATA_FUNCS_V3(SIM_NAME_VELOCITY, Velocity);
    /// Control the angular velocity attribute of this data.
    /// This is specified in degrees per second.
    GETSET_DATA_FUNCS_V3(SIM_NAME_ANGVELOCITY, AngularVelocityDegrees);

    /// These are the access functions that transform our values in
    /// radians that we use internally everywhere into the actual data
    /// values that are stored in degrees.
    void		setAngularVelocity(const UT_Vector3 &value)
			{
			    UT_Vector3	deg;
			    deg = value;
			    deg.radToDeg();
			    setAngularVelocityDegrees(deg);
			}

    const UT_Vector3	getAngularVelocity() const
			{
			    UT_Vector3	rad;

			    rad = getAngularVelocityDegrees();
			    rad.degToRad();
			    return rad;
			}

    /// Gets the motion of a point that is offset from our centre of motion.
    const UT_Vector3	 getVelocityAtPosition(const UT_Vector3 &pos,
				fpreal integrateovertime = 0.0) const;

    /// Gets the motion of a point that is offset from the centre of motion,
    /// using the given pivot and velocity.
    static UT_Vector3 getVelocityAtPosition(const UT_Vector3 &location,
            const UT_Vector3 &velocity, const UT_Vector3 &angvel,
            const UT_Vector3 &position, const UT_Vector3 &pivot,
            fpreal integrateovertime = 0.0);

    /// Determines the maximum velocity inside a sphere of the given
    /// radius.
    const UT_Vector3	 getMaximumVelocity(fpreal radius) const;

protected:
    explicit		 SIM_Motion(const SIM_DataFactory *factory);
    virtual		~SIM_Motion();

private:
    static const SIM_DopDescription	*getMotionDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_Motion,
			SIM_PositionSimple,
			"Motion",
			getMotionDopDescription());
};

#endif

