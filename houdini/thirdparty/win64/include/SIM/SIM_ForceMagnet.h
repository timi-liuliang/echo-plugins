/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ForceMagnet_h__
#define __SIM_ForceMagnet_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

/// This is an implementation of the SIM_Force interface. This implementation
/// calculates the interpolated value from a point cloud with force and
/// torque attributes attached to it.
class SIM_API SIM_ForceMagnet : public SIM_Force,
				public SIM_OptionsUser
{
public:
    /// Scale the calculated force value.
    GETSET_DATA_FUNCS_F(SIM_NAME_FORCESCALE, ForceScale);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceMagnet(const SIM_DataFactory *factory);
    virtual		~SIM_ForceMagnet();

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
    static const SIM_DopDescription	*getForceMagnetDopDescription();
    
    // Builds the transform which goes from the geometry space to the
    // world space.
    void                 getGeometryToWorldTransform(UT_DMatrix4 &xform) const;
    // Builds the inverse transform which moves the world space into
    // the geometry space.
    void		 getWorldToGeometryTransform(UT_DMatrix4 &xform) const;
    
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceMagnet,
			SIM_Force,
			"Magnet Force",
			getForceMagnetDopDescription());
};

#endif

