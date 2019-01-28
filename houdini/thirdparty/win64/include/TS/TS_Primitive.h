/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TS_Primitive (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _TS_Primitive_h_
#define _TS_Primitive_h_

#include "TS_API.h"
#include "TS_Expression.h"
#include "TS_Ray.h"

class TS_MetaKernel;
class GEO_Primitive;

class TS_API TS_MetaPrimitive: public TS_MetaExpression
{
protected:
    TS_MetaPrimitive();
    TS_MetaPrimitive(const TS_MetaPrimitive *);
public:
    virtual ~TS_MetaPrimitive();

    const TS_MetaPrimitive	*isPrimitive() const;
    TS_MetaPrimitive		*isPrimitive();
    virtual void		 setWeight(float);
    virtual float		 getWeight() const;
    const UT_BoundingBox	&getBBox() const { return myBBox; }
    UT_Interval			 getBBox(UT_BoundingBox &) const;
    virtual void		 setKernel(const TS_MetaKernel &);
    virtual void		 transform(const UT_Matrix4 &) = 0;
    virtual void		 raySweepInit(const TS_Ray &ray,
					float clip, float &t0, float &t1);
    virtual void                 init() = 0;
    virtual void                 reset();
    virtual TS_MetaExpression	*prune(const UT_BoundingBox &, UT_Interval &);

    void			 setGeoPrimPtr(const GEO_Primitive *g)
				    { myGeoPrimPtr = g; }
    const GEO_Primitive		*getGeoPrimPtr() const	{ return myGeoPrimPtr; }

    virtual float		 listDensity(const UT_Vector3 &p,
					UT_ValArray<TS_MetaPrimitive *> &);

    int                          rayIndex;
    int				 myOctreeRayIndex;

    // A serial for metaball rendering
    int				 myRenderSerial;

    virtual void		 getAllPrimitives(UT_Array<TS_MetaPrimitive *> &prims);
    
    /// Compute memory usage (including all shared memory)
    /// There's nothing to add at this level, and all subclasses need it, so
    /// it might as well be pure virtual.
    /// NOTE: Subclasses must add in TS_MetaExpression::getMemoryUsage(false).
    virtual int64 getMemoryUsage(bool inclusive) const = 0;

protected:
    float                       weight;
    TS_MetaKernel               kernel;

private:
    /// Pointer to the (unique) GEO_Primitive containing this TS_MetaPrimitive
    const GEO_Primitive         *myGeoPrimPtr;
};

#endif
