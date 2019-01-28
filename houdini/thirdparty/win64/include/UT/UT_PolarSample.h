/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PolarSample.h ( VEX Library, C++)
 *
 * COMMENTS:	Equal area polar mapping of a unit grid to a sphere
 *		The sampler expects that the s,t coordinates passed in are in
 *		the range 0-1.  Anything outside this range will result in
 *		speedy core dumps.
 *		The direction passed in represents the pole of the mapping.
 *		For example, to generate a hemispherical mapping around a
 *		vector, simply pass in t values between 0 and 0.5.  This will
 *		generate a hemisphere around the direction vector passed in.
 */

#ifndef __UT_PolarSample__
#define __UT_PolarSample__

#include "UT_API.h"
#include "UT_Vector3.h"
#include "UT_Interval.h"

template <typename T>
class UT_API UT_PolarSampleT
{
public:
     UT_PolarSampleT(const UT_Vector3T<T> &dir);
     UT_PolarSampleT(const UT_Vector3T<T> &dir,
		    const UT_Vector3T<T> &framex, const UT_Vector3T<T> &framey);
     UT_PolarSampleT();
    ~UT_PolarSampleT()	{ }

    // The polar sampler should be initialized with a direction vector.  This
    // represents the axis around which the samples will be generated.
    void	setDirection(const UT_Vector3T<T> &dir);

    // Alternatively, if the direction is "fixed", you can simply call this
    // method to set the frame of reference for the polar sampling.
    // The z-axis corresponds to the axis/normal for the samplers.
    void	setDirection(const UT_Vector3T<T> &x, const UT_Vector3T<T> &y,
			     const UT_Vector3T<T> &z, bool normalize=true);

    //
    // Given an s, t coordinate compute an "equal area" projection of the
    // coordinate and pass the point on the sphere (or solid angle) back.  For
    // solid angle sampling, the getSolidAngle() method should be used to
    // adjust the "t" coordinate.
    void	compute(UT_Vector3T<T> &d, T s, T t) const;

    //
    // Given an s,t coordinate, compute an "equal area" projection of the
    // coordinate onto a unit disk (with a normal given by the direction
    // vector).
    void	computeDisk(UT_Vector3T<T> &d, T s, T t) const;

    //
    // Given an s,t coordinate, compute an "equal area" projection onto the
    // unit disk.  Unlike the full version, this only computes the XY
    // coordinates of the disk (without using the vectors specified).
    // The resulting x/y coordinates will be in the range [-1,1]
    static void	computeDisk(T &x, T &y, T s, T t);

    //
    // Compute the x/y interval for a given s/t interval.  Uses the 
    // disk mapping (above).
    static void	computeDiskInterval(UT_Interval &x, UT_Interval &y,
				    const UT_Interval &s,
				    const UT_Interval &t);

    //
    // This is a different implementation of computeDisk that uses a
    // quartered square to map points to the circle rather than a polar
    // mapping.  Here, the t-value does not have any special meaning (for
    // computeDisk, t will be the resulting sample point radius.), so this
    // function can't always be used as a drop-in replacement for
    // computeDisk.  The samples are better distributed, however.
    static void	computeDisk2(T &x, T &y, T s, T t);

    //
    // Given an s,t coordinate, compute an "equal area" projection onto the
    // unit disk.  This version creates a vector from the coordinates on
    // the disk by finding the projections of this point onto the unit 
    // sphere.
    void	computeCosine(UT_Vector3T<T> &d, T s, T t) const;

    // You can map to the full sphere without specifying a direction to align
    // with if you choose.
    static void	computeSphere(UT_Vector3T<T> &d, T s, T t);

    // The following method will return the appropriate "t" value for the
    // polar sampler to generate samples over the solid angle specified.
    // theta passed in should be the cone angle in radians (PI/2 for a
    // hemisphere).  If you are using the computeCosine() method, pass true
    // for the cosine parameter.
    static T	getSolidAngle(T theta, bool cosine);


    //
    // Access methods to find out what the frame of reference is.
    const UT_Vector3T<T>	&getX() const	{ return myX; }
    const UT_Vector3T<T>	&getY() const	{ return myY; }
    const UT_Vector3T<T>	&getZ() const	{ return myZ; }

private:
    UT_Vector3T<T>	myX, myY, myZ;
};

using UT_PolarSample  = UT_PolarSampleT<fpreal32>;
using UT_PolarSampleF = UT_PolarSampleT<fpreal32>;
using UT_PolarSampleD = UT_PolarSampleT<fpreal64>;
using UT_PolarSampleR = UT_PolarSampleT<fpreal>;

#endif
