/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_Interpolate.h (GEO Library, C++)
 *
 * COMMENTS:    Declarations for attribute interpolation functions
 */

#pragma once

#ifndef __GEO_Interpolate__
#define __GEO_Interpolate__

#include "GEO_API.h"
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_PolySample.h>
#include <UT/UT_Vector3.h>
#include <SYS/SYS_Math.h>

class GEO_Detail;
class GA_AttributeRefMap;
enum GA_AttributeOwner;
class GA_Attribute;
class GA_Range;

/// Interpoates attributes based on primitive index and (u,v,w)
/// INT must be int32 or int64.
template<typename INT>
void geoInterpolateAttributes(
    const GEO_Detail *source,
    const GA_AttributeRefMap &attribmap,
    const GA_Range &destrange,
    const GA_ROHandleT<INT> &primnumattrib,
    const GA_ROHandleV3 &primuvwattrib);

/// Interpoates attributes based on primitive number and (u,v,w),
/// and fills in computeindexattrib and computeweightattrib
/// with the point or vertex indices and weights that produced
/// the interpolation.  computenumbersof determines whether
/// computeindexattrib receives point or vertex indices.
/// computeindexattrib and computeweightattrib must implement GA_AIFNumericArray.
/// INT must be int32 or int64.
template<typename INT>
void geoInterpolateAttributes(
    const GEO_Detail *source,
    const GA_AttributeRefMap &attribmap,
    const GA_Range &destrange,
    const GA_ROHandleT<INT> &primnumattrib,
    const GA_ROHandleV3 &primuvwattrib,
    GA_AttributeOwner computenumbersof,
    GA_Attribute *computeindexattrib,
    GA_Attribute *computeweightattrib);


/// Interpoates attributes based on arrays of indices and weights specified in
/// indexattrib and weightattrib.  indexowner specifies whether the source
/// indices are for points, vertices, or primitives.
/// indexattrib and weightattrib must implement GA_AIFNumericArray.
GEO_API void geoInterpolateAttributes(
    const GEO_Detail *source,
    const GA_AttributeRefMap &attribmap,
    const GA_Range &destrange,
    GA_AttributeOwner indexowner,
    const GA_Attribute *indexattrib,
    const GA_Attribute *weightattrib);

template<typename T,typename S>
inline void GEOquadInterpolationWeights(T u, T v, S weights[4])
{
    T u1 = 1-u;
    T v1 = 1-v;
    weights[0] = (u1 * v1);
    weights[1] = (u1 * v);
    weights[2] = (u * v);
    weights[3] = (u * v1);
}

template<typename T,typename S>
inline void GEOtriInterpolationWeights(T u, T v, S weights[3])
{
    // Triangle - use barycentric coordinates

    // This is a hack to make sure we are given proper
    // barycentric u, v coordinates.  That is, we require
    // u+v <= 1, and if that's not the case we hack it so
    // u = 1-u, v = 1-v, thus ensuring u+v <= 1.  (This 
    // assumes that u, v are each between 0 and 1)
    // This is used for when evaluateInteriorPoint is
    // called from POP_GenVar for birthing from a surface.
    //
    // Note we actually flip on the u+v = 1 axis instead
    // of what is described above so slightly outside points
    // do not teleport to opposite locations.
    T uv = 1 - u - v;

    if (uv < 0)
    {
        u += uv;
        v += uv;
        uv = -uv;
    }

    weights[0] = uv;
    weights[1] = u;
    weights[2] = v;
}

template <typename T>
inline void
GEOgetXYProjection(const UT_Vector3T<T> &nml, int &x, int &y)
{
    int z = (SYSabs(nml.y()) > SYSabs(nml.x())) ? 1 : 0;
    if (SYSabs(nml.z()) > SYSabs(nml(z))) z = 2;
    switch (z)
    {
        case 0: x = 1; y = 2; break;
        case 1: x = 0; y = 2; break;
        case 2: x = 0; y = 1; break;
    }
}

template<typename POLY>
static void
GEOprimUVWPrecompute(const POLY &poly, int &xaxis, int &yaxis, float &centrex, float &centrey)
{
    // NOTE: This is trying to be equivalent to GU_RayPolyInfo, though that
    //       may not matter.

    GA_Size nvertices = poly.getFastVertexCount();
    UT_Vector3 normal(0, 0, 0);
    UT_Vector3 centre(0, 0, 0);
    UT_Vector3 p2 = poly.getPos3(nvertices-1);
    for (GA_Size i = 0; i < nvertices; ++i)
    {
        UT_Vector3 p1 = p2;
        p2 = poly.getPos3(i);
        centre += p2;
        // Use Newell's method to accumulate the face normal
        normal.normal(p1, p2);
    }
    // NOTE: GEOgetXYProjection works without normalizing normal
    GEOgetXYProjection(normal, xaxis, yaxis);
    centrex = centre[xaxis]/float(nvertices);
    centrey = centre[yaxis]/float(nvertices);
}

template<typename POLY>
static UT_Vector3
GEOprimUVWNonConvex(const POLY &poly, const UT_Vector3 &p, int xaxis, int yaxis, float centrex, float centrey, const float w=0)
{
    // NOTE: Although this is slow to do for every point, it's what GU_RayPolyInfo::intersect does

    float x0 = p[xaxis];
    float y0 = p[yaxis];

    const GA_Size n = poly.getFastVertexCount();

    UT_ASSERT_P(n >= 5);

    // Calculate the u & v values.  Use UT_SampleTri2D.
    UT_SampleTri2D sample;
    GA_Size li = n-1;
    UT_Vector3 p0 = poly.getPos3(li);
    bool found = false;
    UT_Vector3 output(0, 0, w);
    for (GA_Size i = 0; i < n; ++i)
    {
        UT_Vector3 p1 = poly.getPos3(i);
        sample.setVertices(p0[xaxis], p0[yaxis],
                           p1[xaxis], p1[yaxis],
                           centrex, centrey);
        // If we have a valid hit, process into the correct space and break.
        // Because of roundoff error, a point that should be inside may
        // be incorrectly identified as outside, so we allow samples that
        // are in [-1e-6, 1+1e-6] for all 3 barycentric coordinates.
        if (sample.sample(x0, y0, output[0], output[1], 1e-6))
        {
            found = true;

            // With the tolerance, it might be just outside, in which case,
            // keep checking for another triangle that might be a direct hit.
            bool isedgecase = (output[0] < 0 || output[1] < 0 || output[0]+output[1] > 1);
            if (isedgecase)
            {
                // Force it to be in range
                output[0] = SYSclamp(output[0], 0.0f, 1.0f);
                output[1] = SYSclamp(output[1], 0.0f, 1.0f);
                if (output[0]+output[1] > 1)
                    output[1] = 1.0f - output[0];
            }
            // We have v == 1 being barycenter.
            // u == 0 is li, and u == 1 is i.
            if (output[1] != 1.0f)
                output[0] /= 1.0f - output[1];
            output[0] = (output[0] + float(li)) / float(n);

            if (!isedgecase)
                break;
        }
        li = i;
        p0 = p1;
    }
    if (found)
        return output;

    // Nothing hit directly; restart and
    // try clamping to triangle and getting min (2D) distance.
    // NOTE: This is not guaranteed to get the minimum distance,
    //       even in 2D, because we're just clamping in parametric
    //       space.  This is just to ensure that we always return
    //       something in the ballpark of the sample.
    li = n-1;
    p0 = poly.getPos3(li);
    float mind2;
    GA_Size outputli;
    UT_Vector2 puv(p0[xaxis], p0[yaxis]);
    UT_Vector2 pv(centrex, centrey);
    UT_Vector2 ps(p[xaxis], p[yaxis]);
    for (GA_Size i = 0; i < n; ++i)
    {
        UT_Vector3 p1 = poly.getPos3(i);
        // Confusingly, du is p1-p0, and dv is centre-p0
        UT_Vector2 pu(p1[xaxis], p1[yaxis]);
        UT_Vector2 du = pu - puv;
        UT_Vector2 dv = pv - puv;
        UT_Vector2 s = ps - puv;
        float scale = 1.0f / cross(du, dv);
        if (!SYSisFinite(scale))
            continue;
        float u = scale * cross(s, dv);
        float v = scale * cross(du, s);

        // Force it to be in range
        u = SYSclamp(u, 0.0f, 1.0f);
        v = SYSclamp(v, 0.0f, 1.0f);
        if (u + v > 1.0f)
            v = 1.0f - u;

        float weights[3];
        GEOtriInterpolationWeights(u, v, weights);

        UT_Vector2 clampedp = weights[0]*puv + weights[1]*pu + weights[2]*pv;
        float d2 = distance2d(clampedp, ps);
        if (!found || d2 < mind2)
        {
            found = true;
            mind2 = d2;
            output[0] = u;
            output[1] = v;
            outputli = li;
        }
        li = i;
        puv = pu;
    }
    // We have v == 1 being barycenter.
    // u == 0 is li, and u == 1 is i.
    if (output[1] != 1.0f)
        output[0] /= 1.0f - output[1];
    output[0] = (output[0] + float(outputli)) / float(n);
    return output;
}

/// Fill the offsets and weights arrays with the vertex/point offsets
/// and their corresponding interpolation weights for the given primitive
/// parametric space coordinate uvw, for the specified primitive.
///
/// This acts like GEO_Primitive::computeInteriorPointWeights(), except that
/// it avoids hardening the primitive list if the primitive is a polygon or
/// tetrahedron, and if is_point_offsets is true, it will convert the offsets
/// to point offsets, instead of staying with vertex offsets.
GEO_API void
GEOgetInterpolationWeights(
    const GEO_Detail *detail,
    GA_Offset primoff,
    UT_Array<GA_Offset> &offsets,
    UT_FloatArray &weights,
    const UT_Vector3 &uvw,
    bool is_point_offsets=false);

#endif
