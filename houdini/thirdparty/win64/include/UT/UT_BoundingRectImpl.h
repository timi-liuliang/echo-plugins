/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BoundingRectImpl.h (UT Library, C++)
 *
 * COMMENTS:	Implementation for UT_BoundingRect
 */

#ifndef __UT_BoundingRectImpl_H__
#define __UT_BoundingRectImpl_H__

#include <limits>

#define FASTBOX(idx) \
    ray = T(1.0) / (v1(idx) - v0(idx)); \
    positive = (ray > T(0.0)); \
    t1 = (vals[idx][  positive] - v0(idx))*ray; \
    if (t1 < tmax) { if (t1 < tmin) return 0; else tmax = t1; } \
    t1 = (vals[idx][1-positive] - v0(idx))*ray; \
    if (t1 > tmin) { if (t1 > tmax) return 0; else tmin = t1; }

template <typename T>
inline int
UT_BoundingRectT<T>::intersects(
	const UT_Vector2T<T> &v0,
	const UT_Vector2T<T> &v1) const
{
    T	tmin, tmax;
    T	ray;
    T	t1;
    int	positive;

    tmin = 0;
    tmax = 1;

    FASTBOX(0)
    FASTBOX(1)

    return 1;
}

#undef FASTBOX

template <typename T>
inline int
UT_BoundingRectT<T>::intersects(
	const UT_Vector2T<T> &v0,
	const UT_Vector2T<T> &v1,
	const UT_Vector2T<T> &v2) const
{
    bool		 v0eqv1 = v0.isEqual(v1);
    bool		 v0eqv2 = v0.isEqual(v2);
    bool		 v1eqv2 = v1.isEqual(v2);

    // If any triangle edge intersects the rectangle, we intersect.
    if ((!v0eqv1 && intersects(v0, v1)) ||
	(!v1eqv2 && intersects(v1, v2)) ||
	(!v0eqv2 && intersects(v2, v0)))
	return 1;

    // If any two (or more) vertices are the same, the line intersection
    // tests are all we need to do. The rectangle can't be inside a
    // degenerate triangle.
    if (v0eqv1 || v0eqv2 || v1eqv2)
	return 0;

    UT_Vector2T<T>	 l01, l12, l20;
    T			 cp0, cp1, cp2;

    l01 = v1 - v0;
    l12 = v2 - v1;
    l20 = v0 - v2;
    cp0 = cross(l01, v2 - v0);
    cp1 = cross(l12, v0 - v1);
    cp2 = cross(l20, v1 - v2);

    // Another check for degeneracy if all three points are in a line.
    if (cp0 == 0.0 || cp1 == 0.0 || cp2 == 0.0)
	return 0;

    // The only other way we could have intersection is if the rectangle is
    // completely enclosed by the triangle. To test for this case, just test
    // whether the center of the rectangle is inside the triangle by testing
    // that the center point is on the same side of each edge as the extra
    // vertex opposite each edge.
    UT_Vector2T<T>	 center(centerX(), centerY());
    if (cp0 * cross(l01, center - v0) >= 0.0 &&
	cp1 * cross(l12, center - v1) >= 0.0 &&
	cp2 * cross(l20, center - v2) >= 0.0)
	return 1;

    return 0;
}

template <typename T>
inline void
UT_BoundingRectT<T>::project(T &x, T &y, int *touchx, int *touchy) const
{
    int inx, iny;
    T newx, newy, dx, dy;

    inx = iny = 0;

    // Calculate the values for the X direction
    if(x < vals[0][0])
	newx = vals[0][0];
    else if(x > vals[0][1])
	newx = vals[0][1];
    else
    {
	inx = 1;
        newx = ((x-vals[0][0]) < (vals[0][1]-x)) ? vals[0][0] : vals[0][1];
        dx = (newx - x > 0.0) ? newx - x : x - newx;
    }

    // Calculate the values for the Y direction
    if(y < vals[1][0])
	newy = vals[1][0];
    else if(y > vals[1][1])
	newy = vals[1][1];
    else
    {
	iny = 1;
	newy = ((y-vals[1][0]) < (vals[1][1]-y)) ? vals[1][0] : vals[1][1];
        dy = (newy - y > 0.0) ? newy - y : y - newy;
    }

    if(inx) 
    {
	if(iny) 
	{
	    if(dx < dy) 
	    {
		x = newx;
		if (touchx) *touchx = 1;
	    }
	    else 
	    {
		y = newy;
		if (touchy) *touchy = 1;
	    }
	}
	else
	{
	    y = newy;
	    if (touchy) *touchy = 1;
	}
    }
    else
    {
        x = newx;
	if (touchx) *touchx = 1;
        if(!iny) 
	{
	    y = newy;
	    if (touchy) *touchy = 1;
	}
    }
}

#define TESTFACE(face, min, face1) \
	    t = (vals[face][min] - o(face)) * invd(face); \
	    if (t >= (T)0 && t < tmin) \
	    { \
		t1 = o(face1) + t*d(face1); \
		if (t1 >= vals[face1][0] && t1 <= vals[face1][1]) \
		{ \
		    tmin = t; \
		    found = face; \
		} \
	    }

template <typename T>
inline int
UT_BoundingRectT<T>::intersectRay(
	const UT_Vector2T<T> &o, const UT_Vector2T<T> &d,
	T maxdist, T *dist, UT_Vector2T<T> *xsect) const
{
    UT_Vector2T<T>  invd;
    T		    t, t1, tmin;
    int		    found;

    found = -1;
    tmin = maxdist;
    invd = 1.0/d;

    TESTFACE(0, 0, 1)
    TESTFACE(0, 1, 1)

    TESTFACE(1, 0, 0)
    TESTFACE(1, 1, 0)

    if (found == -1) return 0;

    if (dist)	*dist  = tmin;
    if (xsect)	*xsect = o + tmin*d;

    return 1;
}

#undef TESTFACE

#endif // __UT_BoundingRectImpl_H__
