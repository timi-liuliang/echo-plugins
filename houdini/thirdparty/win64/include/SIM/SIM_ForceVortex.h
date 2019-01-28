/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */


#ifndef __SIM_ForceVortex_h__
#define __SIM_ForceVortex_h__

#include "SIM_API.h"
#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <UT/UT_ThreadedAlgorithm.h>

class GU_RayIntersect;
class GU_MinInfo;

/// This is an implementation of the SIM_Force interface. This implementation
/// calculates the interpolated value from a point cloud with force and
/// torque attributes attached to it.
class SIM_API SIM_ForceVortex : public SIM_Force,
			       public SIM_OptionsUser
{
public:
    /// The name of the point attribute holding the radius values
    GETSET_DATA_FUNCS_S(SIM_NAME_RADIUS, RadiusAttribName);
    /// The name of the velocity of the orbit
    GETSET_DATA_FUNCS_S(SIM_NAME_VELOCITY, VelocityAttribName);
    /// Get the veloicty type
    GETSET_DATA_FUNCS_I(SIM_NAME_VELOCITYTYPE, VelocityType);
    /// The name of the direction of the orbit
    GETSET_DATA_FUNCS_S(SIM_NAME_DIRECTION, DirectionAttribName);
    /// Get the direction type
    GETSET_DATA_FUNCS_I(SIM_NAME_DIRECTIONTYPE, DirectionType);
    /// The drag constant for going into orbit
    GETSET_DATA_FUNCS_F(SIM_NAME_DRAGCONSTANT, DragConstant);
    /// The name of maximum distance we are allowed to search for a point.
    GETSET_DATA_FUNCS_S(SIM_NAME_MAXDISTANCE, MaxDistanceAttribName);
    /// The linear orbital density
    GETSET_DATA_FUNCS_F(SIM_NAME_DENSITY, Density);
    /// The lift force
    GETSET_DATA_FUNCS_S(SIM_NAME_LIFTFORCE, LiftForceAttribName);
    /// The falloff of lift force
    GETSET_DATA_FUNCS_F(SIM_NAME_FALLOFF, Falloff);
    /// The lift radius multiplier
    GETSET_DATA_FUNCS_F(SIM_NAME_LIFTMULT, LiftMult);
    /// Whether to sample by point, circle, or sphere.
    GETSET_DATA_FUNCS_I(SIM_NAME_SAMPLEMODE, SampleMode);

protected:
    explicit		 SIM_ForceVortex(const SIM_DataFactory *factory);
    virtual		~SIM_ForceVortex();

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
    /// Returns the forces and torques on a set of points.  The results
    /// will be same as those produced by getForceSubclass, with the
    /// exception that this method computes forces for a set of points
    /// instead of a single point.
    virtual bool	 getForceSetSubclass(const SIM_Object &object,
					const UT_Vector3Array &positions,
					const UT_Vector3Array &velocities,
					const UT_Vector3Array &angvelocities,
					const UT_FloatArray &masses,
					UT_Vector3Array &forces,
					UT_Vector3Array &torques) const;

    virtual SIM_ForceSample	getOptimalForceSamplingSubclass() const;

    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

    bool		 shouldMultiThread() const { return true; }

private:
    void 		createCircleGuide(GU_Detail *gdp,
				    const UT_Vector3 &center,
				    const UT_Quaternion &orient,
				    fpreal rad, fpreal frad,
				    int divisions,
				    const GA_RWHandleV3 &colour_attrib,
				    const UT_Vector3 &colour) const;
    UT_Vector3		getPolyOrbitalDirection(const GEO_Primitive *prim,
						int i, int j) const;
    void 		getPointData(const GEO_Primitive * &prim,
				     GU_RayIntersect &ray,
				     const UT_DMatrix4 &geoxform,
				     const GA_ROHandleV3 &dir_attrib,
				     const GA_ROHandleF &rad_attrib,
				     const GA_ROHandleF &maxd_attrib,
				     GA_Size ind, int defaultval,
				     GA_Offset &pt,
				     UT_Vector3 &x,
				     UT_Vector3 &dir,
				     fpreal &u,
				     fpreal &rad, fpreal &maxd,
				     UT_Quaternion &rot, fpreal hack) const;

    /// A threaded algorithm to compute forces on a set of points.
    THREADED_METHOD6_CONST(SIM_ForceVortex, shouldMultiThread(),
				     calculateForceSet,
				     const UT_Vector3Array &, positions,
				     const UT_Vector3Array &, velocities,
				     const UT_FloatArray &, masses,
				     UT_Vector3Array &, forces,
				     UT_Vector3Array &, torques,
				     GU_RayIntersect *, intersect)
    void		 calculateForceSetPartial(
				     const UT_Vector3Array &positions,
				     const UT_Vector3Array &velocities,
				     const UT_FloatArray &masses,
				     UT_Vector3Array &forces,
				     UT_Vector3Array &torques,
				     GU_RayIntersect *intersect,
				     const UT_JobInfo &info) const;

    static const SIM_DopDescription	*getForceVortexDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ForceVortex,
			SIM_Force,
			"Vortex Force",
			getForceVortexDopDescription());
};

#endif

