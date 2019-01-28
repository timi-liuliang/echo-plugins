/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_QuickHull.h ( UT Library, C++)
 *
 * COMMENTS:	This class is used to compute the convex hull area from a
 *		list of points.
 */

#ifndef __UT_QuickHull__
#define __UT_QuickHull__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_Vector2.h"
#include "UT_Vector3.h"

template <typename T>
class UT_API UT_QuickHullT {
public:
	     UT_QuickHullT();
    virtual ~UT_QuickHullT();

    /// Clear out points
    void	reset()		{ myPoints.entries(0); }

    /// Add a point to the hull
    void	append(const UT_Vector3T<T> &P);

    /// Compute the 2D convex hull area relative to the given plane normal.
    T		area2D(const UT_Vector3T<T> &N) const;

    ///	Return set of points that formed a convex hull based on the given plane
    /// defined by the normal
    void	getQuickHull(UT_ValArray<UT_Vector2T<T> > &points,
					    const UT_Vector3T<T> &N) const;
    ///	Return set of points that formed a convex hull based on the given plane
    /// defined by the two vectors. (X and Y are assumed to be orthogonal)
    void	getQuickHull(UT_ValArray<UT_Vector2T<T> > &points,
					    const UT_Vector3T<T> &X,
					    const UT_Vector3T<T> &Y) const;
    /// Return the area and rotation of mininul bounding rectangle of the
    /// projection of the points on to the given plane.
    T		getMinRectangle(const UT_Vector3T<T> &X,
				    const UT_Vector3T<T> &Y, T &rot) const;

protected:
    static inline void	swap2(UT_Vector2T<T> *list, uint a, uint b);
    static inline void	makeLineEqn(UT_Vector3T<T> &eqn,
					const UT_Vector2T<T> *list,
					uint a, uint b);
    static uint	quickHullSelect(UT_Vector2T<T> *list, uint a,
							uint b,	uint &cidx);
    static T	quickHull(UT_Vector2T<T> *list, uint a, uint b, uint c);
    static void	getQuickHullInternal(UT_ValArray<UT_Vector2T<T> > &points,
					    UT_Vector2T<T> *list, uint a,
					    uint b, uint c);
    static T	getMinRectInternal(const UT_ValArray<UT_Vector2T<T> > &points,
									T &rot);

    UT_Array<UT_Vector3T<T> >	myPoints;
};

typedef UT_QuickHullT<fpreal>	    UT_QuickHullR;
typedef UT_QuickHullT<fpreal32>	    UT_QuickHullF;
typedef UT_QuickHullT<fpreal64>	    UT_QuickHullD;
typedef UT_QuickHullT<float>	    UT_QuickHull;	// deprecated

template <typename T>
inline void
UT_QuickHullT<T>::swap2(UT_Vector2T<T> *list, uint a, uint b)
{
    UT_Vector2T<T>	tmp = list[a];
    list[a] = list[b];
    list[b] = tmp;
}

template <typename T>
inline void
UT_QuickHullT<T>::makeLineEqn(UT_Vector3T<T> &eqn, const UT_Vector2T<T> *list,
					uint a, uint b)
{
    eqn.x() = list[a].y() - list[b].y();
    eqn.y() = list[b].x() - list[a].x();
    eqn.z() = -(eqn.x()*list[a].x() + eqn.y()*list[a].y());
}

#endif
