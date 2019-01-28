/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_PrimPolySoup.h (GEO Library, C++)
 *
 * COMMENTS:    Implements a set of polygons sharing primitive attributes
 *              and some vertices, to save memory
 */

#pragma once

#ifndef __GEO_PrimPolySoup__
#define __GEO_PrimPolySoup__

#include "GEO_TriMesh.h"
#include "GEO_PrimType.h"
#include "GEO_PrimTypeCompat.h"
#include "GEO_API.h"
#include "GEO_PolyCounts.h"
#include <GA/GA_Detail.h>
#include <GA/GA_ElementWrangler.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_SafeFloat.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Inline.h>

class GA_Detail;
class GEO_PolySoupParms;
class GA_PrimitiveGroup;
class GEO_PrimPoly;

/// A soup of polygons
class GEO_API GEO_PrimPolySoup : public GEO_TriMesh
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimPolySoup(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_TriMesh(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimPolySoup() {}

public:
    class PolygonIterator : public GA_PolyCounts::const_iterator
    {
    public:
        SYS_FORCE_INLINE PolygonIterator() {}
        SYS_FORCE_INLINE PolygonIterator(const GEO_PrimPolySoup &prim)
            : GA_PolyCounts::const_iterator(prim.myPolygonSizeList)
            , myPrim(&prim)
        {}
        SYS_FORCE_INLINE PolygonIterator(const GEO_PrimPolySoup &prim, GA_Size polygon)
            : GA_PolyCounts::const_iterator(prim.myPolygonSizeList, polygon)
            , myPrim(&prim)
        {}
        SYS_FORCE_INLINE const GEO_PrimPolySoup &prim() const { return *myPrim; }
        SYS_FORCE_INLINE PolygonIterator &operator=(const PolygonIterator &that)
        {
            this->GA_PolyCounts::const_iterator::operator=(that);
            this->myPrim = that.myPrim;
            return *this;
        }
        SYS_FORCE_INLINE GA_Offset getVertexOffset(GA_Size i) const
        {
            UT_ASSERT_P(i >= 0 && i < nvertices());
            return myPrim->myPolygonVertexList(start()+i);
        }
        SYS_FORCE_INLINE GA_Index getVertexIndex(GA_Size i) const
        {
            UT_ASSERT_P(i >= 0 && i < nvertices());
            const GA_Detail &detail = myPrim->getDetail();
            return detail.vertexIndex(myPrim->myPolygonVertexList(start()+i));
        }
        SYS_FORCE_INLINE GA_Offset getPointOffset(GA_Size i) const
        {
            UT_ASSERT_P(i >= 0 && i < nvertices());
            return myPrim->getDetail().vertexPoint(myPrim->myPolygonVertexList(start()+i));
        }
        SYS_FORCE_INLINE GA_Index getPointIndex(GA_Size i) const
        {
            UT_ASSERT_P(i >= 0 && i < nvertices());
            const GA_Detail &detail = myPrim->getDetail();
            return detail.pointIndex(detail.vertexPoint(myPrim->myPolygonVertexList(start()+i)));
        }
        SYS_FORCE_INLINE UT_Vector3 getPos3(GA_Size i) const
        {
            UT_ASSERT_P(i >= 0 && i < nvertices());
            const GA_Detail &detail = myPrim->getDetail();
            return detail.getPos3(detail.vertexPoint(myPrim->myPolygonVertexList(start()+i)));
        }
        SYS_FORCE_INLINE UT_Vector4 getPos4(GA_Size i) const
        {
            UT_ASSERT_P(i >= 0 && i < nvertices());
            const GA_Detail &detail = myPrim->getDetail();
            return detail.getPos4(detail.vertexPoint(myPrim->myPolygonVertexList(start()+i)));
        }
        SYS_FORCE_INLINE UT_Vector3 baryCenter() const
        { return myPrim->baryCenter(*this); }
        SYS_FORCE_INLINE UT_Vector3 computeNormal() const
        { return myPrim->computeNormal(*this); }
        SYS_FORCE_INLINE fpreal calcPerimeter() const
        { return myPrim->calcPerimeter(*this); }
        SYS_FORCE_INLINE fpreal calcArea() const
        { return myPrim->calcArea(*this); }
        SYS_FORCE_INLINE fpreal calcVolume(const UT_Vector3 &refpt) const
        { return myPrim->calcVolume(refpt, *this); }
        SYS_FORCE_INLINE float getW() const
        { return UTpackIntToSafeFloat(polygon()); }

        /// These are just here for interchangeability with GEO_Face
        SYS_FORCE_INLINE GA_Offset getFastVertexOffset(GA_Size i) const
        { return getVertexOffset(i); }
        SYS_FORCE_INLINE GA_Size getFastVertexCount() const
        { return nvertices(); }
        SYS_FORCE_INLINE GA_Size getVertexCount() const
        { return nvertices(); }
        SYS_FORCE_INLINE bool isClosed() const
        { return true; }
    private:
        const GEO_PrimPolySoup *myPrim;
    };

    class VertexIterator
    {
    public:
        VertexIterator(const PolygonIterator &it, GA_Size vtxnum = 0)
            : myPrim(it.prim())
            , myIndex(it.start()+vtxnum)
            , myEnd(it.start()+it.nvertices())
        {}
        bool atEnd() const
        {
            return myIndex == myEnd;
        }
        VertexIterator &operator++()
        {
            ++myIndex;
            return *this;
        }
        VertexIterator &operator--()
        {
            --myIndex;
            return *this;
        }
        VertexIterator operator+(GA_Size i)
        {
            return VertexIterator(myPrim, myIndex + i, myEnd);
        }
        VertexIterator operator-(GA_Size i)
        {
            return VertexIterator(myPrim, myIndex - i, myEnd);
        }
        GA_Size number() const
        {
            return myIndex;
        }
        GA_Offset offset() const
        {
            return myPrim.myPolygonVertexList(myIndex);
        }
    private:
        VertexIterator(const GEO_PrimPolySoup &prim, GA_Size index, GA_Size end)
            : myPrim(prim)
            , myIndex(index)
            , myEnd(end)
        {}
        const GEO_PrimPolySoup &myPrim;
        GA_Size myIndex;
        GA_Size myEnd;
    };

    /// Have we been deactivated and stashed?
    void	stashed(bool beingstashed,
			GA_Offset offset = GA_INVALID_OFFSET) override;

    /// @{
    /// Interface from GEO_TriMesh
    UT_Vector3	computeNormal() const override;
    void	reverse() override;

    fpreal	calcVolume(const UT_Vector3 &refpt) const override;
    fpreal	calcArea() const override;
    fpreal	calcPerimeter() const override;
    /// @}

    // This is here only to avoid a compile error on baryCenter(it) below
    UT_Vector3 baryCenter() const override
    { return GEO_TriMesh::baryCenter(); }

    UT_Vector3 computeNormal(const PolygonIterator &it) const;
    UT_Vector3 baryCenter(const PolygonIterator &it) const;
    fpreal     calcVolume(const UT_Vector3 &refpt, const PolygonIterator &it) const;
    fpreal     calcArea(const PolygonIterator &it) const;
    fpreal     calcPerimeter(const PolygonIterator &it) const;

    /// Get number of polygons in the soup
    GA_Size getPolygonCount() const
    {
        return myPolygonSizeList.getNumPolygons();
    }

    /// Get the number of vertices in the polygon
    GA_Size getPolygonSize(GA_Size polygon) const
    {
        UT_ASSERT_P(myPolygonSizeList.getArray().entries() != 0);
        return myPolygonSizeList.getSize(polygon);
    }

    /// Get the start offset into myPolygonVertexList of vertices for a
    /// single polygon in the soup and the number of vertices in the polygon
    void  getPolygonVertexRange(GA_Size polygon,
                                GA_Size &start,
                                GA_Size &count) const
    {
        UT_ASSERT_P(myPolygonSizeList.getArray().entries() != 0);
        myPolygonSizeList.getVertexRange(polygon, start, count);
    }

    GA_Offset getPolygonVertexOffset(GA_Size polygon,
                                     GA_Size vertexofpolygon) const
    {
        GA_Size start;
        GA_Size count;
        getPolygonVertexRange(polygon, start, count);
        UT_ASSERT_P(vertexofpolygon < count);
        return myPolygonVertexList(start + vertexofpolygon);
    }

    GA_Offset getPolygonVertexOffset(const PolygonIterator &it,
                                     GA_Size vertexofpolygon) const
    {
        return myPolygonVertexList(it.start() + vertexofpolygon);
    }

    inline void	getTriangleVertexOffsets(GA_Size start,
	    GA_Offset &v0,
	    GA_Offset &v1,
	    GA_Offset &v2) const
    {
        v0 = myPolygonVertexList(start);
        v1 = myPolygonVertexList(start+1);
        v2 = myPolygonVertexList(start+2);
    }

    inline void	getQuadVertexOffsets(GA_Size start,
	    GA_Offset &v0,
	    GA_Offset &v1,
	    GA_Offset &v2,
	    GA_Offset &v3) const
    {
        v0 = myPolygonVertexList(start);
        v1 = myPolygonVertexList(start+1);
        v2 = myPolygonVertexList(start+2);
        v3 = myPolygonVertexList(start+3);
    }

    /// Return the maximum number of vertices in any face
    inline GA_Size getMaxPolygonSize() const
    {
        const UT_Array<GA_PolyCounts::SizeCount> &array
                                           = myPolygonSizeList.getArray();
        if (!array.entries())
            return -1;
        GA_Size mx = array(0).size();
        for (GA_Size i = 1; i < array.entries(); ++i)
            mx = SYSmax(mx, array(i).size());
        return mx;
    }

    /// Return the minimum number of vertices in any face
    inline GA_Size getMinPolygonSize() const
    {
        const UT_Array<GA_PolyCounts::SizeCount> &array
                                           = myPolygonSizeList.getArray();
        if (!array.entries())
            return -1;
        GA_Size mn = array(0).size();
        for (GA_Size i = 1; i < array.entries(); ++i)
            mn = SYSmin(mn, array(i).size());
        return mn;
    }

    /// Return the minimum and maximum number of vertices for any face
    inline void getMinMaxPolygonSize(GA_Size &mn, GA_Size &mx) const
    {
        const UT_Array<GA_PolyCounts::SizeCount> &array
                                           = myPolygonSizeList.getArray();
        if (!array.entries())
	{
	    mn = mx = -1;
            return;
	}
        mn = mx = array(0).size();
        for (GA_Size i = 1; i < array.entries(); ++i)
	{
            mn = SYSmin(mn, array(i).size());
            mx = SYSmax(mx, array(i).size());
	}
    }

    const GA_PrimitiveJSON	*getJSON() const override;
    /// @{
    /// Save/Load polygon list to a JSON stream
    bool		savePolygonInfo(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadPolygonInfo(UT_JSONParser &p,
				const GA_LoadMap &map);
    /// @}

    /// Appends a copy of a single polygon to this PolySoup,
    /// with all vertices unique
    void appendPolygonCopyUnique(const GEO_PrimPoly *polygon,
        GA_VertexWrangler &wrangler);
    /// Appends a copy of a single polygon to this PolySoup,
    /// with any vertices whose attributes match being shared among
    /// polygons in this PolySoup
    void appendPolygonCopyShared(const GEO_PrimPoly *polygon,
        GA_VertexWrangler &wrangler, GA_AttributeRefMap &attributemap);
    /// Appends a copy of a single polygon to this PolySoup,
    /// with all vertices unique
    void appendPolygonCopyUnique(
        const GA_OffsetListRef &polygon, const GA_Detail &srcdetail,
        GA_VertexWrangler &wrangler);
    /// Appends a copy of a single polygon to this PolySoup,
    /// with any vertices whose attributes match being shared among
    /// polygons in this PolySoup
    void appendPolygonCopyShared(
        const GA_OffsetListRef &polygon, const GA_Detail &srcdetail,
        GA_VertexWrangler &wrangler, GA_AttributeRefMap &attributemap);
    /// Appends a copy of another PolySoup to this PolySoup,
    /// with all vertices unique
    void appendPolySoupCopyUnique(const GEO_PrimPolySoup *that,
        GA_VertexWrangler &wrangler);
    /// Appends a copy of another PolySoup to this PolySoup,
    /// with any vertices whose attributes match being shared among
    /// polygons in this PolySoup
    void appendPolySoupCopyShared(const GEO_PrimPolySoup *that,
        GA_VertexWrangler &wrangler, GA_AttributeRefMap &attributemap);

    /// Appends a polygon to this PolySoup from an array of vertex offsets
    /// NOTE: This will not wire the vertices to points or to this primitive,
    ///       so that must be done manually.  This assumes the vertices have
    ///       already been added to the GEO_TriMesh level with appendVertex
    ///       or appendVertices.
    void appendPolygon(const UT_Array<GA_Offset> &vertices);

    /// Appends polygons to this PolySoup from an array of vertex offsets
    /// NOTE: This will not wire the vertices to points or to this primitive,
    ///       so that must be done manually.  This assumes the vertices have
    ///       already been added to the GEO_TriMesh level with appendVertex
    ///       or appendVertices.
    void appendPolygons(const GA_PolyCounts &sizes,
                        const GA_OffsetList &vertices);

    /// Returns true iff none of the vertices of the PolySoup are shared
    /// among multiple polygons in this PolySoup.
    /// NOTE: This assumes that every vertex of the PolySoup is in at least
    ///       one polygon in this PolySoup.
    bool areAllVerticesUnique() const
    { return myPolygonVertexList.entries() == myVertexList.entries(); }

    /// Forces all vertices in this polysoup to be unshared, i.e. creates
    /// new vertices if any are used by multiple polygons in this polysoup.
    /// NOTE: This assumes that every vertex of the PolySoup is in at least
    ///       one polygon in this PolySoup, since it calls
    ///       areAllVerticesUnique().
    void makeAllVerticesUnique();

    /// Optimizes the internal representation of the PolySoup
    void optimize();

    /// Remove repeated point offsets in the polygon vertices, optionally
    /// checking distance to remove points within a specified tolerance,
    /// then remove all degenerate polygons.
    GA_Size cleanData(const bool removeNearPoints = false, float tol = 0.001F,
                      bool deleteOrphanedPoints = true);

    /// Remove polygons that have 0, 1, or 2 vertices, optionally deleting
    /// orphaned points.
    GA_Size remove012(bool deleteOrphanedPoints = true);

    /// Remove vertices that are not used by any of the soup's polygons,
    /// and optionally remove their points if they are also unused.
    void deleteUnusedVertices(bool andpoints = true);

    /// Replaces specified polygons with new polygons using the same vertices,
    /// which is necessary for convexing. polygonstoremesh must be monotone
    /// increasing.
    void remeshPolygons(const UT_Array<GA_Size> &polygonstoremesh,
                        const GA_OffsetList     &newvertices,
                        const GA_PolyCounts     &newpolygonsizelist);

    GA_Primitive::GA_DereferenceStatus dereferencePoints(
                    const GA_RangeMemberQuery &point_query,
                    bool dry_run) override;

    void copyPrimitive(const GEO_Primitive *psrc) override;
    GEO_Primitive *copy(int preserve_shared_pts) const override;
    void copyUnwiredForMerge(const GA_Primitive *prim_src,
			     const GA_MergeMap &map) override;

    /// Defragmentation
    void swapVertexOffsets(const GA_Defragment &defrag) override;

#if 0
    /// Converts myPolygonVertexList from storing GA_Offsets to storing
    /// indices into myVertexList.
    void offsetsToIndirection();
    /// Converts myPolygonVertexList from indices of myVertexList back
    /// into GA_Offsets
    void indirectionToOffsets();
#endif

    GA_Size getPolygonVertexCount() const
    {
        return myPolygonVertexList.entries();
    }

    /// These are only here because of C++ compilers erroring on the
    /// appendVertex function below, thinking it should hide these.
    /// NOTE: This will not add the vertex to myPolygonVertexList,
    ///       in case it is to be shared, so use
    ///       appendPolygon(const UT_Array<GA_Offset> &vertices)
    ///       to add a polygon with the vertex.
    /// NOTE: If you leave vertices in the polysoup that are not used by any
    ///       polygons, areAllVerticesUnique() may return incorrect results,
    ///       since it just checks if the size of myPolygonVertexList is
    ///       larger than the size of myVertexList
    GA_Size appendVertex(GA_Offset ptoff) override
    { return GEO_TriMesh::appendVertex(ptoff); }

    /// This appends one vertex for each of nvertices points starting
    /// from startptoff, in parallel, and wires them to the points and
    /// this polysoup, then returns the starting vertex offset.  Since
    /// each point has only one vertex, this applies to making shared
    /// vertices.
    /// NOTE: The points can't be used by any other primitives at the
    ///       time that this is called.
    /// NOTE: If you leave vertices in the polysoup that are not used by any
    ///       polygons, areAllVerticesUnique() may return incorrect results,
    ///       since it just checks if the size of myPolygonVertexList is
    ///       larger than the size of myVertexList
    GA_Offset appendSharedVertices(GA_Offset startptoff, GA_Size nvertices);

    void iterateEdges(GA_IterateEdgesFunc apply_func) const override
    {
        GA_Detail &detail = getDetail();
        for (PolygonIterator it(*this); !it.atEnd(); ++it)
        {
            if (it.nvertices() == 0)
                continue;
            VertexIterator vit(it);
            const GA_Offset startptoff = detail.vertexPoint(vit.offset());
            GA_Offset prevptoff = startptoff;
            for (++vit; !vit.atEnd(); ++vit)
            {
                const GA_Offset currptoff = detail.vertexPoint(vit.offset());
                if (!apply_func(GA_Edge(prevptoff, currptoff)))
                    return;
                prevptoff = currptoff;
            }
            // NOTE: GEO_Face::edgeApply always has a wrapping edge for
            //       closed polygons, even with just 1 or 2 vertices.
            //if (it.nvertices() > 2)
		if (!apply_func(GA_Edge(prevptoff, startptoff)))
		    return;
        }
    }
    void iterateEdgesByVertex(GA_IterateEdgesByVertexFunc apply_func) const override;

    /// This does a linear search through the whole soup.
    /// You may regret calling it for each edge of a large soup.
    bool hasEdge(const GA_Edge &edge) const override
    {
        GA_Offset a = edge.p0();
        GA_Offset b = edge.p1();
        for (PolygonIterator it(*this); !it.atEnd(); ++it)
        {
            GA_Size n = it.nvertices();
            if (n == 0)
                continue;
            const GA_Offset startptoff = it.getPointOffset(0);
            GA_Offset p0 = startptoff;
            for (GA_Size i = 1; i < n; ++i)
            {
                GA_Offset p1 = it.getPointOffset(i);
                if ( ((p0 == a) && (p1 == b)) || ((p0 == b) && (p1 == a)) )
                    return true;
                p0 = p1;
            }
            // NOTE: GEO_Face::edgeApply always has a wrapping edge for
            //       closed polygons, even with just 1 or 2 vertices, even
            //       though GEO_Face::hasEdge doesn't accept the self-edge
            //       if there's only one vertex.
            //if (it.nvertices() > 2)
            GA_Offset p1 = startptoff;
            if ( ((p0 == a) && (p1 == b)) || ((p0 == b) && (p1 == a)) )
                return true;
        }
        return false;
    }

    virtual void computeInteriorPointWeights(
			    UT_Array<GA_Offset> &vtxlist,
			    UT_Array<float> &weightlist,
			    fpreal u, fpreal v, fpreal w) const override;

    /// Create polygon soup(s), i.e. GEO_PrimPolySoup, where possible in dest_detail.
    /// src_detail *can* be the same as dest_detail.
    static void convertDetail(GEO_Detail *dest_detail, const GEO_Detail *src_detail, const GEO_PolySoupParms &parms);

    virtual void convex(GA_Size maxpts = 3, const GA_Detail *restgdp = nullptr,
        bool flipedges = false, bool avoiddegeneracy = false) = 0;

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_TriMesh::buildFamilyMask(); }

    // Declare intrinsic definitions
    GA_DECLARE_INTRINSICS(override)

    void appendUniqueVertexCopy(const GA_Offset vertex,
                                const GA_Detail &srcdetail,
                                GA_VertexWrangler &wrangler);
    void appendSharedVertexCopy(const GA_Offset vertex,
                                const GA_Detail &srcdetail,
                                GA_VertexWrangler &wrangler,
                                GA_AttributeRefMap &attributemap);

    /// Evaluates the position at domain point (u,v) in the interior of the
    /// geometry, rather than on the perimeter of the poly.
    /// (Note that u, v don't have to be converted to unit domain.  We assume
    /// that the barycentric coords or bilinear interpolants are already 
    /// correctly scaled).
    bool evaluateInteriorPointRefMap(GA_Offset result_vtx,
				     GA_AttributeRefMap &hlist,
				     fpreal u, fpreal v, fpreal w) const override;
    int evaluateInteriorPointV4(UT_Vector4 &pos,
				fpreal u, fpreal v, fpreal w) const override;
    int	evaluateNormalVector(UT_Vector3 &nml, 
			     float u, float v, float w) const override;

    bool	savePrivateH9(std::ostream &, bool binary) const override;
    bool	loadPrivateH9(UT_IStream &) override;

    GA_PolyCounts myPolygonSizeList;
    GA_OffsetList myPolygonVertexList;

private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimPolySoup &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

/// Structure for the PolySouping code
class GEO_API GEO_PolySoupParms
{
public:
    const GA_PrimitiveGroup *primGroup;
    bool                     ignorePrimAttributes;
    bool                     ignorePrimGroups;
    int                      minPolys;
    bool                     convex;
    int                      maxSides;
    bool                     deleteSoupedPrimitives;
    GA_PrimCompat::TypeMask  typeMask;
    bool                     mergeVertices;

    GEO_PolySoupParms(const GA_PrimitiveGroup *pg = 0,
        bool ignattr = false,
        bool igngrp = false,
        int minp = 1,
        bool cvx = false,
        int maxs = 3,
        bool delprims = true,
        GA_PrimCompat::TypeMask mask = GEO_PrimTypeCompat::GEOPRIMALL,
        bool mergeverts = true)
        : primGroup(pg)
        , ignorePrimAttributes(ignattr)
        , ignorePrimGroups(igngrp)
        , minPolys(minp)
        , convex(cvx)
        , maxSides(maxs)
        , deleteSoupedPrimitives(delprims)
        , typeMask(mask)
        , mergeVertices(mergeverts)
    { }
};

#endif

