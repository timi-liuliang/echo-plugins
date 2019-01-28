/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConRelSpring_h__
#define __SIM_ConRelSpring_h__

#include "SIM_API.h"
#include "SIM_ConRel.h"
#include "SIM_OptionsUser.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix3.h>

class SIM_ConAnchorSpatial;

/// Defines a spring force between two anchors to enforce the
/// constraint between them.
///
/// The force is expressed mathematically as:
///    F = -k * (|x| - L) * ( x/|x| )
/// with a damping force of 
///    Fd = -kd * (v . (x/|x|)) * (x/|x|)
/// where x is the vector connecting the two anchors and v is dx/dt.
///
/// Torque is defined similarly.
class SIM_API SIM_ConRelSpring : public SIM_ConRel,
				 public SIM_OptionsUser
{
public:
    /// Retrieve the force (and optional jacobian) acting on objAnchor from a
    /// spring.
    UT_Vector3		 getForce(const SIM_Time &time,
				const SIM_ConAnchorSpatial &objAnchor,
				const SIM_ConAnchorSpatial &otherAnchor,
				UT_Matrix3 *jacobianPos = NULL,
				UT_Matrix3 *jacobianVel = NULL);
    UT_Vector3		 getForce(const UT_Vector3 &objAnchorPos,
				const UT_Vector3 &objAnchorVel,
				const UT_Vector3 &otherAnchorPos,
				const UT_Vector3 &otherAnchorVel,
				const UT_Matrix3 &dofFilter);
    UT_Vector3		 getTorque(const UT_Quaternion &objOrient,
				const UT_Vector3 &objAngVel,
				const UT_Quaternion &anchorOrient,
				const UT_Vector3 &anchorAngVel);
    /// Get Jacobian with respect to position, dF/dx. The returned Jacobian
    /// is w.r.t. the obj point (dF_i/dx_i); the Jacobian for the other
    /// point is the same thing, just negated (dF_i/dx_j = -dF_i/dx_i)
    UT_Matrix3		 getJacobianPos(const UT_Vector3 &objAnchorPos,
				const UT_Vector3 &objAnchorVel,
				const UT_Vector3 &otherAnchorPos,
				const UT_Vector3 &otherAnchorVel,
				const UT_Matrix3 &dofFilter) const;
    /// Get Jacobian with respect to velocity, dF/dv. Same caveats as dF/dx.
    UT_Matrix3		 getJacobianVel(const UT_Vector3 &objAnchorPos,
				const UT_Vector3 &objAnchorVel,
				const UT_Vector3 &otherAnchorPos,
				const UT_Vector3 &otherAnchorVel,
				const UT_Matrix3 &dofFilter) const;

    /// k
    GETSET_DATA_FUNCS_F(SIM_NAME_STRENGTH, Strength);
    /// L
    GETSET_DATA_FUNCS_F(SIM_NAME_RESTLENGTH, RestLength);
    /// kd
    GETSET_DATA_FUNCS_F(SIM_NAME_DAMPING, Damping);

    GETSET_DATA_FUNCS_I(SIM_NAME_NUMITERATIONS, NumIterations);
    GETSET_DATA_FUNCS_I(SIM_NAME_DISABLECOLLISIONS, DisableCollisions);

protected:
    explicit		 SIM_ConRelSpring(const SIM_DataFactory *factory);
    virtual		~SIM_ConRelSpring();

    virtual void	 setStateForceSubclass(fpreal force);
    virtual fpreal	 getStateForceSubclass() const;
    virtual void	 setStateDistanceSubclass(fpreal distance);
    virtual fpreal	 getStateDistanceSubclass() const;

    virtual void	 buildConRelGuide(
				const GU_DetailHandle &gdh,
				const SIM_Options &options,
				const UT_Vector3 &anchor_pos_1,
				const UT_Vector3 &anchor_pos_2,
				const UT_Quaternion &anchor_rot_1,
				const UT_Quaternion &anchor_rot_2,
                                const ConRelData &c_options) const;
    virtual UT_Vector3	 getForceSubclass(const UT_Vector3 &objAnchorPos,
			        const UT_Vector3 &objAnchorVel,
			        const UT_Vector3 &otherAnchorPos,
			        const UT_Vector3 &otherAnchorVel,
			        const UT_Matrix3 &dofFilter);
    virtual UT_Vector3	 getTorqueSubclass(const UT_Quaternion &objOrient,
				const UT_Vector3 &objAngVel,
				const UT_Quaternion &anchorOrient,
				const UT_Vector3 &anchorAngVel);
    virtual UT_Matrix3	 getJacobianPosSubclass(const UT_Vector3 &objAnchorPos,
				const UT_Vector3 &objAnchorVel,
				const UT_Vector3 &otherAnchorPos,
				const UT_Vector3 &otherAnchorVel,
				const UT_Matrix3 &dofFilter) const;
    virtual UT_Matrix3	 getJacobianVelSubclass(const UT_Vector3 &objAnchorPos,
				const UT_Vector3 &objAnchorVel,
				const UT_Vector3 &otherAnchorPos,
				const UT_Vector3 &otherAnchorVel,
				const UT_Matrix3 &dofFilter) const;

    UT_Vector3		 project(const UT_Vector3 &vec,
				 const UT_Matrix3 &dofFilter) const;

private:
    static const SIM_DopDescription	*getConRelSpringDopDescription();

    fpreal		 myStateForce;
    fpreal		 myStateDistance;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConRelSpring,
			SIM_ConRel,
			"Spring Constraint Relationship",
			getConRelSpringDopDescription());
};

#endif
