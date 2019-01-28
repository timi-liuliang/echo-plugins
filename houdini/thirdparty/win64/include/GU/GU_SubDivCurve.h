/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GU_SubDivCurve.h (GU Library, C++)
 *
 * COMMENTS:    Functions for evaluating subdivision curves, similar to
 *              subdivision surfaces.
 */

#pragma once

#ifndef __GU_SubDivCurve__
#define __GU_SubDivCurve__

#include "GU_API.h"
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <UT/UT_VectorTypes.h>
#include <stddef.h>

class GA_Detail;
class GA_PrimitiveGroup;
class GEO_Face;

/// Returns the number of open polygons in the given detail.
GA_Size GU_API GUcountOpenPolys(
    const GA_Detail &detail,
    const GA_PrimitiveGroup *group=NULL);

/// Creates new polygon curves in dest from any curves in src (srcgroup if
/// specified), that are subdivided levels times.
/// If dest==src and treatasindependent is false, it will reuse any points that
/// are used by primitives that are not being subdivided, to maintain
/// equivalent connectivity.  Curves with zero vertices or one vertex
/// will be left untouched.  Other points, vertices, and primitives will
/// usually be new.
/// Returns the number of polygon curves that were generated in dest.
/// NOTE: The number of vertices generated is exponential in levels, not linear!
GA_Size GU_API GUsubdivideCurves(
    GA_Detail &dest,
    const GA_Detail &src,
    int segmentsperedge,
    bool treatasindependent=false,
    const GA_PrimitiveGroup *srcgroup=NULL);

/// Evaluates this curve (or face boundary if closed) as the specified
/// cubic spline at u (in [0,1]).
///
/// If midbasis and startbasis are not specified, the bases for
/// subdivision curves will be used.
/// Column 0 of midbasis is the unit coefficients for p[-1],p[0],p[1],p[2].
/// Column 1 is the t coefficients; column 2 is the t^2 coefficients, and
/// column 3 is the t^3 coefficients.  firstbasis is only used if the
/// face is open and has rows in the order p[0],p[1],p[2],p[3] for the
/// first edge, and is used in reverse for the last edge.
///
/// attrib can have any owner, including primitive and detail,
/// in which case the value is constant.  Otherwise, if this face has no
/// vertices, the output will be zeroed.  If it has one vertex, the value
/// will be copied.  If it has two vertices, the values will be lerped.
/// If it has 3 vertices and firstbasis has a non-zero row 3,
/// the quadratic interpolating basis will be used (this may change).
///
/// This is instantiated for fpreal16, fpreal32, fpreal64, and the
/// corresponding UT_Vector2T, UT_Vector3T, UT_Vector4T, UT_Matrix2T,
/// UT_Matrix3T, and UT_Matrix4T types.
///
/// If T is a scalar type and component=-1, all components of the attribute
/// will be evaluated into vals, instead of just one, so make sure
/// it's large enough.  In the version that takes a T&val, only the
/// specified component is evaluated into val.
/// @{
template<typename T>
void GUevaluateCubicCurve(
    const GEO_Face &face,
    float u, T &val, const GA_ROHandleT<T> &attrib,
    const UT_Matrix4 *midbasis=NULL,
    const UT_Matrix4 *firstbasis=NULL,
    int component=0);
template<typename T>
void GUevaluateCubicCurve(
    const GEO_Face &face,
    float startu, float endu, exint nus,
    T *vals, const GA_ROHandleT<T> &attrib,
    const UT_Matrix4 *midbasis=NULL,
    const UT_Matrix4 *firstbasis=NULL,
    int component=-1);
template<typename T>
void GUevaluateCubicCurve(
    const GEO_Face &face,
    const float *us, exint nus,
    T *vals, const GA_ROHandleT<T> &attrib,
    const UT_Matrix4 *midbasis=NULL,
    const UT_Matrix4 *firstbasis=NULL,
    int component=-1);
/// @}

/// This moves the values of the given attribute such that
/// if this polygon were subdivided as a subdivision curve,
/// the limit curve would interpolate the starting values.
/// In a sense, this "undoes" the subdivision so that it
/// can be done later to produce the same result.
///
/// If T is a scalar type and component=-1, all components of the attribute
/// will be evaluated into vals, instead of just one, so make sure
/// it's large enough.
/// @{
template<typename T>
void GUinterpolateCurveAsSubD(
    const GEO_Face &face,
    const GA_RWHandleT<T> &attrib,
    int component=-1);
template<typename ARRAY,typename T>
void GUinterpolateCurveAsSubD(
    const ARRAY &offsets,
    GA_AttributeOwner offsetowner,
    const GA_RWHandleT<T> &attrib,
    int component=-1,
    bool closed=false);
/// @}

#endif
