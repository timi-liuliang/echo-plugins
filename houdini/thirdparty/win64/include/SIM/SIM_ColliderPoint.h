/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ColliderPoint_h__
#define __SIM_ColliderPoint_h__

#include "SIM_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_DMatrix4.h>
#include "SIM_Collider.h"

class GU_SDF;
class GU_RayIntersect;
class SIM_Geometry;
class SIM_Position;
class SIM_Impacts;

/// This class implements a collider that detects collisions of geometry
/// points with solid geometry. Each point in the object geometry is tested
/// for collision with the geometry attached to each affector object.
class SIM_API SIM_ColliderPoint : public SIM_Collider
{
public:
    GETSET_DATA_FUNCS_F(SIM_NAME_TOLERANCE, Tolerance);
    GETSET_DATA_FUNCS_F(SIM_NAME_VOLUMEOFFSET, VolumeOffset);
    GETSET_DATA_FUNCS_B("usepointvelocity", UsePointVelocity);
    GETSET_DATA_FUNCS_B("usesdfvelocity", UseSDFVelocity);

protected:
    /// This SIM_ColliderPoint constructor.
    explicit		 SIM_ColliderPoint(const SIM_DataFactory *factory);
    /// This SIM_ColliderPoint destructor.
    virtual		~SIM_ColliderPoint();

    /// Required override of the collision detection function.
    virtual bool	 collideObjectsCachedSubclass(SIM_Engine &engine,
					SIM_Object &object,
					SIM_Object &affector,
					const SIM_Time &starttime,
					const SIM_Time &endtime,
					SIM_ImpactApplyType impactapplytype,
					int impactflags,
					UT_ValArray<SIM_ColliderCacheData *> &objcache,
					UT_ValArray<SIM_ColliderCacheData *> &affectorcache) const;

private:
    bool		 doCollide(SIM_Object &object,
					SIM_Object &affector,
					const SIM_Time &starttime,
					const SIM_Time &endtime,
					SIM_Impacts *impacts,
					SIM_Impacts *affimpacts,
					int impactflags,
					UT_ValArray<SIM_ColliderCacheData *> &objcache,
					UT_ValArray<SIM_ColliderCacheData *> &affectorcache) const;

    /// Sends rays against the affector geometry to find the likely
    /// primitive & uv coordinates of the given hit location.
    /// affintersect should be null and is modified to allow the
    /// caching of looking up the ray intersect tree.
    void		 calculateCollisionPrimUVW(
					const SIM_Geometry &affsimgeo,
					GU_RayIntersect *&affintersect,
					const UT_Vector3 &hitpos,
					const UT_Vector3 &hitnormal,
					fpreal tolerance,
					int &primnum,
					fpreal &primu, 
					fpreal &primv,
					fpreal &primw) const;
    bool		 addCollisions(const int objid,
					const SIM_Geometry &oldsimgeo,
					const SIM_Geometry &simgeo,
					const SIM_Geometry &affsimgeo,
					const UT_DMatrix4 &oldxform,
					const UT_DMatrix4 &xform,
					SIM_Impacts *impacts,
					const int affobjid,
					const UT_DMatrix4 &oldaffixform,
					const UT_DMatrix4 &affixform,
					const UT_DMatrix4 &oldaffxform,
					const UT_DMatrix4 &affxform,
					SIM_Impacts *affimpacts,
					const SIM_Time &starttime,
					const SIM_Time &endtime,
					const GU_SDF &affsdf,
					const SIM_Object &obj,
					const SIM_Object &affobj,
					int impactflags,
					UT_ValArray<SIM_ColliderCacheData *> &objcache,
					UT_ValArray<SIM_ColliderCacheData *> &affectorcache) const;

    static SIM_DopDescription	*getPointColliderDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ColliderPoint,
			SIM_Collider,
			"Point Collider",
			getPointColliderDopDescription());
};

#endif

