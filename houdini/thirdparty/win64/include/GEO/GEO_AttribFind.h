/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_AttribFind.h (GEO Library, C++)
 *
 * COMMENTS:    Declarations of functions for finding interpolated
 *              attribute values
 */

#pragma once

#ifndef __GEO_AttribFind__
#define __GEO_AttribFind__

#include "GEO_API.h"
#include <GA/GA_Handle.h>
#include <UT/UT_Array.h>
#include <UT/UT_KDTree.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

class GEO_Detail;
class GEO_PrimTetrahedron;
template<typename T>
struct GEO_TriOrQuadT;

class GEO_API GEO_2DTree : public UT_KDTree
{
public:
    GEO_2DTree(const UT_Array<UT_Vector2> &points)
        : UT_KDTree(2, points.entries())
        , myPoints(points)
    {
        // Override the balancer to be centroid, it should behave nicer
        // for very large data sets.  (Mantra uses centroid for photons already)
        setBalancer(UT_KD_CENTROID);

        // Doing this prior to querying in multithreaded code
        // avoids extra locks.
        balance();
    }

    /// NOTE: Although this is non-const, it's safe to call from multiple threads.
    int findAllInsideTri(
        UT_IntArray &list,
        UT_Array<UT_Vector2> &offs,
        ut_KDPQueue &q,
        const UT_Vector2 &a,
        const UT_Vector2 &b,
        const UT_Vector2 &c,
        float tolerance = 1e-7f,
        bool wrapunitsquare = false);

    void updatePoints(bool enable_multithreading = true)
    {
        flagDirty();
        balance(enable_multithreading);
    }
    virtual void buildIfNeeded(bool enable_multithreading = true)
    {
        updatePoints(enable_multithreading);
    }
    const UT_Vector2 &getValue(int idx) const
    {
        return myPoints(idx);
    }
private:
    virtual const float *getP(int idx) const
    {
        return myPoints(idx).data();
    }
    virtual int comparePosition(int idx0, int idx1, int dim) const
    {
        float delta = myPoints(idx0)(dim) - myPoints(idx1)(dim);
        if (delta < 0)
            return -1;
        if (delta > 0)
            return 1;
        return 0;
    }
    const UT_Array<UT_Vector2> &myPoints;
};

class GEO_API GEO_3DTree : private UT_KDTree
{
public:
    GEO_3DTree(const UT_Array<UT_Vector3> &points)
        : UT_KDTree(3, points.entries())
        , myPoints(points)
    {
        // Override the balancer to be centroid, it should behave nicer
        // for very large data sets.  (Mantra uses centroid for photons already)
        setBalancer(UT_KD_CENTROID);

        // Doing this prior to querying in multithreaded code
        // avoids extra locks.
        balance();
    }

    /// NOTE: Although this is non-const, it's safe to call from multiple threads.
    int findAllInsideTet(
        UT_IntArray &list,
        UT_Array<UT_Vector3> &offs,
        ut_KDPQueue &q,
        const UT_Vector3 &a,
        const UT_Vector3 &b,
        const UT_Vector3 &c,
        const UT_Vector3 &d,
        float tolerance = 1e-7f,
        bool wrapunitcube = false);

    void updatePoints()
    {
        flagDirty();
        balance();
    }
    const UT_Vector3 &getValue(int idx) const
    {
        return myPoints(idx);
    }
private:
    virtual const float *getP(int idx) const
    {
        return myPoints(idx).data();
    }
    virtual int comparePosition(int idx0, int idx1, int dim) const
    {
        float delta = myPoints(idx0)(dim) - myPoints(idx1)(dim);
        if (delta < 0)
            return -1;
        if (delta > 0)
            return 1;
        return 0;
    }
    const UT_Array<UT_Vector3> &myPoints;
};

/// NOTE: Although tree is non-const, this is safe to call from multiple
///       threads with the same tree.
template<typename POLY>
void GEOpickUpAttribPoints(
    const POLY &poly,
    const UT_Vector2 *attribvalues,// 3 or 4 values, depending on tri or quad
    GEO_2DTree &tree,
    ut_KDPQueue *kdqueue,        // Just here to avoid reallocating on every query
    UT_IntArray &temp,           // ''
    UT_Array<UT_Vector2> &temp2, // ''
    UT_Array<UT_Vector3> &primuvwsarray,
    UT_Array<exint> &treepointnumsarray,
    float tolerance = 1e-7f,
    bool wrapunitsquare = false);

/// NOTE: Although tree is non-const, this is safe to call from multiple
///       threads with the same tree.
void GEO_API GEOpickUpAttribPoints(
    const GEO_PrimTetrahedron &tet,
    const UT_Vector3 *attribvalues,// 4 values
    GEO_3DTree &tree,
    ut_KDPQueue *kdqueue,        // Just here to avoid reallocating on every query
    UT_IntArray &temp,           // ''
    UT_Array<UT_Vector3> &temp2, // ''
    UT_Array<UT_Vector3> &primuvwsarray,
    UT_Array<exint> &treepointnumsarray,
    float tolerance = 1e-7f,
    bool wrapunitcube = false);

/// Finds, for each element in queryrange, the primitive number and primitive
/// uvw in searchindetail at which searchingattrib matches queryattrib.
///
/// If appropriate, it interpolates inside (convexed) polygons and tetrahedra
/// to find matching values.  Anything that has no match will not be
/// written-to, in case you want to do multiple queries, so make sure to
/// initialize foundprimnums to -1 (default value -1 suffices).
///
/// Returns false if the attributes aren't compatible with this querying.
/// searchinattrib must be a numeric, float, point or vertex attribute,
/// queryattrib must be a numeric, float or int attribute of any owner,
/// and the two must have the same tuple size.  searchinrange must be
/// a primitive range.
template<typename INT>
bool GEOfindAttrib(
    const GA_Attribute *queryattrib,
    const GA_Range &queryrange,
    const GEO_Detail &searchindetail,
    const GA_Range &searchinrange,
    const GA_Attribute *searchinattrib,
    const GA_RWHandleT<INT> &foundprimnums,
    const GA_RWHandleV3 &foundprimuvws,
    float tolerance = 1e-7f,
    bool wrapunitcube = false);

template<typename INT>
bool GEOfindAttrib(
    const GA_Attribute *queryattrib,
    const GA_Range &queryrange,
    const GEO_Detail &searchindetail,
    const GA_Range &searchinrange,
    const GA_Attribute *searchinattrib,
    const GA_RWHandleT<UT_ValArray<INT> > &foundprimnums,
    const GA_RWHandleT<UT_Fpreal32Array> &foundprimuvws,
    float tolerance = 1e-7f,
    bool wrapunitcube = false);

template<typename INT>
void GEOfindAttrib(
    const UT_Array<UT_Vector3> &queries,
    const GEO_Detail &searchindetail,
    const GA_Range &searchinrange,
    const GA_Attribute *searchinattrib,
    UT_Array<INT> &foundprimnums,
    UT_Array<UT_Vector3> &foundprimuvws,
    float tolerance = 1e-7f,
    bool wrapunitcube = false);

/// Finds, for each element in queryrange, the vertex or point numbers
/// and corresponding weights in searchindetail at which searchingattrib
/// matches queryattrib.
///
/// If appropriate, it interpolates inside (convexed) polygons and tetrahedra
/// to find matching values.  Anything that has no match will not be
/// written-to, in case you want to do multiple queries, so make sure to
/// initialize foundprimnums to -1 (default value -1 suffices).
///
/// With this version, for polygons with 5 or more
/// sides, only up to 4 indices and weights are produced, because it
/// considers the convexed polygons produced by GEOconvexPoly, producing
/// results more consistent with Mantra.
GEO_API void GEOfindAttrib(
    const GA_Attribute *queryattrib,
    const GA_Range &queryrange,
    const GEO_Detail &searchindetail,
    const GA_Range &searchinrange,
    const GA_Attribute *searchinattrib,
    GA_AttributeOwner indexattrib,
    GA_Attribute *foundindexattrib,
    GA_Attribute *foundweightsattrib,
    bool wrapunitcube = false);

GEO_API void GEOfindAttrib(
    const UT_Array<UT_Vector3> &queries,
    const GEO_Detail &searchindetail,
    const GA_Range &searchinrange,
    const GA_Attribute *searchinattrib,
    GA_AttributeOwner indexattrib,
    UT_Array<GEO_TriOrQuadT<GA_Offset> > &foundindexattrib,
    UT_Array<GEO_TriOrQuadT<float> > &foundweightsattrib,
    bool wrapunitcube = false);

#endif
