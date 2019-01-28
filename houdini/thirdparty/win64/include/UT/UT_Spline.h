/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Spline.h ( UT Library, C++)
 *
 * COMMENTS:	Simple spline class.
 * 
 * The linear and catmull-rom splines expect a parametric evaluation coordinate
 * between 0 and 1.
 */

#ifndef __UT_Spline__
#define __UT_Spline__

#include "UT_API.h"
#include "UT_BoundingBox.h"
#include "UT_Color.h"
#include "UT_Matrix4.h"
#include "UT_RootFinder.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Inline.h>

typedef enum {
    // These splines all work on uniform keys.
    UT_SPLINE_CONSTANT,			// Constant between keys
    UT_SPLINE_LINEAR,			// Linear interpolation between keys
    UT_SPLINE_CATMULL_ROM,		// Catmull-Rom Cardinal Spline
    UT_SPLINE_MONOTONECUBIC,		// Monotone Cubic Hermite Spline

    // This interpolation works a little differently.  It takes a set of scalar
    // values, and "fits" the parametric coordinate into the keys.  That is, it
    // performs the binary search to find where the parametric coordinate maps,
    // then, it performs a linear interpolation between the two nearest key
    // values to figure out what the coordinate should be.
    UT_SPLINE_LINEAR_SOLVE,

    UT_SPLINE_BEZIER,			// Bezier Curve
    UT_SPLINE_BSPLINE,			// B-Spline
    UT_SPLINE_HERMITE,			// Hermite Spline
} UT_SPLINE_BASIS;

UT_API extern const char *	UTnameFromSplineBasis(UT_SPLINE_BASIS basis);
UT_API extern UT_SPLINE_BASIS	UTsplineBasisFromName(const char *name);


class UT_API UT_SplineCubic
{
public:
    /// Evaluates an open spline at a given location.
    /// The given CV list must have 4 elements in it!
    /// The cvs should be for the current segment, and t in [0, 1]
    template <typename T>
    static T		evalOpen(const T *cvs, float t)
    {
	UT_Matrix4	weightmatrix = getOpenWeights();
	float		t2 = t*t;
	float		t3 = t2*t;
	UT_Vector4	powers(1, t, t2, t3);

	UT_Vector4	weights = colVecMult(weightmatrix, powers);

	T		value;

	value = cvs[0] * weights[0];
	value += cvs[1] * weights[1];
	value += cvs[2] * weights[2];
	value += cvs[3] * weights[3];

	return value;
    }

    /// Evaluates a range of t values in uniform increasing manner.
    /// The cvs list should have 3 + nseg entries.
    template <typename T>
    static void evalRangeOpen(T *results, const T *cvs, float start_t, float step_t, int len_t, int nseg)
    {
	int		curseg;
	curseg = SYSfastFloor(start_t);
	curseg = SYSclamp(curseg, 0, nseg-1);
	float		t = start_t - curseg;

	for (int i = 0; i < len_t; i++)
	{
	    results[i] = evalOpen(&cvs[curseg], t);
	    t += step_t;
	    if (t > 1)
	    {
		while (curseg < nseg-1)
		{
		    curseg++;
		    t -= 1;
		    if (t <= 1)
			break;
		}
	    }
	}
    }

    /// Evaluates a closed spline at the given location.  The given
    /// cv list must have 4 elements.  Whether we are end interpolated or
    /// not depends on which segment this represents.  The cvs list
    /// should be the cvs for the current segment and t in [0, 1]
    template <typename T>
    static T		evalClosed(const T *cvs, float t, int seg, int nseg, bool deriv = false)
    {
	UT_Matrix4	weightmatrix = getClosedWeights(seg, nseg, deriv);
	float		t2 = t*t;
	float		t3 = t2*t;
	UT_Vector4	powers(1, t, t2, t3);

	UT_Vector4	weights = colVecMult(weightmatrix, powers);

	T		value;

	value = cvs[0] * weights[0];
	value += cvs[1] * weights[1];
	value += cvs[2] * weights[2];
	value += cvs[3] * weights[3];

	return value;
    }

    /// Evaluates a range of t values in uniform increasing manner.
    /// The cvs list should have 3 + nseg entries.
    template <typename T>
    static void evalRangeClosed(T *results, const T *cvs, float start_t, float step_t, int len_t, int nseg, bool deriv = false)
    {
	int		curseg;
	curseg = SYSfastFloor(start_t);
	curseg = SYSclamp(curseg, 0, nseg-1);
	float		t = start_t - curseg;

	for (int i = 0; i < len_t; i++)
	{
	    results[i] = evalClosed(&cvs[curseg], t, curseg, nseg, deriv);
	    t += step_t;
	    if (t > 1)
	    {
		while (curseg < nseg-1)
		{
		    curseg++;
		    t -= 1;
		    if (t <= 1)
			break;
		}
	    }
	}
    }

    template <typename T>
    static T evalSubDStart(const T *cvs, float t)
    {
        // First segment is (1 - t + (1/6)t^3)*P0 + (t - (1/3)*t^3)*P1 + ((1/6)t^3)*P2
        const float onesixth = 0.16666666666666667f;
        float onesixtht3 = onesixth*t*t*t;
        float w0 = 1 - t + onesixtht3;
        float w1 = t - 2*onesixtht3;
        float w2 = onesixtht3;

        T value = w0*cvs[0];
        value += w1*cvs[1];
        value += w2*cvs[2];
        return value;
    }

    template <typename T>
    static T evalSubDEnd(const T *cvs, float t)
    {
        // Reverse t relative to evalSubDStart
        t = 1.0f-t;

        const float onesixth = 0.16666666666666667f;
        float onesixtht3 = onesixth*t*t*t;
        float w0 = 1 - t + onesixtht3;
        float w1 = t - 2*onesixtht3;
        float w2 = onesixtht3;

        // Also reverse point order relative to evalSubDStart
        T value = w0*cvs[2];
        value += w1*cvs[1];
        value += w2*cvs[0];
        return value;
    }

    template <float (func)(const float *,float)>
    SYS_STATIC_FORCE_INLINE void enlargeBoundingBoxCommon(
        UT_BoundingBox &box, const UT_Vector3 *cvs,
        const UT_Vector3 &a, const UT_Vector3 &b, const UT_Vector3 &c,
        float rootmin, float rootmax)
    {
        // If the value of the quadratic has equal signs at zero
        // and one, AND the derivative has equal signs at zero and one,
        // it can't have crossed zero between zero and one, so we
        // can skip the root find in that case.  The other rejection
        // case of a negative b^2-4ac is already checked by
        // UT_RootFinder, because it doesn't depend on the range
        // limits.

        // a+b+c = value of quadratic at one
        // (a+b+c)*c > 0 iff signs of values are equal
        UT_Vector3 abc = a + b + c;
        abc *= c;
        // 2a+b = derivative of quadratic at one
        // (2a+b)*b > 0 iff signs of derivatives are equal
        UT_Vector3 b2a = a * 2.0F + b;
        b2a *= b;

        for (int DIM = 0; DIM < 3; DIM++)
        {
            // No chance of crossing zero in case descirbed above
            // NOTE: The abc == 0 case can be rejected, because we
            //       already did enlargeBounds on both p values.
            //       The abc > 0 && b2a == 0 case can be rejected,
            //       because the peak of the quadratic has the same
            //       sign as the rest, so never crosses zero.
            if (abc[DIM] >= 0 && b2a[DIM] >= 0)
                continue;

            float t1, t2;
            int nroots = UT_RootFinder::quadratic(a[DIM], b[DIM], c[DIM], t1, t2);
            if (nroots == 0)
                continue;

            float fcvs[4];
            fcvs[0] = cvs[0][DIM];
            fcvs[1] = cvs[1][DIM];
            fcvs[2] = cvs[2][DIM];
            fcvs[3] = cvs[3][DIM];

            // Add any minima/maxima to the bounding box
            if (t1 > rootmin && t1 < rootmax)
            {
                float v = func(fcvs, t1);
                box.vals[DIM][0] = SYSmin(box.vals[DIM][0], v);
                box.vals[DIM][1] = SYSmax(box.vals[DIM][1], v);
            }
            if (nroots == 2 && t2 > rootmin && t2 < rootmax)
            {
                float v = func(fcvs, t2);
                box.vals[DIM][0] = SYSmin(box.vals[DIM][0], v);
                box.vals[DIM][1] = SYSmax(box.vals[DIM][1], v);
            }
        }
    }

    /// Enlarges box by any minima/maxima of the cubic curve defined by 4 cvs, that lie between rootmin and rootmax.
    /// NOTE: This must be defined below so that it doesn't instantiate evalOpen before its specialization below.
    static inline void enlargeBoundingBoxOpen(UT_BoundingBox &box, const UT_Vector3 *cvs, float rootmin, float rootmax);

    /// Enlarges box by any minima/maxima of the cubic curve defined by 3 cvs, that lie between rootmin and rootmax.
    /// The curve in this case is the start segment of a subdivision curve.
    static inline void enlargeBoundingBoxSubDStart(UT_BoundingBox &box, const UT_Vector3 *cvs, float rootmin, float rootmax);

    /// Enlarges box by any minima/maxima of the cubic curve defined by cvs, that lie between rootmin and rootmax.
    /// The curve in this case is the end segment of a subdivision curve.
    static inline void enlargeBoundingBoxSubDEnd(UT_BoundingBox &box, const UT_Vector3 *cvs, float rootmin, float rootmax);

    /// Returns the weights for a power-basis evaluation of a segment.
    /// The t values should be normalized inside the segment.
    /// The format is (1, t, t^2, t^3), and colVecMult.
    /// Assumes uniform knots.
    static UT_Matrix4	getOpenWeights()
    {
	return UT_Matrix4( 1/6., -3/6.,  3/6., -1/6.,
			  4/6.,  0/6., -6/6.,  3/6.,
			  1/6.,  3/6.,  3/6., -3/6.,
			  0/6.,  0/6.,  0/6.,  1/6. );
    }
    static UT_Matrix4	getOpenWeightsTranspose()
    {
	return UT_Matrix4( 1/6., 4/6.,  1/6.,  0/6.,
			  -3/6., 0/6.,  3/6.,  0/6.,
			  3/6., -6/6.,  3/6.,  0/6.,
			  -1/6., 3/6., -3/6.,  1/6. );
    }

    template<typename T>
    static T evalMatrix(const UT_Matrix4 &basis, const T cvs[4], float t)
    {
        float t2 = t*t;
        UT_Vector4 tpow(1.0f, t, t2, t2*t);

        UT_Vector4 coeff = colVecMult(basis, tpow);

        T val = cvs[0]*coeff[0] + cvs[1]*coeff[1] + cvs[2]*coeff[2] + cvs[3]*coeff[3];

        return val;
    }

    /// This function is for evaluating a subdivision curve that is open.
    /// For simplicitly, the parameter range is [0,1].
    /// It's implemented in a way that maximizes stability
    /// and readability, not necessarily performance.
    template<typename T>
    static T evalSubDCurve(const T *cvs, float t, int npts, bool deriv=false)
    {
        T p0;
        T diff; // p1-p0
        T c0;   // Average of neighbours of p0, minus p0
        T c1;   // Average of neighbours of p1, minus p1

        // npts-1 segments, since npts points in whole curve
        t *= (npts-1);

        int i = int(t);

        if (i < 0)
            i = 0;
        else if (i > npts-1)
            i = npts-1;

        t -= i;
        p0 = cvs[i];
        diff = cvs[i+1]-cvs[i];

        if (i > 0)
            c0 = 0.5*(cvs[i-1]+cvs[i+1]) - cvs[i];
        else
            c0 = 0;

        if (i < npts-1)
            c1 = 0.5*(cvs[i]+cvs[i+2]) - cvs[i+1];
        else
            c1 = 0;

        float ti = 1-t;
        if (!deriv)
        {
            float t3 = t*t*t/3;
            float ti3 = ti*ti*ti/3;
            // Order of addition should reduce roundoff in common cases.
            return p0 + (diff*t + (c0*ti3 + c1*t3));
        }
        else
        {
            float t2 = t*t;
            float ti2 = ti*ti;
            // Order of addition should reduce roundoff in common cases.
            return diff + (c1*t2 - c0*ti2);
        }
    }

    /// Basis for first segment of subd curve.  Evaluation is:
    /// [p[0] p[1] p[2] p[3]] * theSubDFirstBasis * [1 t t^2 t^3]^T
    /// The last segment can be evaluated as: (NOTE the reversed order and 1-t)
    /// [p[n-1] p[n-2] p[n-3] p[n-4]] * theSubDFirstBasis * [1 (1-t) (1-t)^2 (1-t)^3]^T
    /// FYI: The last row is all zero, since it only depends on 3 points.
    static const UT_Matrix4 theSubDFirstBasis;

    /// Basis for derivative of first segment of subd curve.  Evaluation is:
    /// [p[0] p[1] p[2] p[3]] * theSubDFirstDerivBasis * [1 t t^2 t^3]^T
    /// The last segment derivative can be evaluated as: (NOTE the reversed order and 1-t)
    /// [p[n-1] p[n-2] p[n-3] p[n-4]] * theSubDFirstDerivBasis * [1 (1-t) (1-t)^2 t^3]^T
    /// FYI: The last row is all zero, since it only depends on 3 points.
    ///      The last column is all zero, since the derivative has no cubic component.
    static const UT_Matrix4 theSubDFirstDerivBasis;

    /// Basis for middle segment of subd curve or uniform, open, cubic NURBS.
    /// Evaluation is:
    /// [p[-1] p[0] p[1] p[2]] * theOpenBasis * [1 t t^2 t^3]^T
    static const UT_Matrix4 theOpenBasis;

    /// Basis for derivative of middle segment of subd curve or uniform, open, cubic NURBS.
    /// Evaluation is:
    /// [p[-1] p[0] p[1] p[2]] * theOpenDerivBasis * [1 t t^2 t^3]^T
    /// FYI: The last column is all zero, since the derivative has no cubic component.
    static const UT_Matrix4 theOpenDerivBasis;

    /// Basis for first segment of interpolating curve.  Evaluation is:
    /// [p[0] p[1] p[2] p[3]] * theInterpFirstBasis * [1 t t^2 t^3]^T
    /// The last segment can be evaluated as: (NOTE the reversed order and 1-t)
    /// [p[n-1] p[n-2] p[n-3] p[n-4]] * theInterpFirstBasis * [1 (1-t) (1-t)^2 (1-t)^3]^T
    /// FYI: The last row is all zero, since it only depends on 3 points.
    static const UT_Matrix4 theInterpFirstBasis;

    /// Basis for middle segment of interpolating curve.  Evaluation is:
    /// [p[-1] p[0] p[1] p[2]] * theInterpBasis * [1 t t^2 t^3]^T
    static const UT_Matrix4 theInterpBasis;

    /// Basis for Hermite cubic curve using two values (p) and two derivatives (v):
    /// Evaluation is:
    /// [p[0] p[1] v[0] v[1]] * theHermiteBasis * [1 t t^2 t^3]^T
    static const UT_Matrix4 theHermiteBasis;

    /// Basis for derivative of Hermite cubic curve using two values (p) and two derivatives (v):
    /// Evaluation is:
    /// [p[0] p[1] v[0] v[1]] * theHermiteDerivBasis * [1 t t^2 t^3]^T
    /// FYI: The last column is all zero, since the derivative has no cubic component.
    static const UT_Matrix4 theHermiteDerivBasis;

    /// Uniform knots with closed end conditions.  seg is which segment
    /// is being evaluates, nseg is the total.  nseg should be
    /// number of vertices minus three as we have cubics.
    static UT_Matrix4	getClosedWeights(int seg, int nseg, bool deriv = false)
    {
	// these matrices come from $GEO/support/computespline.py
	// which computes the power basis form of end-interpolated
	// uniform bsplines.

	if (deriv == false)
	{
	    if (nseg <= 1)
	    {
		// Bezier.
		return UT_Matrix4( 1, -3,  3, -1,
				   0,  3, -6,  3,
				   0,  0,  3, -3,
				   0,  0,  0,  1 );
	    }
	    else if (nseg == 2)
	    {
		// 0, 0, 0, 1, 2, 2, 2,
		if (seg == 0)
		    return UT_Matrix4( 1, -3,  3, -1,
				       0,  3, -4.5, 1.75,
				       0,  0, 1.5, -1,
				       0,  0,  0,  0.25 );
		else
		    return UT_Matrix4( .25, -.75,  .75, -0.25,
					0.5,  0, -1.5, 1,
					0.25,  0.75, 0.75, -1.75,
					0,  0,  0,  1 );
	    }
	    else if (nseg == 3)
	    {
		// 0, 0, 0, 1, 2, 3, 3, 3
		if (seg == 0)
		    return UT_Matrix4( 1, -3,  3, -1,
				       0,  3, -4.5, 1.75,
				       0,  0, 1.5, -11/12.,
				       0,  0,  0,  1/6.);
		else if (seg == 1)
		    return UT_Matrix4( .25, -.75,  .75, -0.25,
				       7/12.,  0.25, -1.25, 7/12.,
				       1/6.,  0.5, 0.5, -7/12.,
				       0,  0,  0,  0.25 );
		else
		    return UT_Matrix4( 1/6., -.5,  .5, -1/6.,
				       7/12.,  -0.25, -1.25, 11/12.,
				       0.25,  0.75, 0.75, -1.75,
				       0,  0,  0,  1 );
	    }
	    else
	    {
		// Either on an end, or in the middle
		if (seg >= 2 && seg < nseg-2)
		    return UT_Matrix4( 1/6., -3/6.,  3/6., -1/6.,
				      4/6.,  0/6., -6/6.,  3/6.,
				      1/6.,  3/6.,  3/6., -3/6.,
				      0/6.,  0/6.,  0/6.,  1/6. );
		else if (seg == 0)
		    return UT_Matrix4( 1, -3,  3, -1,
				       0,  3, -4.5,  1.75,
				       0,  0,  1.5, -11/12.,
				       0,  0,  0,  1/6. );
		else if (seg == 1)
		    return UT_Matrix4( 0.25, -0.75,  0.75, -.25,
				       7/12.,  0.25, -1.25,  7/12.,
				       1/6., 0.5, 0.5, -0.5,
				       0,  0,  0,  1/6. );
		else if (seg == nseg-2)
		    return UT_Matrix4( 1/6., -3/6.,  3/6., -1/6.,
				       2/3.,  0, -1,  0.5,
				       1/6.,  0.5,  0.5, -7/12.,
				       0,  0,  0,  0.25 );
		else // if (seg == nseg-1)
		    return UT_Matrix4( 1/6., -3/6.,  3/6., -1/6.,
				       7/12.,  -.25, -1.25,  11/12.,
				       0.25,  0.75,  0.75, -1.75,
				       0,  0,  0,  1 );
	    }
	}
	else
	{
	    if (nseg <= 1)
	    {
		// Bezier.
		return UT_Matrix4( -3,   6, -3, 0,
				    3, -12,  9, 0,
				    0,   6, -9, 0,
				    0,   0,  3, 0 );
	    }
	    else if (nseg == 2)
	    {
		// 0, 0, 0, 1, 2, 2, 2,
		if (seg == 0)
		    return UT_Matrix4( -3,  6, -3, 0,
				        3, -9, 5.25, 0,
				        0,  3, -3, 0,
				        0,  0,  0.75, 9 );
		else
		    return UT_Matrix4(  -.75,  1.5, -0.75, 0,
					   0,   -3, 3, 0,
					0.75,  1.5, -5.25, 0,
				  	   0,  0,  3, 0 );
	    }
	    else if (nseg == 3)
	    {
		// 0, 0, 0, 1, 2, 3, 3, 3
		if (seg == 0)
		    return UT_Matrix4( -3,  6,     -3, 0,
				        3, -9,   5.25, 0,
				        0,  3, -11/4., 0,
				        0,  0,     .5, 0);
		else if (seg == 1)
		    return UT_Matrix4( -.75,  1.5, -0.75, 0,
				       0.25, -2.5,  7/4., 0,
				        0.5,    1, -7/4., 0,
				          0,    0,  0.75, 0);
		else
		    return UT_Matrix4(   -.5,    1,  -0.5, 0,
				       -0.25, -2.5, 11/4., 0,
				        0.75,  1.5, -5.25, 0,
				           0,    0,     3, 0);
	    }
	    else
	    {
		// Either on an end, or in the middle
		if (seg >= 2 && seg < nseg-2)
		    return UT_Matrix4( -3/6.,  1.0, -0.5, 0,
				        0/6., -2.0,  1.5, 0,
				        3/6.,  1.0, -1.5, 0,
				        0/6.,    0,  0.5, 0);
		else if (seg == 0)
		    return UT_Matrix4( -3,  6,     -3, 0,
				        3, -9,   5.25, 0,
				        0,  3, -11/4., 0,
				        0,  0,    0.5, 0);
		else if (seg == 1)
		    return UT_Matrix4( -0.75,   1.5, -.75, 0,
				        0.25,  -2.5, 7/4., 0,
				         0.5,     1, -1.5, 0,
				           0,     0,  0.5, 0 );
		else if (seg == nseg-2)
		    return UT_Matrix4( -3/6.,  1,  -0.5, 0,
				           0, -2,   1.5, 0,
				         0.5,  1, -7/4., 0,
				           0,  0,  0.75, 0 );
		else // if (seg == nseg-1)
		    return UT_Matrix4(-3/6.,    1,   -.5, 0,
				       -.25, -2.5, 11/4., 0,
				       0.75,  1.5, -5.25, 0,
				          0,    0,     3, 0);
	    }
	}
    }
    static UT_Matrix4	getClosedWeightsTranspose(int seg, int nseg, bool deriv = false)
    {
	if (deriv == false)
	{
	    // these matrices come from $GEO/support/computespline.py
	    // which computes the power basis form of end-interpolated
	    // uniform bsplines.
	    if (nseg <= 1)
	    {
		// Bezier.
		return UT_Matrix4( 1,  0,  0, 0,
				  -3,  3,  0,  0,
				   3,  -6, 3, 0,
				  -1,  3,  -3,  1 );
	    }
	    else if (nseg == 2)
	    {
		// 0, 0, 0, 1, 2, 2, 2,
		if (seg == 0)
		    return UT_Matrix4( 1,  0,    0, 0,
				      -3,  3,    0, 0,
				       3, -4.5, 1.5, 0,
				      -1,  1.75,  -1,  0.25 );
		else
		    return UT_Matrix4( .25,     .5,  .25,   0,
					-.75,   0,  .75,    0,
					0.75, -1.5,  0.75,  0,
					-0.25,  1,   -1.75, 1 );
	    }
	    else if (nseg == 3)
	    {
		// 0, 0, 0, 1, 2, 3, 3, 3
		if (seg == 0)
		    return UT_Matrix4( 1,  0,  0,  0,
				      -3,  3,  0,  0,
				       3,-4.5,1.5, 0,
				       -1,1.75,-11/12.,1/6. );
		else if (seg == 1)
		    return UT_Matrix4( 0.25, 7/12., 1/6., 0,
				       -.75, 0.25, 0.5,   0,
				       .75,-1.25, 0.5,  0,
				       -.25,7/12.,-7/12.,0.25 );
		else
		    return UT_Matrix4( 1/6., 7/12., 0.25, 0,
				      -.5,  -0.25,  0.75, 0,
				       .5,  -1.25,  0.75, 0,
				      -1/6.,11/12.,-1.75, 1 );

	    }
	    else
	    {
		// Either on an end, or in the middle
		if (seg >= 2 && seg < nseg-2)
		    return UT_Matrix4( 1/6., 4/6.,  1/6.,  0/6.,
				      -3/6., 0/6.,  3/6.,  0/6.,
				      3/6., -6/6.,  3/6.,  0/6.,
				      -1/6., 3/6., -3/6.,  1/6. );
		else if (seg == 0)
		    return UT_Matrix4( 1,  0,  0,  0,
				      -3,  3,  0,  0,
				       3,-4.5,1.5, 0,
				      -1,1.75,-11/12., 1/6. );
		else if (seg == 1)
		    return UT_Matrix4( 0.25, 7/12., 1/6., 0,
				      -0.75, 0.25,  0.5,  0,
				       0.75,-1.25,  0.5,  0,
				      -0.25,7/12., -0.5, 1/6. );
		else if (seg == nseg-2)
		    return UT_Matrix4( 1/6., 2/3., 1/6., 0,
				      -3/6.,   0,  0.5,  0,
				       3/6.,  -1,  0.5,  0,
				      -1/6., 0.5,-7/12.,0.25 );
		else // if (seg == nseg-1)
		    return UT_Matrix4( 1/6., 7/12., 0.25, 0,
				      -3/6., -.25,  0.75, 0,
				       3/6.,-1.25,  0.75, 0,
				      -1/6.,11/12.,-1.75, 1 );
	    }
	}
	else
	{
	    if (nseg <= 1)
	    {
		// Bezier.
		return UT_Matrix4(-3,  3,  0,  0,
				   6,  -12, 6, 0,
				  -3,  9,  -9,  3,
				   0,  0,   0,  0);
	    }
	    else if (nseg == 2)
	    {
		// 0, 0, 0, 1, 2, 2, 2,
		if (seg == 0)
		    return UT_Matrix4(-3,  3,  0, 0,
				       6, -9,  3, 0,
				      -3,  5.25,  -3,  0.75,
				      0, 0, 0, 0);
		else
		    return UT_Matrix4(-.75,   0,  .75,    0,
				      1.5, -3,  1.5,  0,
				      -0.75,  3,   -5.25, 3,
				      0, 0, 0, 0);
	    }
	    else if (nseg == 3)
	    {
		// 0, 0, 0, 1, 2, 3, 3, 3
		if (seg == 0)
		    return UT_Matrix4(-3,  3,  0,  0,
				       6, -9,  3, 0,
				       -3, 5.25, -11/4., .5,
				       0, 0, 0, 0);
		else if (seg == 1)
		    return UT_Matrix4(-.75, 0.25, 0.5,   0,
				       1.5,-2.5, 1,  0,
				      -.75,7/4.,-7/4.,0.75,
				      0, 0, 0, 0);
		else
		    return UT_Matrix4(-.5, -0.25,  0.75, 0,
				       1,  -2.5,  1.5, 0,
				      -.5, 11/4., -5.25, 3,
				      0, 0, 0, 0);

	    }
	    else
	    {
		// Either on an end, or in the middle
		if (seg >= 2 && seg < nseg-2)
		    return UT_Matrix4(-3/6., 0/6.,  3/6.,  0/6.,
				      1, -2,  1,  0,
				      -0.5, 1.5, -1.5,  0.5,
				      0, 0, 0, 0);
		else if (seg == 0)
		    return UT_Matrix4(-3,  3,  0,  0,
				       6, -9,  3, 0,
				      -3, 5.25, -11/4., .5,
				      0, 0, 0, 0);
		else if (seg == 1)
		    return UT_Matrix4(-0.75, 0.25,  0.5,  0,
				       1.5, -2.5,  1,  0,
				      -0.75, 7/4., -1.5, .5,
				      0, 0, 0, 0);

		else if (seg == nseg-2)
		    return UT_Matrix4(-3/6.,   0,  0.5,  0,
				       1,  -2,  1,  0,
				      -0.5, 1.5, -7/4., 0.75,
				      0, 0, 0, 0);
		else // if (seg == nseg-1)
		    return UT_Matrix4(-3/6., -.25,  0.75, 0,
				       1, -2.5,  1.5, 0,
				      -.5, 11/4.,-5.25, 3,
				      0, 0, 0, 0);
	    }
	}
    }
};


/// The Linear & Catmull-Rom splines expect a parametric coordinate for
/// evaluation between 0 and 1.  The Catmull-Rom spline requires additional
/// key values at the beginning and end of the spline to evaluate the slopes
/// of the Hermite spline properly.
///
/// The LinearSolve only works on scalar values.  It will compute the
/// parametric coordinate associated with the value passed in.  This can be
/// used to simulate non-uniform keys on the spline.
class UT_API UT_Spline {
public:
     UT_Spline();
    ~UT_Spline();

    /// Return the amount of memory owned by this UT_Spline in bytes
    int64 getMemoryUsage(bool inclusive) const;

    /// Query the basis or knot length of the spline
    UT_SPLINE_BASIS	getGlobalBasis() const	{ return myGlobalBasis; }
    int			getVectorSize() const	{ return myVectorSize; }
    int			getKnotLength() const	{ return myKnotLength; }
    fpreal64		getTension() const	{ return myTension; }

    void		setGlobalBasis(UT_SPLINE_BASIS b)
			    { myGlobalBasis = b; }

    /// Construction of the spline object.  All values are initialized to 0.
    /// Warning, calling setSize() will clear all existing values.
    void		setSize(int nkeys, int vector_size);
    /// Cubic splines may have a "tension".  The tension defaults to 0.5 which
    /// results in Catmull-Rom splines.
    void		setTension(fpreal64 t);

    /// Once the spline has been constructed, the values need to be set.
    /// It is possible to change values between evaluations.
    // @{
    void		setValue(int key, const fpreal32 *value, int size);
    void		setValue(int key, const fpreal64 *value, int size);
    // @}

    /// Set the basis for the given key index.
    /// This will also set the global basis.
    void		setBasis(int key, UT_SPLINE_BASIS b);

    /// Evaluate the spline using the global basis.
    /// When interp_space is not UT_RGB, then values are treated as UT_RGBA
    /// and converted into the desired color space before being interpolated.
    /// The result is always returned as UT_RGBA.
    // @{
    bool		evaluate(fpreal t, fpreal32 *result, int size,
				 UT_ColorType interp_space) const;
    bool		evaluate(fpreal t, fpreal64 *result, int size,
				 UT_ColorType interp_space) const;
    // @}

    /// Evaluate the spline using multiple basis types depending on t.
    /// Also unlike evaluate(), evaluateMulti() doesn't require extra keys for
    /// Catmull-Rom on the ends. It always evaluates using a 0 slope at the
    /// ends.
    // @{
    bool		evaluateMulti(fpreal t, fpreal32 *result, int n,
				      UT_ColorType interp_space,
				      int knot_segment_hint = -1) const;
    bool		evaluateMulti(fpreal t, fpreal64 *result, int n,
				      UT_ColorType interp_space,
				      int knot_segment_hint = -1) const;
    // @}

    /// Return _monotone_ cubic Hermite slopes at the current knot given the
    /// previous and next knots.
    // @{
    /// Fritsch-Carlson (local) method.
    /// It gives relatively good looking C1 curves but might not give a C2
    /// solution even if it exists.
    /// 1. Fritsch, F.N., Carlson, R.E., Monotone piecewise cubic interpolant,
    ///    SIAM J. Numer. Anal., 17(1980), 238-246.
    static fpreal64	getMonotoneSlopeFC(fpreal64 v_cur,  fpreal64 t_cur,
					   fpreal64 v_prev, fpreal64 t_prev,
					   fpreal64 v_next, fpreal64 t_next);
    /// Paul Kupan's method.
    /// Similar to Fritsch-Carlson method except it gives more visually
    /// pleasing results when the intervals next to the knot are uneven. 
    /// 2. Kupan, P.A., Monotone Interpolant Built with Slopes Obtained by
    ///    Linear Combination, Studia Universitatis Babes-Bolyai Mathematica,
    ///    53(2008), 59-66.
    static fpreal64	getMonotoneSlopePA(fpreal64 v_cur,  fpreal64 t_cur,
					   fpreal64 v_prev, fpreal64 t_prev,
					   fpreal64 v_next, fpreal64 t_next);
    // @}
 
    /// Given the position within the two knots and the first knot index
    /// number, normalize the position from knot-length domain to unit domain.
    fpreal		normalizeParameter(fpreal parm, fpreal t0, fpreal t1,
					   int t0_index) const;

    // Given parm in [0,1] interval in which CVs are at 'knots' values,
    // find the parameter t in [0,1] interval in which CVs are evenly spaced.
    // The returned reparameterized value is calculated in such a way that it
    // yields smooth curve at CVs (unlike normalizeation that uses linear
    // interpolation that yields tangent disconinuity at CVs).
    // If 'parm_knot_segment' is given, this function sets it to the index
    // of the knot segment into which the parm falls; it can be used as a hint
    // to the evaluateMulti() method, especially for b-splines that may remap 
    // parameters to an adjacent segment (for continuity at end control points).
    fpreal		solveSpline(fpreal parm, 
				    const UT_FprealArray &knots,
				    int *parm_knot_segment = NULL) const;

    /// Given the keys surrounding a channel segment, evaluate it as cubic
    /// hermite spline. This function assumes dt > 0.
    /// kt is [0,1] which maps over the dt time interval.
    template <typename T>
    static T evalCubic(T kt, T dt, T iv, T im, T ov, T om)
    {
	T x0 = iv;
	T x1 = im*dt;
	T x3 = 2*(iv - ov) + (im + om)*dt;
	T x2 = ov - iv - im*dt - x3;
	return x0 + kt*(x1 + kt*(x2 + kt*x3));
    }

private:
    UT_Spline(const UT_Spline &copy);		    // not implemented yet
    UT_Spline &operator =(const UT_Spline &copy);   // not implemented yet

private:
    void		 grow(int size);
    int			 getInterp(fpreal t, int knots[], fpreal weights[]);

    int64		 getSizeOfValues() const
			 { return myKnotLength*myVectorSize*sizeof(fpreal64); }
    int64		 getSizeOfBases() const
			 { return myKnotLength*sizeof(UT_SPLINE_BASIS); }

    template <typename T>
    inline void		 combineKeys(T *result, int vector_size,
				     fpreal64 weights[], int indices[],
				     int num_indices,
				     UT_ColorType interp_space) const;

    template <typename T>
    inline void		 evalMonotoneCubic(fpreal t, T *result,
					   int n, UT_ColorType interp_space,
					   bool do_multi) const;

    template <typename T>
    inline void		setValueInternal(int key, const T *value, int size);

    template <typename T>
    inline bool		evaluateInternal(fpreal t, T *result, int size,
				 UT_ColorType interp_space) const;

    template <typename T>
    inline bool		evaluateMultiInternal(fpreal t, T *result, int n,
				      UT_ColorType interp_space,
				      int knot_segment_hint) const;

    fpreal64		*myValues;
    UT_SPLINE_BASIS	*myBases;
    fpreal64		 myTension;
    int			 myVectorSize;
    int			 myKnotLength;
    UT_SPLINE_BASIS	 myGlobalBasis;
};

#include <VM/VM_SIMD.h>

template <>
SYS_FORCE_INLINE UT_Vector3	
UT_SplineCubic::evalOpen(const UT_Vector3 *cvs, float t)
{
#if defined(CPU_HAS_SIMD_INSTR)
    v4uf row1(1/6., 4/6.,  1/6.,  0/6.);
    v4uf row2(-3/6., 0/6.,  3/6.,  0/6.);
    v4uf row3(3/6., -6/6.,  3/6.,  0/6.);
    v4uf row4(-1/6., 3/6., -3/6.,  1/6. );

    v4uf	vcvsx(cvs[0].x(), cvs[1].x(), cvs[2].x(), cvs[3].x());
    v4uf	vcvsy(cvs[0].y(), cvs[1].y(), cvs[2].y(), cvs[3].y());
    v4uf	vcvsz(cvs[0].z(), cvs[1].z(), cvs[2].z(), cvs[3].z());
    v4uf	vt(t);
    v4uf	vt2 = vt*vt;
    v4uf	vt3 = vt2*vt;

    v4uf	weights;

    weights = row1;
    weights += row2 * vt;
    weights += row3 * vt2;
    weights += row4 * vt3;

    vcvsx *= weights;
    vcvsx += vcvsx.swizzle<1, 1, 3, 3>();
    vcvsx += vcvsx.swizzle<2, 2, 2, 2>();
    vcvsy *= weights;
    vcvsy += vcvsy.swizzle<1, 1, 3, 3>();
    vcvsy += vcvsy.swizzle<2, 2, 2, 2>();
    vcvsz *= weights;
    vcvsz += vcvsz.swizzle<1, 1, 3, 3>();
    vcvsz += vcvsz.swizzle<2, 2, 2, 2>();

    return UT_Vector3( vcvsx[0], vcvsy[0], vcvsz[0] );
#else
    UT_Matrix4	weightmatrix = getOpenWeights();
    float		t2 = t*t;
    float		t3 = t2*t;
    UT_Vector4	powers(1, t, t2, t3);

    UT_Vector4	weights = colVecMult(weightmatrix, powers);

    UT_Vector3		value;

    value = cvs[0] * weights[0];
    value += cvs[1] * weights[1];
    value += cvs[2] * weights[2];
    value += cvs[3] * weights[3];

    return value;
#endif
}

template <>
SYS_FORCE_INLINE float	
UT_SplineCubic::evalOpen(const float *cvs, float t)
{
#if defined(CPU_HAS_SIMD_INSTR)
    v4uf row1(1/6., 4/6.,  1/6.,  0/6.);
    v4uf row2(-3/6., 0/6.,  3/6.,  0/6.);
    v4uf row3(3/6., -6/6.,  3/6.,  0/6.);
    v4uf row4(-1/6., 3/6., -3/6.,  1/6. );

    v4uf	vcvs(cvs);
    v4uf	vt(t);
    v4uf	vt2 = vt*vt;
    v4uf	vt3 = vt2*vt;

    v4uf	weights;

    weights = row1;
    weights += row2 * vt;
    weights += row3 * vt2;
    weights += row4 * vt3;

    vcvs *= weights;
    vcvs += vcvs.swizzle<1, 1, 3, 3>();
    vcvs += vcvs.swizzle<2, 2, 2, 2>();

    return vcvs[0];
#else
    UT_Matrix4	weightmatrix = getOpenWeights();
    float		t2 = t*t;
    float		t3 = t2*t;
    UT_Vector4	powers(1, t, t2, t3);

    UT_Vector4	weights = colVecMult(weightmatrix, powers);

    float		value;

    value = cvs[0] * weights[0];
    value += cvs[1] * weights[1];
    value += cvs[2] * weights[2];
    value += cvs[3] * weights[3];

    return value;
#endif
}

template <>
inline void 
UT_SplineCubic::evalRangeOpen(UT_Vector3 *results, const UT_Vector3 *cvs, float start_t, float step_t, int len_t, int nseg)
{
    int		curseg;
    curseg = SYSfastFloor(start_t);
    curseg = SYSclamp(curseg, 0, nseg-1);
    float		t = start_t - curseg;

#if defined(CPU_HAS_SIMD_INSTR)
    v4uf row1(1/6., 4/6.,  1/6.,  0/6.);
    v4uf row2(-3/6., 0/6.,  3/6.,  0/6.);
    v4uf row3(3/6., -6/6.,  3/6.,  0/6.);
    v4uf row4(-1/6., 3/6., -3/6.,  1/6. );

    v4uf	vcvsx(cvs[curseg].x(), cvs[curseg+1].x(), cvs[curseg+2].x(), cvs[curseg+3].x());
    v4uf	vcvsy(cvs[curseg].y(), cvs[curseg+1].y(), cvs[curseg+2].y(), cvs[curseg+3].y());
    v4uf	vcvsz(cvs[curseg].z(), cvs[curseg+1].z(), cvs[curseg+2].z(), cvs[curseg+3].z());

    for (int i = 0; i < len_t; i++)
    {
	{
	    v4uf	weights;
	    float t2 = t*t;
	    float t3 = t2*t;

	    weights = row1;
	    weights += row2 * t;
	    weights += row3 * t2;
	    weights += row4 * t3;

	    v4uf vx = vcvsx * weights;
	    vx += vx.swizzle<1, 1, 3, 3>();
	    vx += vx.swizzle<2, 2, 2, 2>();
	    v4uf vy = vcvsy * weights;
	    vy += vy.swizzle<1, 1, 3, 3>();
	    vy += vy.swizzle<2, 2, 2, 2>();
	    v4uf vz = vcvsz * weights;
	    vz += vz.swizzle<1, 1, 3, 3>();
	    vz += vz.swizzle<2, 2, 2, 2>();
	    results[i] = UT_Vector3( vx[0], vy[0], vz[0] );
	}

	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	    if (i < len_t-1)
	    {
		vcvsx = v4uf(cvs[curseg].x(), cvs[curseg+1].x(), cvs[curseg+2].x(), cvs[curseg+3].x());
		vcvsy = v4uf(cvs[curseg].y(), cvs[curseg+1].y(), cvs[curseg+2].y(), cvs[curseg+3].y());
		vcvsz = v4uf(cvs[curseg].z(), cvs[curseg+1].z(), cvs[curseg+2].z(), cvs[curseg+3].z());
	    }
	}
    }
#else
    for (int i = 0; i < len_t; i++)
    {
	results[i] = evalOpen(&cvs[curseg], t);
	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	}
    }
#endif
}

template <>
inline void 
UT_SplineCubic::evalRangeOpen(float *results, const float *cvs, float start_t, float step_t, int len_t, int nseg)
{
    int		curseg;
    curseg = SYSfastFloor(start_t);
    curseg = SYSclamp(curseg, 0, nseg-1);
    float		t = start_t - curseg;

#if defined(CPU_HAS_SIMD_INSTR)
    v4uf row1(1/6., 4/6.,  1/6.,  0/6.);
    v4uf row2(-3/6., 0/6.,  3/6.,  0/6.);
    v4uf row3(3/6., -6/6.,  3/6.,  0/6.);
    v4uf row4(-1/6., 3/6., -3/6.,  1/6. );

    v4uf	vcvs(&cvs[curseg]);

    for (int i = 0; i < len_t; i++)
    {
	{
	    v4uf	weights;
	    float t2 = t*t;
	    float t3 = t2*t;

	    weights = row1;
	    weights += row2 * t;
	    weights += row3 * t2;
	    weights += row4 * t3;

	    v4uf v = vcvs * weights;
	    v += v.swizzle<1, 1, 3, 3>();
	    v += v.swizzle<2, 2, 2, 2>();
	    results[i] = v[0];
	}

	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	    if (i < len_t-1)
	    {
		vcvs = v4uf(&cvs[curseg]);
	    }
	}
    }
#else
    for (int i = 0 ; i < len_t; i++)
    {
	results[i] = evalOpen(&cvs[curseg], t);
	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	}
    }
#endif
}

template <>
SYS_FORCE_INLINE UT_Vector3	
UT_SplineCubic::evalClosed(const UT_Vector3 *cvs, float t, int seg, int nseg, bool deriv)
{
#if defined(CPU_HAS_SIMD_INSTR)
    UT_Matrix4	weightmatrix = getClosedWeightsTranspose(seg, nseg, deriv);

    v4uf	row1(weightmatrix.data());
    v4uf	row2(weightmatrix.data()+4);
    v4uf	row3(weightmatrix.data()+8);
    v4uf	row4(weightmatrix.data()+12);

    v4uf	vcvsx(cvs[0].x(), cvs[1].x(), cvs[2].x(), cvs[3].x());
    v4uf	vcvsy(cvs[0].y(), cvs[1].y(), cvs[2].y(), cvs[3].y());
    v4uf	vcvsz(cvs[0].z(), cvs[1].z(), cvs[2].z(), cvs[3].z());
    v4uf	vt(t);
    v4uf	vt2 = vt*vt;
    v4uf	vt3 = vt2*vt;

    v4uf	weights;

    weights = row1;
    weights += row2 * vt;
    weights += row3 * vt2;
    weights += row4 * vt3;

    vcvsx *= weights;
    vcvsx += vcvsx.swizzle<1, 1, 3, 3>();
    vcvsx += vcvsx.swizzle<2, 2, 2, 2>();
    vcvsy *= weights;
    vcvsy += vcvsy.swizzle<1, 1, 3, 3>();
    vcvsy += vcvsy.swizzle<2, 2, 2, 2>();
    vcvsz *= weights;
    vcvsz += vcvsz.swizzle<1, 1, 3, 3>();
    vcvsz += vcvsz.swizzle<2, 2, 2, 2>();

    return UT_Vector3( vcvsx[0], vcvsy[0], vcvsz[0] );
#else
    UT_Matrix4	weightmatrix = getClosedWeights(seg, nseg);
    float		t2 = t*t;
    float		t3 = t2*t;
    UT_Vector4	powers(1, t, t2, t3);

    UT_Vector4	weights = colVecMult(weightmatrix, powers);

    UT_Vector3		value;

    value = cvs[0] * weights[0];
    value += cvs[1] * weights[1];
    value += cvs[2] * weights[2];
    value += cvs[3] * weights[3];

    return value;
#endif
}

template <>
SYS_FORCE_INLINE float	
UT_SplineCubic::evalClosed(const float *cvs, float t, int seg, int nseg, bool deriv)
{
#if defined(CPU_HAS_SIMD_INSTR)
    UT_Matrix4	weightmatrix = getClosedWeightsTranspose(seg, nseg, deriv);

    v4uf	row1(weightmatrix.data());
    v4uf	row2(weightmatrix.data()+4);
    v4uf	row3(weightmatrix.data()+8);
    v4uf	row4(weightmatrix.data()+12);

    v4uf	vcvs(cvs);
    float	t2 = t*t;
    float	t3 = t2*t;

    v4uf	weights;

    weights = row1;
    weights += row2 * t;
    weights += row3 * t2;
    weights += row4 * t3;

    vcvs *= weights;

    vcvs += vcvs.swizzle<1, 1, 3, 3>();
    vcvs += vcvs.swizzle<2, 2, 2, 2>();

    return vcvs[0];
#else
    UT_Matrix4	weightmatrix = getClosedWeights(seg, nseg);
    float		t2 = t*t;
    float		t3 = t2*t;
    UT_Vector4	powers(1, t, t2, t3);

    UT_Vector4	weights = colVecMult(weightmatrix, powers);

    float		value;

    value = cvs[0] * weights[0];
    value += cvs[1] * weights[1];
    value += cvs[2] * weights[2];
    value += cvs[3] * weights[3];

    return value;
#endif
}

template <>
inline void 
UT_SplineCubic::evalRangeClosed(UT_Vector3 *results, const UT_Vector3 *cvs, float start_t, float step_t, int len_t, int nseg, bool deriv)
{
    int		curseg;
    curseg = SYSfastFloor(start_t);
    curseg = SYSclamp(curseg, 0, nseg-1);
    float		t = start_t - curseg;

#if defined(CPU_HAS_SIMD_INSTR)
    UT_Matrix4	weightmatrix = getClosedWeightsTranspose(curseg, nseg, deriv);

    v4uf	row1(weightmatrix.data());
    v4uf	row2(weightmatrix.data()+4);
    v4uf	row3(weightmatrix.data()+8);
    v4uf	row4(weightmatrix.data()+12);

    v4uf	vcvsx(cvs[curseg].x(), cvs[curseg+1].x(), cvs[curseg+2].x(), cvs[curseg+3].x());
    v4uf	vcvsy(cvs[curseg].y(), cvs[curseg+1].y(), cvs[curseg+2].y(), cvs[curseg+3].y());
    v4uf	vcvsz(cvs[curseg].z(), cvs[curseg+1].z(), cvs[curseg+2].z(), cvs[curseg+3].z());

    for (int i = 0; i < len_t; i++)
    {
	{
	    v4uf	weights;
	    float t2 = t*t;
	    float t3 = t2*t;

	    weights = row1;
	    weights += row2 * t;
	    weights += row3 * t2;
	    weights += row4 * t3;

	    v4uf vx = vcvsx * weights;
	    vx += vx.swizzle<1, 1, 3, 3>();
	    vx += vx.swizzle<2, 2, 2, 2>();
	    v4uf vy = vcvsy * weights;
	    vy += vy.swizzle<1, 1, 3, 3>();
	    vy += vy.swizzle<2, 2, 2, 2>();
	    v4uf vz = vcvsz * weights;
	    vz += vz.swizzle<1, 1, 3, 3>();
	    vz += vz.swizzle<2, 2, 2, 2>();
	    results[i] = UT_Vector3( vx[0], vy[0], vz[0] );
	}

	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	    if (i < len_t-1)
	    {
		weightmatrix = getClosedWeightsTranspose(curseg, nseg, deriv);

		row1 = v4uf(weightmatrix.data());
		row2 = v4uf(weightmatrix.data()+4);
		row3 = v4uf(weightmatrix.data()+8);
		row4 = v4uf(weightmatrix.data()+12);

		vcvsx = v4uf(cvs[curseg].x(), cvs[curseg+1].x(), cvs[curseg+2].x(), cvs[curseg+3].x());
		vcvsy = v4uf(cvs[curseg].y(), cvs[curseg+1].y(), cvs[curseg+2].y(), cvs[curseg+3].y());
		vcvsz = v4uf(cvs[curseg].z(), cvs[curseg+1].z(), cvs[curseg+2].z(), cvs[curseg+3].z());
	    }
	}
    }
#else
    for (int i = 0; i < len_t; i++)
    {
	results[i] = evalClosed(&cvs[curseg], t, curseg, nseg, deriv);
	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	}
    }
#endif
}

template <>
inline void 
UT_SplineCubic::evalRangeClosed(float *results, const float *cvs, float start_t, float step_t, int len_t, int nseg, bool deriv)
{
    int		curseg;
    curseg = SYSfastFloor(start_t);
    curseg = SYSclamp(curseg, 0, nseg-1);
    float		t = start_t - curseg;

#if defined(CPU_HAS_SIMD_INSTR)
    UT_Matrix4	weightmatrix = getClosedWeightsTranspose(curseg, nseg, deriv);

    v4uf	row1(weightmatrix.data());
    v4uf	row2(weightmatrix.data()+4);
    v4uf	row3(weightmatrix.data()+8);
    v4uf	row4(weightmatrix.data()+12);

    v4uf	vcvs(&cvs[curseg]);

    for (int i = 0; i < len_t; i++)
    {
	{
	    v4uf	weights;
	    float t2 = t*t;
	    float t3 = t2*t;

	    weights = row1;
	    weights += row2 * t;
	    weights += row3 * t2;
	    weights += row4 * t3;

	    v4uf v = vcvs * weights;
	    v += v.swizzle<1, 1, 3, 3>();
	    v += v.swizzle<2, 2, 2, 2>();
	    results[i] = v[0];
	}

	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	    if (i < len_t-1)
	    {
		weightmatrix = getClosedWeightsTranspose(curseg, nseg, deriv);

		row1 = v4uf(weightmatrix.data());
		row2 = v4uf(weightmatrix.data()+4);
		row3 = v4uf(weightmatrix.data()+8);
		row4 = v4uf(weightmatrix.data()+12);

		vcvs = v4uf(&cvs[curseg]);
	    }
	}
    }
#else
    for (int i = 0 ; i < len_t; i++)
    {
	results[i] = evalClosed(&cvs[curseg], t, curseg, nseg, deriv);
	t += step_t;
	if (t > 1)
	{
	    while (curseg < nseg-1)
	    {
		curseg++;
		t -= 1;
		if (t <= 1)
		    break;
	    }
	}
    }
#endif
}

void
UT_SplineCubic::enlargeBoundingBoxOpen(UT_BoundingBox &box, const UT_Vector3 *cvs, float rootmin, float rootmax)
{
    // We need to find any minimum or maximum in each dimension
    // to enlarge the bounding box.
    // To do this, for each, dimension, we take the derivative
    // of the cubic, leaving a quadratic, and find the zeros of it.
    // The quadratic is such that its ith derivatives at zero are
    // the (i+1)th derivatives of the curve segment at zero.
    // a = (1/2) * 3rd derivative of curve segment at zero
    UT_Vector3 a = -cvs[0] + cvs[1] * 3.0F + cvs[2] * (-3.0F) + cvs[3];
    a *= 0.5F;

    // b = 2nd derivative of curve segment at zero
    //     (this is equivalent to the 2nd difference)
    UT_Vector3 b = cvs[0] + cvs[1] * (-2.0F) + cvs[2];
    // c = 1st derivative of curve segment at zero
    //     (this is equivalent to the central difference)
    UT_Vector3 c = cvs[2] - cvs[0];
    c *= 0.5F;

    enlargeBoundingBoxCommon<UT_SplineCubic::evalOpen<float> >(box, cvs, a, b, c, rootmin, rootmax);
}

void
UT_SplineCubic::enlargeBoundingBoxSubDStart(UT_BoundingBox &box, const UT_Vector3 *cvs, float rootmin, float rootmax)
{
    // We need to find any minimum or maximum in each dimension
    // to enlarge the bounding box.
    // To do this, for each, dimension, we take the derivative
    // of the cubic, leaving a quadratic, and find the zeros of it.
    // The quadratic is such that its ith derivatives at zero are
    // the (i+1)th derivatives of the curve segment at zero.

    // First segment is (1 - t + (1/6)t^3)*P0 + (t - (1/3)*t^3)*P1 + ((1/6)t^3)*P2
    // 1st derivative is (-1 + (1/2)t^2)*P0 + (1 - t^2)*P1 + ((1/2)t^2)*P2
    // 2nd derivative is (t)*P0 + (-2t)*P1 + (t)*P2
    // 3rd derivative is (1)*P0 + (-2)*P1 + (1)*P2

    // a = (1/2) * 3rd derivative of curve segment at zero
    UT_Vector3 a = cvs[0] - 2.0f*cvs[1] + cvs[2];
    a *= 0.5F;

    // b = 2nd derivative of curve segment at zero
    //     (this is equivalent to the 2nd difference)
    UT_Vector3 b(0,0,0);
    // c = 1st derivative of curve segment at zero
    //     (this is equivalent to the central difference)
    UT_Vector3 c = cvs[1] - cvs[0];

    enlargeBoundingBoxCommon<UT_SplineCubic::evalSubDStart<float> >(box, cvs, a, b, c, rootmin, rootmax);
}

void
UT_SplineCubic::enlargeBoundingBoxSubDEnd(UT_BoundingBox &box, const UT_Vector3 *cvs, float rootmin, float rootmax)
{
    // We need to find any minimum or maximum in each dimension
    // to enlarge the bounding box.
    // To do this, for each, dimension, we take the derivative
    // of the cubic, leaving a quadratic, and find the zeros of it.
    // The quadratic is such that its ith derivatives at zero are
    // the (i+1)th derivatives of the curve segment at zero.

    // First segment is ((1/6)(1-t)^3)*P0 + ((1-t) - (1/3)*(1-t)^3)*P1 + (1 - (1-t) + (1/6)(1-t)^3)*P2
    // 1st derivative is (-(1/2)(1-t)^2)*P0 + (-1 + (1-t)^2)*P1 + (1 - (1/2)(1-t)^2)*P2
    // 2nd derivative is (1-t)*P0 + (-2(1-t))*P1 + (1-t)*P2
    // 3rd derivative is (-1)*P0 + (2)*P1 + (-1)*P2

    // a = (1/2) * 3rd derivative of curve segment at zero
    // b = 2nd derivative of curve segment at zero
    //     (this is equivalent to the 2nd difference)
    UT_Vector3 b = cvs[0] - 2.0f*cvs[1] + cvs[2];
    UT_Vector3 a = -0.5f*b;

    // c = 1st derivative of curve segment at zero
    //     (this is equivalent to the central difference)
    UT_Vector3 c = cvs[2] - cvs[0];
    c *= 0.5f;

    enlargeBoundingBoxCommon<UT_SplineCubic::evalSubDEnd<float> >(box, cvs, a, b, c, rootmin, rootmax);
}

#endif
