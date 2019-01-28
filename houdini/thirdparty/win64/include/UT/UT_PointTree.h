/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PointTree.h (C++)
 *
 * COMMENTS:
 *	THIS CLASS IS DEPRECATED.  
 *	You probably want to use GEO_PointTree.
 *
 *	This implements an octree variant designed for sorting
 * points in space for easy access.
 *	It does not support the addition of points in a piecemeal
 * fashion, they must all be given at once.
 *
 */

#ifndef __UT_PointTree_h__
#define __UT_PointTree_h__

#include "UT_API.h"
#include "UT_BoundingBox.h"
#include "UT_Array.h"
#include "UT_FloatArray.h"

typedef int (*UT_PointTreeValidNearestFunc)(void *data, void *userdata);

class ut_PointTreeQueue;

class utPointNode;
#include "UT_VectorTypes.h"

class UT_API UT_PointTree 
{
public:
    // Constructors/destructors:
    UT_PointTree();
    virtual		~UT_PointTree(void);

    
    // Builds (And possibly rebuilds) the tree with the given data:
    // Max size is the maximum size of a leaf.
    int			 build(const UT_Array<void *> &data,
			       const UT_Vector3Array	 &points,
			       int maxsize = 5);

    // Finds the nearest point not more than maxdist away.  If there
    // is none, returns null.
    void		*findNearest(const UT_Vector3 &pt, float maxdist,
				    UT_PointTreeValidNearestFunc = NULL,
				    void *user_data = NULL);

    // Finds the nearest [groupsize] points not more than maxdist away.
    // Returns the number of points found and stores the points in group.
    // The returned groupdist2 array returns their squared distances.
    void		 findNearestGroup(const UT_Vector3 &pt, float maxdist,
					    int groupsize,
					    UT_Array<void *> &group,
					    UT_FloatArray &groupdist2,
					    UT_PointTreeValidNearestFunc = NULL,
					    void *user_data = NULL);

    // Find all points within the specified distance
    int			 findAllClose(const UT_Vector3 &pt, float maxdist,
				    UT_Array<void *> &data);

    // Finds all the points within a specified tube:
    int			 findAllInTube(const UT_Vector3 &orig, 
				    const UT_Vector3 &dir, fpreal rad,
				    UT_Array<void *> &data);

    void		 getBBox(UT_BoundingBox &box) const { box = myBBox; }

    // Returns 1 if the tree is properly constructed.
    int			 verifyTree() const;

    void		 destroyTree();

protected:
    // Recursively destroyes the tree.
    void		 destroyNode(utPointNode *node);

    // Verifies a node.
    int			 verifyNode(UT_BoundingBox &box, utPointNode *node) 
				const;

    // Merely counts the number of points that will land in each
    // bucket.
    void		 testAxis(const UT_BoundingBox &box, int axis,
				 const UT_Vector3Array	 &points,
				 int &splitleft, int &splitright);

    // Actually divides the points/data into two lists, placed
    // into *left & *right
    void		 splitOnAxis(const UT_BoundingBox &box, int axis,
				     const UT_Array<void *> &data,
				     const UT_Vector3Array &points,
				     UT_Array<void *> &dataleft,
				     UT_Vector3Array &pointsleft,
				     UT_Array<void *> &dataright,
				     UT_Vector3Array &pointsright);

    utPointNode		*buildChildNode(const UT_BoundingBox &box,
				    const UT_Array<void *> &data,
				    const UT_Vector3Array &points,
				    int inbboxnode = 0);

    void 		*findNearestNode(UT_BoundingBox &box, 
				    utPointNode *node, const UT_Vector3 &pt,
				    float &maxdist,
				    UT_PointTreeValidNearestFunc = NULL,
				    void *user_data = NULL);
    void		 findNearestNodeGroup(UT_BoundingBox &box,
				    utPointNode *node, const UT_Vector3 &pt,
				    ut_PointTreeQueue &q,
				    UT_PointTreeValidNearestFunc = NULL,
				    void *user_data = NULL);

    int 		 findCloseNodes(UT_BoundingBox &box, 
				    utPointNode *node, const UT_Vector3 &pt,
				    float &maxdist,
				    UT_Array<void *> &data);

    int			 findInTube(UT_BoundingBox &box,
				    utPointNode *node, const UT_Vector3 &orig,
				    const UT_Vector3 &dir, 
				    fpreal radius, fpreal radius2,
				    UT_Array<void *> &data);
    
    utPointNode		*myRoot;
    UT_BoundingBox	 myBBox;
    int			 myMaxSize;
};

#endif
