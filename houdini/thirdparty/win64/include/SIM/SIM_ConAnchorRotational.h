/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ConAnchorRotational_h__
#define __SIM_ConAnchorRotational_h__

#include "SIM_API.h"
#include "SIM_ConAnchor.h"
#include <UT/UT_Quaternion.h>
#include <UT/UT_Vector3.h>

/// Rotational constraints are a bit odd.
/// We categorize constraints by the shape created by sweeping the
/// possible rotation axes through unconstrained space.
/// The numerical representation is thus the number of degrees that
/// have been constrained. "DOF" stands for "degrees of freedom".
class SIM_API SIM_ConAnchorRotational : public SIM_ConAnchor
{
public:
    GETSET_DATA_FUNCS_V3(SIM_NAME_ROTATION, RotationInput);

    /// We need better names for these.
    enum SIM_DOFType {
	/// DOF_FREE is a complete absence of any restrictions on
	/// orientation. Any orientation is valid.
	DOF_FREE = 0,
	/// DOF_PLANEJOINT provides the mobility of a thick wire system.
	/// The plane's axis can be made to look in any direction, but can't
	/// be rotated freely.
	/// Only rotations whose axes lie in a given plane are allowed.
	DOF_PLANEJOINT = 1,
	/// DOF_HINGE allows any orientation around a specific axis.
	/// A specific axis is locked, rotating around that is allowed.
	DOF_HINGE = 2,
	/// DOF_ORIENT provides no free orientation.
	/// Orientation completely locked down.
	DOF_ORIENT = 3
    };

    /// Static method for rendering the guide geometry for this type of anchor
    static GA_Offset     buildAnchorGuideGeometry(GU_Detail* gdp,
                                    const UT_Vector3& anchor_pos,
				    const UT_Quaternion& q,
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
    /// 
    /// If O is the orientation to constrain, and Q the rest orientation,
    /// and V the vector:
    ///   Free constraint:
    ///		O = O
    ///		No constraint occurs.
    ///
    ///   Plane constraint:
    ///		(O / Q) . V = 0
    ///		The rotation which maps from Q to O should not have any
    ///		component of V in it.  This allows all orientations which
    ///		can be reached from Q by rotating around a vector perpendicular
    ///		to V.
    ///
    ///   Hinge constraint:
    ///		(O / Q) = s V, for some s
    ///		There is one degree of freedom.  The set of valid orientations
    ///		are those that can be reached from Q by rotating around V.
    ///
    ///   Orient constraint:
    ///         O = Q
    ///         The quaternion is fully constrained.
    ///
    /// Through further abuse of notation, we can define the UnconDOFFilter
    /// which when applied to an angular velocity will cause it to fulfill
    /// those conditions.  We can then clamp a quaternion O into the desired
    /// value Q according to the filter uF with:
    ///  O' = ((uF) * (O / Q)) * Q
    ///
    /// (Note that uF is symmetric and O/Q a vector, 
    ///  so we don't have to worry which side we multiply it on.)
    ///
    /// This vector is defined in *object* space.
    /// (Ie, a value of (1, 0, 0) means that the X axis of the object should
    /// point in the fashion that the X axis of O does)
    GETSET_DATA_FUNCS_V3(SIM_NAME_CONDIR, DOFVectorInput);

    /// Overridable get/set for Rotation and DOF of constraint
    const UT_Vector3	 getRotation() const;
    void		 setRotation(const UT_Vector3 &rotation);
    SIM_DOFType		 getDOFType() const;
    void		 setDOFType(const SIM_DOFType type);
    const UT_Vector3	 getDOFVector() const;
    void		 setDOFVector(const UT_Vector3 &vector);

    /// Retrieve the world-space position for the guide geometry
    UT_Vector3		 getGuidePosition(const SIM_Time &t) const;

    /// Retrieve the world-space orientation to constrain to.
    UT_Quaternion	 getOrientation(const SIM_Time &time) const;
    UT_Vector3		 getAngularVelocity(const SIM_Time &time) const;

    /// Retrieve number of constrained degrees of freedom.
    int			 getNumConDOFs() const;

    /// Returns a matrix that, when multiplied by a angular velocity, gives
    /// only the components of the angular velocity that are constrained.
    void		 getConDOFFilter(UT_Matrix3 &result,
				const SIM_Time &time) const;

    /// Opposite of getConDOFFilter().
    /// getUnconDOFFilter() = identity - getConDOFFilter()
    void		 getUnconDOFFilter(UT_Matrix3 &result,
				const SIM_Time &time) const;

protected:
    explicit		 SIM_ConAnchorRotational(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorRotational();

    virtual void	 buildAnchorGuideGeometrySubclass(
					const SIM_Options &options,
					const GU_DetailHandle &gdh,
					const SIM_Relationship &rel,
					const SIM_Time &t) const;

    virtual UT_Vector3		 getGuidePositionSubclass(
					const SIM_Time &time) const;

    virtual UT_Quaternion	 getOrientationSubclass(
					const SIM_Time &time) const = 0;
    virtual UT_Vector3		 getAngularVelocitySubclass(
					const SIM_Time &time) const = 0;

    virtual const UT_Vector3	getRotationSubclass() const;
    virtual SIM_DOFType		getDOFTypeSubclass() const;
    virtual const UT_Vector3	getDOFVectorSubclass() const;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_ConAnchorRotational, SIM_ConAnchor);
};

#endif

