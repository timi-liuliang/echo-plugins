/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO Library.
 *
 * COMMENTS:	This file contains useful tools for finding closures.
 *
 */

#pragma once

#ifndef __GEO_Closure_h__
#define __GEO_Closure_h__

#include "GEO_API.h"
#include "GEO_PrimType.h"
#include "GEO_PrimTypeCompat.h"
#include <GA/GA_Types.h>

#include <UT/UT_IntArray.h>

class GA_Group;
class GA_PointGroup;
class GA_EdgeGroup;
class GA_PrimitiveGroup;
class GA_VertexGroup;
class GA_BreakpointGroup;
namespace GA_PrimCompat { class TypeMask; }
class GEO_Face;
class GEO_Detail;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;


class GEO_API GEO_Closure
{
public:
    GEO_Closure(GEO_Detail &gdp);
    ~GEO_Closure();

    /// This uses the GA_Topology attributes to find primitives
    /// using an edge.
    void		 findPrimsUsingEdge(const GA_Edge &edge,
			                    UT_IntArray &primlist);
    /// NOTE: This sorts and removes duplicates for compatibility
    /// NOTE: Unlike that findPrimsUsingEdge, this checks
    ///       GA_Primitive::hasEdge, not just that both points are contained.
    void		 findPrimsUsingEdgeSort(const GA_Edge &edge,
			                    UT_IntArray &primlist);

    /// This uses the GA_Topology attributes to find polygons
    /// using an edge.
    void		 findPolysUsingEdge(const GA_Edge &edge,
			                    UT_IntArray &primlist);

    /// This uses the GA_Topology attributes to find primitives
    /// using a point.
    void		 findPrimsUsingPoint(const GA_Offset pt,
			                    UT_IntArray &primlist);

    /// This uses the GA_Topology attributes to find polygons
    /// using a point.
    void		 findPolysUsingPoint(const GA_Offset pt,
			                    UT_IntArray &primlist);

    // Return 1 if the two faces share a point, and 0 otherwise.  Also return 
    // the indices of the vertices on the two faces in vtxidx0 (for face 0) and
    // vtxidx1 (for face 1).  Assumes both faces are open.
    static int 		 getSharedPoint(GEO_Face *face0, GEO_Face *face1, 
				        int &vtxidx0, int &vtxidx1);

    // Assumes both faces are open and returns 1 if they share a common point,
    // and 0 otherwise.
    static int		 haveSharedPoint(GEO_Face *face0, GEO_Face *face1);

    // Returns the number of unique edges shared by this point.
    GA_Size		 getNumSharedEdges(GA_Offset pt);

    /// @note Caller is responsible for calling destroyPrimitiveGroup() AND
    ///       for calling delete prefarr (for the functions using a prefarr).
    /// @{
    GA_PrimitiveGroup   *getPrimitiveClosure(const GA_EdgeGroup &edgegroup,
                                             GA_PrimitiveGroup *primgrp = 0);
    GA_PrimitiveGroup	*getEdgePointClosure(const GA_Edge &edge,
					     GA_PrimitiveGroup *primgrp = 0);
    /// @}

    /// If the group is not given, all the prims of type mask are returned
    /// in the primitive group.
    /// @note Caller is responsible for calling destroyPrimitiveGroup()
    GA_PrimitiveGroup	*getPrimitiveClosure(const GA_BreakpointGroup *grp = 0,
					     const GA_PrimCompat::TypeMask
					     &mask=GEO_PrimTypeCompat::GEOSPLINE);

    /// Point closures
    /// @note Caller is responsible for calling destroyPointGroup()
    /// @{
    GA_PointGroup	*getPointClosure(const GA_Group &group);
    GA_PointGroup 	*getPointClosure(const GA_PointGroup &ptgroup);
    GA_PointGroup 	*getPointClosure(const GA_EdgeGroup &edgegroup);
    GA_PointGroup 	*getPointClosure(const GA_PrimitiveGroup &primgroup);
    GA_PointGroup 	*getPointClosure(const GA_VertexGroup &vtxgroup);
    /// @}

    /// Vertex closures
    /// @note Caller is responsible for calling destroyVertexGroup()
    /// @{
    GA_VertexGroup	*getVertexClosure(const GA_Group &group);
    GA_VertexGroup	*getVertexClosure(const GA_PointGroup &ptgroup);
    GA_VertexGroup	*getVertexClosure(const GA_EdgeGroup &edgegroup);
    GA_VertexGroup	*getVertexClosure(const GA_PrimitiveGroup &primgroup);
    GA_VertexGroup	*getVertexClosure(const GA_VertexGroup &vtxgroup);
    /// @}

    // Primitive closures
    /// @note Caller is responsible for calling destroyPrimitiveGroup()
    /// @{
    GA_PrimitiveGroup	*getPrimitiveClosure(const GA_Group &group);
    GA_PrimitiveGroup	*getPrimitiveClosure(const GA_PointGroup &ptgroup);
    GA_PrimitiveGroup	*getPrimitiveClosure(
					    const GA_PrimitiveGroup &primgroup);
    GA_PrimitiveGroup	*getPrimitiveClosure(const GA_VertexGroup &vtxgroup);
    /// @}
 
    /// Detached point closures
    /// @note Caller is responsible for deleting the group.
    /// @{
    static GA_PointGroup	*getDetachedPointClosure(
					    const GEO_Detail &gdp,
					    const GA_Group &group);
    static GA_PointGroup 	*getDetachedPointClosure(
					    const GEO_Detail &gdp,
					    const GA_PointGroup &ptgroup);
    static GA_PointGroup 	*getDetachedPointClosure(
					    const GEO_Detail &gdp,
					    const GA_EdgeGroup &edgegroup);
    static GA_PointGroup 	*getDetachedPointClosure(
					    const GEO_Detail &gdp,
					    const GA_PrimitiveGroup &primgroup);
    static GA_PointGroup 	*getDetachedPointClosure(
					    const GEO_Detail &gdp,
					    const GA_VertexGroup &vtxgroup);
    /// @}

    /// Detached vertex closures
    /// @note Caller is responsible for deleting the group.
    /// @{
    static GA_VertexGroup	*getDetachedVertexClosure(
					    const GEO_Detail &gdp,
					    const GA_Group &group);
    static GA_VertexGroup	*getDetachedVertexClosure(
					    const GEO_Detail &gdp,
					    const GA_PointGroup &ptgroup);
    static GA_VertexGroup	*getDetachedVertexClosure(
					    const GEO_Detail &gdp,
					    const GA_EdgeGroup &edgegroup);
    static GA_VertexGroup	*getDetachedVertexClosure(
					    const GEO_Detail &gdp,
					    const GA_PrimitiveGroup &primgroup);
    static GA_VertexGroup	*getDetachedVertexClosure(
					    const GEO_Detail &gdp,
					    const GA_VertexGroup &vtxgroup);
    /// @}

    // Detached primitive closures
    /// @note Caller is responsible for deleting the group.
    /// @{
    static GA_PrimitiveGroup	*getDetachedPrimitiveClosure(
					    const GEO_Detail &gdp,
					    const GA_Group &group);
    static GA_PrimitiveGroup	*getDetachedPrimitiveClosure(
					    const GEO_Detail &gdp,
					    const GA_PointGroup &ptgroup);
    static GA_PrimitiveGroup	*getDetachedPrimitiveClosure(
					    const GEO_Detail &gdp,
					    const GA_PrimitiveGroup &primgroup);
    static GA_PrimitiveGroup	*getDetachedPrimitiveClosure(
					    const GEO_Detail &gdp,
					    const GA_VertexGroup &vtxgroup);
    /// @}

private:
    GEO_Detail &myGdp;
};
#endif
