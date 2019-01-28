/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __BV_OBBTree_h__
#define __BV_OBBTree_h__

#include "BV_API.h"
#include "BV_Tree.h"
#include <UT/UT_Array.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_LinkListT.h> 
#include <UT/UT_Matrix3.h>
#include <UT/UT_Vector3.h>

#include <set>
#include <map>

class UT_IStream;
struct bvTransform;
class BV_OBB_Extra;

// Set to 1 to allow debug output statements.
#define CONVEX_HULL_DEBUG   0

/**********************************************************************************************************************************/
class BV_CHTriangle;
class BV_CHEdgeInfo; 

// A linked list holding all temporary triangles created while building the hull
typedef UT_Array<UT_Vector3> TPointArray;
typedef UT_Array<int> TPointIndexArray;
typedef UT_Array<fpreal> TFloatArray;
typedef UT_LinkListT<BV_CHTriangle*> TTriArray;
/**********************************************************************************************************************************/
// Temporary structure to get the triangles out of the convex hull algorithm.
// Should probably be replaced with something better later on.
struct BV_CHSimpleTriangle
{
    UT_Vector3 myVertices[3];
};
typedef UT_Array<BV_CHSimpleTriangle> TSimpleTriangleArray;
/**********************************************************************************************************************************/
// A helper structure that holds edge information as the algorithm is running.
class BV_CHEdgeInfo
{
public:
    BV_CHEdgeInfo();

    void reset(void);

    bool operator()(const BV_CHEdgeInfo& s1, const BV_CHEdgeInfo& s2) const;

    uint myV1, myV2;
    int myUsedCount;

    // Only relevant when myUsedCount is one. If true, the edge remaining
    // in use goes from myV1 to myV2, opposite if false.
    // Not valid for myUsedCount == 2 (both are used), or myUsedCount == 0
    // (none are used).
    bool myIsV1Starting;
};
typedef std::set<BV_CHEdgeInfo,BV_CHEdgeInfo> TEdgeInfoSet;
/**********************************************************************************************************************************/
class BV_CHPointInfo
{
public:
    BV_CHPointInfo();
    BV_CHPointInfo(const UT_Vector3 point_in);

    /// Accumulated the point position
    void addPoint(const UT_Vector3 point_in);

    /// Returns an average point position accumulated so far
    UT_Vector3 getAveragePoint(void) const;

    bool operator()(const UT_Vector3& s1, const UT_Vector3& s2) const;

private:
    UT_Vector3 myAccumPosition;
    int myAccumCount;
};
typedef std::map<UT_Vector3, BV_CHPointInfo, BV_CHPointInfo> TPointInfoMap;
/**********************************************************************************************************************************/
class BV_CHDataManager
{
public:

    /// Constructs the manager, welds input points.
    BV_CHDataManager(const TPointArray& points_in);

    /// Find an edge between two vertices
    const BV_CHEdgeInfo* findEdge(int curr_v1_idx, int curr_v2_idx) const;

    /// Traverse edges
    const BV_CHEdgeInfo* edgeTraversalBegin(void);
    const BV_CHEdgeInfo* edgeTraversalNext(void);

    /// Checks for valid edge counts. Debug.
    void edgeSanityCheck(void);

    /// Sets a flag telling the manager we will do a reset next,
    /// so that no triangle-edge management code is called to save time.
    void setAboutToReset(void);

    /// Resets the manager for the next iteration.
    void reset(void);

    /// Welds points from the points_in array and puts the new set of points
    /// into the merged_pts_out array. Currently welds within FLT_EPSILON.
    void weldPoints(const TPointArray& points_in, TPointArray& merged_pts_out);

    TPointArray& getPoints(void) 
	{ return myPoints; }
    bool getDoRestart(void) const
	{ return myDoRestart; }

    /// Called every time a triangle is created.
    void onTriangleDeleted(BV_CHTriangle* triangle_in);

    /// Called every time a triangle is deleted or is marked as deleted.
    void onTriangleCreated(BV_CHTriangle* triangle_in);

    /// Jiggles a point to lie more than FLT_EPSILON away from the triangle.
    fpreal jigglePointTetra(BV_CHTriangle *coplanar_triangle, int iPointIndex);

private:

    // A copy of all points
    TPointArray myPoints;

    // A set of all edges currently in existence.
    TEdgeInfoSet myEdges;
    TEdgeInfoSet::iterator myTraversalIt;

    bool myDoRestart;
    bool myIsAboutToReset;

    // Multiplier array, per point. Not cleared in reset().
    TFloatArray myPointJiggleMults;

#if CONVEX_HULL_DEBUG
    int myDBJigglePath;
#endif

};
/**********************************************************************************************************************************/
class BV_CHTriangle
{
public:

    BV_CHTriangle(BV_CHDataManager& parent_manager, uint point1_idx,uint point2_idx,uint point3_idx);
    ~BV_CHTriangle();

    /// Marks the triangle as deleted. If bCallManager is true, calls CCHDataManager::onTriangleDeleted()
    /// to manage related edges.
    void markDeleted(bool do_call_manager = true);

    /// Gets signed distance from the given point to the triangle
    inline fpreal getSignedDistance(const uint point_index);

    /// Checks whether this triangle can see the given point or not.
    bool canSee(const uint point_index);

    /// Associates all points that this triangle can see that are still not associated
    /// from the points_in array. If do_ignore_restart_flag is true, keeps on going even
    /// if a restart becomes necessary.
    void associatePoints(TPointIndexArray& points_in, bool do_ignore_restart_flag);

    /// Returns true if this is a "bad" triangle, false otherwise.
    bool getIsBad(void) const;

    /// Clears all points associated with this triangle.
    void clearAssocPoints(void);

    /// Find the most distant point associated with this triangle.
    bool findMostDistantAssocPoint(uint& point_idx_out);

    /// Gets triangle normal. Cached.
    void getNormal(UT_Vector3 &normal);

    /// Appends this triangle's associated points to rPointsOut array.
    void getAssocPoints(TPointIndexArray& points_idx_out);
    void printAssocPoints(void);

    bool getIsDeleted(void) const
	{ return myIsDeleted; }
    inline uint getVertex(int index)  const
	{ return myVertices[index]; }

private:
    BV_CHDataManager *myManager;
    uint myVertices[3];
    bool myIsDeleted;

    UT_Vector3 myCachedNormal;
    bool myIsNormalValid;

    // An array of associated points
    TPointIndexArray myAssocPoints;

    TPointArray& myAllPoints;
};
/**********************************************************************************************************************************/

/// A single node in a BV_OBBTree
class BV_API BV_OBB
{
public:
		 BV_OBB(int startLeaf, int numLeaves);
		 BV_OBB(const BV_OBB &);
		~BV_OBB();

    bool	 isLeaf() const;

    void	   save(std::ostream &os, bool onlyStructure) const;
    static BV_OBB *load(UT_IStream &is, bool onlyStructure);

    int64	 getMemoryUsage() const;

    /// Hackish way of determining if a leaf is disabled or not.
    bool	 isDisabled() const
		 { return mySize < 0; }


    /// Orientation of the bounding box, relative to parent
    UT_Matrix3	 myRot;
    /// Center, relative to parent.
    UT_Vector3	 myTrans;
    
    /// Radii in each axis (half of the dimension)
    UT_Vector3	 myRadii;
    /// Length of myRadii.
    /// Special value of "-1" is used to signal that a node is disabled.
    fpreal	 mySize;

    /// Index of first leaf.
    int		 myStartLeaf;
    /// How many leaves I own.
    int		 myNumLeaves;

    BV_OBB	*myLeft, *myRight;

    BV_OBB_Extra*myExtra;

    void        *myUserData;
    
private:
    /// Disallowed.
    BV_OBB	&operator=(const BV_OBB &);
};

/// Bounding volume hierarchy based on Oriented Bounding Boxes (OBBs).
///
/// Build:          O(n^2)
/// Update extents: O(n^2)

// Source:
// Gottschalk, Lin and Manocha, "OBB-Tree: A Hierarchical Structure for
// Rapid Interference Detection," Proceedings of ACM SIGGRAPH 1996, pages
// 171-180. ACM Press, 1996.
//   ftp://ftp.cs.unc.edu/pub/users/gottscha/obbt.ps.gz
class BV_API BV_OBBTree : public BV_Tree
{
public:
    typedef BV_Tree BaseClass;

			 BV_OBBTree();
			 BV_OBBTree(const BV_OBBTree &);
    virtual		~BV_OBBTree();

    BV_OBBTree		&operator=(const BV_OBBTree &);

    virtual const char	*getType() const;
    virtual const BV_Tree	
			*castTo(const char *type) const;
    virtual BV_Tree	*castTo(const char *type);

    /// Alternative lazy building: sections of the tree are only built when a
    /// query needs them. In this case, the leaf iterator *must* not be
    /// destroyed until the tree is destroyed, or until a fresh call to
    /// buildLazy.
    void		 buildLazy(BV_LeafIterator &leafIt)
			 {
			     buildInternal(leafIt, true);
			 }

    void		 getRootOBB(UT_Matrix4 &xform, UT_Vector3 &radii) const;

    /// If true, a somewhat different version of the algorithm is used to compute
    ///	OBBs around nodes, which uses a convex hull around the points instead of
    /// the points themselves. While slower, it results in a consistent bbox 
    /// that is not influenced by the density/inner points on the mesh. False by default.
    void		 setUseConvexHull(bool bValue);
    bool		 getUseConvexHull(void);
    
protected:
    class bvLeaf
    {
    public:
        // Default copy constructor is fine.
        // Default assignment operator is fine.

        // Sort key *must* be first member of this class.
        float	     sortkey;
        int	     leafId;
        UT_Vector3   barycenter;
    };

    virtual BV_Tree     *cloneSubclass() const;
    virtual int64	 getMemoryUsageSubclass() const;
    virtual int		 getNumLeavesSubclass() const;
    /// Saving and loading only works for non-lazy building.
    // @{
    virtual void	 saveSubclass(std::ostream &os, bool onlyStructure) const;
    virtual bool	 loadSubclass(UT_IStream &is, bool onlyStructure);
    // @}

    virtual void	 buildSubclass(BV_LeafIterator &leafIt) 
			 {
			     buildInternal(leafIt, false);
			 }
    virtual void	 updateExtentsSubclass(BV_LeafIterator &leafIt);
    /// WARNING: not thread-safe if tree was lazily built!
    virtual BV_Status	 intersectSubclass(BV_Callback &callback,
				    const BV_Tree &treeb,
				    const UT_DMatrix4 &startxforma, 
				    const UT_DMatrix4 &startxformb,
				    const UT_DMatrix4 &endxforma, 
				    const UT_DMatrix4 &endxformb,
				    fpreal tol) const;

    BV_OBB		*getRoot()
	    		 {
			     return myRoot;
			 }
    void		 buildInternal(BV_LeafIterator &leafIt, bool lazy);
    /// Build the tree. If depth is non-negative, the algorithm is lazy and
    /// stops after depth levels.
    BV_OBB		*createTree(int startprim, int numleaves,
				    const UT_Matrix3 &rootBasis,
				    const UT_Vector3 &rootPos, int depth = -1);
    void		 createChildren(BV_OBB &root, int depth = -1);
    bool		 updateExtentsRecurse(BV_OBB &node,
				    const UT_Matrix3 &parentBasis,
				    const UT_Vector3 &parentPos);
    
    void		 getBounds(const UT_Matrix3 &basis,
				    UT_BoundingBox &bbox, bool &firstPrim);
    /// Calculate the OBB associated with the given node. Stores
    /// orientation in node->myRot, and calls calcSize to calculate the
    /// extents. May disable the node if it contains no enabled leaves.
    void		 calcOBB(BV_OBB &node);
    /// Given the input OBB orientation, calculate the OBB extents.
    /// Stores extents (radii) and position in node.
    void		 calcSize(BV_OBB &node);

    /// Compute a convex hull around the points provided, output result as a series of triangles
    /// to the array provided. This is currently slower than it should be. Also, note that
    /// if coplanar points are present, the convex hull constructed might be around jiggled versions
    /// of those points.
    bool		 computeConvexHull(const TPointArray& source_points_in, TSimpleTriangleArray& final_triangles_out);
    bool		 doConvexHullIteration(BV_CHDataManager& data_manager, TTriArray& working_triangles);
    
    bool		 intersectRecurse(BV_Callback &callback,
			     const BV_OBBTree &treeb,
			     const BV_OBB &a, const BV_OBB &b,
			     const bvTransform &t1, const bvTransform &t2,
			     const bvTransform &t3, const bvTransform &t4,
			     fpreal tol, int identities) const;


    static int		 compareLeaves(const void *t1, const void *t2);

    void		 calculateSortKeys(bvLeaf *prims, int numleaves,
				    UT_Matrix3 &basis, int axis) const;

    BV_OBB		*myRoot;
    UT_Array<bvLeaf>	 myLeaves;
    BV_LeafIterator	*myLeafIt;

    bool myUseConvexHullForBBoxes;
};

/// Extra data for a lazily constructed BV_OBB node.
class BV_OBB_Extra
{
public:
    UT_Matrix3		rot;
    UT_Vector3		trans;
};


struct BV_API bvTransform
{
    /// relative rotation
    UT_Matrix3		R;
    /// relative translation
    UT_Vector3		T;
    /// relative transformation
    UT_DMatrix4		xform[2];
    /// create a new bvTransform that has updated relative rotation/translation
    /// for the left tree (tree a)
    void		updateL(const UT_Matrix3 &rot, 
				const UT_Vector3 &trans,
				bvTransform &xform) const;
    /// same as updateL, except for the right tree (tree b)
    void		updateR(const UT_Matrix3 &rot, 
				const UT_Vector3 &trans,
				bvTransform &xform) const;
    /// update myself given (xforma, xformc) and (xformb, xformb) with
    /// left tree (ra, ta) and right tree (rb, tb)
    void			update(const UT_Matrix3 &ra,
			        const UT_Matrix3 &rb,
				const UT_Vector3 &ta,
				const UT_Vector3 &tb,
				const UT_DMatrix4 &xforma,
				const UT_DMatrix4 &xformb,
				const UT_DMatrix4 &xformc,
				const UT_DMatrix4 &xformd);
};


#endif
