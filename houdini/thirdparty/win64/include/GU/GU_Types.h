/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Types.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_Types__
#define __GU_Types__

// Circle types
enum GU_CircleType
{
	GU_CIRCLE_CLOSED        = 0,
	GU_CIRCLE_OPEN_ARC      = 1,
	GU_CIRCLE_CLOSED_ARC    = 2,
	GU_CIRCLE_SLICED_ARC    = 3
};

// Axis
enum GU_AxisType
{
	GU_XAXIS		= 0,
	GU_YAXIS		= 1,
        GU_ZAXIS		= 2
};

// Orientation
enum GU_OrientationType
{
	GU_PLANE_XY		= 0,
	GU_PLANE_YZ		= 1,
	GU_PLANE_XZ		= 2
};

// A "group" of primitives can be swept along a curve in a number
// of ways which effect the ordering of the resulting primitives:
//
//     GU_XSECTION_ALL:   All primitives are simply placed at each of
//                   	  the backbone's points.
//     GU_XSECTION_EACH:  The first primitive is swept along the backbone,
//                    	  followed by the second, etc.
//     GU_XSECTION_CYCLE: The first primitive is placed at the first
//                        backbone point, the second primitive is placed
//                        at the second backbone point, etc.
enum GU_CrossSectionType
{
    GU_XSECTION_ALL        = 0,
    GU_XSECTION_EACH       = 1,
    GU_XSECTION_CYCLE      = 2
};

//
//  Enumeration for non-linear deformation
enum GU_DeformType
{
    GU_DEFORM_TWIST		= 0,	// Define as numbers for standard
    GU_DEFORM_BEND		= 1,	// UI usage (i.e. these can be mapped
    GU_DEFORM_SHEAR		= 2,	// to menus).
    GU_DEFORM_TAPER		= 3,
    GU_DEFORM_LINEAR_TAPER	= 4,
    GU_DEFORM_SQUASH		= 5
};

//
// Ennumeration for grid Type
enum GU_GridType
{
    GU_GRID_POLY,
    GU_GRID_MESH,
    GU_GRID_NURBS,
    GU_GRID_BEZIER
};

// Types of weights for smoothing
enum GU_WeightingType
{
    NUM_VERTICES = 0,
    EDGE_LENGTH = 1,
    SURFACE_AREA = 2
};

// Metric Types (spaces for distance measurement)
enum GU_MetricType
{
    GU_METRIC_UV = 0,
    GU_METRIC_UVW = 1,
    GU_METRIC_XYZ = 2
};

enum GU_TextureType
{
	GU_ORTHO_TEX,
	GU_POLAR_TEX,
	GU_CYL_TEX,
	GU_ROW_COL_TEX,
	GU_FACE_TEX,
	GU_MODIFY_TEX,
	GU_UNIFORM_SPLINE_TEX,
	GU_GREVILLE_SPLINE_TEX,
	GU_CHORD_SPLINE_TEX,
	GU_PERSPECTIVE_TEX
};

// Lattice interpolation types
enum GU_LatticeType
{
    GU_LATTICE_LINEAR = 0,
    GU_LATTICE_BEZIER = 1
};

///
// Defines
///

#define GU_POLY_CLOSED		0
#define GU_POLY_OPEN		1
#define GU_WRAPU		1
#define GU_WRAPV		1

#define GU_HIDDEN_3D_PRIMS_GROUP	"_3d_hidden_primitives"

#endif
