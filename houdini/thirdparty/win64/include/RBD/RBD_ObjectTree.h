/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RBD_ObjectTree.h ( RBD Library, C++)
 *
 * COMMENTS: 
 *	This provides an implementation independent way to
 *	do quick culling of RBD_Objects
 */

#ifndef __RBD_ObjectTree_H__
#define __RBD_ObjectTree_H__

#include "RBD_API.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <GEO/GEO_PointTree.h>

class RBD_Object;
class BV_OBBTree;

///
/// RBD_ObjectTree is the consistent interface for object culling trees.
///
class RBD_API RBD_ObjectTree
{
public:
		 RBD_ObjectTree() {}
    virtual	~RBD_ObjectTree() {}

    void	 appendObject(RBD_Object *obj, int idx);
    void	 findOverlapIdx(RBD_Object *obj, UT_IntArray &overlap);
    void	 setDirty();
    
protected:
    virtual void appendObjectSubclass(RBD_Object *obj, int idx) = 0;
    virtual void findOverlapIdxSubclass(RBD_Object *obj,
					UT_IntArray &overlap) = 0;
    virtual void setDirtySubclass() = 0;
};

///
/// RBD_ObjectTreeSphere treats each object like a sphere and acts accordingly.
///
class RBD_API RBD_ObjectTreeSphere : public RBD_ObjectTree
{
public:
		 RBD_ObjectTreeSphere();
    virtual	~RBD_ObjectTreeSphere();

protected:
    virtual void appendObjectSubclass(RBD_Object *obj, int idx);
    virtual void findOverlapIdxSubclass(RBD_Object *obj,
					UT_IntArray &overlap);
    virtual void setDirtySubclass();

private:
    void			 rebuildTree();		

    UT_ValArray<RBD_Object *>	 myObjList;
    UT_IntArray			 myInfiniteObjIdx;
    UT_IntArray			 myIndices;
    GEO_PointTreeInt		 myPointTree;
    bool			 myDirty;
};

///
/// RBD_ObjectTreeOBB builds a BV_OBBTree to manage the tree.
///
class RBD_API RBD_ObjectTreeOBB : public RBD_ObjectTree
{
public:
		 RBD_ObjectTreeOBB();
    virtual	~RBD_ObjectTreeOBB();

protected:
    virtual void appendObjectSubclass(RBD_Object *obj, int idx);
    virtual void findOverlapIdxSubclass(RBD_Object *obj,
					UT_IntArray &overlap);
    virtual void setDirtySubclass();

private:
    void			 rebuildTree();		
    
    UT_ValArray<RBD_Object *>	 myObjList;
    UT_IntArray			 myInfiniteObjIdx;
    UT_IntArray			 myIndices;
    bool			 myDirty;
    BV_OBBTree			*myOBBTree;
};

///
/// This is a no-op tree.  All objects are returned as possilbe
/// overlaps.   (Useful verifying behaviour)
///
class RBD_ObjectTreeNone : public RBD_ObjectTree
{
public:
		 RBD_ObjectTreeNone();
    virtual	~RBD_ObjectTreeNone();

protected:
    virtual void appendObjectSubclass(RBD_Object *obj, int idx);
    virtual void findOverlapIdxSubclass(RBD_Object *obj,
					UT_IntArray &overlap);
    virtual void setDirtySubclass();

private:  
    UT_IntArray		myIndices;
};

#endif
