/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorSpatial_h__
#define __SIM_ConAnchorSpatial_h__

#include "SIM_API.h"
#include "SIM_ConAnchor.h"
#include <UT/UT_Quaternion.h>
#include <UT/UT_Vector3.h>

// TODO: Read over this comment and make sure it all still makes sense:

/// Normally, constraints affect all three positional degrees of freedom
/// of an object. Some constraints, however, can limit their action to
/// only one or two degrees of freedom. For example, a hard constraint
/// might only force geometry to follow the anchor in y, and leave the x
/// and z DOFs unconstrained. In these situations, the constraint is
/// completely independent of the unconstrained DOFs - movement of the
/// geometry in unconstrained directions has no effect on the constraint
/// (e.g., doesn't affect springs), and the constraint never has an effect
/// in these DOFs (e.g., the constraint will never cause movement in x or z
/// in the earlier example).
class SIM_API SIM_ConAnchorSpatial : public SIM_ConAnchor
{
public:
    /// We categorize constraints by the shape created by sweeping a point
    /// through unconstrained space.  The numerical representation
    /// is thus the number of degrees that have been constrained.
    /// "DOF" stands for "degrees of freedom".
    enum SIM_DOFType {
	DOF_SPACE = 0,	/// Any point in space is valid.
			///  (i.e., constraint has no effect)
	DOF_PLANE = 1,	/// Any point on a plane is valid.
	DOF_LINE = 2,	/// Any point on a line is valid.
	DOF_POINT = 3	/// Only one point is valid
			///  (i.e., constraint prevents all movement)
    };

    /// Static method for rendering the guide geometry for this type of anchor
    static GA_Offset     buildAnchorGuideGeometry(GU_Detail* gdp,
                                    const UT_Vector3& anchor_pos,
                                    int dofType, UT_Vector3 dofVector,
				    fpreal scale,
                                    const GA_RWHandleV3& colour_attrib,
				    const UT_Vector3& color,
				    bool show_object_link,
				    bool is_affecting_object,
                                    const UT_Vector3& obj_pos);

    GETSET_DATA_FUNCS_E(SIM_NAME_CONDOF, DOFTypeInput, SIM_DOFType);
    /// All constraints are represented by a vector which is interpreted
    /// differently depending on how many DOF will be constrained.
    /// This is troublesome as it only works in 3d.  Ideally, we
    /// should specify two types of constraints:
    /// If P is the point to constrain, and R the rest point, and V
    /// the vector, we have:
    ///   Normal constraint:
    ///     dot(V, P) = dot(V, R)
    ///     This removes one degree of freedom.  Equivalent to PLANE.
    ///   Vector constraint:
    ///     P - R = s V, for some s
    ///     This removes all but one degree of freedom.  Equivalent to LINE.
    /// The result of this is always normalized.
    ///
    /// We can then define the UnconDOFFilter which, when applied to a velocity,
    /// will cause it to fulfill those conditions.  We can then clamp
    /// a position P to a rest position R with the filter uF with:
    ///  P' = (uF * (P - R)) + R
    /// (Note that we can also use the ConDOFFilter with the following
    ///  equation:
    ///  P' = (F * (R - P)) + P
    ///  We present it with the unconstrained case to match the rotational
    ///  case which is harder to rearrange)
    ///
    /// This vector is defined in simulation space, and may not be
    /// normalized.
    GETSET_DATA_FUNCS_V3(SIM_NAME_CONDIR, DOFVectorInput);

    /// Overridable get/set for the DOF of the constraint
    SIM_DOFType		 getDOFType() const;
    void		 setDOFType(const SIM_DOFType type);
    const UT_Vector3	 getDOFVector() const;
    void		 setDOFVector(const UT_Vector3 &vector);

    /// Retrieve the world-space position to constrain to.
    UT_Vector3		 getPosition(const SIM_Time &time) const;
    UT_Vector3		 getVelocity(const SIM_Time &time) const;

    /// Retrieve number of constrained degrees of freedom.
    int			 getNumConDOFs() const;
    /// Returns a matrix that, when multiplied by a vector, gives only the
    /// components of the vector that are constrained.
    void		 getConDOFFilter(UT_Matrix3 &result,
				const SIM_Time &time) const;

    /// Opposite of getConDOFFilter().
    /// getUnconDOFFilter() = identity - getConDOFFilter()
    void		 getUnconDOFFilter(UT_Matrix3 &result,
				const SIM_Time &time) const;

protected:
    explicit		 SIM_ConAnchorSpatial(const SIM_DataFactory *factory);
    virtual		~SIM_ConAnchorSpatial();

    virtual void	 buildAnchorGuideGeometrySubclass(
					const SIM_Options &options,
					const GU_DetailHandle &gdh,
					const SIM_Relationship &rel,
					const SIM_Time &t) const;

    virtual UT_Vector3	 getPositionSubclass(const SIM_Time &time) const = 0;
    virtual UT_Vector3	 getVelocitySubclass(const SIM_Time &) const;

    virtual SIM_DOFType		getDOFTypeSubclass() const;
    virtual const UT_Vector3	getDOFVectorSubclass() const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConAnchorSpatial, SIM_ConAnchor);
};

#endif
