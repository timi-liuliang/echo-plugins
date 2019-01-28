/* TS_Meta.h * * Copyright 1993-1995 Algorithmic Arts, Inc.  
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Algorithmic Arts, Inc.;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Algorithmic Arts, Inc.
 *
 * AUTHOR:
 *	Dave Gordon
 *
 * Public interface to the TS_Meta2 Library 
 *
 * $XRevision: 1.4 $
 * $XDate: 1996/04/27 03:51:52 $
 *
 */

#ifndef _TS_Expression_h_
#define _TS_Expression_h_

#include "TS_API.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Interval.h>
#include <UT/UT_IntrusivePtr.h>

#include <UT/UT_ValArray.h>
#include "TS_Quadric.h"
#include "TS_Ray.h"

class UT_MemoryCounter;
class TS_MetaExpression;
class TS_SweepNode;
class TS_Octree;
class TS_MetaPrimitive;
class TS_MetaSum;
class TS_MetaMin;
class TS_MetaMax;
class ts_PrimNode;

typedef UT_IntrusivePtr<TS_MetaExpression>	TS_MetaExpressionPtr;
typedef UT_Array<TS_MetaExpressionPtr>		TS_ExpressionList;
typedef UT_Array<TS_SweepNode *>		TS_SweepList;

enum TS_SerialOp
{
    TS_OP_INVALID	= -1,	// Invalid operation
    TS_OP_SUM		= 0,	// Sum operation
    TS_OP_MAX		= 1,	// Max operation
    TS_OP_MIN		= 2,	// Min operation

    TS_SERIALIZE_MAGIC	= 99,	// Magic number for serialization arrays
};

/////////////////////////////////////////////////////////////////////

class TS_API TS_MetaKernel
{
public:
    const char		*myToken;
    const char		*myLabel;

    float		(*p)(float /* squared distance */);
    UT_Interval		(*pr)(const UT_Interval &);
    float		(*dp)(float);
    UT_Interval		(*dpr)(const UT_Interval &);
};

/////////////////////////////////////////////////////////////////////


class TS_API TS_MetaExpression
    : public UT_IntrusiveRefCounter<TS_MetaExpression>
{
public:
    TS_MetaExpression();
    virtual ~TS_MetaExpression();

    virtual const TS_MetaPrimitive	*isPrimitive() const;
    virtual TS_MetaPrimitive		*isPrimitive();
    virtual TS_MetaSum			*isSum();
    virtual TS_MetaMin			*isMin();
    virtual TS_MetaMax			*isMax();

    virtual float	density(const UT_Vector3 &p) const = 0;
    virtual UT_Interval density(const UT_BoundingBox &p) const = 0;

    // This density function both computes a density at the given point and
    // also populates a list of primitives affecting that point.
    virtual float	listDensity(const UT_Vector3 &p,
				UT_ValArray<TS_MetaPrimitive *> &) = 0;

    virtual float	grad(const UT_Vector3 &point,
				UT_Vector3 &grad) const = 0;

    virtual void	raySweepInit(const TS_Ray &ray, float clip,
				float &t0, float &t1) = 0;
    virtual void	raySweepComplete();

    virtual float	rayDensity(float t) const = 0;
    virtual UT_Interval rayDensity(const UT_Interval &t) const = 0;

    virtual int		rayIntersect(const TS_Ray &ray,
					const UT_Interval &start,
					float &result,
					float tol = -1.0f);
    virtual int		rayIntersect(const TS_Ray &ray, 
					const UT_Interval &start,
					UT_FloatArray &result,
					float tol = -1.0f);

    const UT_BoundingBox	&getBBox() const { return myBBox; }
    virtual UT_Interval		 getBBox(UT_BoundingBox &) const = 0;

    virtual TS_MetaExpression	*prune(const UT_BoundingBox &,
					UT_Interval &range) = 0;

    virtual TS_MetaExpression 	*duplicate() const = 0;

    virtual int			 setRay(const TS_Ray &ray) = 0;

    virtual TS_SweepNode		*allocSweepNode(const TS_Ray &ray);
    virtual const TS_ExpressionList	*getAllKids() const;

    const TS_ExpressionList	*getCellPrimitives(const UT_Vector3 &pos);

    TS_Octree			*getOctree()
				 {
				     if (!myOctree)
					 buildOctree();
				     return myOctree;
				 }

    void	setOctreeIndex(int idx) { myOctreeRayIndex = idx; }
    int		getOctreeIndex() const { return myOctreeRayIndex; }

    virtual void	getAllPrimitives(UT_Array<TS_MetaPrimitive *> &prims) {}

    // Returns the renderman expression opcode associated with this
    // expression - return -1 unless this is actually a MetaOp
    virtual int			getRibOpcode() const { return -1; }
    /// Return the serialization operation
    virtual TS_SerialOp		getOpCode() const  { return TS_OP_INVALID; }

    // Returns this expression's thread number
    int			getThreadNum() const { return myThreadNum; }

    // Sets this expression's thread number
    virtual void	setThreadNum(int threadNum) 
				 { myThreadNum = threadNum; }

    // A static function, used to set up the TS expression
    // architecture to allow ray intersection with the specified
    // number of threads
    static void		setupThreads(int numThreads);

    /// Compute memory usage (includes all shared memory)
    virtual int64	getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

public:
    // Stores the thread number to be used for this
    // expression.  This is used to determine, for
    // instance, which SweepNode object pool should
    // be used when doing ray intersection
    int		 myThreadNum;

protected:
    void		clearOctree();
    virtual void	buildOctree();

    TS_Octree		*myOctree;
    UT_BoundingBox	 myBBox;
    int			 myRayIndex, myOctreeRayIndex;

private:

};

class TS_API TS_MetaOp
    : public TS_MetaExpression
{
public:
    TS_MetaOp(int n);
    virtual ~TS_MetaOp();

    virtual void	append(const TS_MetaExpressionPtr &arg);

    virtual const TS_ExpressionList	*getAllKids() const override;
    virtual int		setRay(const TS_Ray &ray) override;
    virtual void	raySweepInit(const TS_Ray &ray,
				float clip, float &t0, float &t1) override;
    virtual void	raySweepComplete() override;

    virtual void	getAllPrimitives(
				UT_Array<TS_MetaPrimitive *> &prims) override;

    // Override the setThreadNum function here, so that
    // meta ops can also set it on all children
    virtual void	setThreadNum(int threadNum) override;

    // A reset function, to return this expression back to
    // a basically empty state as if it were just
    // constructed
    void		reset();

    /// Compute memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const override;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter,
			bool inclusive) const override;

public:
    void	initSweepList(const TS_Ray &ray);
    TS_ExpressionList	 myArray;
    TS_SweepNode	*myActive;
    TS_SweepNode	*myQueue;

protected:
    // Functions used to clean up active metaball information
    void	freeList(ts_PrimNode *listStart);
    void	clearAndDestroy();
    
private:
};

class TS_API TS_MetaSum
    : public TS_MetaOp
{
public:
    TS_MetaSum(int n = 0);
    virtual ~TS_MetaSum();

    virtual TS_MetaSum	*isSum() override final;
    virtual float	 density(const UT_Vector3 &p) const override final;
    virtual UT_Interval	 density(const UT_BoundingBox &p) const override final;
    virtual float	 listDensity(const UT_Vector3 &p,
				UT_ValArray<TS_MetaPrimitive *> &)
				override final;
    virtual float	 grad(const UT_Vector3 &point, UT_Vector3 &grad) const
				override final;
    virtual float	 rayDensity(float t) const
				override final;
    virtual UT_Interval	 rayDensity(const UT_Interval &t) const override final;
    virtual UT_Interval	 getBBox(UT_BoundingBox &) const override final;
    virtual TS_MetaExpression	*prune(const UT_BoundingBox &, UT_Interval &range)
			override final;
    virtual TS_MetaExpression	*duplicate() const override final;

    // Returns the renderman opcode for this expression
    virtual int			getRibOpcode() const override final
				    { return 0; }
    virtual TS_SerialOp		getOpCode() const override final
				    { return TS_OP_SUM; }
};

class TS_API TS_MetaMax
    : public TS_MetaOp
{
public:
    TS_MetaMax(int n = 0);
    virtual ~TS_MetaMax();

    virtual TS_MetaMax	*isMax() override final;
    virtual float	 density(const UT_Vector3 &p) const override final;
    virtual UT_Interval	 density(const UT_BoundingBox &p) const override final;
    virtual float	 listDensity(const UT_Vector3 &p,
				UT_ValArray<TS_MetaPrimitive *> &) override final;
    virtual float	 grad(const UT_Vector3 &point,
				UT_Vector3 &grad) const override final;
    virtual float	 rayDensity(float t) const override final;
    virtual UT_Interval  rayDensity(const UT_Interval &t) const override final;
    virtual UT_Interval  getBBox(UT_BoundingBox &) const override final;
    virtual TS_MetaExpression	*prune(const UT_BoundingBox &,
					UT_Interval &range) override final;
    virtual TS_MetaExpression	*duplicate() const override final;

    // Returns the renderman opcode for this expression
    virtual int		getRibOpcode() const override final { return 2; }
    virtual TS_SerialOp	getOpCode() const override final { return TS_OP_MAX; }
};

class TS_API TS_MetaMin
    : public TS_MetaOp
{
public:
    TS_MetaMin(int n = 0);
    virtual ~TS_MetaMin();
    virtual void	 append(const TS_MetaExpressionPtr &arg) override final;
    virtual TS_MetaMin	*isMin() override final;
    virtual float	 density(const UT_Vector3 &p) const override final;
    virtual UT_Interval	 density(const UT_BoundingBox &p) const override final;
    virtual float	 listDensity(const UT_Vector3 &p,
				UT_ValArray<TS_MetaPrimitive *> &)
				override final;
    virtual float	 grad(const UT_Vector3 &point,
				UT_Vector3 &grad) const override final;
    virtual float	 rayDensity(float t) const override final;
    virtual UT_Interval	 rayDensity(const UT_Interval &t) const override final;
    virtual UT_Interval	 getBBox(UT_BoundingBox &) const override final;

    virtual TS_MetaExpression	*prune(const UT_BoundingBox &,
					UT_Interval &range) override final;
    virtual TS_MetaExpression	*duplicate() const override final;

    // Returns the renderman opcode for this expression
    virtual int		getRibOpcode() const override final { return 3; }
    virtual TS_SerialOp getOpCode() const override final { return TS_OP_MIN; }
};

//----------------------------------------------------------------------//


#define TS_MetaPotentialThreshold   1.0F

#endif // _TS_Expression_h_ 
