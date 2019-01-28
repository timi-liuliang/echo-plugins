/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Octree (C++)
 *
 * COMMENTS:
 *
 */


#ifndef __TS_Octree_H__
#define __TS_Octree_H__

#include "TS_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_BoundingBox.h>
#include "TS_Expression.h"
#include "TS_Primitive.h"

#define TS_OCTREE_INITIALIZED	1	// Normal octree intersect
#define TS_OCTREE_DOFAST	2	// For small octrees (1 deep)

//
//  Here's the class for a voxel.  Each voxel contains a list of primitives
//	as well as a bounding box and a split direction.  Each voxel can
//	be split into two children (creating a binary tree).
//
class TS_API TS_OctreeVoxel
{
public:
    TS_OctreeVoxel();
   ~TS_OctreeVoxel();

    /// Compute memory usage
    int64 getMemoryUsage(bool inclusive) const;

    TS_OctreeVoxel		*myKids[2];	// Left and right
    TS_ExpressionList		 myList;	// List of objects in this voxel
    unsigned		 	 myAxis;	// My split direction (axis)
    float		 	 myAvg;		// The split distance
    UT_BoundingBox	 	 myBBox;	// Bounding box

    float		 xmin() const { return myBBox(0, 0); }
    float		 xmax() const { return myBBox(0, 1); }
    float		 ymin() const { return myBBox(1, 0); }
    float		 ymax() const { return myBBox(1, 1); }
    float		 zmin() const { return myBBox(2, 0); }
    float		 zmax() const { return myBBox(2, 1); }
    TS_OctreeVoxel	*left() { return myKids[0]; }
    TS_OctreeVoxel	*right() { return myKids[1]; }

    void		 addThing(TS_MetaPrimitive *object);

    void		 subdivide(const TS_ExpressionList &nodes,
				    unsigned maxEntries, unsigned level);
private:
    void		 bestSplit(unsigned maxEntries, 
				   const TS_ExpressionList &nodes);
    void		 getKids(TS_ExpressionList &kids,
				 const TS_ExpressionList &nodes,
				 int left = 1);
};

//
//  This class is used in octree traversal.  This maintains a stack
//	of where we are in the octree...
//
class TS_API TS_OctreeStack
{
public:
    TS_OctreeVoxel	*myVoxel;
    float		 myMin, myMax;

    TS_OctreeStack	&operator=(const TS_OctreeStack &d) = default;
    unsigned		 operator==(const TS_OctreeStack &) const
			 {
			    return 0;
			 }
};

//
//  Here is the real octree class.
//
class TS_API TS_Octree
{
public:
    TS_Octree();
    TS_Octree(const TS_MetaExpression *e);
    ~TS_Octree();

    void		 setSweepList(const TS_Ray &ray, TS_SweepList &sweep,
					fpreal tmin=0, fpreal tmax=1e20);
    TS_OctreeVoxel	*findVoxel(const UT_Vector3 &P) const;

    /// Compute memory usage
    int64 getMemoryUsage(bool inclusive) const;

private:
    int			 buildMe();	// Other people may have to do things

private:
    TS_OctreeVoxel		*myHead;	// My BSP tree
    unsigned			 myMaxLevel;

    UT_Array<TS_OctreeStack>	 myStack;	// used for intersect
    const TS_MetaExpression	*myOwner;
    unsigned			 myStackPtr;
};

#endif
