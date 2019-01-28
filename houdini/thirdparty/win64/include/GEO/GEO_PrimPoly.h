/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:	polygon definition
 * 
 */

#pragma once

#ifndef __GEO_PrimPoly_H__
#define __GEO_PrimPoly_H__

#include "GEO_API.h"
#include "GEO_Face.h"
#include "GEO_PrimType.h"
#include <iosfwd>

class GEO_Detail;
class GA_ElementWranglerCache;

class GEO_API GEO_PrimPoly : public GEO_Face
{
protected:
public:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    SYS_FORCE_INLINE
    GEO_PrimPoly(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Face(d, offset)
    {}

    /// This constructor is for making a representation of a polygon
    /// on the stack, so that you can call GU_PrimPoly functions on
    /// the polygon without needing the detail to allocate one.
    SYS_FORCE_INLINE
    GEO_PrimPoly(GA_Detail *gdp, GA_Offset offset, const GA_OffsetListRef &vertex_list)
        : GEO_Face(gdp, offset)
    { myVertexList = vertex_list; }

    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT_P(theDefinition);
        return *theDefinition;
    }

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    // Assuming the poly is closed, "unroll" it so that the CVs that are
    // shared to form a wrapped poly are made unique. Also, the poly becomes
    // open. The base class method only flips the open flag. If the poly is
    // not closed, the method returns -1. Otherwise it returns 0.
    virtual int		unroll(int append_pts = 1);

    // Raise the number of CVs to match the newcount. The shape of the poly
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int		loft(int newcount, int start=-1, int stop=-1);

    // Warp the poly at u by the given delta. The bias makes sense
    // only when changing 2 CVs, and will be ignored altogether if < 0.
    // We return the index of the affected CV in the sequence, or -1 if
    // there's an error.
    virtual int		 warp(float u, const UT_Vector3 &delta,
			      GA_AttributeRefMap &map,
			      float sharpness = 0.0f, float bias = -1.0f);

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		 evaluateBreakpoint(int uidx, UT_Vector4 &pos,
					    int du=0) const;

    virtual void computeInteriorPointWeights(
        UT_Array<GA_Offset> &vtxlist,
        UT_Array<float> &weightlist,
        fpreal u, fpreal v, fpreal w) const final;

    // Append another face to us in one of two ways: blend the two endpoints
    // or connect them straight. The bias ranges from 0 to 1 and is
    // relevant only to blending. The tolerance is for blending: if 0, the two
    // endpoints will merge into one point with a discontinuity; if less than
    // 1, we insert points into the polys to minimize the affected areas; if 1,
    // no refinement is done. We return 0 if OK and -1 if error.
    // Both polys must be open.
    virtual int		attach(const GEO_Face &face, int blend = 1,
			       float bias = .5f, float tolerance = 1.f, 
			       int=0, GA_PointGroup *ptgroup=0);

    // Build a planar (domain) face of the same type as us and with the same
    // number of vertices. Copying the (x,y) values of our points to the planar
    // face is optional.
    virtual GD_Face	*planar(GD_Detail &dgdp, int copyxy = 0) const;

    virtual unsigned	getOrder() const;
    virtual bool	isDegenerate() const;
    virtual int		breakCount() const;

    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve. The tolerance specifies max
    // error of the returned value (cannot be 0.0f)
    virtual float	 unitLengthToUnitDomain(float ulength, 
						float tolerance = 1e-05F) const;
    virtual float	 unitToUnitLengthDomain(float  uparm) const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

    virtual void	getAdjacentBoundaryVertices(GA_Offset vtx,
						    GA_Offset &prev_vtx,
						    GA_Offset &next_vtx) const;
    virtual int		deleteVertex(GA_Size num);
    virtual GA_Size	insertVertex(GA_Offset ptoff, GA_Size where=0);
    virtual GA_Size	appendVertex(GA_Offset ptoff);
    virtual bool	supportsHedge() const
			{ return true; }

    virtual GA_Size	stealVertex(GA_Offset vtx, GA_Offset insert_before_vtx=GA_INVALID_OFFSET);

    /// Builds a GEO_PrimPoly with nvertices vertices, and
    /// optionally adds a point to the detail for each vertex.
    static GEO_PrimPoly *build(GA_Detail *gdp, GA_Size nvertices,
                               bool open=false,
                               bool appendpts=true);

    /// Builds npoints new points with the specified positions, and then
    /// creates polygons that use those points, as dictated by polygonsizelist
    /// and polygonpointnumbers, in parallel. polygonpointnumbers lists the
    /// numbers of the points used by each polygon, relative to the first
    /// point created by this method.  The offset of the first polygon is
    /// returned, and the rest are at consecutive offsets. All
    /// polygonpointnumbers must be between 0 (inclusive) and
    /// npoints (exclusive).
    ///
    /// NOTE: It's not strictly necessary that the polygons being created use
    /// all of the points created.
    /// @{
    static GA_Offset buildBlock(GA_Detail *detail,
        const UT_Vector3 *points,
        const GA_Size npoints,
        const GEO_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool closed = true);
    static GA_Offset buildBlock(GA_Detail *detail,
        const UT_Vector3 *points,
        const GA_Size npoints,
        const GA_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool closed = true);
    /// @}

    /// Builds polygons using the specified range of point offsets,
    /// as dictated by polygonsizelist and polygonpointnumbers, in parallel.
    /// polygonpointnumbers lists the *offsets* of the points used by
    /// each polygon *MINUS* startpt, i.e. they are offsets relative to startpt,
    /// *not* indices relative to startpt. The offset of the first polygon is
    /// returned, and the rest are at consecutive offsets. All
    /// polygonpointnumbers must be between 0 (inclusive) and
    /// npoints (exclusive).
    ///
    /// NOTE: Existing primitives *are* allowed to be using the points in
    /// the specified range already, and the polygons being created do not
    /// need to use all of the points in the range.  However,
    /// these cases may impact performance.
    /// @{
    static GA_Offset buildBlock(
        GA_Detail *detail,
        const GA_Offset startpt,
        const GA_Size npoints,
        const GEO_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool closed = true)
    {
        return GEO_Face::buildBlock(
            GA_PRIMPOLY,
            detail,
            startpt,
            npoints,
            polygonsizelist,
            polygonpointnumbers,
            closed);
    }
    static GA_Offset buildBlock(
        GA_Detail *detail,
        const GA_Offset startpt,
        const GA_Size npoints,
        const GA_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool closed = true)
    {
        return GEO_Face::buildBlock(
            GA_PRIMPOLY,
            detail,
            startpt,
            npoints,
            polygonsizelist,
            polygonpointnumbers,
            closed);
    }
    /// @}

    /// Flips a shared edge of two triangles
    ///						|
    ///      j+2                j+2 i+1		|
    ///      / \                  /|\		|
    ///     /   \                / | \		|
    ///    / that\              / t|t \		|
    ///   j-------j+1 flips to j  h|h  i	|
    /// i+1\ this/i             \ a|i /		|
    ///     \   /                \t|s/		|
    ///      \ /                  \|/		|
    ///      i+2                j+1 i+2		|
    ///
    /// i and j are indices into this and that, respectively,
    /// and are on opposite ends of the common edge,
    /// which is (i, i+1) in this and (j, j+1) in the
    /// opposite direction in that.
    /// The vertex i+1 in this is moved to where j+2 is in that.
    /// The vertex j+1 in that is moved to where i+2 is in this.
    /// NOTE: Vertex attributes will be copied from j+2 onto i+1,
    ///       and from i+2 onto j+1.
    /// NOTE: The two triangles must be in the same detail.
    void                flipEdge(GEO_PrimPoly *that,
                                 GA_Size i, GA_Size j,
                                 GA_ElementWranglerCache &wranglers);

    /// Determine if the polygon is planar
    bool                 isPlanar(float tolerance = 0.0001F) const;

    /// Determine if the polygon is convex
    bool                 isConvex(float tolerance = 0.0001F) const;

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_Face::buildFamilyMask(); }

    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Face::registerIntrinsics(defn); }

    // We don't need to save anything other than what's in a face
    virtual bool	savePrivateH9(std::ostream &, bool binary) const;
    virtual bool	loadPrivateH9(UT_IStream &);

    // Return the squared max distance between any two successive vertices
    // and the index of the "left" vertex. If the polygon is closed we
    // compare the 1st and last vertices as well and if their distance is the
    // largest, we return the index of the last vertex.
    int			findMaxDistance(float &maxdist2, 
					int start, int stop) const;
    // Evaluates the position at domain point (u,v) in the interior of the
    // geometry, rather than on the perimeter of the poly.
    // (Note that u, v don't have to be converted to unit domain.  We assume
    // that the barycentric coords or bilinear interpolants are already 
    // correctly scaled).
    virtual bool	 evaluateInteriorPointRefMap(GA_Offset result_vtx,
					GA_AttributeRefMap &hlist,
					fpreal u, fpreal v, fpreal w = 0) const;
    virtual int		 evaluateInteriorPointV4(UT_Vector4 &pos,
				       fpreal u, fpreal v, fpreal w = 0) const;

    GA_Offset    	nextBoundaryVertex(unsigned int i) const
			    { return (i < getFastVertexCount() - 1) ? getFastVertexOffset(i+1) :
				(isClosed() ? getFastVertexOffset(0) : GA_INVALID_OFFSET); }
    GA_Offset		prevBoundaryVertex(unsigned int i) const
			    { return i > 0 ? getFastVertexOffset(i - 1) :
    				(isClosed() ? getFastVertexOffset(getFastVertexCount() - 1) : GA_INVALID_OFFSET); }


    // The following are just stubs, so that a GEO_PrimPoly can be made
    // on the stack.  You can't all any of these functions unless they
    // get moved to GEO_PrimPoly!

    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
        GA_PointGroup *usedpts = 0)
    {
        UT_ASSERT_MSG(0, "Implemented in GU_PrimPoly!  This is just a stub so that GEO_PrimPoly is not abstract!");
        return nullptr;
    }

    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms)
    {
        UT_ASSERT_MSG(0, "Implemented in GU_PrimPoly!  This is just a stub so that GEO_PrimPoly is not abstract!");
        return nullptr;
    }

    virtual int intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
        float tmax = 1E17F, float tol = 1E-12F,
        float *distance = 0, UT_Vector3 *pos = 0,
        UT_Vector3 *nml = 0, int accurate = 0,
        float *u = 0, float *v = 0, 
        int ignoretrim=1) const
    {
        UT_ASSERT_MSG(0, "Implemented in GU_PrimPoly!  This is just a stub so that GEO_PrimPoly is not abstract!");
        return 0;
    }

    virtual int intersect(const GEO_Primitive &prim,
        UT_Array<GU_RayInfoHit> &hitList,
        float tol = 0.01F, int ignoretrim=1) const
    {
        UT_ASSERT_MSG(0, "Implemented in GU_PrimPoly!  This is just a stub so that GEO_PrimPoly is not abstract!");
        return 0;
    }

    virtual void clip(UT_Vector3 normal, float distance=0,
        GA_PrimitiveGroup *clipgrp = NULL)
    { UT_ASSERT_MSG(0, "Implemented in GU_PrimPoly!  This is just a stub so that GEO_PrimPoly is not abstract!"); }


private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimPoly &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}

    static GA_PrimitiveDefinition *theDefinition;
    friend class GU_PrimitiveFactory;
};
#endif
