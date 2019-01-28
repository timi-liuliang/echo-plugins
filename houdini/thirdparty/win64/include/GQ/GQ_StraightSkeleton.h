/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Straight skeleton construction tool
 *
 */

#ifndef __GQ_StraightSkeleton_h__
#define __GQ_StraightSkeleton_h__

#include "GQ_API.h"
#include <GA/GA_Edge.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Pair.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Error.h>

class GU_Detail;
class GQ_Detail;
class GQ_Point;
class GQ_Edge;
class GQ_Face;
class GA_PrimitiveGroup;
class GA_PointGroup;
class UT_ErrorManager;

/// These internal helper classes are named generically enough that they could
/// potentially conflict with classes in other files.  We don't want to expose
/// their implementation, and we can't forward declare embedded helper classes,
/// so we resort to wrapping them in a name space as the next best thing.
namespace GQ_StraightSkeletonImpl {
}

/// Straight skeleton computation for general polygonal figures in a 2D
/// plane.
///
/// The algorithm used is a kinetic triangulation, originally described by
/// Aichholzer, Aurenhammer in "Straight Skeletons for General Polygonal
/// Figures in the Plane", with some implementation details filled in by
/// Palfrader, Held and Huber in "On Computing Straight Skeletons by Means
/// of Kinetic Triangulations".
///
/// We use GQ_PolyDelaunay internally for the initial triangulation.

class GQ_API GQ_StraightSkeleton
{
public:
    /// Some local typedefs for our helper classes to avoid having to fully
    /// qualify the names everywhere.

    explicit	     GQ_StraightSkeleton();
    virtual	    ~GQ_StraightSkeleton();

    /// Set the plane where triangulation happens.
    void	     setPlane(const UT_Vector3 &normal, fpreal distance);
    /// Fit a plane to the input points. Returns true if it succeeded.
    bool	     fitPlane(const GU_Detail *gdp,
			      const GA_PointGroup *pointGroup = 0,
			      UT_Vector3 *outNormal = 0,
			      fpreal *outDistance = 0);

    /// How to determine inside/outside from the input graph.
    enum InOutType
    {
	/// The vertex order of any primitives referencing an edge determines
	/// which side of the edge is considered the inside or the outside.
	///
	/// NB: This is the fallback method if any of the other ones fail.
	VERTEX_ORDER = 0,

	/// Outermost edge loops, if any, separate the outside region from the
	/// inside(s).
	SIMPLE_REACHABILITY,

	/// A nested separation of the plane into outside and inside regions.
	ALTERNATING_REACHABILITY,

	/// A nested separation of the plane into outside and inside regions
	/// with inside regions extending through shared edges.
	ALTERNATING_REACHABILITY_WITH_SHARED_EDGES
    };

    void	     setInOutType(InOutType type) { myInOutType = type; }
    InOutType	     getInOutType() const { return myInOutType; }

    /// Assigning attributes to use for edge weights turns this into a
    /// weighted straight skeleton.  We only support positive weighted
    /// straight skeletons, ignoring any zero or negative edge weights
    /// and replacing them with a weight of 1.
    ///
    /// Edge weights can be queried from a vertex, point, or primitive
    /// attribute, in that order of priority.
    void	     setInsideWeightAttrib(const char *attrib)
					{ myInsideWeightAttrib = attrib; }
    const UT_StringHolder &getInsideWeightAttrib() const
					{ return myInsideWeightAttrib; }
    void	     setOutsideWeightAttrib(const char *attrib)
					{ myOutsideWeightAttrib = attrib; }
    const UT_StringHolder &     getOutsideWeightAttrib() const
					{ return myOutsideWeightAttrib; }

    /// The tolerance used to identify coincident points during triangle
    /// collapses.
    void	     setDistanceTol(fpreal64 tol);
    fpreal64	     getDistanceTol() const { return myDistanceTol; }

    /// The tolerance used to identify parallel edges to detect infinitely
    /// fast moving wavefront points during triangle collapsed.  Bisectors
    /// of edges with angles (in radians) less than this will be considered
    /// infinitely fast.
    void	     setParallelismTol(fpreal64 tol);
    fpreal64	     getParallelismTol() const { return myParallelTol; }

    void	     computeSkeleton(GU_Detail *gdp,
				     const GA_PrimitiveGroup *prim_group);
    UT_ErrorSeverity getSkeletonComputationErrorSeverity();
    void	     getSkeletonComputationErrors(UT_ErrorManager &errors);

    enum OffsetGeoType
    {
	OFFSET_CURVES = 0,
	OFFSET_SURFACES = 1
    };

    /// The distance parameter is actually interpreted as the time during the
    /// wavefront propagation.  This is the same thing for skeletons computed
    /// without local edge weights and, for skeletons with local edge weights,
    /// where our interest really lies.
    void	     buildOffsetGeometry(GU_Detail *gdp, OffsetGeoType type,
					 fpreal64 distance, int ndivs,
					 bool inside, bool outside,
					 bool keep_input,
					 const GA_PrimitiveGroup *input_group,
					 GA_PrimitiveGroup *inside_group,
					 GA_PrimitiveGroup *outside_group,
					 bool split_curves_to_omit_end_caps,
					 const char *edge_dist_attrib_name,
					 const char *edge_speed_attrib_name,
					 bool updatePointNormals = true);

    /// Project 3D point onto plane.
    UT_Vector2	     make2D(const UT_Vector3 &pos) const;
    UT_Vector3	     make3D(const UT_Vector2 &pos) const;

private:
    class	 Skeleton;

    /// Set up my*Transform member variables from the plane
    /// normal/distance.
    void	     calcTransforms(const UT_Vector3 &normal, fpreal distance);

    /// Transform from 3D space to plane
    UT_Matrix4	     my3to2Transform;
    /// Transform from plane to 3D space
    UT_Matrix4	     my2to3Transform;
    UT_Vector3	     myPlaneNormal;
    fpreal	     myPlaneDistance;

    InOutType	     myInOutType;
    fpreal64	     myDistanceTol;
    fpreal64	     myParallelTol;

    UT_StringHolder  myInsideWeightAttrib;
    UT_StringHolder  myOutsideWeightAttrib;

    Skeleton	    *mySkel;
};

#endif
