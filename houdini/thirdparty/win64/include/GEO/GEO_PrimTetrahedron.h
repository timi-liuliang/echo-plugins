/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimTetrahedron.h (GEO Library, C++)
 *
 * COMMENTS:	This is the tetrahedron primitive type.
 */

#pragma once

#ifndef __GEO_PrimTetrahedron__
#define __GEO_PrimTetrahedron__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include "GEO_VolumeElement.h"
#include <GA/GA_Defines.h>
#include <UT/UT_Array.h>
#include <SYS/SYS_Inline.h>

class GA_Detail;
class GEO_ConvertParms;

class GEO_API GEO_PrimTetrahedron : public GEO_VolumeElement
{
public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    SYS_FORCE_INLINE
    GEO_PrimTetrahedron(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_VolumeElement(d, offset)
    {}

    /// This constructor is for making a representation of a tetrahedron
    /// on the stack, so that you can call GEO_PrimTetrahedron functions on
    /// the tetrahedron without needing the detail to allocate one.
    SYS_FORCE_INLINE
    GEO_PrimTetrahedron(GA_Detail *gdp, GA_Offset offset, const GA_OffsetListRef &vertex_list)
        : GEO_VolumeElement(gdp, offset)
    {
        UT_ASSERT_P(vertex_list.size() == 4);
        myVertexList = vertex_list;
    }

    virtual const GA_PrimitiveDefinition &getTypeDef() const override;

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const override;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const override;

    void	reverse() override;
    UT_Vector3	 computeNormal() const override;

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    fpreal	calcVolume(const UT_Vector3 &refpt) const override;
    fpreal	calcArea() const override;
    fpreal	calcPerimeter() const override;

    /// @{
    /// Save/Load vertex list to a JSON stream
    bool		saveVertexArray(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadVertexArray(UT_JSONParser &p,
				const GA_LoadMap &map);
    /// @}

    bool  	isDegenerate() const override;

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    int		 detachPoints (GA_PointGroup &grp) override;

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    GA_DereferenceStatus dereferencePoint(
	GA_Offset point, bool dry_run=false) override;
    GA_DereferenceStatus dereferencePoints(
	const GA_RangeMemberQuery &pt_q, bool dry_run=false) override;

    //
    // Tetrahedron layout
    //
    //    0     //
    //   /|\    //
    //  1-|-3   //
    //   \|/    //
    //    2     //
    //
    // Face 0 is the face *opposite* to vertex 0, so is 1..2..3
    // Each face starts with the least vertex #.

    SYS_FORCE_INLINE GA_Offset fastVertexOffset(GA_Size index) const
    { return getFastVertexOffset(index); }
    SYS_FORCE_INLINE GA_Size getFastVertexCount() const
    { return 4; }
    SYS_FORCE_INLINE GA_Offset getFastVertexOffset(GA_Size index) const
    {
        UT_ASSERT_P(index >= 0 && index < 4);
        return getVertexOffset(index);
    }

    /// Return the topology of the tetrahedron
    GA_Size	getFaceCount() const override;
    int getFaceIndices(GA_Size faceno, UT_IntArray &vtxlist) const override;

    SYS_FORCE_INLINE
    static const int *fastFaceIndices(GA_Size faceno)
    {
        static const int faceidx[4][3] =
        {
            { 1, 2, 3 },
            { 0, 3, 2 },
            { 0, 1, 3 },
            { 0, 2, 1 }
        };

        return faceidx[faceno];
    }

    /// Get the vertex offsets of the specified tri.
    void getTriangleVertexOffsets(
	GA_Size tri, GA_Offset &v0, GA_Offset &v1, GA_Offset &v2) const
    {
        const int *indices = fastFaceIndices(tri);
        v0 = fastVertexOffset(indices[0]);
        v1 = fastVertexOffset(indices[1]);
        v2 = fastVertexOffset(indices[2]);
    }

    GA_Size	getEdgeCount() const override;
    void	getEdgeIndices(GA_Size edgeno, int &e0, int &e1) const override;

    bool hasEdge(const GA_Edge &edge) const override
    {
        for (GA_Size i = 0; i < 6; ++i)
        {
            int i0, i1;
            getEdgeIndices(i, i0, i1);
            GA_Offset p0 = getPointOffset(i0);
            GA_Offset p1 = getPointOffset(i1);
            if ((edge.p0() == p0 && edge.p1() == p1) ||
        	(edge.p0() == p1 && edge.p1() == p0))
        	return true;
        }
        return false;
    }
    

    void iterateEdges(GA_IterateEdgesFunc apply_func) const override
    {
        for (int i = 0; i < 6; ++i)
        {
            int i0; int i1;
            getEdgeIndices(i, i0, i1);
            // Both directions are present
            if (!apply_func(GA_Edge(getPointOffset(i0), getPointOffset(i1))))
        	return;
            if (!apply_func(GA_Edge(getPointOffset(i1), getPointOffset(i0))))
        	return;
        }
    }
    
    void iterateEdgesByVertex(GA_IterateEdgesByVertexFunc apply_func) const override
    {
        for (int i = 0; i < 6; ++i)
        {
            int i0; int i1;
            getEdgeIndices(i, i0, i1);
            
            // Both directions are present
            if (!apply_func(i0, i1) || !apply_func(i1, i0))
        	return;
        }
    }

    // Given two vertex indices giving a tet edge, this routine returns
    // another vertex index, which forms an outward facing tet face with the
    // original edge vertices e1 and e2.
    GA_Size getVertexCompletingFace(GA_Size v1, GA_Size v2, GA_Size &v3) const;

    GA_Offset findSharedTet(GA_Size faceno) const;
    SYS_FORCE_INLINE
    bool isFaceShared(GA_Size faceno) const
    { return GAisValid(findSharedTet(faceno)); }

    virtual GA_Offset findSharedFace(GA_Size faceno) const override
    { return findSharedTet(faceno); }

    // These methods find the index of the given vertex (vtx or the vertex
    // that contains a pt). Return -1 if not found. 
    GA_Size 	findVertex(GA_Offset vtx) const
			{
			    for (GA_Size i = 0; i < 4; i++)
				if (fastVertexOffset(i) == vtx) return i;
			    return -1;
    			}

    GA_Size 	find(GA_Offset pt) const
			{ 
			    for (GA_Size i = 0; i < 4; i++)
				if (vertexPoint(i) == pt) return i;
			    return -1;
			}

    void	setVertexPoint(int i, GA_Offset pt)
			{
			    if (i < 4)
				wireVertex(fastVertexOffset(i), pt);
			}

    const GA_PrimitiveJSON	*getJSON() const override;

    /// Takes (u,v,w) between 0 and 1, and reflects them inside the unit cube
    /// so that they fall in the region where (1-u-v-w) >= 0 (and <= 1).
    /// That value is written into uvw.
    /// This is instantiated for float and double.
    template<typename T>
    static void remapTetCoords(T &u, T &v, T &w, T &uvw);

    class Face
    {
    public:
        Face(const GEO_PrimTetrahedron &tet,int face)
            : myTet(tet)
            , myI(face)
            // This is the face opposite myI
            , myF(GEO_PrimTetrahedron::fastFaceIndices(myI))
        {}
        inline GA_Size getFastVertexCount() const
        { return 3; }
        inline GA_Size getVertexCount() const
        { return 3; }
        inline GA_Offset getFastVertexOffset(GA_Size i) const
        { return myTet.getFastVertexOffset(myF[i]); }
        inline GA_Offset getVertexOffset(GA_Size i) const
        { return getFastVertexOffset(i); }
        inline GA_Offset getPointOffset(GA_Size i) const
        { return myTet.getPointOffset(myF[i]); }
        inline UT_Vector3 getPos3(GA_Size i) const
        { return myTet.getPos3(myF[i]); }
        inline bool isClosed() const
        { return true; }
    private:
        const GEO_PrimTetrahedron &myTet;
        int myI;
        const int *const myF;
    };

    /// NOTE: Do not use this under normal circumstances!
    ///       This is for if a tet has been added using
    ///       GA_Detail::appendPrimitive[Block] and vertices have
    ///       to be added one at a time, e.g. from VEX.
    ///       It will return GA_INVALID_OFFSET and won't add a vertex
    ///       if all 4 have already been assigned.
    GA_Offset unsafeAppendVertex();

    virtual void normal(NormalComp &output) const override;

    // Conversion Methods
    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
                                   GA_PointGroup *usedpts = nullptr) override;
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms) override;

    // A convenience member to determine if a particular location lies within
    // the tetrahedron
    bool isContainedInside(const UT_Vector3 &pt_pos,
        fpreal64 tolerance = SYS_FTOLERANCE_D) const;

    /// Builds a single tetrahedron primitive, optionally creating new points
    /// and wiring them to its vertices.
    static GEO_PrimTetrahedron *build(GEO_Detail *gdp, bool appendPts = true);

    /// Builds tetrahedrons using the specified range of point offsets,
    /// as dictated by ntets and tetpointnumbers, in parallel.
    /// tetpointnumbers lists the *offsets* of the points used by
    /// each tetrahedron *MINUS* startpt, i.e. they are offsets relative to
    /// startpt, *not* indices relative to startpt. The offset of the first
    /// tetrahedron is returned, and the rest are at consecutive offsets. All
    /// tetpointnumbers must be between 0 (inclusive) and npoints (exclusive).
    ///
    /// NOTE: Existing primitives *are* allowed to be using the points in
    /// the specified range already, and the tetrahedrons being created do not
    /// do not need to use all of the points in the range.  However,
    /// these cases may impact performance.
    static GA_Offset buildBlock(GA_Detail *detail,
        const GA_Offset startpt,
        const GA_Size npoints,
        const GA_Size ntets,
        const int *tetpointnumbers);

    virtual void computeInteriorPointWeights(
        UT_Array<GA_Offset> &vtxlist, UT_Array<float> &weightlist,
        fpreal u, fpreal v, fpreal w) const override final;

protected:
    void allocateVertices();

    GA_DECLARE_INTRINSICS(override)

    // Check the validity of the data. Meant to be called especially at loading
    // time. The method returns 1 if OK and 0 if trouble.
    virtual bool validate(void) const;

    /// @warning vertexPoint() doesn't check the bounds.  Use with caution.
    GA_Offset vertexPoint(GA_Size i) const
    { return getDetail().vertexPoint(fastVertexOffset(i)); }

    // Evaluates the position at domain point (u,v,w) in the interior of the
    // geometry, rather than on the perimeter of the poly.
    // (Note that u, v, w don't have to be converted to unit domain.  We assume
    // that the barycentric coords or bilinear interpolants are already 
    // correctly scaled).
    // Vertex 0 is the origin of this system, Vertex 1 the U axis
    // Vertex 2 the V, and Vertex 3 the W.
    bool evaluateInteriorPointRefMap(
	GA_Offset result_vtx, GA_AttributeRefMap &map,
	fpreal u, fpreal v, fpreal w = 0) const override;
    int	 evaluateInteriorPointV4(
	UT_Vector4 &pos, fpreal u, fpreal v, fpreal w = 0) const override;

    static GA_PrimitiveDefinition *theDefinition;
    friend class GU_PrimitiveFactory;
private:
};

#endif

