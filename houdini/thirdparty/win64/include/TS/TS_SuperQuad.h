/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TS_SuperQuad (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _TS_SuperQuad_h_
#define _TS_SuperQuad_h_

#include "TS_API.h"
#include "TS_Primitive.h"

class UT_LUT;

class TS_API TS_MetaSuperQuadric: public TS_MetaPrimitive
{
public:
    TS_MetaSuperQuadric();
    TS_MetaSuperQuadric(const TS_MetaSuperQuadric *);
    virtual ~TS_MetaSuperQuadric();
    virtual float       density(const UT_Vector3 &p) const;
    virtual UT_Interval density(const UT_BoundingBox &p) const;
    virtual float       grad(const UT_Vector3 &point, UT_Vector3 &grad) const;
    virtual int         setRay(const TS_Ray &ray);
    virtual float       rayDensity(float t) const;
    virtual UT_Interval rayDensity(const UT_Interval &t) const;
    virtual void        transform(const UT_Matrix4 &);
    virtual TS_MetaExpression *duplicate() const;
    void                setXYexp(float arg);
    float               getXYexp() const;
    void                setZexp(float arg);
    float               getZexp() const;
    virtual void        init();
    virtual void        reset();
    /// Compute memory usage (includes all shared memory)
    /// NOTE: Must add in TS_MetaExpression::getMemoryUsage(false).
    virtual int64 getMemoryUsage(bool inclusive) const;

private:
    void                setLUTs();
    float               xyexp;
    float               zexp, xyzexp;
    float               izexp1, xyzexp1;
    UT_LUT              *xylut, *zlut, *izlut;
    UT_LUT              *xy1lut, *z1lut;
    UT_Matrix4          m;
    UT_Matrix4          im;
    UT_Vector3          dir, anchor;
    int                 hitFlag;
};

#endif
