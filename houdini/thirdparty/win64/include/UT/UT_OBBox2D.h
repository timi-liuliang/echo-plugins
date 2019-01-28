/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OBBox.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_OBBox2D_h__
#define __UT_OBBox2D_h__

#include "UT_API.h"
#include "UT_Matrix2.h"
#include "UT_Vector2.h"

template<typename T>
class UT_Array;

template <typename T>
class UT_API UT_OBBox2DT
{
public:

    /// points is an array of 2d vectors and tol is the tolerance parameter tol
    /// allows the returned answer to be (1 + tol) larger than the smallest
    /// possible. The returned answer will then be the oriented bounding box
    /// "last" encountered which is up to (1 + tol) larger than the best found
    /// box. This can be useful for avoiding rapid changes to the result when
    /// input positions are only slightly perturbed.
    /// The bd_advantage can be useful when the points list the edges of a
    /// polygon in order. If bd_advantage is nonzero, then the area of an
    /// oriented bounding box that is tangent to a boundary edge of the
    /// input polygon (consecutive points in the array) is regarded as
    /// multiplied by (1.0 - bd_advantage). This prioritizes solutions that
    /// though suboptimal are close enough to the optimal and align with an
    /// input edge when that is preferred.
    ///
    /// NB. tol and bd_advantage my fight each other. Make sure you
    /// pick a larger values for more important one.

			 UT_OBBox2DT(const UT_Array<UT_Vector2T<T> > &points,
				T tol = 0.05,
				T bd_advantage = 0.0);

    /// Return the basis that defines the orientation of the OBB
    UT_Matrix2T<T>  	 getBasis() const {return myBasis;}
    /// Return the half radii(side length) of the OBB
    UT_Vector2T<T>  	 getRadii() const {return myRadii;}
    /// Return the center of the OBB
    UT_Vector2T<T>  	 getCenter() const {return myCenter;}

    /// Return the non-oriented bounding box.
    UT_Vector2T<T> 	 getMinAxis() const
    {
        int imin = SYSargmin(myRadii.x(), myRadii.y());
        return UT_Vector2T<T>(myBasis(imin, 0), myBasis(imin, 1));
    }

    /// Return the non-oriented bounding box.
    UT_Vector2T<T> 	 getMaxAxis() const
    {
        int imax = SYSargmax(myRadii.x(), myRadii.y());
        return UT_Vector2T<T>(myBasis(imax, 0), myBasis(imax, 1));
    }

    /// Return the axis (or one of the axes) with the appropriate sign along
    /// which the bounding oriented box is tangent to the convex hull of the
    /// point set.
    UT_Vector2T<T>	 getTangentAxis() const
    {
	if  (myYPivot0Tangent)
	    return UT_Vector2T<T>(myBasis(0, 0), myBasis(0, 1));
	if  (myYPivot1Tangent)
	    return UT_Vector2T<T>(-myBasis(0, 0), -myBasis(0, 1));
	if  (myXPivot1Tangent)
	    return UT_Vector2T<T>(myBasis(1, 0), myBasis(1, 1));
	return UT_Vector2T<T>(-myBasis(1, 0), -myBasis(1, 1));
    }

    // Fill the indices of the points of tangency. Returns the mask
    // indicating at which ones the bounding box is tangent to an edge of
    // the convex hull. (0th bit for x0, 1st bit for x1, etc).
    int		 	 getSupportPoints(int &x0, int &x1,
				int &y0, int &y1) const;

    bool                 operator==(const UT_OBBox2DT<T> &obb) const;
    bool                 operator!=(const UT_OBBox2DT<T> &obb) const
    {
        return !(*this == obb);
    }

    bool                 isEqual(const UT_OBBox2DT<T> &obb,
                                 T tolerance = T(SYS_FTOLERANCE)) const;

protected:
    void    		 calcOBB(const UT_Array<UT_Vector2T<T> > &points,
				T tol, T bd_advantage);

    UT_Matrix2T<T>	 myBasis;
    UT_Vector2T<T>	 myCenter;
    UT_Vector2T<T>	 myRadii;

    int			 myXPivot0, myXPivot1, myYPivot0, myYPivot1;
    bool		 myXPivot0Tangent, myXPivot1Tangent;
    bool		 myYPivot0Tangent, myYPivot1Tangent;

private:

    class Caliper
    {
    public:
			 Caliper(int num_vertices,
				int p0, UT_Vector2T<T> l0,
				int p1, UT_Vector2T<T> l1);

	// The caliper tracks two vertices (pivots) of the polygon tangent to
	// its two supporting lines (even though it is possible for one or both
	// of the lines to overlap edges the polygon).
	inline int	 getPivot0() const { return myPivot0; }
	inline int	 getPivot1() const { return myPivot1; }


	// For each of the the min and max supporting vertex indices, the caliper
	// holds the direction of the outgoing edge out of that vertex. These
	// *limit* directions are used to limit the rotation of the caliper forward.
	UT_Vector2T<T>	 getLimit0() const { return myLimit0; }
	UT_Vector2T<T>	 getLimit1() const { return myLimit1; }

	// The caliper knows the number of points around the polygon (num_idxs
	// passed to the constructor) and can can advance each of the supporting
	// vertex indices it holds.

	inline void	 advancePivot0(UT_Vector2T<T> new_limit);
	inline void	 advancePivot1(UT_Vector2T<T> new_limit);

	// rotationRoom() returns a real value which for *any* set of two parallel
	// lines supporting our convex polygon and passing through our pivots
	// indicate that a clockwise rotation will sooner hit
	//   the min limit direction if the returned value is positive,
	//   the max limit direction if the returned value is negative, or
	//   both limit directions if the returned value is zero.
	//
	// NB. This measure uses an exact geometry predicate which means it can be
	// use very reliably to make a robust algorithmic decision.

	inline T	 rotationRoom() const;

	UT_Vector2T<T>	 getAxis() const { return myAxis; }
	void	 	 setAxis(UT_Vector2T<T> a) { myAxis = a; }

    private:
	int		 myNumVertices, myPivot0, myPivot1;
	UT_Vector2T<T>	 myLimit0, myLimit1, myAxis;
    };

    // OrthoCalipers encapsulates the idea of two calipers around the same
    // convex polygon whose lines are mutually orthogonal to each other.
    //

    class OrthoCalipers
    {
    public:
			 OrthoCalipers(int num_idxs,
				int xp0, UT_Vector2T<T> xl0,
				int xp1, UT_Vector2T<T> xl1,
				int yp0, UT_Vector2T<T> yl0,
				int yp1, UT_Vector2T<T> yl1);

	// rotate calipers while maintaining them orthogonal by the smallest
	// possible amount around the current pivots until one of the supporting
	// lines becomes tangent to an edge of the polygon (this would also be the
	// largest possible amount before one of the calipers intersects the
	// interior of the polygon. Returns the number of pivots that advance.
	int	 	 rotate(UT_Vector2T<T> xl0, UT_Vector2T<T> xl1,
				    UT_Vector2T<T> yl0, UT_Vector2T<T> yl1);

	void	 	 getAxes(UT_Vector2T<T> &x, UT_Vector2T<T> &y) const;
	void	 	 getPivots(int &xp0, int &xp1, int &yp0, int &yp1) const;

	bool		 isTangentAtX0() { return myXTangent0; }
	bool		 isTangentAtX1() { return myXTangent1; }
	bool		 isTangentAtY0() { return myYTangent0; }
	bool		 isTangentAtY1() { return myYTangent1; }

    private:
	Caliper		 myXCaliper, myYCaliper;
	bool		 myXTangent0, myXTangent1, myYTangent0, myYTangent1;
    };
};


/// Auxiliary function to compute the convex hull of a set of points in 2D.
/// Unlike the implementation in UT_QuickHull, this is a robust implementation
/// of Fortune's algorithm using exact predicates. Robustness implies that the
/// resulting convex hull passes the counter-clockwise test with infinite
/// precision for any edge of the convex hull and a third point in the input set.
///
/// The output of the algorithm is the indices of the points that appear on
/// the boundary of the convex hull in the order they do.
///
/// If set to false, the optional parameter 'exclude_on_edge_pts' makes the
/// algorithm return points that lie along the interior of convex hull
/// edges in the output sequence. Otherwise, only strict convex hull vertices
/// are returned, i.e. those that cannot be written as nontrivial convex
/// combinations of other points.
///
/// The return value is the index of a convex hull point p1 which together
/// with the first point p0 in the generated convex hull, have the property
/// that the entire convex hull projects to the line p0p1 between p0 and p1.

template <typename T>
UT_API int	 UTconvexHull2D(const UT_Array< UT_Vector2T<T> > &pts,
				UT_IntArray &hull,
				bool exclude_on_edge_pts = true);


typedef UT_OBBox2DT<fpreal>	UT_OBBox2DR;
typedef UT_OBBox2DT<fpreal32>	UT_OBBox2DF;
typedef UT_OBBox2DT<fpreal64>	UT_OBBox2DD;

#endif
