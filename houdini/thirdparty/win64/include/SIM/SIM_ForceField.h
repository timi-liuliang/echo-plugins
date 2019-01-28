/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceField_h__
#define __SIM_ForceField_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"
#include <UT/UT_Matrix.h>
#include <GA/GA_Handle.h>

/// This is an implementation of the SIM_Force interface. This implementation
/// calculates the interpolated value from a point cloud with force and
/// torque attributes attached to it.
class SIM_API SIM_ForceField : public SIM_Force,
			       public SIM_OptionsUser
{
public:
    /// The name of the point attribute holding the force values.
    GETSET_DATA_FUNCS_S(SIM_NAME_FORCEATTRIBNAME, ForceAttribName);
    /// Scale the calculated force value.
    GETSET_DATA_FUNCS_F(SIM_NAME_FORCESCALE, ForceScale);
    /// The name of the point attribute holding the torque values.
    GETSET_DATA_FUNCS_S(SIM_NAME_TORQUEATTRIBNAME, TorqueAttribName);
    /// Scale the calculated torque value.
    GETSET_DATA_FUNCS_F(SIM_NAME_TORQUESCALE, TorqueScale);
    /// Determines whether or not there is a maximum distance to check.
    GETSET_DATA_FUNCS_B(SIM_NAME_USEMAXDISTANCE, UseMaxDistance);
    /// The maximum distance we are allowed to search for a point.
    GETSET_DATA_FUNCS_F(SIM_NAME_MAXDISTANCE, MaxDistance);
    /// Whether to treat the force as a wind or a force.
    GETSET_DATA_FUNCS_B(SIM_NAME_TREATASWIND, TreatAsWind);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceField(const SIM_DataFactory *factory);
    virtual		~SIM_ForceField();

    /// Returns the force and torque on a point. The returned values are
    /// exact copies of the force and torque attributes. The
    /// getForceOnCircleSubclass() and getForceOnSphereSubclass()
    /// functions use the default implementations which scale the point
    /// force by the area or volume of the circle or sphere. This means
    /// that regardless of the size of samples used to quantize an object,
    /// the total force on the object should remain roughly constant.
    virtual void	 getForceSubclass(const SIM_Object &object,
					const UT_Vector3 &position,
					const UT_Vector3 &velocity,
					const UT_Vector3 &angvel,
					const fpreal mass,
					UT_Vector3 &force,
					UT_Vector3 &torque) const;
    virtual SIM_ForceResolver *getForceResolverSubclass(const SIM_Object &object) const;

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

    /// We want to use area sampling if treat as wind is on.
    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;
    
    /// Override this to return a SIM_GuideShared so we only have to draw
    /// our guide geometry once regardless of how many objects we are
    /// attached to.
    virtual SIM_Guide	*createGuideObjectSubclass() const;
    /// Build guide geometry by copying the points from the source geometry,
    /// and adding lines to represent the force and torque vectors.
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:
    void		 createVectorGuide(GU_Detail *gdp,
					   const UT_Vector3 &pos,
					   UT_Vector3 f,
					   fpreal scale,
					   const GA_RWHandleV3 &colour_attrib,
					   const UT_Vector3 &colour) const;
    void		 getAttribOffsets(const GU_Detail *gdp,
					  const char *forceattribname,
					  GA_ROHandleV3 &force_attrib,
					  const char *torqueattribname,
					  GA_ROHandleV3 &torque_attrib) const;
    void		 getPositionTransform(UT_DMatrix4 &xform) const;
    void		 getPositionInverseTransform(UT_DMatrix4 &ixform) const;

    static const SIM_DopDescription	*getForceFieldDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceField,
			SIM_Force,
			"Field Force",
			getForceFieldDopDescription());

    friend class SIM_ForceResolverField;
};

#endif

