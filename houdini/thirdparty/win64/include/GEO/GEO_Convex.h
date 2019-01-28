/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_Convex.h (GEO Library, C++)
 *
 * COMMENTS:    Functions for convexing without modifying polygons
 */

#pragma once

#ifndef __GEO_Convex__
#define __GEO_Convex__

#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_Convex.h>
#include <UT/UT_OBBox.h>
#include <UT/UT_Vector3.h>
#include <SYS/SYS_Inline.h>

template<typename T>
struct GEO_TriOrQuadT
{
    GEO_TriOrQuadT() {}
    GEO_TriOrQuadT(T v0, T v1, T v2, T v3=T(-1))
    {
        vertices[0] = v0;
        vertices[1] = v1;
        vertices[2] = v2;
        vertices[3] = v3;
    }
    SYS_FORCE_INLINE void makeInvalid()
    {
        vertices[0] = T(-1);
        vertices[1] = T(-1);
        vertices[2] = T(-1);
        vertices[3] = T(-1);
    }
    SYS_FORCE_INLINE void makeTri()
    { vertices[3] = T(-1); }
    SYS_FORCE_INLINE bool isTri() const
    { return vertices[3] == T(-1); }

    T vertices[4];
};
typedef GEO_TriOrQuadT<GA_Size> GEO_TriOrQuad;

template<typename POLY>
class GEO_TriOrQuadWrapper {
public:
    GEO_TriOrQuadWrapper(const POLY &poly, const GEO_TriOrQuad &convex)
        : myPoly(poly)
        , myConvex(convex)
    {}
    SYS_FORCE_INLINE GA_Size getFastVertexCount() const
    { return myConvex.isTri() ? 3 : 4; }
    SYS_FORCE_INLINE GA_Offset getFastVertexOffset(GA_Size i) const
    { return myPoly.getFastVertexOffset(myConvex.vertices[i]); }
    SYS_FORCE_INLINE GA_Offset getPointOffset(GA_Size i) const
    { return myPoly.getPointOffset(myConvex.vertices[i]); }
    SYS_FORCE_INLINE UT_Vector3 getPos3(GA_Size i) const
    { return myPoly.getPos3(myConvex.vertices[i]); }
    float calcArea() const
    {
        UT_Vector3 p[4];
        for (GA_Size i = 0; i < getFastVertexCount(); ++i)
            p[i] = getPos3(i);
        float area = cross(p[0]-p[1],p[2]-p[1]).length();
        if (!myConvex.isTri())
            area += cross(p[0]-p[3],p[2]-p[3]).length();
        area *= 0.5f;
        return area;
    }
private:
    const POLY &myPoly;
    const GEO_TriOrQuad &myConvex;
};

namespace {
template<typename POLY>
class geo_Convexer : private UT_Convex
{
public:
    geo_Convexer(const POLY &poly, UT_Array<GEO_TriOrQuad> &output,
                 const UT_Vector3 &x, const UT_Vector3 &y,
                 bool triangulate)
        : myInputPoly(poly)
        , myOutputPolys(output)
        , myX(x)
        , myY(y)
    {
        GA_Size nvertices = poly.getFastVertexCount();

        // Turn everything into convex quads and triangles
        // NOTE: We keep degenerate output polygons so that we reduce the
        //       risk of messing up the case where we must scatter an exact
        //       number of points on a degenerate input polygon.
        // Only allow interruption for large polygons, since it's slow.
        UT_Convex::convex(triangulate ? 3 : 4, nvertices > 256);
    }
private:
    virtual int getPointCount() const
    { return myInputPoly.getFastVertexCount(); }
    virtual void getPoint(int num, float &x, float &y) const
    {
        UT_Vector3 pos = myInputPoly.getPos3(num);
        x = dot(pos, myX);
        y = dot(pos, myY);
    }
    virtual void beginPolygon(int npoints, bool lastone)
    {
        UT_ASSERT_P(npoints == 3 || npoints == 4);
        myOutputPolys.append();
        myOutputPolys.last().makeTri();
        myI = 0;
    }
    virtual void polyVertex(int num)
    {
        UT_ASSERT_P(myI < 4);
        myOutputPolys.last().vertices[myI] = num;
        ++myI;
    }
    virtual void endPolygon()
    { UT_ASSERT_P(myI == 3 || myI == 4); }

    const POLY &myInputPoly;
    int myI;
    UT_Array<GEO_TriOrQuad> &myOutputPolys;
    UT_Vector3 myX;
    UT_Vector3 myY;
};
}

/// Convexes polygon into tris and quads.
/// Writes to output if polygon needed convexing.
/// Returns true if output was written-to.
/// Mantra keeps non-convex quads and doesn't triangulate, hence the default.
template<typename POLY>
bool GEOconvexPoly(const POLY &poly, UT_Array<GEO_TriOrQuad> &output, bool keepnonconvexquads = true, bool triangulate = false)
{
    output.setSize(0);
    GA_Size nvertices = poly.getFastVertexCount();
    if (nvertices <= 3)
        return false;
    if (nvertices == 4)
    {
        if (keepnonconvexquads)
            return false;

        // Avoid using UT_Convex::convex for quads; it's slow.
        UT_Vector3 p0 = poly.getPos3(0);
        UT_Vector3 p1 = poly.getPos3(1);
        UT_Vector3 p2 = poly.getPos3(2);
        UT_Vector3 p3 = poly.getPos3(3);
        bool split02 = UT_Convex::canSplitQuad02(p0, p1, p2, p3);
        if (!split02)
        {
            // Split 1-3
            // Be sure to use the same winding order!
            output.setSize(2);
            output(0) = GEO_TriOrQuad(0, 1, 3);
            output(1) = GEO_TriOrQuad(1, 2, 3);
            return true;
        }
        bool split13 = !triangulate && UT_Convex::canSplitQuad02(p1, p2, p3, p0);
        if (!split13)
        {
            // Split 0-2
            output.setSize(2);
            output(0) = GEO_TriOrQuad(0, 1, 2);
            output(1) = GEO_TriOrQuad(0, 2, 3);
            return true;
        }
        return false;
    }

    output.setSize(0);
    // Minimum capacity needed is (n-1)/2, which is hit if divides into
    // all quads except possibly one triangle if odd number of vertices.
    GA_Size minquads = (nvertices-1)/2;
    output.bumpCapacity(minquads);

    UT_Vector3 normal = poly.computeNormal();

    // In the unlikely event that our normal is garbage, go all out
    // with computing an oriented bounding box to get a reasonable
    // normal.
    if (normal.isEqual(UT_Vector3(0,0,0)))
    {
        UT_Array<UT_Vector3> temppoints(nvertices, nvertices);
        for (GA_Size i = 0; i < nvertices; ++i)
            temppoints(i) = poly.getPos3(i);
        UT_OBBoxF obbox(temppoints);
        normal = obbox.getMinAxis();
    }
    UT_Vector3 x;
    UT_Vector3 y;
    normal.getFrameOfReference(y, x);
    geo_Convexer<POLY>(poly, output, x, y, triangulate);
    UT_ASSERT_P(output.size() >= minquads);
    return true;
}

/// Triangulates polygon.
/// Writes to output if polygon had more than 3 vertices.
/// Returns true if output was written-to.
template<typename POLY>
SYS_FORCE_INLINE bool
GEOtriangulatePoly(const POLY &poly, UT_Array<GEO_TriOrQuad> &output)
{
    return GEOconvexPoly(poly, output, false, true);
}

#endif
