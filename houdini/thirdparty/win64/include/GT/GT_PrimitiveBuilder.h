/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimitiveBuilder.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimitiveBuilder__
#define __GT_PrimitiveBuilder__

#include "GT_API.h"
#include "GT_Handles.h"
#include "GT_Types.h"
#include "GT_Primitive.h"
#include "GT_Transform.h"
#include "GT_AttributeBuilder.h"

class GT_TrimNuCurves;

/// @brief Create specific primitives by building from existing arrays
///
/// These methods construct GT primitives based on the attribute parsing style
/// defined in GT_AttributeBuilder.
///
/// Each builder function specifies a number of fixed arguments describing the
/// topology of the primitive.  The function also takes variadic arguments
/// which are passed through to the GT_AttributeBuilder function.
///
/// A simple example creating a simple point mesh
/// @code
///	// Build a point mesh
///	#define NPOINTS		4
///	UT_Vector3	P[NPOINTS];
///	fpreal32	width[NPOINTS];
///	initialize(P, w);
///	prim = GT_PrimitiveBuilder::pointMesh(err, NPOINTS,
///		"vertex point32 P", P,
///		"vertex real32 width", width,
///		NULL);
/// @endcode
/// A simple example creating a curve mesh that has deformation motion blur
/// @code
///	#define NCURVES 3
///	#define PTS_PER_CURVE 3
///	UT_Vector3	P0[NCURVES*PTS_PER_CURVE];
///	UT_Vector3	P1[NCURVES*PTS_PER_CURVE];
///	UT_Vector3	Cd[NCURVES*PTS_PER_CURVE];
///
///	int		points_per_curve[NCURVES];
///	for (int i = 0; i < ncurves; ++i)
///	{
///	    initCurveP(i, P0+i*PTS_PER_CURVE, time=0);
///	    initCurveP(i, P1+i*PTS_PER_CURVE, time=1);
///	    initCurveColor(i, Cd + i*PTS_PER_CURVE);
///	    points_per_curve[i] = PTS_PER_CURVE;
///	}
///	prim = GT_PrimitiveBuilder::curveMesh(err, GT_BEZIER_BASIS,
///		NCURVES, points_per_curve, false,
///		"vertex point32 P", P0,		// P for first segment
///		"vertex point32 P", P1,		// P for next segment
///		"vertex color32 Cd", Cd,	// Color for curves
///		NULL);
/// @endcode
///
class GT_API GT_PrimitiveBuilder {
public:
    /// Construct a point mesh
    ///	- varying[npoints]
    ///	- vertex[npoints]
    ///	- primitive[1]
    ///	- detail[1]
    static GT_PrimitiveHandle pointMesh(GT_BuilderStatus &err,
		int npoints,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Construct a single polygon
    ///	- varying[npoints]
    ///	- vertex[npoints]
    ///	- primitive[1]
    ///	- detail[1]
    static GT_PrimitiveHandle polygon(GT_BuilderStatus &err,
		int npoints,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Construct a mesh of @c npolys polygons. @n
    /// The @c counts array specifies the number of vertices for each polygon.
    /// The @c vtxlist array contains an index for each vertex.  The @b varying
    /// attributes are indexed using these indices.  There should be @c
    /// sum(counts) entries in the @c vtxlist array.
    ///	- varying[] @n
    ///   These values can be shared between polygons and are referenced using
    ///   the @c vtxlist array.  The array should have at least
    ///   @c max(vtxlist)+1 entries.
    ///	- vertex[] @n
    ///   These values specify unique values for each vertex of each polygon.
    ///   There should be @c sum(counts) entries in each of these arrays.
    ///	- primitive[npolys] @n
    ///   The primitive attributes are specified with a single value per face.
    ///	- detail[1]
    static GT_PrimitiveHandle polygonMesh(GT_BuilderStatus &err,
		int npolys, const int *counts,
		const int *vtxlist,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Construct a subdivision mesh of @c nfaces faces. @n
    /// The @c counts array specifies the number of vertices for each face.
    /// The @c vtxlist array contains an index for each vertex.  The @b varying
    /// attributes are indexed using these indices.  There should be @c
    /// sum(counts) entries in the @c vtxlist array.
    ///	- varying[] @n
    ///   These values can be shared between faces and are referenced using
    ///   the @c vtxlist array.  The array should have at least
    ///   @c max(vtxlist)+1 entries.
    ///	- vertex[] @n
    ///   These values specify unique values for each vertex of each face.
    ///   There should be @c sum(counts) entries in each of these arrays.
    ///	- primitive[npolys] @n
    ///   The primitive attributes are specified with a single value per face.
    ///	- detail[1]
    static GT_PrimitiveHandle subdivisionMesh(GT_BuilderStatus &err,
		GT_Scheme scheme,
		int npolys, const int *counts,
		const int *vtxlist,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Construct a single open curve. @n
    /// The @c counts array specifies the number of vertices for the curve.
    ///	- varying[], vertex[] @n
    ///   There are values for each vertex of the curve.  Each array
    ///   should have @c npoints entries.
    ///	- primitive/detail[1] @n
    static GT_PrimitiveHandle curve(GT_BuilderStatus &err,
		GT_Basis basis, int npoints, bool wrap,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());
    /// Construct a mesh of @c ncurves open curves. @n
    /// The curves are constructed using the @c basis given.
    /// The @c counts array specifies the number of vertices for each curve.
    /// The @c vtxlist array contains an index for each vertex.  The @b varying
    /// attributes are indexed using these indices.  There should be @c
    /// sum(counts) entries in the @c vtxlist array.
    ///	- varying[], vertex[] @n
    ///   There are distinct values for each vertex of each curve.  Each array
    ///   should have @c sum(counts) entries.
    ///	- primitive[ncurves] @n
    ///   The primitive attributes are specified with a single value per curve.
    ///	- detail[1]
    static GT_PrimitiveHandle curveMesh(GT_BuilderStatus &err,
		GT_Basis basis, int ncurves,
		const int *counts, bool wrap,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Construct a patch @n
    /// The patch is constructed using the given @c basis.  When the @c
    /// uwrap or @c vwrap flags are set, the patch will be wrapped (or
    /// periodic) in the given direction.  The @c nu and @c nv values must
    /// match the periodicity and basis.
    ///
    ///	- varying[], vertex[] @n
    ///   There are distinct values for each vertex of the patch.  Each array
    ///   should have @c nu*nv entries.
    ///	- primitive[npatches] @n
    ///   The primitive attributes are specified per sub-patch.  The number of
    ///   sub-patches is determined by the basis and periodicity.
    ///	- detail[1]
    static GT_PrimitiveHandle patch(GT_BuilderStatus &err,
		GT_Basis basis,
		int nu, bool uwrap, int nv, bool vwrap,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Construct a patch mesh @n
    /// A patch mesh is a collection of patches (all with the same basis)
    /// - @c nu, @c nv @n
    ///   Arrays which should have @c npatches entries.  These are the number
    ///   of vertices in the u and v directions for each individual patch.
    /// - @c uwrap, @c vwrap @n
    ///   Arrays which should have @c npatches entries.  These indicate whether
    ///   each individual patch is wrapped (periodic) in each direction.
    /// - varying[], vertex[] @n
    ///   Distinct values for each vertex of every patch.  Each array should be
    ///   sum(nu[i]*nv[i]) entries.
    /// - uniform[] @n
    ///   Distinct values for each face of each patch.  This value is
    ///   determined by the wrapped flags and the basis type.
    /// - detail[1]
    static GT_PrimitiveHandle patchMesh(GT_BuilderStatus &err,
		int npatches,
		GT_Basis basis,
		const int *nu, const bool *uwrap,
		const int *nv, const bool *vwrap,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Construct a mesh of NURBS curves @n
    /// NURBS curves are defined by a hulls of control vertices.
    /// - @c ncurves - The number of individual curves in the mesh
    /// - @c counts - The number of vertices in each curve
    /// - @c orders - The order of each curve
    /// - @c knots - Each curve should have @c counts[i]+orders[i] knots
    /// - @c tmin, @c tmax - The parametric range for the curve.  @c tmin
    ///     should be >= to the first knot for the curve.  @c tmax should be >=
    ///     tmin and <= the last knot for the curve.
    /// - varying[], vertex[] attributes @n
    ///   Distinct values for each vertex on the patch.  Each array should be
    ///   nu*nv entries.
    /// - uniform[] attributes @n
    ///   Distinct values for each curve.  Arrays should have ncurves entries.
    /// - detail[1] @n
    ///	  Detail attributes are constant across the curve mesh.
    /// Please note that for a cubic NURBS curve, the order is 4.
    static GT_PrimitiveHandle	nucurves(GT_BuilderStatus &err,
		int ncurves,
		const int *counts,
		const int *orders,
		const fpreal *knots,
		const fpreal *tmin,
		const fpreal *tmax,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());


    /// Construct a NURBS patch @n
    /// A NURBS patch is defined by a hull of control vertices.
    /// The number of elements in the uknots array should be @c nu+uorder.  The
    /// number of elements in the vknots array should be @c nv+vorder.
    /// - varying[], vertex[] attributes @n
    ///   Distinct values for each vertex on the patch.  Each array should be
    ///   nu*nv entries.
    /// - uniform[1], detail[1] @n
    ///	  Uniform and detail attributes are uniform across the patch.
    /// Please note that for a cubic NURBS curve, the order is 4.
    static GT_PrimitiveHandle	nupatch(GT_BuilderStatus &err,
		int nu, int uorder, const fpreal *uknots,
		int nv, int vorder, const fpreal *vknots,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Build geometry for the given bounding box.
    /// - varying[8]
    ///	  Varying attributes are defined for each shared point of the box.  You
    ///	  should @b not give a "P" attribute since that's built for you
    ///	  automatically.
    /// - vertex[24]
    ///	  Vertex attributes are defined per-vertex of each face
    /// - uniform[6]
    ///	  Uniform attributes are defined per face
    /// - detail[1] @n
    ///   Detail attributes are uniform over all faces of the geometry
    static GT_PrimitiveHandle	box(GT_BuilderStatus &err,
		const UT_BoundingBox &box,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());
    /// Build a curve mesh for the given bounding box.
    /// - At the current time, vertex/varying attributes are not supported and
    ///   are interpreted as uniform/detail attributes.  In the future, they
    ///   may be defined differently.
    /// - uniform[1]/detail[1] @n
    ///   Detail & uniform attributes are uniform over all faces of the geometry
    static GT_PrimitiveHandle	wireBox(GT_BuilderStatus &err,
		const UT_BoundingBox &box,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Build a sphere.
    /// - varying[1], vertex[1], primitive[1], detail[1]
    ///   All storage classes should have a single entry
    static GT_PrimitiveHandle	sphere(GT_BuilderStatus &err,
		const GT_TransformHandle &transform,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());
    /// Build a circle.
    /// - varying[1], vertex[1], primitive[1], detail[1]
    ///   All storage classes should have a single entry
    static GT_PrimitiveHandle	circle(GT_BuilderStatus &err,
		const GT_TransformHandle &transform,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());
    /// Build a tube.
    /// - varying[1], vertex[1], primitive[1], detail[1]
    ///   All storage classes should have a single entry
    static GT_PrimitiveHandle	tube(GT_BuilderStatus &err,
		const GT_TransformHandle &transform,
		fpreal taper,
		bool caps,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());
    /// Build a hyperboloid sheet
    /// - varying[1], vertex[1], primitive[1], detail[1]
    ///   All storage classes should have a single entry
    static GT_PrimitiveHandle	hyperboloid(GT_BuilderStatus &err,
		const GT_TransformHandle &transform,
		const UT_Vector3D &p0,
		const UT_Vector3D &p1,
		const GT_VariadicAttributes &attribs=GT_VariadicAttributes());

    /// Build trim curves.
    /// - The @c curves_per_loop array should be nloops long.  The number of
    ///   curves (@c ncurves) is the sum of the entries in this array.
    /// - The @c points_per_curve array should be @c ncurves long (i.e. one
    ///   entry for each individual curve)
    /// - The @c curve_orders array should be @c ncurves long
    /// - The @c knots array should have
    ///      <tt>sum(points_per_curve) + sum(curve_orders)</tt> entries
    /// - The @c min and @c max arrays should be @c ncurves long
    /// - The @c uvw array should be @c sum(points_per_curve) entries long
    static GT_TrimNuCurves	*trimcurves(GT_BuilderStatus &err,
				    int nloops,
				    const int *curves_per_loop,
				    const int *points_per_curve,
				    const int *curve_orders,
				    const fpreal *knots,
				    const fpreal *min,
				    const fpreal *max,
				    const UT_Vector3D *uvw);
};

#endif
