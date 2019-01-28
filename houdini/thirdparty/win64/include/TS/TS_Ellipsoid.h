/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TS_Ellipsoid (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _TS_Ellipsoid_h_
#define _TS_Ellipsoid_h_

#include "TS_API.h"
#include <UT/UT_Interval.h>
#include "TS_Primitive.h"

class TS_API TS_MetaEllipsoid: public TS_MetaPrimitive
{
public:
    TS_MetaEllipsoid();
    TS_MetaEllipsoid(const TS_MetaEllipsoid *);
    virtual ~TS_MetaEllipsoid();

    virtual float               density(const UT_Vector3 &p) const;
    virtual UT_Interval         density(const UT_BoundingBox &p) const;
    virtual float               grad(const UT_Vector3 &point, UT_Vector3 &grad) const;
    virtual int                 setRay(const TS_Ray &ray);
    virtual float               rayDensity(float t) const;
    virtual UT_Interval         rayDensity(const UT_Interval &t) const;
    virtual void                transform(const UT_Matrix4 &);

    // Get the matrix of this ellipsoid's quadric.
    void		getQuadricMatrix4(UT_Matrix4 &m) const;

    virtual TS_MetaExpression   *duplicate() const;
    virtual void                init();
    virtual void                reset();
    /// Compute memory usage (includes all shared memory)
    /// NOTE: Must add in TS_MetaExpression::getMemoryUsage(false).
    virtual int64 getMemoryUsage(bool inclusive) const;

private:
    TS_Quadric          q;
    fpreal64		myT2, myT1, myT0;     // computed data for ray tracing
};

#endif
