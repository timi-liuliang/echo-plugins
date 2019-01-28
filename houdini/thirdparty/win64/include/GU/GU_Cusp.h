/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Csup.h (GU Library, C++)
 *
 * COMMENTS:
 *
 */


#ifndef __GU_Cusp_h__
#define __GU_Cusp_h__

#include "GU_API.h"
#include <GA/GA_Handle.h>
#include <SYS/SYS_Types.h>

class GU_Detail;
class GEO_DetachedHedgeInterface;
class GA_Attribute;
class GA_AttributeRefMap;
class GA_EdgeGroup;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_VertexGroup;
class UT_Classifier;
class GEO_Hedge;

// Cusping operations are defined over a set of "cut-edges", which can be
// specified in various ways, and a primitive group. When no primitive group
// is specified, the primitive group is taken to be all primitives. In this
// more straight-forward case, a cusp operations computes a classification
// of vertices wired into each point into equivalence classes. The
// classes are formed by initially putting each vertex in its own class and
// then merging classes of vertices separated by non-cut-edges.
// Note that both cut-edge and non-cut-edges can be incident to 1, 2,
// or more faces but the classification defined this way is unique.
//
// If a primitive group is also specified, then classes of two
// vertices separated by non-cut-edge are merged as before but only if in
// addition both vertices belong to primitives in the specified group. If a
// non-cut-edge separates a vertex of a primitive in the group and
// another from a non-group primitive, then class of the former vertex
// will be called a "run-over" class, meaning that the class would have been
// larger if the operation were not limited to the given primitive group.
//
// Once vertex classes are determined, cusp operations equalize/uniques the the
// specified attribute(s) of vertices in each equivalence class:
//
// 	Point cusping makes the points wired to each class unique,
// 	Attribute cusping averages out the specified attributes in each class
// 		and overwrites those attributes with the average value of the
//		class for all vertices in the class.

// Cusp the edges in the given edge group. If cut_boundaries is false,
// classes running over unshared edges that are not in the given edge group,
// or those running over outside the primitive group, are not wired to their
// unique copy of the point.

GU_API void	 GUcusp(GU_Detail *gdp, const GA_EdgeGroup &edges,
			bool cut_boundaries = false,
           	        const GA_PrimitiveGroup *prims = 0);


// Simlar to the above cusp but uses an array of half-edges instead of an
// edge group as input. The optional hedge interface is used to avoid rebuilding
// a const hedge interface. Note that cusping edges would render an existing
// const hedge interface invalid.

GU_API void	 GUcusp(GU_Detail *gdp, const UT_Array<GEO_Hedge> &hedges,
           	        bool cut_boundaries = false,
           	        const GA_PrimitiveGroup *prims = 0,
			const GEO_DetachedHedgeInterface *hip = 0);

// Simlar to the above cusp but uses an vertex group instead. Any edge matching
// a half-edge determined by vertex group is a cut edge.

GU_API void	 GUcusp(GU_Detail *gdp, const GA_VertexGroup *vtxs,
           	        bool cut_boundaries = false,
           	        const GA_PrimitiveGroup *prims = 0,
			const GEO_DetachedHedgeInterface *hip = 0);


// Cusp vertex normal attributes based on cut-edges defined as manifold edges
// whose two incident primitives have normals deviating by more than
// "cusp_angle" degrees. vnh is the handle for the attribute to be written
// into. pnh is the primitive normal attribute handle which will be computed
// if not supplied.
//
// * If prims is given, only vertices of the primitives in prims are used in
//   doing the calculations.
//
// * If vtxs are given, only values for classes whose root is contained
//   in vtxs is overwritten and the others are left unchanged.
//
// * If always_cusp is given, edges in always_cusp are cusped regardless of
//   their diehedral angle.
//
// * If never_cups is given, edges in never_cusp are not cusped unless they
//   are nonmanifold edges.
//
// * If cattrib_cusp is false,
//   then it does point cusp instead of attribute cusp.

GU_API void	 GUcomputeVertexNormals(GU_Detail *gdp,
			GA_RWHandleV3 vnh,
			fpreal cusp_angle,
			GA_ROHandleV3 pnh = GA_ROHandleV3(nullptr),
			bool attrib_cusp = true,
           	        const GA_PrimitiveGroup *prims = nullptr,
			GA_VertexGroup *vtxs = nullptr,
			GA_EdgeGroup *always_cusp = nullptr,
			GA_EdgeGroup *never_cusp = nullptr,
			GEO_DetachedHedgeInterface *hip = nullptr,
           	        bool use_angle_weights = false);


// Generic function for creating equivalence classes which can be used
// for point or attribute cusping. The template class T must have a method
//
//    inline bool isHedgeSelected(GEO_Hedge e)
//
// which decides whether the edge described by half-edge e is a cut edge
// or not. If not null, nontrivial_ptgrp will include points which are
// incident to at least one cut-edge. If runover_vtxgrp is not null, then
// it will include roots of run-over equivalence classes.

template <class T>
GU_API
UT_Classifier 	*GUclassifyVertices(const GU_Detail *gdp,
		   const GEO_DetachedHedgeInterface *dhip,
		   const GA_PointGroup *pts,
		   T &hedge_selector,
		   const GA_PrimitiveGroup *prims,
		   GA_PointGroup *nontrivial_ptgrp,
		   GA_VertexGroup *runover_vtxgrp);


// Cusp points or attributes according to the given vertex classifier:
// GUpointCusp() splits the points (there will be a unique point for each
// class of vertices). GUattributeCusp() unifies the attributes values over
// each vertex class based on cusp_fn by either
//     - taking an arbitrary member's value (GU_CUSP_ANY),
//     - taking the value of the member with smallest weight (GU_CUSP_MIN), or
//     - taking the value of the member with largest weight (GU_CUSP_MAX), or
//     - taking the weighted average of all members.
// wth is the handle for the weight attribute to be used in weighted
// average computation. If wth is invalid, all eights are taken to be 1.
//
// vtx_classes must have (at least) the same number of elements as the
// number of vertex offsets.
//
// If ignore_classes is specified, classes whose root elements are contained
// in the ignore_classes group are left unchanged.
//
// There are two version of GUattributeCusp, one taking a single attribute
// and the other taking an GA_AttributeRefMap which allows simultaneous cusping
// of multiple attributes.

enum GU_CuspFunction
{
    GU_CUSP_ANY = 0,
    GU_CUSP_AVERAGE,
    GU_CUSP_MIN,
    GU_CUSP_MAX
};

enum GU_CuspMaskType
{
    GU_CUSP_INCLUDE,
    GU_CUSP_EXCLUDE
};

GU_API void	 GUpointCusp(GU_Detail *gdp,
			const UT_Classifier *vtx_classes,
			const GA_PointGroup *pts = 0,
			const GA_PrimitiveGroup *prims = 0,
			const GA_VertexGroup *mask_classes = 0,
			GU_CuspMaskType mask_type = GU_CUSP_INCLUDE);

GU_API void	 GUattributeCusp(GU_Detail *gdp,
			const UT_Classifier *vtx_classes,
			GA_Attribute *attrib,
			GA_ROHandleF &wth,
			GU_CuspFunction cusp_fn = GU_CUSP_ANY,
			const GA_PrimitiveGroup *prims = 0,
			const GA_VertexGroup *mask_classes = 0,
			GU_CuspMaskType mask_type = GU_CUSP_INCLUDE);

GU_API void	 GUattributeCusp(GU_Detail *gdp,
			const UT_Classifier *vtx_classes,
			GA_AttributeRefMap &refmap,
			GA_ROHandleF &wth,
			GU_CuspFunction cusp_fn = GU_CUSP_ANY,
			const GA_PrimitiveGroup *prims = 0,
			const GA_VertexGroup *mask_classes = 0,
			GU_CuspMaskType mask_type = GU_CUSP_INCLUDE);

#endif
