/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_BoundingRect.h (UT Library, C++)
 *
 * COMMENTS:	Bounding Rectangle (floating point)
 */

#ifndef __UT_BoundingRect_H__
#define __UT_BoundingRect_H__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Vector2.h"

#include <SYS/SYS_Math.h>

#include <iosfwd>


class UT_JSONWriter;
class UT_JSONValue;
class UT_JSONParser;

template <typename T>
class UT_API UT_BoundingRectT
{
public:
    typedef	UT_BoundingRectT<T>	this_type;

		UT_BoundingRectT() {}
    		UT_BoundingRectT(T xmin, T ymin, T xmax, T ymax)
		{
		    vals[0][0] = xmin;
		    vals[1][0] = ymin;
		    vals[0][1] = xmax;
		    vals[1][1] = ymax;
		}

		UT_BoundingRectT(const UT_Vector2T<T> &lowerbound,
				 const UT_Vector2T<T> &upperbound)
		{
		    vals[0][0] = lowerbound.vec[0];
		    vals[0][1] = upperbound.vec[0];
		    vals[1][0] = lowerbound.vec[1];
		    vals[1][1] = upperbound.vec[1];
		}

    template<typename U>
    explicit	UT_BoundingRectT(const UT_BoundingRectT<U> &src)
		{
		    vals[0][0] = T(src.vals[0][0]);
		    vals[0][1] = T(src.vals[0][1]);
		    vals[1][0] = T(src.vals[1][0]);
		    vals[1][1] = T(src.vals[1][1]);
		}

    T		 operator()(unsigned m, unsigned n) const
		 {
		     UT_ASSERT_P( m < 2 && n < 2 );
		     return vals[m][n];
		 }
    T		&operator()(unsigned m, unsigned n)
		 {
		     UT_ASSERT_P( m < 2 && n < 2 );
		     return vals[m][n];
		 }

    int		 operator==(const UT_BoundingRectT<T> &brect) const
		 {
		    return (vals[0][0] == brect.vals[0][0] &&
			    vals[0][1] == brect.vals[0][1] &&
		            vals[1][0] == brect.vals[1][0] &&
			    vals[1][1] == brect.vals[1][1] );
		 }
    bool	 operator!=(const UT_BoundingRectT<T> &brect) const
		 {
		     return !(*this == brect);
		 }

    int		contains(const UT_Vector2T<T> &pt) const
		{
		    if (vals[0][0] > pt.x() || vals[0][1] < pt.x()) return 0;
		    if (vals[1][0] > pt.y() || vals[1][1] < pt.y()) return 0;
		    return 1;
		}
    int		contains(const UT_Vector2T<T> &pt, T tol) const
		{
		    if (vals[0][0] > pt.x() + tol ||
			vals[0][1] < pt.x() - tol)
			return 0;
		    if (vals[1][0] > pt.y() + tol ||
			vals[1][1] < pt.y() - tol)
			return 0;
		    return 1;
		}
    int		contains(T x, T y) const
		{
		    return (x >= vals[0][0] && x <= vals[0][1] &&
			    y >= vals[1][0] && y <= vals[1][1] );
		}
    UT_Vector2T<T> closestPoint(const UT_Vector2T<T> &pt) const
		{
		    UT_Vector2T<T>	 closest_pt(pt.x(), pt.y());

		    if (closest_pt.x() < xmin())
			closest_pt.x() = xmin();
		    else if (closest_pt.x() > xmax())
			closest_pt.x() = xmax();

		    if (closest_pt.y() < ymin())
			closest_pt.y() = ymin();
		    else if (closest_pt.y() > ymax())
			closest_pt.y() = ymax();

		    return closest_pt;
		}

    int		isInside(const UT_BoundingRectT<T> &brect) const
		{
		    // are we inside brect?
		    return (vals[0][0] >= brect.vals[0][0] &&
			    vals[0][1] <= brect.vals[0][1] &&
		            vals[1][0] >= brect.vals[1][0] &&
			    vals[1][1] <= brect.vals[1][1] );
		}

    // Determine whether a triangle defined by (v0, v1, v2) intersects the rect
    inline int	intersects(const UT_Vector2T<T> &v0,
			   const UT_Vector2T<T> &v1,
			   const UT_Vector2T<T> &v2) const;
    // Determine whether a line between v0 & v1 intersects the rect
    inline int	intersects(const UT_Vector2T<T> &v0,
			   const UT_Vector2T<T> &v1) const;
    inline int	intersects(const UT_BoundingRectT<T> &rect) const
		{
		    if (vals[0][0] > rect.vals[0][1] ||
			vals[0][1] < rect.vals[0][0])
			return 0;
		    if (vals[1][0] > rect.vals[1][1] ||
			vals[1][1] < rect.vals[1][0])
			return 0;
		    return 1;
		}
    inline int	intersects(const UT_BoundingRectT<T> &rect, T tol) const
		{
		    if (vals[0][0] > rect.vals[0][1] + tol ||
			vals[0][1] < rect.vals[0][0] - tol) 
			return 0;
		    if (vals[1][0] > rect.vals[1][1] + tol || 
			vals[1][1] < rect.vals[1][0] - tol)
			return 0;
		    return 1;
		}

    bool	intersectIfOverlapping(const UT_BoundingRectT<T> &src)
		{
		    if (!intersects(src))
			return false;
		    intersectBounds(src);
		    return true;
		}
    void	intersectBounds(const UT_BoundingRectT<T> &src)
		{
		    vals[0][0] = SYSmax(vals[0][0], src.vals[0][0]);
		    vals[0][1] = SYSmin(vals[0][1], src.vals[0][1]);
		    vals[1][0] = SYSmax(vals[1][0], src.vals[1][0]);
		    vals[1][1] = SYSmin(vals[1][1], src.vals[1][1]);
		}
    void	clampX(T min, T max)
		{
		    vals[0][0] = SYSmax(vals[0][0], min);
		    vals[0][1] = SYSmin(vals[0][1], max);
		}
    void	clampY(T min, T max)
		{
		    vals[1][0] = SYSmax(vals[1][0], min);
		    vals[1][1] = SYSmin(vals[1][1], max);
		}

    /// Check whether the bounding box contains at least one point.
    bool	isValid() const
		{
		    return vals[0][0] <= vals[0][1] &&
			   vals[1][0] <= vals[1][1];
		}
    void	makeInvalid()	{ initBounds(); }

    /// Initialize the box such that
    ///	- No points are contained in the box
    ///	- The box occupies no position in space
    void	initBounds()
		{
		    vals[0][0] =
			vals[1][0] =
			    0.5 * std::numeric_limits<T>::max();
		    vals[0][1] =
			vals[1][1] =
			    -0.5 * std::numeric_limits<T>::max();
		}
    void	initBounds(const UT_Vector2T<T> &pt)
		{
		    vals[0][0] = vals[0][1] = pt.x();
		    vals[1][0] = vals[1][1] = pt.y();
		}
    void	initBounds(T x, T y)
		{
		    vals[0][0] = vals[0][1] = x;
		    vals[1][0] = vals[1][1] = y;
		}
    void	initBounds(T xmin, T xmax, T ymin, T ymax)
		{
		    vals[0][0] = xmin; vals[0][1] = xmax;
		    vals[1][0] = ymin; vals[1][1] = ymax;
		}
    void	initBounds(const fpreal32 *v) { initBounds(v[0], v[1]); }
    void	initBounds(const fpreal64 *v) { initBounds(v[0], v[1]); }
    void	initBounds(const UT_BoundingRectT<T> &rect)
		{
		    vals[0][0] = rect.vals[0][0];
		    vals[0][1] = rect.vals[0][1];
		    vals[1][0] = rect.vals[1][0];
		    vals[1][1] = rect.vals[1][1];
		}

    /// Initialize the box to the largest size
    void	initMaxBounds()
		{
		    vals[0][0] = vals[1][0] =
			-0.5*std::numeric_limits<T>::max();
		    vals[0][1] = vals[1][1] =
			 0.5*std::numeric_limits<T>::max();
		}

    void	enlargeBounds(const UT_Vector2T<T> &pt)
		{
		    enlargeBounds(pt.x(), pt.y());
		}
    void	enlargeBounds(T x, T y)
		{
		    vals[0][0] = SYSmin(vals[0][0], x);
		    vals[0][1] = SYSmax(vals[0][1], x);
		    vals[1][0] = SYSmin(vals[1][0], y);
		    vals[1][1] = SYSmax(vals[1][1], y);
		}
    void	enlargeBounds(const fpreal32 *v) { enlargeBounds(v[0], v[1]); }
    void	enlargeBounds(const fpreal64 *v) { enlargeBounds(v[0], v[1]); }
    void	enlargeBounds(T xmin, T xmax, T ymin, T ymax)
		{
		    vals[0][0] = SYSmin(vals[0][0], xmin);
		    vals[0][1] = SYSmax(vals[0][1], xmax);
		    vals[1][0] = SYSmin(vals[1][0], ymin);
		    vals[1][1] = SYSmax(vals[1][1], ymax);
		}
    void	enlargeBounds(const UT_BoundingRectT<T> &rect)
		{
		    vals[0][0] = SYSmin(vals[0][0], rect(0, 0));
		    vals[0][1] = SYSmax(vals[0][1], rect(0, 1));
		    vals[1][0] = SYSmin(vals[1][0], rect(1, 0));
		    vals[1][1] = SYSmax(vals[1][1], rect(1, 1));
		}
    void	expandBounds(T dx, T dy)
		{
		    vals[0][0] -= dx;
		    vals[0][1] += dx;
		    vals[1][0] -= dy;
		    vals[1][1] += dy;
		}
    void	stretch(T percent = 0.001, T min = 0.001)
		{
		    T d;
		    d = min + sizeX()*percent;
		    vals[0][0] -= d; vals[0][1] += d;
		    d = min + sizeY()*percent;
		    vals[1][0] -= d; vals[1][1] += d;
		}
    void	translate(T x, T y)
		{
		    vals[0][0] += x;
		    vals[0][1] += x;
		    vals[1][0] += y;
		    vals[1][1] += y;
		}
    void	scale(T xscale, T yscale)
		{
		    vals[0][0] *= xscale; vals[0][1] *= xscale;
		    vals[1][0] *= yscale; vals[1][1] *= yscale;
		}

    uint8	cohenSutherland(const UT_BoundingRectT<T> &box) const
		{
		    int		flag = 0;

                    if (xmin() > box.xmax())
                        flag |= 0x01;
                    else if (xmax() < box.xmin())
                        flag |= 0x02;

                    if (ymin() > box.ymax())
                        flag |= 0x04;
                    else if (ymax() < box.ymin())
                        flag |= 0x08;

                    return flag;
		}

    T		sizeX() const { return vals[0][1] - vals[0][0]; }
    T		sizeY() const { return vals[1][1] - vals[1][0]; }

    T		centerX() const { return (vals[0][0] + vals[0][1]) * 0.5; }
    T		centerY() const { return (vals[1][0] + vals[1][1]) * 0.5; }

    // projects (x,y) orthogonally to the closest point on the rectangle.
    // Result is stored back into x and y.  If valid addresses are passed into
    // touchx and touchy, then, the values in those addresses would indicate
    // if the new (x,y) now touches the x edges or y edges of the rectangle.
    inline void	project(T &x, T &y, int *touchx=0, int *touchy=0) const;

    T		LX() const	{ return vals[0][0]; }
    T		LY() const	{ return vals[1][0]; }
    T		UX() const	{ return vals[0][1]; }
    T		UY() const	{ return vals[1][1]; }

    T		&LX() 		{ return vals[0][0]; }
    T		&LY()		{ return vals[1][0]; }
    T		&UX()		{ return vals[0][1]; }
    T		&UY()		{ return vals[1][1]; }

    void	setX0(T v)	{ vals[0][0] = v; }
    void	setX1(T v)	{ vals[0][1] = v; }
    void	setY0(T v)	{ vals[1][0] = v; }
    void	setY1(T v)	{ vals[1][1] = v; }

    T		getX0() const	{ return vals[0][0]; }
    T		getX1() const	{ return vals[0][1]; }
    T		getY0() const	{ return vals[1][0]; }
    T		getY1() const	{ return vals[1][1]; }

    T		&getX0()	{ return vals[0][0]; }
    T		&getX1()	{ return vals[0][1]; }
    T		&getY0()	{ return vals[1][0]; }
    T		&getY1()	{ return vals[1][1]; }

    T		xmin() const	{ return vals[0][0]; }
    T		xmax() const	{ return vals[0][1]; }
    T		ymin() const	{ return vals[1][0]; }
    T		ymax() const	{ return vals[1][1]; }

    T		xsize() const	{ return vals[0][1] - vals[0][0]; }
    T		ysize() const	{ return vals[1][1] - vals[1][0]; }

    // Intersect a ray with the rect.  Returns 0 if no intersection found.
    // distance will be set to the intersection distance (between 0 & tmax)
    inline int	intersectRay(const UT_Vector2T<T>   &orig,
			     const UT_Vector2T<T>   &dir,
			     T			     tmax = 1E17,
			     T			    *distance = 0,
			     UT_Vector2T<T>	    *xsect = 0) const;

    // I/O friends:
    friend std::ostream	&operator<<(std::ostream &os,
				    const UT_BoundingRectT<T> &brect)
			{
			    brect.save(os);
			    return os;
			}
    /// @{
    /// Methods to serialize to a JSON stream.  The vector is stored as an
    /// array of 4 reals (xmin, xmax, ymin, ymax)
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    void		dump(const char *msg="") const;
    void		dump(std::ostream &os) const;

    /// @{
    /// Access to the serialized data
    const T	*getSerialized() const	{ return myFloats; }
    const T	*data() const { return myFloats; }
    T		*data() { return myFloats; }
    /// @}

    /// @{
    /// Iterate over the data serially
    const T	*begin() const { return &myFloats[0]; }
    const T	*end() const { return &myFloats[4]; }
    T		*begin() { return &myFloats[0]; }
    T		*end() { return &myFloats[4]; }
    /// @}

    /// @{
    /// Compute a hash
    uint64		hash() const;
    friend std::size_t	hash_value(const this_type &t) { return t.hash(); }
    /// @}

    union {
	T	vals[2][2];
	T	myFloats[4];
    };
private:
    void	save(std::ostream &os) const;
};

template <typename T>
UT_API size_t format(char *buf, size_t bufsize, const UT_BoundingRectT<T> &v);

typedef UT_BoundingRectT<int32>	    UT_BoundingRecti;
typedef UT_BoundingRectT<int64>	    UT_BoundingRectI;
typedef UT_BoundingRectT<fpreal>    UT_BoundingRectR;
typedef UT_BoundingRectT<fpreal32>  UT_BoundingRectF;
typedef UT_BoundingRectT<fpreal64>  UT_BoundingRectD;
typedef UT_BoundingRectT<float>	    UT_BoundingRect; // deprecated

#include "UT_BoundingRectImpl.h"

#endif
