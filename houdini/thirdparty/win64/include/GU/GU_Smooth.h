/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:     GU_Smooth.h (GU Library, C++)
 *
 * COMMENTS: Declarations of functions for smoothing meshes
 */

#pragma once

#include "GU_API.h"
#include <GA/GA_ElementGroup.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_Map.h>

class GA_Attribute;
class GA_Detail;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_Range;

/// Returns true iff the attribute is able to be smoothed,
/// (i.e. a point or vertex numeric attribute), and the
/// function is not interrupted.
/// primgroup is used to determine the point range and connectivity.
GU_API bool GUsmooth(
    GA_Attribute *attrib,
    float frequency,
    int iterations,
    const GA_PrimitiveGroup *primgroup = NULL);

/// Returns true iff the attribute is able to be smoothed,
/// (i.e. a point or vertex numeric attribute), and the
/// function is not interrupted.
/// primgroup is only used if the attribute is a vertex attribute.
GU_API bool GUsmooth(
    GA_Attribute *attrib,
    const GA_Range &pointrange,
    float frequency,
    int iterations,
    const UT_Array<GA_OffsetArray> &ringzeropoints,
    const GA_PrimitiveGroup *primgroup = 0);

/// Like the GUsmooth above, except with weights specified explicitly,
/// instead of using frequency.
GU_API bool GUsmooth(
    GA_Attribute *attrib,
    const GA_Range &pointrange,
    float weight1,
    float weight2,
    int iterations,
    const UT_Array<GA_OffsetArray> &ringzeropoints,
    const GA_PrimitiveGroup *primgroup = 0);

enum GU_SmoothWeightMethod
{
    GU_SMOOTH_WEIGHT_UNIFORM,	    // 1.0 aka "umbrella" weights
    GU_SMOOTH_WEIGHT_EDGE_LENGTH,   // inverse edge length aka
				    // "scale dependent umbrella" weights
//    GU_SMOOTH_WEIGHT_COTAN,	    // 0.5*sum_angles_facing_edge aka
				    // "mean curvature flow"
    GU_SMOOTH_WEIGHT_N		    // sentinel
};

GU_API bool
GUsmoothComputeEdgeWeights(
	UT_Array<UT_Array<fpreal>>& edge_weights,
	const GU_SmoothWeightMethod method,
	const GA_Attribute& pt_attrib,
	const GA_Range& pt_range,
	const UT_Array<GA_OffsetArray>& pt_ringzero);

struct GU_API GU_SmoothParms
{
    int					myIterations;
    fpreal				myStepSize;
    GU_SmoothWeightMethod		myWeightMethod;
    const UT_Array<GA_OffsetArray> *	myRingZero;	// optional
    const UT_Array<UT_Array<fpreal>> *	myEdgeWeights;	// optional

    explicit
    GU_SmoothParms(
	    int iterations = 1,
	    fpreal step_size = 1.0,
	    GU_SmoothWeightMethod method = GU_SMOOTH_WEIGHT_UNIFORM,
	    const UT_Array<GA_OffsetArray> *ring_zero = nullptr,
	    const UT_Array<UT_Array<fpreal>> *edge_weights = nullptr
	    )
	: myIterations(iterations)
	, myStepSize(step_size)
	, myWeightMethod(method)
	, myRingZero(ring_zero)
	, myEdgeWeights(edge_weights)
    {
    }
};

/// Smooth a point V3 attribute using simple laplacian smoothing via average of
/// point neighbours. This method is faster but causes shrinkage.
GU_API bool GUsmoothLaplace(
    GA_Attribute &pt_attrib,
    const GU_SmoothParms &parms,
    const GA_PointGroup *pt_group = nullptr,
    GA_Attribute *work_attrib = nullptr);

// Strategy for implicit smoothing
enum GU_ImplicitSmoothMethod
{
    GU_IMPLICIT_SMOOTH_METHOD_UNIFORM = 0,
    GU_IMPLICIT_SMOOTH_METHOD_SCALE_DOMINANT,
    GU_IMPLICIT_SMOOTH_METHOD_CURVATURE_DOMINANT
};

// Bitmask for boundary type
enum GU_SmoothBoundaryType {
    GU_SMOOTH_BOUNDARY_NONE      = 0,
    GU_SMOOTH_BOUNDARY_SURFACE   = 1, // 0001
    GU_SMOOTH_BOUNDARY_SELECTION = 2, // 0010
    GU_SMOOTH_BOUNDARY_ALL       = 3  // 0011
};
typedef unsigned int GU_SmoothBoundaryMask;

struct GU_API GU_ImplicitSmoothParms
{
    // The magnitude of smoothing in the range [0, infty) to apply to the mesh.
    // The higher this value, the more smoothing is applied.
    fpreal64 myStrength;

    // The exponent of the Laplacian matrix that is used to fair the mesh in the
    // range [1, infty). The higher this exponent, the closer to a low-pass
    // filter this operation becomes and thus the more details are preserved.
    // Computation time is significantly increased by increasing this number so
    // in practice we limit this to small values.
    int myFilterQuality;

    // The smoothing algorithm to apply.
    GU_ImplicitSmoothMethod myMethod;

    // The mode of boundary preservation to use.
    GU_SmoothBoundaryMask myBoundaryMask;

    // Points/vertices in this group will be held in place. If this is not a
    // GA_PointGroup/GA_VertexGroup, then it will be converted to a
    // GA_PointGroup. This parameter is optional.
    const GA_Group *myConstrainGroup;

    // A map from points/vertices to a float value in the range [0, infty). The
    // weight will scale how much its respective point is moved; i.e. a point
    // with a higher weight will be smoothed more than a point with a lower one.
    // If this is provided and a point is not in the map, its weight is assumed
    // to be 0.
    const UT_Map<GA_Offset, fpreal> *myWeights;

    GU_ImplicitSmoothParms(
        fpreal64 strength = 1.0,
        int filterQuality = 2,
        GU_ImplicitSmoothMethod method = GU_IMPLICIT_SMOOTH_METHOD_UNIFORM,
        GU_SmoothBoundaryMask boundaryMask = GU_SMOOTH_BOUNDARY_ALL,
        const GA_Group *constrainGroup = NULL,
        const UT_Map<GA_Offset, fpreal> *weights = NULL
    )
    : myStrength(strength)
    , myFilterQuality(filterQuality)
    , myMethod(method)
    , myBoundaryMask(boundaryMask)
    , myConstrainGroup(constrainGroup)
    , myWeights(weights)
    {}
};

// Performs implicit fairing on the points/vertices contained in the given
// element group for the given list of point/vertex attributes.
// Points/vertices/edges in the constraingroup will be held in place.
// The type of the group determines how boundary type is computed. For
// primitive/vertex groups, a point is a selection boundary point only if it has
// a vertex not in the group. For other group types, a point is a selection
// boundary point only if it has a neighbour not in the group.
GU_API void GUimplicitSmooth(
    UT_Array<GA_Attribute *> &attribs,
    const GU_ImplicitSmoothParms &parms,
    const GA_Group *group);

// Performs implicit fairing on the given attribute and writes the changes
// to the given map. The same boundary type rules for the group from above
// applies here.
GU_API void GUimplicitSmooth(
    const GA_Attribute &attrib,
    const GU_ImplicitSmoothParms &parms,
    const GA_Group *group,
    UT_Map<GA_Offset, UT_Vector3> &edits);
