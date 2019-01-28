/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimitiveTypes.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimitiveTypes__
#define __GT_PrimitiveTypes__

#include "GT_API.h"

typedef enum {
    GT_PRIM_UNDEFINED = -1,

    // A collection of primitives
    GT_PRIM_COLLECT,		// GT_PrimCollect

    // A detail collection (GU_Detail)
    GT_PRIM_DETAIL,

    // Simple GEO primitives
    GT_GEO_PRIMITIVE,
    GT_GEO_PRIMTPSURF,
    GT_GEO_PACKED,		// Packed prim
    GT_GEO_PRIMLIST,		// List of GEO primitives

    // Meshes of simple GEO primitives
    GT_GEO_PRIMTPSURF_MESH,

    // Simple primitives
    GT_PRIM_CURVE,		// Single open polygon or cubic curve
    GT_PRIM_POLYGON,		// Single closed polygon
    GT_PRIM_PATCH,		// Single linear or cubic patch primitive
    GT_PRIM_NUPATCH,		// A NURBS Patch

    // Meshes of simple primitives
    GT_PRIM_CURVE_MESH,		// Soup of linear or cubic curves
    GT_PRIM_POLYGON_MESH,	// Polygon soup
    GT_PRIM_PATCH_MESH,		// Mesh of patch primitives
    GT_PRIM_SUBDIVISION_MESH,	// Polygon soup as subdivision surface
    GT_PRIM_SUBDIVISION_CURVES,	// Curves as subdivision curves
    GT_PRIM_TRIANGLE_STRIP,	// Triangle strip (specialized patch mesh)
    GT_PRIM_TRIANGLE_FAN,	// Triangle fan (specialized polygon mesh)

    // NURBS primitives
    GT_PRIM_NURBS_CURVE,	// Single NURBS curve
    GT_PRIM_NURBS_SURFACE,	// NURBS surface

    // Meshes of NURBS primitives
    GT_PRIM_NURBS_CURVE_MESH,	// Soup of NURBS curves

    // Quadric primitives
    GT_PRIM_CIRCLE,		// Primitive circle
    GT_PRIM_SPHERE,		// Primitive sphere
    GT_PRIM_TUBE,		// Primitive tube
    GT_PRIM_HYPERBOLOID,	// Primitive hyperboloid

    // Point-based primitives
    GT_PRIM_POINT_MESH,		// Point set
    GT_PRIM_PARTICLE,		// Particle primitive

    // Metaballs
    GT_PRIM_METAEXPR,		// Metaball expression

    // Volume primitives
    GT_PRIM_VOXEL_VOLUME,	// Simple volume primitive
    GT_PRIM_VDB_VOLUME,		// VDB volume primitive
    GT_PRIM_TET_MESH,		// Tetrahedrons

    // Bezier patch (not a Bezier surface)
    GT_PRIM_BEZIER_PATCH,

    // Quadric meshes (collections of quadrics)
    GT_PRIM_CIRCLE_MESH,
    GT_PRIM_SPHERE_MESH,
    GT_PRIM_TUBE_MESH,
    GT_PRIM_HYPERBOLOID_MESH,

    GT_PRIM_CAPTURE_REGIONS,	// Capture region pills

    GT_PRIM_INSTANCE,		// Instance primitive
    GT_PRIM_FRAGMENTS,		// Fragments primitive
    GT_PRIM_AGENTS,		// Collection of Agents 
    GT_PRIM_AGENT_SHAPE,	// Instances of an agent shape

    GT_PRIM_ALEMBIC_ARCHIVE,	// Collection of shapes and instances
    GT_PRIM_ALEMBIC_SHAPE,	// Single alembic geometry shape
    GT_PRIM_ALEMBIC_SHAPE_MESH,	// Multiple alembic shapes combined into one
    GT_PRIM_ALEMBIC_INSTANCE,	// Instanced alembic geometry shape

    GT_PRIM_PACKED_DISK,	// Packed disk primitive (single or sequence)

    GT_MAX_PRIM_TYPES,
} GT_PrimitiveType;

static inline bool	GTvalidPrimitive(int id)
			{
			    return id > GT_PRIM_UNDEFINED &&
				   id < GT_MAX_PRIM_TYPES;
			}

extern GT_API const char	*GTprimitiveType(int type);
extern GT_API GT_PrimitiveType	 GTprimitiveType(const char *name);

#endif
