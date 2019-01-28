/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Mantra (C++)
 *
 * COMMENTS:	Ray class used by mantra's ray tracer
 *
 */

#ifndef __VGEO_Ray_H__
#define __VGEO_Ray_H__

#include "VGEO_API.h"
#include <VM/VM_SIMD.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Defines.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Vector3.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <iostream>

#define VGEO_RAY_VECTORIZED 1

class VPRM_Space;
using VPRM_SpacePtr = UT_IntrusivePtr<VPRM_Space>;

class VGEO_API VGEO_Ray {
public:
    VGEO_Ray() {}

    VGEO_Ray(const UT_Vector3 &P, const UT_Vector3 &D)
    {
        init(P, D);
    }

    /// Initialize the ray for the given position and direction
    void	init(const UT_Vector3 &P, const UT_Vector3 &D);

    /// Construct a transformed version of this ray
    void	transform(VGEO_Ray &newRay, const VPRM_SpacePtr &space,
			    float now) const;

    /// Construct a translated version of this ray
    void	translate(VGEO_Ray &newRay, const UT_Vector3 &t) const
    {
        newRay = *this;
#if !VGEO_RAY_VECTORIZED
        newRay.myP += t;
#else
        (*reinterpret_cast<UT_Vector3*>(&newRay.myP)) += t;
#endif
    }
    void	itranslate(VGEO_Ray &newRay, const UT_Vector3 &t) const
    {
        newRay = *this;
#if !VGEO_RAY_VECTORIZED
        newRay.myP -= t;
#else
        (*reinterpret_cast<UT_Vector3*>(&newRay.myP)) -= t;
#endif
    }

    /// This method adjusts the ranges for the ray so that tmin is where
    /// the ray enters the bounding box, and tmax is where it exists.
    /// Returns true when the ray intersects the bounding box.
    SYS_FORCE_INLINE bool getBoxRange(const UT_BoundingBox &box,
			    float &tmin, float &tmax) const
    {
#if !VGEO_RAY_VECTORIZED
	getRangeAxis<0>(box, tmin, tmax);
	getRangeAxis<1>(box, tmin, tmax);
	getRangeAxis<2>(box, tmin, tmax);
#else
        //v4uf ta(box.vals[0][0], box.vals[1][0], box.vals[2][0], 0);
        //v4uf tb(box.vals[0][1], box.vals[1][1], box.vals[2][1], 0);
        //v4uf t1 = (ta & mySign) | andn(mySign, tb)
        //v4uf t2 = (tb & mySign) | andn(mySign, ta)
	int sign0 = mySign[0];
	int sign1 = mySign[1];
	int sign2 = mySign[2];
        v4uf t1(box.vals[0][sign0  ], box.vals[1][sign1  ], box.vals[2][sign2  ], tmax);
        v4uf t2(box.vals[0][sign0^1], box.vals[1][sign1^1], box.vals[2][sign2^1], tmin);
        t1 -= myP;
        t2 -= myP;
        t1 *= myID;
        t2 *= myID;
        v4uf tmaxtemp = vmin(t1, t1.swizzle<2,3,2,3>());
        v4uf tmintemp = vmax(t2, t2.swizzle<2,3,2,3>());
        float tmaxlocal = vmin(tmaxtemp, tmaxtemp.swizzle<1,1,1,1>())[0];
        float tminlocal = vmax(tmintemp, tmintemp.swizzle<1,1,1,1>())[0];
        tmax = tmaxlocal;
        tmin = tminlocal;
        return tminlocal <= tmaxlocal;
#endif

#if 0
	int sign0 = mySign[0];
	int sign1 = mySign[1];
	int sign2 = mySign[2];
	UT_Vector3 t1(box.vals[0][sign0  ], box.vals[1][sign1  ], box.vals[2][sign2  ]);
	UT_Vector3 t2(box.vals[0][sign0^1], box.vals[1][sign1^1], box.vals[2][sign2^1]);
        t1 -= myP;
        t2 -= myP;
        t1 *= myID;
        t2 *= myID;
        tmin = SYSmax(tmin, t2(0), t2(1), t2(2));
        tmax = SYSmin(tmax, t1(0), t1(1), t1(2));
#endif

	return tmin <= tmax;
    }

    SYS_FORCE_INLINE
    const UT_Vector3 &getP() const
#if !VGEO_RAY_VECTORIZED
    { return myP; }
#else
    { return *reinterpret_cast<const UT_Vector3*>(&myP); }
#endif
    SYS_FORCE_INLINE
    const UT_Vector3 &getID() const
#if !VGEO_RAY_VECTORIZED
    { return myID; }
#else
    { return *reinterpret_cast<const UT_Vector3*>(&myID); }
#endif
    SYS_FORCE_INLINE
    const UT_Vector3 &getD() const { return myD; }
    SYS_FORCE_INLINE
    UT_Vector3 getPt(const float &t) const { return getP() + getD() * t; }
    SYS_FORCE_INLINE
    const int *getSign() const { return mySign; }

    /// @{
    /// The following forms are used to represent the ray as a set of two
    /// orthogonal planes.  The planes are not orthonormal and you MUST call
    /// getN1 before you call getN2 or getMaxD.
    const UT_Vector3	&getN1() const
    {
        if (myMaxD < 0) computeN();
        return myN1;
    }
    const UT_Vector3	&getN2() const		{ return myN2; }
    int			 getMaxD() const	{ return myMaxD; }
    /// @}

    /// Save the ray as text to the stream
    void	save(std::ostream &os) const;

    /// Dump the ray to stderr
    void	dump() const { save(std::cerr); }

private:
#if !VGEO_RAY_VECTORIZED
    UT_Vector3		myP;    // Position
    UT_Vector3		myID;   // Inverse direction
#else
    /// Position (component 3 must be 0)
    v4uf myP;
    /// 1 / direction (component 3 must be 1)
    v4uf myID;
    /// Sign of direction (component 3 is ignored)
    /// -1 == -ve; 0 == +ve
    //v4uu mySign;

#endif
    int			mySign[3];  // Sign of direction (1 == +)

    /// Direction
    UT_Vector3 myD;

#if !VGEO_RAY_VECTORIZED
    template <int axis>
    inline void getRangeAxis(const UT_BoundingBox &box,
		      float &tmin, float &tmax) const
    {
	int	sign = mySign[axis];
	float	t1 = (box.vals[axis][sign]   - myP(axis)) * myID(axis);
	float	t2 = (box.vals[axis][sign^1] - myP(axis)) * myID(axis);
	tmax = t1 < tmax ? t1 : tmax;
	tmin = t2 > tmin ? t2 : tmin;
    }
#endif
    void		computeN() const;

    mutable UT_Vector3	myN1, myN2;		// Plane representations of ray
    mutable int		myMaxD;			// Maximum ray direction
};

#endif
