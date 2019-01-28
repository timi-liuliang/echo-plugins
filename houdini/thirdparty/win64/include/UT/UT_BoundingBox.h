/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_BoundingBox_h__
#define __UT_BoundingBox_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Vector3.h"
#include "UT_VectorTypes.h"
#include <iosfwd>

/// Axis-aligned bounding box (AABB).
template <typename T>
class UT_API UT_BoundingBoxT
{
public:
    using this_type = UT_BoundingBoxT<T>;

		 UT_BoundingBoxT() {}
		 // Default copy constructor is fine.
		 // UT_BoundingBoxT(const UT_BoundingBoxT &);
    		 UT_BoundingBoxT(T axmin, T aymin, T azmin,
                   	         T axmax, T aymax, T azmax)
		 {
		     setBounds(axmin, aymin, azmin, axmax, aymax, azmax);
		 }

		 UT_BoundingBoxT(const UT_Vector3T<T> &lowerbound,
			         const UT_Vector3T<T> &upperbound)
		 {
		     vals[0][0] = lowerbound[0];
		     vals[0][1] = upperbound[0];
		     vals[1][0] = lowerbound[1];
		     vals[1][1] = upperbound[1];
		     vals[2][0] = lowerbound[2];
		     vals[2][1] = upperbound[2];
		 }

		 template <typename S>
		 UT_BoundingBoxT(const UT_BoundingBoxT<S> &bbox)
		 {
		     vals[0][0] = bbox.vals[0][0];
		     vals[0][1] = bbox.vals[0][1];
		     vals[1][0] = bbox.vals[1][0];
		     vals[1][1] = bbox.vals[1][1];
		     vals[2][0] = bbox.vals[2][0];
		     vals[2][1] = bbox.vals[2][1];
		 }

    template <typename S>
    UT_BoundingBoxT &operator=(const UT_BoundingBoxT<S> &bbox)
		 {
		     vals[0][0] = bbox.vals[0][0];
		     vals[0][1] = bbox.vals[0][1];
		     vals[1][0] = bbox.vals[1][0];
		     vals[1][1] = bbox.vals[1][1];
		     vals[2][0] = bbox.vals[2][0];
		     vals[2][1] = bbox.vals[2][1];
		     return *this;
		 }

    T		 operator()(unsigned m, unsigned n) const
		 {
		     UT_ASSERT_P( m < 3 && n < 2 );
		     return vals[m][n];
		 }
    T		&operator()(unsigned m, unsigned n)
		 {
		     UT_ASSERT_P( m < 3 && n < 2 );
		     return vals[m][n];
		 }
    bool	 operator==(const UT_BoundingBoxT<T> &bbox) const
		 {
		     return vals[0][0] == bbox.vals[0][0] &&
			    vals[0][1] == bbox.vals[0][1] &&
			    vals[1][0] == bbox.vals[1][0] &&
			    vals[1][1] == bbox.vals[1][1] &&
			    vals[2][0] == bbox.vals[2][0] &&
			    vals[2][1] == bbox.vals[2][1];
		 }
    bool	 operator!=(const UT_BoundingBoxT<T> &bbox) const
		 {
		     return !(*this == bbox);
		 }

    bool	 isEqual(const UT_BoundingBoxT<T> &bbox,
			 T tol = SYS_FTOLERANCE_R) const
		 {
		     return SYSisEqual(vals[0][0], bbox.vals[0][0], tol) &&
			    SYSisEqual(vals[0][1], bbox.vals[0][1], tol) &&
			    SYSisEqual(vals[1][0], bbox.vals[1][0], tol) &&
			    SYSisEqual(vals[1][1], bbox.vals[1][1], tol) &&
			    SYSisEqual(vals[2][0], bbox.vals[2][0], tol) &&
			    SYSisEqual(vals[2][1], bbox.vals[2][1], tol);
		 }

    T		 xmin() const	{ return vals[0][0]; }
    T		 xmax() const	{ return vals[0][1]; }
    T		 ymin() const	{ return vals[1][0]; }
    T		 ymax() const	{ return vals[1][1]; }
    T		 zmin() const	{ return vals[2][0]; }
    T		 zmax() const	{ return vals[2][1]; }

    UT_Vector3T<T>  minvec() const 
		 { return UT_Vector3T<T>(vals[0][0], vals[1][0], vals[2][0]); }
    UT_Vector3T<T>  maxvec() const 
		 { return UT_Vector3T<T>(vals[0][1], vals[1][1], vals[2][1]); }

    int		 isInside(const UT_Vector3T<T> &pt) const;
    int		 isInside(const UT_Vector4T<T> &pt) const;
    int		 isInside(T x, T y, T z) const;

    /// Am I totally enclosed in the bounding box passed in
    /// ("intersects" method tests for partially inside)
    int		 isInside(const UT_BoundingBoxT<T> &bbox) const;

    /// Determine whether a line intersects the box.  v0 is one end-point of
    /// the line, and idir is the inverse direction vector along the line.
    int		 isLineInside(const UT_Vector3T<T> &v0,
			      const UT_Vector3T<T> &idir) const;

    /// Determine the minimum distance of the box to a line segment, or 0
    /// if the line segment overlaps the box.  v0 is one end-point of the
    /// line, and dir is the direction vector along the line.  This method
    /// conservatively underestimates the distance, so the true line/box
    /// distance may be greater than the reported value.
    T		 approxLineDist2(const UT_Vector3T<T> &v0,
				 const UT_Vector3T<T> &dir) const;

    /// Check whether the bounding box contains at least one point.
    SYS_FORCE_INLINE
    bool	 isValid() const;
    SYS_FORCE_INLINE
    void	 makeInvalid()	{ initBounds(); }

    /// Efficient test for an invalid bounding box (one comparison instead of
    /// 3 for a valid bounding box).  This only checks X, not Y or Z ranges, so
    /// only works if the box is fully invalid.
    SYS_FORCE_INLINE
    bool	 isInvalidFast() const { return vals[0][0] > vals[0][1]; }

    void	 setBounds(T x_min, T y_min, T z_min,
			   T x_max, T y_max, T z_max)
		 {
		     vals[0][0] = x_min;
		     vals[1][0] = y_min;
		     vals[2][0] = z_min;
		     vals[0][1] = x_max;
		     vals[1][1] = y_max;
		     vals[2][1] = z_max;
		 }

    /// @{
    /// Set/Get bounds in "serialized" fashion.  The serialized order is
    /// (xmin, xmax, ymin, ymax, zmin, zmax).
    void	setSerialized(const fpreal32 floats[6])
		{
		    for (int i = 0; i < 6; ++i)
			myFloats[i] = floats[i];
		}
    void	setSerialized(const fpreal64 floats[6])
		{
		    for (int i = 0; i < 6; ++i)
			myFloats[i] = floats[i];
		}
    const T	*getSerialized() const	{ return myFloats; }
    /// @}

    /// @{
    /// Access to the serialized data
    const T	*data() const { return myFloats; }
    T		*data() { return myFloats; }
    /// @}

    /// @{
    /// Iterate over the data serially
    const T	*begin() const { return &myFloats[0]; }
    const T	*end() const { return &myFloats[6]; }
    T		*begin() { return &myFloats[0]; }
    T		*end() { return &myFloats[6]; }
    /// @}

    /// @{
    /// Compute a hash
    uint64		hash() const;
    friend std::size_t	hash_value(const this_type &t) { return t.hash(); }
    /// @}

    /// Initialize the box to the largest size
    void	 initMaxBounds();

    /// Initialize the box such that
    ///	- No points are contained in the box
    ///	- The box occupies no position in space
    SYS_FORCE_INLINE
    void	 initBounds();

    /// Initialize the bounds with the bounds given in min and max. No check
    /// is made to ensure that min is smaller than max.
    void	 initBounds(const UT_Vector3T<T> &min,
			    const UT_Vector3T<T> &max);

    /// Initialize zero-sized bounds at the location of the point given by pt.
    SYS_FORCE_INLINE
    void	 initBounds(const UT_Vector3T<T> &pt);

    /// Initialize zero-sized bounds at the location of the point given by pt.
    void	 initBounds(const UT_Vector4T<T> &pt);

    /// Initialize zero-sized bounds at the location of the point defined by
    /// x, y, and z;
    SYS_FORCE_INLINE
    void	 initBounds(T x, T y, T z);

    /// Initialize zero-sized bounds at the location of the point given by v.
    void	 initBounds(const fpreal32 *v)
		    { initBounds(v[0], v[1], v[2]); }

    /// Initialize zero-sized bounds at the location of the point given by v.
    void	 initBounds(const fpreal64 *v)
		    { initBounds(v[0], v[1], v[2]); }

    /// Initialize the bounds to the same as given by box.
    void	 initBounds(const UT_BoundingBoxT<T> &box);

    /// Enlarge the existing bounds to encompass the bounds given by min and
    /// max.
    void	 enlargeBounds(const UT_Vector3T<T> &min,
			       const UT_Vector3T<T> &max);

    /// Enlarge the existing bounds to encompass the point given by pt.
    SYS_FORCE_INLINE
    void	 enlargeBounds(const UT_Vector3T<T> &pt);

    /// Enlarge the existing bounds to encompass the point given by pt.
    void	 enlargeBounds(const UT_Vector4T<T> &pt);

    /// Enlarge the existing bounds to encompass the point defined by
    /// x, y, and z.
    SYS_FORCE_INLINE
    void	 enlargeBounds(T x, T y, T z);

    /// Enlarge the existing bounds to encompass the point given in v.
    void	 enlargeBounds(const fpreal32 *v)
		    { enlargeBounds(v[0], v[1], v[2]); }

    /// Enlarge the existing bounds to encompass the point given in v.
    void	 enlargeBounds(const fpreal64 *v)
		    { enlargeBounds(v[0], v[1], v[2]); }

    /// Enlarge the existing bounds to encompass the bounds given by box.
    SYS_FORCE_INLINE
    void	 enlargeBounds(const UT_BoundingBoxT<T> &box);

    /// Expand the bounding box on all axes, as a relative fraction of the
    /// current bbox dimensions, and/or using an absolute offset.
    SYS_FORCE_INLINE
    void	 expandBounds(T relative, T absolute);

    /// Expand the bounding box on all sides using separate absolute offsets
    /// for each axis.
    SYS_FORCE_INLINE
    void	 expandBounds(T dltx, T dlty, T dlyz);

    /// Perform a minimal enlargement of the floating point values in this
    /// bounding box.  This enlargement guarantees that the new floating
    /// point values are always different from the prior ones.  The number
    /// of mantissa bits to be changed can be adjusted using the bits
    /// parameter, and a minimum enlargement amount can be specified in min.
    void	 enlargeFloats(int bits = 1, T min = 1e-5);

    /// Find the intersections of two bounding boxes
    void	 clipBounds(const UT_BoundingBoxT<T> &box);

    /// Splits a box into two disjoint subboxes at the given splitting
    /// point.  This box is set to the left subbox for splitLeft() and the
    /// right subbox for splitRight().
    void	 splitLeft(UT_BoundingBoxT<T> &box, int axis, T split)
		 {
		     box = *this;
		     box.vals[axis][0] = split;
		     vals[axis][1] = split;
		 }
    void	 splitRight(UT_BoundingBoxT<T> &box, int axis, T split)
		 {
		     box = *this;
		     box.vals[axis][1] = split;
		     vals[axis][0] = split;
		 }

    template <typename MATRIX>
    void	 transform(const MATRIX &mat);
    template <typename MATRIX>
    void	 transform(const MATRIX &mat, 
			   UT_BoundingBoxT<T> &newbbox) const;

    /// Adds the given translate to each component of the bounding box.
    void	 translate(const UT_Vector3T<T> &delta);
				  
    T		 xsize() const { return sizeX(); }
    T		 ysize() const { return sizeY(); }
    T		 zsize() const { return sizeZ(); }
    T		 sizeX() const { return vals[0][1] - vals[0][0]; }
    T		 sizeY() const { return vals[1][1] - vals[1][0]; }
    T		 sizeZ() const { return vals[2][1] - vals[2][0]; }

    UT_Vector3T<T>  size() const 
		 { return UT_Vector3T<T>(vals[0][1] - vals[0][0],
					 vals[1][1] - vals[1][0],
					 vals[2][1] - vals[2][0]); }
    T		 sizeAxis(int axis) const 
		 {
		     UT_ASSERT(axis >= 0 && axis < 3);
		     return vals[axis][1] - vals[axis][0];
		 }

    /// Return the size of the largest dimension
    T		 sizeMax() const;  
    /// Return the size of the largest dimension, and store the dimension
    /// index in "axis"
    T		 sizeMax(int &axis) const;  
    /// Returns the minimum delta vector from the point to the bounding
    /// box or between two bounding boxes.
    UT_Vector3T<T>  minDistDelta(const UT_Vector3T<T> &p) const;
    UT_Vector3T<T>  minDistDelta(const UT_BoundingBoxT<T> &box) const;
    /// Returns minimum distance from point to bounding box squared.
    /// Returns 0 if point in bouding box.
    T		 minDist2(const UT_Vector3T<T> &p) const
		 { return minDistDelta(p).length2(); }
    /// Minimum disance between two bboxes squared.
    T		 minDist2(const UT_BoundingBoxT<T> &box) const
		 { return minDistDelta(box).length2(); }

    /// Returns the smallest absolute translation from this to box that
    /// produces the maximum overlap between the two boxes.
    UT_Vector3T<T>  minDistToMaxOverlap(const UT_BoundingBoxT<T> &box) const;

    /// Returns the radius of a sphere that would fully enclose the box.
    T		 getRadius() const	{ return 0.5*size().length(); }

    /// Finds the out code of the point relative to this box:
    int		 getOutCode(const UT_Vector3T<T> &pt) const;

    T		 xcenter() const { return centerX(); }
    T		 ycenter() const { return centerY(); }
    T		 zcenter() const { return centerZ(); }
    T		 centerX() const { return (vals[0][0] + vals[0][1])*0.5; }
    T		 centerY() const { return (vals[1][0] + vals[1][1])*0.5; }
    T		 centerZ() const { return (vals[2][0] + vals[2][1])*0.5; }
    T		 centerAxis(int axis) const
		 { return (vals[axis][0] + vals[axis][1])*0.5; }
    UT_Vector3T<T>  center() const
		 { return UT_Vector3T<T>((vals[0][0] + vals[0][1])*0.5,
					 (vals[1][0] + vals[1][1])*0.5,
					 (vals[2][0] + vals[2][1])*0.5); }

    T		 area() const;
    T		 volume() const	{ return xsize()*ysize()*zsize(); }
    void	 addToMin(const UT_Vector3T<T> &vec);
    void	 addToMax(const UT_Vector3T<T> &vec);

    /// Scale then offset a bounding box.
    void	 scaleOffset(const UT_Vector3T<T> &scale,
			     const UT_Vector3T<T> &offset);
    int		 maxAxis() const;
    int		 minAxis() const;

    /// Intersect a ray with the box.  Returns 0 if no intersection found.
    /// distance will be set to the intersection distance (between 0 & tmax)
    /// The normal will also be set.  The direction of the normal is
    /// indeterminant (to fix it, you might want to dot(dir, *nml) to check
    /// the orientation.
    int		 intersectRay(const UT_Vector3T<T> &org,
			      const UT_Vector3T<T> &dir,
			      T tmax=1E17F,
			      T *distance=0, UT_Vector3T<T> *nml=0) const;
    int		 intersectRange(const UT_Vector3T<T> &org,
			        const UT_Vector3T<T> &dir,
			        T &min, T &max) const;
    
    /// This determines if the tube, capped at distances tmin & tmax,
    /// intersects this.
    int		 intersectTube(const UT_Vector3T<T> &org,
			       const UT_Vector3T<T> &dir,
			       T radius,
			       T tmin=-1E17f, T tmax=1E17f) const;

    int		 intersects(const UT_BoundingBoxT<T> &box) const;

    /// Changes the bounds to be those of the intersection of this box
    /// and the supplied BBox.  Returns 1 if intersects, 0 otherwise.
    int		 computeIntersection(const UT_BoundingBoxT<T> &box);

    /// Here's the data for the bounding box
    union {
	T	 vals[3][2];
	T	 myFloats[6];
    };

    void	 getBBoxPoints(UT_Vector3T<T> (&ptarray)[8]) const; 
    void	 getBBoxPoints(UT_Vector4T<T> (&ptarray)[8]) const; 
    template <typename MATRIX>
    int          getBBoxPoints(UT_Vector3T<T> (&ptarray)[8],
			       const MATRIX &transform_matrix) const;

    /// Dump the bounding box to stderr.  The msg is printed before the bounds
    void	 dump(const char *msg=0) const;
    /// Dump the bounding box geometry to a draw file
    void	 dumpGeo(FILE *fp) const;

    /// @{
    /// Methods to serialize to a JSON stream.  The vector is stored as an
    /// array of 6 reals (xmin, xmax, ymin, ymax, zmin, zmax)
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}


protected:
    friend
    std::ostream &operator<<(std::ostream &os, const UT_BoundingBoxT<T> &box)
		 {
		     box.outTo(os);
		     return os;
		 }

    void	 outTo(std::ostream &os) const;
    // Ugly helper function to allow instantation with int64.
    static bool  SYSisEqual(int64 a, int64 b, int64) { return a==b; }
};

template <typename T>
UT_API size_t format(char *buf, size_t bufsize, const UT_BoundingBoxT<T> &v);

template <typename T>
bool
UT_BoundingBoxT<T>::isValid() const
{
    return vals[0][0] <= vals[0][1] &&
	   vals[1][0] <= vals[1][1] &&
	   vals[2][0] <= vals[2][1];
}

template <typename T>
void
UT_BoundingBoxT<T>::initBounds()
{
    // Initialize with min and max reversed, so that it's empty
    const T maxv =  0.5*std::numeric_limits<T>::max();
    const T minv = -maxv;
    vals[0][0] = maxv;
    vals[0][1] = minv;
    vals[1][0] = maxv;
    vals[1][1] = minv;
    vals[2][0] = maxv;
    vals[2][1] = minv;
}

template <typename T>
void
UT_BoundingBoxT<T>::initBounds(const UT_Vector3T<T> &pt)
{
    vals[0][0] = pt.x();
    vals[0][1] = pt.x();
    vals[1][0] = pt.y();
    vals[1][1] = pt.y();
    vals[2][0] = pt.z();
    vals[2][1] = pt.z();
}

template <typename T>
void
UT_BoundingBoxT<T>::initBounds(T x, T y, T z)
{
    vals[0][0] = x;
    vals[0][1] = x;
    vals[1][0] = y;
    vals[1][1] = y;
    vals[2][0] = z;
    vals[2][1] = z;
}

template <typename T>
void
UT_BoundingBoxT<T>::enlargeBounds(const UT_Vector3T<T> &pt)
{
    vals[0][0] = SYSmin(vals[0][0], pt.x());
    vals[0][1] = SYSmax(vals[0][1], pt.x());
    vals[1][0] = SYSmin(vals[1][0], pt.y());
    vals[1][1] = SYSmax(vals[1][1], pt.y());
    vals[2][0] = SYSmin(vals[2][0], pt.z());
    vals[2][1] = SYSmax(vals[2][1], pt.z());
}

template <typename T>
void
UT_BoundingBoxT<T>::enlargeBounds(T x, T y, T z)
{
    vals[0][0] = SYSmin(vals[0][0], x);
    vals[0][1] = SYSmax(vals[0][1], x);
    vals[1][0] = SYSmin(vals[1][0], y);
    vals[1][1] = SYSmax(vals[1][1], y);
    vals[2][0] = SYSmin(vals[2][0], z);
    vals[2][1] = SYSmax(vals[2][1], z);
}

template <typename T>
void
UT_BoundingBoxT<T>::enlargeBounds(const UT_BoundingBoxT<T> &box)
{
    vals[0][0] = SYSmin(vals[0][0], box(0, 0));
    vals[0][1] = SYSmax(vals[0][1], box(0, 1));
    vals[1][0] = SYSmin(vals[1][0], box(1, 0));
    vals[1][1] = SYSmax(vals[1][1], box(1, 1));
    vals[2][0] = SYSmin(vals[2][0], box(2, 0));
    vals[2][1] = SYSmax(vals[2][1], box(2, 1));
}

template <typename T>
void
UT_BoundingBoxT<T>::expandBounds(T relative, T absolute)
{
    T d;

    // Don't factor out percent for improved numerical stability when
    // dealing with large boxes.
    d = absolute + vals[0][1]*relative - vals[0][0]*relative;
    vals[0][0] -= d; vals[0][1] += d;
    d = absolute + vals[1][1]*relative - vals[1][0]*relative;
    vals[1][0] -= d; vals[1][1] += d;
    d = absolute + vals[2][1]*relative - vals[2][0]*relative;
    vals[2][0] -= d; vals[2][1] += d;
}

template <typename T>
void
UT_BoundingBoxT<T>::expandBounds(T dltx, T dlty, T dltz)
{
    vals[0][0] -= dltx;		vals[0][1] += dltx;
    vals[1][0] -= dlty;		vals[1][1] += dlty;
    vals[2][0] -= dltz;		vals[2][1] += dltz;
}

using UT_BoundingBoxR = UT_BoundingBoxT<fpreal>;
using UT_BoundingBoxF = UT_BoundingBoxT<fpreal32>;
using UT_BoundingBoxD = UT_BoundingBoxT<fpreal64>;
using UT_BoundingBoxI = UT_BoundingBoxT<int64>;
using UT_BoundingBox = UT_BoundingBoxT<float>;	// deprecated

#endif
