/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SweptCollisionUtility_h__
#define __SIM_SweptCollisionUtility_h__

#include "SIM_API.h"
#include "SIM_SweptCollisionData.h"
#include <UT/UT_VectorTypes.h>
#include <GA/GA_PageHandle.h>
#include <GU/GU_DetailHandle.h>
#include <vector>

class SIM_Engine;
class SIM_Object;
class SIM_Impacts;
class SIM_Geometry;
class SIM_GeometryCopy;
class SIM_Time;
class GU_Detail;
class GU_PrimPacked;

//////////////////////////////////////////////////////////////////////////////
//
// Data structures for merging geometry from an array of SIM_Objects
//
//////////////////////////////////////////////////////////////////////////////

inline bool SIMisValidIndex(const int i, const int n)
{
    return (0 <= i) && (i < n);
}

// Identifies a point:
// the object index it belongs to and the point-id for that geometry
struct SIM_API SIM_MergedIdentityPoint
{
    SIM_MergedIdentityPoint() :
        myIndexObject(-1),
        myPid(-1)
    {
    }

    // Index in std::vector of SIM_Object
    int myIndexObject;
    // point-id relative to object
    int myPid;
};

// MergedPoints represents a set of points that belong to a sequence of objects
class SIM_API SIM_MergeMap
{
public:
    // default construct: merge map for zero objects
    SIM_MergeMap() :
        myIdentities(),
        myRanges()
    {
    }

    // Number of merged points (total for all objects)
    inline int getNumPointsMerged() const
    {
        return myIdentities.size();
    }

    // For a merged point, obtain the object index and object point index (pid)
    void getObjectAndPid(int& index_object, int& pid, const int p) const
    {
        UT_ASSERT( (0 <= p) && (p < myIdentities.size()) );

        const SIM_MergedIdentityPoint& id(myIdentities[p]);
        index_object = id.myIndexObject;
        pid = id.myPid;
    }

    // Return the number of objects for this merge map.
    // Each object index for this merge map lies in the half-open range
    // [0, getNumObjects() - 1)
    inline int getNumObjects() const
    {
        return myRanges.size();
    }

    // Number of points for a object, given the object index
    inline int getNumPointsObject(const int index_object) const
    {
        if( !SIMisValidIndex(index_object, myRanges.size()) )
        {
            UT_ASSERT( !"not a valid object index!" );
            return 0;
        }

        const std::pair< int, int >& object_range(myRanges[index_object]);

        const int num_points_range(object_range.second - object_range.first);

        UT_ASSERT( num_points_range >= 0 );

        return num_points_range;
    }

private:
    // Joint array of point identities for object sequence
    std::vector<SIM_MergedIdentityPoint> myIdentities;
    // Contiguous sub-range [begin, end) of indices into myIdentities
    // for each geometry index
    std::vector< std::pair< int, int > > myRanges;

    friend void SIM_API
    SIMmergePoints(
        SIM_MergeMap& merged_points,
	const UT_Array<const GU_Detail *> &gdps
    );

    friend class SIM_MergeMapObject;

    // Disallow
    SIM_MergeMap(const SIM_MergeMap&);
    SIM_MergeMap& operator=(const SIM_MergeMap&);
};

// Get a map for a fixed object that maps local point indices (pids)
// to indices for the merged points.
// This map is valid only as long as the SIM_MergeMap object
// that it was created from exists unchanged.
class SIM_API SIM_MergeMapObject
{
public:
    SIM_MergeMapObject(
        const SIM_MergeMap& merged_points,
        const int index_object
    ) :
        myIdentities(merged_points.myIdentities),
        myBegin(merged_points.myRanges[index_object].first),
        myEnd(merged_points.myRanges[index_object].second)
    {
        UT_ASSERT( 0 <= index_object );
        UT_ASSERT( SIMisValidIndex(index_object, merged_points.myRanges.size()) );
    }

    // Get merged point index from object point index (pid)
    // Return -1 if the pid isn't merged.
    inline int getP(const int pid) const
    {
        UT_ASSERT( (0 <= pid) && (pid < myEnd - myBegin) );

        return myBegin + pid;
    }

    inline int getNumPointsObject() const
    {
        return myEnd - myBegin;
    }

    inline int getNumPointsMerged() const
    {
        return myEnd - myBegin;
    }

private:
    const std::vector<SIM_MergedIdentityPoint>& myIdentities;
    const int myBegin;
    const int myEnd;

    // Disallow
    SIM_MergeMapObject(const SIM_MergeMapObject&);
    SIM_MergeMapObject operator=(const SIM_MergeMapObject&);
};

/// Unpacks any packed primitives in the given SIM_Geometry and stores the
/// original primitive index of each primitive (e.g. for correctly recording
/// impacts).
class SIM_API SIM_UnpackedGeometry
{
public:
    SIM_UnpackedGeometry() {}
    SIM_UnpackedGeometry(const GU_Detail &gdp);
    SIM_UnpackedGeometry(const SIM_Geometry &simgeo);

    /// Returns the unpacked geometry.
    inline GU_ConstDetailHandle getGeometry() const
    {
        return myGeo;
    }

    /// Returns the primitive index in the source geometry that the given
    /// primitive in the unpacked geometry corresponds to.
    inline GA_Index getOriginalPrimIndex(GA_Index i) const
    {
        return myOriginalPrims(i);
    }

private:
    void setGeometry(const GU_ConstDetailHandle &gdh);

    GU_ConstDetailHandle myGeo;
    GA_IndexArray myOriginalPrims;
};

//////////////////////////////////////////////////////////////////////////////
//
// Extract merged position, velocity, mass and thickness from 
// an array of SIM_Objects
//
//////////////////////////////////////////////////////////////////////////////

// Construct merged array of points for an array of geometries.
void SIM_API
SIMmergePoints(
    SIM_MergeMap& merged_points,
    const UT_Array<const GU_Detail *> &gdps
);

/// Extract the triangle connectivity for a single object.
void SIM_API
SIMextractTriangleConnectivityMerged(
    SIM_TriangleConnectivity &triangles,
    const SIM_MergeMapObject &merged_points,
    const SIM_UnpackedGeometry &unpacked_geometry,
    const char *geopath
);

// Extract the geometry and the corresponding transform from an object.
// The resulting (transform, geometry) pairs can be used with 
// SIMextractPositionPoints and SIMextractPositionTriangles
extern bool SIM_API
SIMgetGeometryAndTransform(
    const SIM_Geometry*& geometry,
    UT_DMatrix4& geometry_to_sim,
    const SIM_Object& object
);

// Extract the geometry and the corresponding transform
// that correspond to time t. Return false if this information
// couldn't be extracted from updated_object.
// This assumes that substep objects are set up correctly.
// (This is not the case for particle fluid objects, which
// store the substep positions in the final end state object).
// The resulting (transform, geometry) pairs can be used with 
// SIMextractPositionPoints and SIMextractPositionTriangles
extern bool SIM_API
SIMgetGeometryAndTransformAtTime(
    const SIM_Geometry*& geometry,
    UT_DMatrix4& geometry_to_sim,
    const SIM_Engine& engine,
    const SIM_Object& undated_object,
    const SIM_Time& t
);

// Extract merged point thickness array for a set of point-set objects
// (e.g., particle objects). The thickness is taken from the "pscale"
// attribute, if this attribute is present. If there is no "pscale" attribute,
// then default_thickness is used.
extern void SIM_API
SIMreadThicknessPoints(
    std::vector<fpreal32>& thickness_points,
    const SIM_MergeMap& merged_points, 
    const UT_Array<const GU_Detail *> &gdps_end,
    const fpreal default_thickness 
);

// Read the bounce parameter for an object
extern fpreal SIM_API
SIMreadBounceParameterObject(
    const SIM_Object& object,
    const fpreal default_bounce
);

// Read the friction parameter for an object
extern fpreal SIM_API
SIMreadFrictionParameterObject(
    const SIM_Object& object,
    const fpreal default_friction
);

// Read the bounceforward parameter for an object
extern fpreal SIM_API
SIMreadBounceForwardParameterObject(
    const SIM_Object& object,
    const fpreal default_bounceforward
);

//////////////////////////////////////////////////////////////////////////////
//
// Extract merged swept-collision state from an array of SIM_Objects
//
//////////////////////////////////////////////////////////////////////////////

class SIM_Position;
class SIM_Object;
class SIM_Engine;

//
// SIMreadSweptState reads from a set of objects all the topological and
// spatial data that is needed to perform swept-collision detection for
// all these objects.
// The data returned by this functions matches the geometry's end topology
// (the topology associated with t_end).
// (points and primitives may be added/deleted between t_begin and t_end).
// The "id" point-attribute will be used to carry over position data between
// the possibly different start and end topologies.
//
// This function will work correctly *only* if the substep objects have been
// set up correctly: 
// It assumes that
// engine.getObjectAtTime(object, t_start) and
// engine.getObjectAtTime(object, t_end) 
// return the correct states for the substep times t_start and t_end,
// respectively.
// (This assumption is broken by the particle fluid solver, which
// stores the substep state in the end state for optimization purposes.
// Therefore, the particle fluid solver does not use this function;
// it has a special code path to get the right information.)
//
// The topological and spatial data that is returned by SIMreadSweptState
// can be used as inputs for the function SIMdetectAndResolveCollisions,
// which detects and resolves collisions between two merged sets
// of geometry.
// The "pscale" attribute is used to initialize the thickness for each point.
// If this attribute is absent, then default_thickness is used
//
extern void SIM_API
SIMreadSweptState(
    SIM_MergeMap& merged_points,
    std::vector<SIM_UnpackedGeometry> &unpacked_geometries_end,
    std::vector<int>& id_objects,
    std::vector<fpreal32> *thickness_points,
    std::vector<fpreal32> *bounce_points,
    std::vector<fpreal32> *friction_points,
    std::vector<fpreal32> *bounceforward_points,
    SIM_PositionPoints& position_points_start,
    SIM_PositionPoints& position_points_end,
    SIM_VelocityPoints *velocity_points_end,
    const SIM_Engine& engine,
    const std::vector<const SIM_Object*>& objects,
    const fpreal default_thickness,
    const fpreal64 t_start,
    const fpreal64 t_end
);

// This version reads from geometry.
extern void SIM_API
SIMreadSweptState(
    SIM_MergeMap& merged_points,
    SIM_UnpackedGeometry &unpacked_geo_end,
    std::vector<fpreal32> *thickness_points,
    SIM_PositionPoints& position_points_start,
    SIM_PositionPoints& position_points_end,
    SIM_VelocityPoints *velocity_points_end,
    const GU_Detail *gdp_start,
    const UT_DMatrix4 &geoxform_start,
    const GU_Detail *gdp_end,
    const UT_DMatrix4 &geoxform_end,
    const fpreal default_thickness,
    const fpreal64 t_start,
    const fpreal64 t_end
);

// This version reads from a live object.  It uses the previous
// position attribute to get the start positions rather than
// relying on the cache.
// You should provide arange.  The page handles will be set up
// for you.
extern bool SIM_API
SIMreadSweptState(
    const SIM_Object *object,
    GU_Detail *gdp,
    GA_Range &arange,
    GA_ROPageHandleF *thickness_points,
    GA_ROPageHandleF *bounce_points,
    GA_ROPageHandleF *friction_points,
    GA_ROPageHandleF *dynamicfriction_points,
    GA_ROPageHandleF *bounceforward_points,
    GA_ROPageHandleF *mass_points,
    GA_RWPageHandleV3 &position_points_start,
    GA_RWPageHandleV3 &position_points_end,
    GA_RWPageHandleV3 *velocity_points_end,
    fpreal default_thickness);

/// Extract the triangle connectivity for a set of objects.
extern void SIM_API
SIMextractTriangleConnectivityMerged(
    SIM_TriangleConnectivity& triangles,
    const SIM_MergeMap& merged_points,
    const std::vector<SIM_UnpackedGeometry>& unpacked_geometries,
    const std::vector<const SIM_Object*>& objects
);

//////////////////////////////////////////////////////////////////////////////
//
// Read/Write merged position and merged velocity for an array
// of SIM_Objects
//
//////////////////////////////////////////////////////////////////////////////

// Read the merged position sequence for a set of points
// that belong to geometries.
extern void SIM_API
SIMreadPositionPoints(
    SIM_PositionPoints& position_points, 
    const SIM_MergeMap& merged_points,
    const std::vector<UT_DMatrix4>& transforms,
    const UT_Array<const GU_Detail *> &gdps
);

// Read the merged velocity sequence
// for a set of points that belong to geometries.
// Use the velocity attribute for this
extern void SIM_API
SIMreadVelocityPointsFromAttribute(
    SIM_VelocityPoints& velocity_points, 
    const SIM_MergeMap& merged_points,
    const std::vector<UT_DMatrix4>& transforms,
    const UT_Array<const GU_Detail*> &gdps
);

// Set the position and velocity for all points in an array of geometries.
// position_points is written to the point positions
// velocity_points is written to the velocity attribute "v"
// it is assumed that for geometry[i], the map from object space to
// world simulation space is given by transforms[i].
// The a merged positions and velocities are transformed into 
// the geometry's object space when they're applied to a point.
extern void SIM_API
SIMwritePositionAndVelocityPoints(
    const std::vector<SIM_GeometryCopy*>& geometries,
    const SIM_MergeMap& merged_points,
    const SIM_PositionPoints& position_points,
    const SIM_VelocityPoints& velocity_points,
    const std::vector<UT_DMatrix4>& transforms
);

// Read the previous positions for all objects from the previous-position
// attribute. The attribute stores the position in world coordinates
// (no transform). Elements in r_points for points that don't have
// the attribute will retain the values from before the call.
extern void SIM_API
SIMreadPreviousPositionAttribute(
    SIM_PositionPoints& position_points,
    const SIM_MergeMap& merge_map, 
    const std::vector<const SIM_Geometry*>& geometries_end
);

// Store the previous positions for all objects to the previous-position
// attribute. The attribute stores the position in world coordinates
// (no transform). Elements in r_points for points that don't have
// the attribute will not have their values written.
extern void SIM_API
SIMwritePreviousPositionAttribute(
    std::vector<SIM_GeometryCopy*>& geometries_end,
    const SIM_MergeMap& merge_map,
    const SIM_PositionPoints& position_points
);

// Copy the given position attribute into the previous position
// attribute wherever the exist flag is false.  Then set the exist
// flag to true everywhere.
// pos_ph should be initialized to your position attribute.
// prevpos_ph will be created to point to the previous position.
extern void SIM_API
SIMcreatePreviousPositionAttribute(
    GU_Detail *gdp,
    GA_RWPageHandleV3 &prevpos_ph,
    GA_RWPageHandleV3 &pos_ph
);

// Copies the given position attribute blindly into the
// previous position attribute.
extern void SIM_API
SIMcopyAttribute(
    GU_Detail *gdp,
    GA_RWPageHandleV3 &prevpos_ph,
    GA_RWPageHandleV3 &pos_ph
);

//////////////////////////////////////////////////////////////////////////////
//
// Detect collisions, resolve collisions, and generate impacts for 
// an array of SIM_Objects.
//
//////////////////////////////////////////////////////////////////////////////

// This function detects collisions and generates impacts for
// a pair of objects (a, b).
// that aim to resolve the collisions.
extern bool SIM_API
SIMdetectCollisionsAndGenerateImpulses(
    SIM_Impacts* impacts_a,
    SIM_Impacts* impacts_b,
    const SIM_Engine& engine,
    SIM_Object*const undated_object_a,
    const SIM_Object*const undated_object_b,
    const SIM_Time& t_start,
    const SIM_Time& t_end,
    const int impact_flags,
    const bool pscaleisradius
);

#endif
