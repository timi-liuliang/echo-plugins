/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __BV_KDOPTree_h__
#define __BV_KDOPTree_h__

#include "BV_API.h"
#include "BV_Tree.h"
#include <iosfwd>

class UT_IStream;

template <class T>
class UT_Array;

/// A single node of a BV_KDOPTree
template <int K>
class BV_API BV_KDOPNode
{
public:
		 BV_KDOPNode(int leafId = -1);
		 BV_KDOPNode(const BV_KDOPNode &rhs);
		~BV_KDOPNode();

    bool	 isLeaf() const
    {
	UT_ASSERT_P((NULL == myLeft && NULL == myRight && myLeafId >= 0) ||
		    (NULL != myLeft && NULL != myRight && myLeafId < 0));
	return myLeafId >= 0;
    }

    bool         overlaps(const BV_KDOPNode &rhs, fpreal tol) const;

    void	 save(std::ostream &os, bool onlyStructure) const;
    static BV_KDOPNode
                *load(UT_IStream &is, bool onlyStructure);

    int64	 getMemoryUsage() const;

    // TODO: we need a way to disable.

    /// Minimum/maximum extent in each of the K/2 directions.
    fpreal       myExtents[K];
    
    int          myLeafId;
    BV_KDOPNode	*myLeft, *myRight;

private:
    /// Disallowed.
    BV_KDOPNode	&operator=(const BV_KDOPNode &);
};

/// Bounding Volume Tree using k-Discrete Oriented Polytopes.
///
/// Build:          O(n^2)
/// Update extents: O(n log n)
///
/// Defined for:
///   k=6  (axis aligned bounding box)
///   k=14 (AABB with cut corners)
///   k=18 (AABB with cut edges)
///   k=26 (AABB with cut corners and edges).
///
/// Source:
/// Klosowski, Held, Mitchell, Sowizral and Zikan, "Efficient Collision
/// Detection Using Bounding Volume Hierarchies of k-DOPs," IEEE Transactions
/// on Visualization and Computer Graphics 4(1):21-36, 1998.
///   ftp://ams.sunysb.edu/pub/geometry/cd-tvcg-97.ps.gz
template <int K>
class BV_API BV_KDOPTree : public BV_Tree
{
public:
    typedef BV_Tree BaseClass;

			 BV_KDOPTree();
			 BV_KDOPTree(const BV_KDOPTree &);
    virtual		~BV_KDOPTree();

    BV_KDOPTree		&operator=(const BV_KDOPTree &);

    virtual const char	*getType() const;
    virtual const BV_Tree	
			*castTo(const char *type) const;
    virtual BV_Tree	*castTo(const char *type);

    /// This defines the number of "slabs" used to define the polytope.
    static int           getNumSlabs()
                         { return K/2; }
    /// Retrieve the orientation of the k-th slab.
    const UT_Vector3    &getPlaneDir(int k) const
                         {
                             UT_ASSERT_P(k < K/2);
                             return myPlaneDirs[k];
                         }

protected:
    class bvLeaf
    {
    public:
        // Default copy constructor is fine.
        // Default assignment operator is fine.

        // Sort key *must* be first member of this class.
        float	     mySortKey;
        int	     myLeafId;
        UT_Vector3   myBarycenter;
    };

    virtual BV_Tree     *cloneSubclass() const;
    virtual int64	 getMemoryUsageSubclass() const;
    virtual int		 getNumLeavesSubclass() const;
    virtual void	 saveSubclass(std::ostream &os, bool onlyStructure) const;
    virtual bool	 loadSubclass(UT_IStream &is, bool onlyStructure);

    virtual void	 buildSubclass(BV_LeafIterator &leafIt);
    virtual void	 updateExtentsSubclass(BV_LeafIterator &leafIt);
    virtual BV_Status	 intersectSubclass(BV_Callback &callback,
				    const BV_Tree &treeb,
				    const UT_DMatrix4 &startxforma, 
				    const UT_DMatrix4 &startxformb,
				    const UT_DMatrix4 &endxforma, 
				    const UT_DMatrix4 &endxformb,
				    fpreal tol) const;
    static bool          intersectRecurse(BV_Callback &callback,
                                     const BV_KDOPNode<K> &nodea,
                                     const BV_KDOPNode<K> &nodeb,
                                     fpreal tol);

    static BV_KDOPNode<K>*buildRecurse(UT_Array<bvLeaf> &leafData,
                                      int startLeaf, int numLeaves);
    static void          updateExtentsRecurse(BV_LeafIterator &leafIt,
                                              BV_KDOPNode<K> &node);

    static const UT_Vector3  myPlaneDirs[K/2];

protected:
    const BV_KDOPNode<K>*getRoot() const
			 { return myRoot; }

private:
    BV_KDOPNode<K>	*myRoot;
};

typedef BV_KDOPTree<6>   BV_AABBTree;
typedef BV_KDOPTree<14>  BV_14DOPTree;
typedef BV_KDOPTree<18>  BV_18DOPTree;
typedef BV_KDOPTree<26>  BV_26DOPTree;

#endif
