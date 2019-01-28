/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_RTree.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_RTreeGeneric__
#define __UT_RTreeGeneric__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_BoundingBox.h"
#include "UT_Vector3.h"
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <vector>
#include <limits>

// UT_BoxT represents either an axis-aligned box or the empty set.
// UT_BoxT is closed as a set; all operations assume that 
// the boundary is included.
template< typename T >
class UT_BoxT
{
public:
    typedef T Scalar;

    // Default construct: the empty set
    UT_BoxT();

    // Construct box containing a single point position,
    // which is represented by its array of three coordinates.
    template< typename U >
    explicit UT_BoxT(const U p[3]);
    template< typename U >
    explicit UT_BoxT(UT_Vector3T<U> p);

    explicit UT_BoxT(const UT_BoundingBox &bbox);

    // Return whether this represents the empty set
    bool isEmpty() const;

    // Get minimum coordinate for given standard axis
    // Assumes this is not empty!
    T getMin(const int c) const;

    // Get maximum coordinate for given standard axis
    // Assumes this is not empty!
    T getMax(const int c) const;

    UT_Vector3T<T> getMin() const
    { return UT_Vector3T<T>(myMin[0], myMin[1], myMin[2]); }
    UT_Vector3T<T> getMax() const
    { return UT_Vector3T<T>(myMax[0], myMax[1], myMax[2]); }

    UT_Vector3T<T> getCenter() const
    {
	return UT_Vector3T<T>((myMin[0]+myMax[0])/2,
			      (myMin[1]+myMax[1])/2,
			      (myMin[2]+myMax[2])/2);
    }
    UT_Vector3T<T> getSize() const
    {
	return UT_Vector3T<T>((myMax[0]-myMin[0]),
			      (myMax[1]-myMin[1]),
			      (myMax[2]-myMin[2]));
    }
    T		   getRadius2() const
    {
	UT_Vector3T<T> size = getSize();

	size *= 0.5;
	return size.length2();
    }
    T		  getRadius() const
    {
	return SYSsqrt(getRadius2());
    }

    // Make a box that's a single point
    template< typename U >
    void assignPoint(const U p[3]);
    template< typename U >
    void assignPoint(UT_Vector3T<U> p) { assignPoint(p.vec); }

    // Expand the box just enough so that it contains a point p
    template< typename U >
    void absorbPoint(const U p[3]);
    template< typename U >
    void absorbPoint(UT_Vector3T<U> p) { absorbPoint(p.vec); }

    // Expand the box just enough so that it contains a box b
    void absorbBox(const UT_BoxT<T>& b);
    void absorbBox(const UT_BoundingBox &b);

    // Expand in the directions of the axes:
    // In case this is the empty set, the result is still the empty set
    // This is equivalent to making *this the union of all cubes with
    // radius l placed at points of the original set.
    void expandDistance(const T l);

    // As above but expand distance in a specific axis
    void expandDistance(const T l, int axis);

    // Return whether a point is contained in the closed set
    bool contains(const T*const p) const;

    // Return whether the closed set *this intersects the closed set b
    bool intersects(const UT_BoxT<T>& b) const;

    // Return the axis in which the box is the widest.
    // For the empty set, this always returns axis 0
    int getLargestAxis() const;

private:
    // This represents set of all points x
    // with myMin[c] <= x[c] <= myMax[c] for each c = 0, 1, 2
    T myMin[3];
    T myMax[3];
};

// UT_VelBoxT represents an axis-aligned box in motion.
// UT_BoxT is closed as a set; all operations assume that 
// the boundary is included.
template< typename T >
class UT_VelBoxT
{
public:
    typedef T Scalar;

    // Default construct: the empty set
    UT_VelBoxT();

    // Constructs a stationary box of the given dimensions
    explicit UT_VelBoxT(const UT_BoundingBox &bbox);

    // Constructs a moving box moving from one location to the other
    UT_VelBoxT(const UT_BoundingBox &start,
		     const UT_BoundingBox &end);

    // Return whether this represents the empty set
    bool isEmpty() const;

    // Get minimum coordinate for given standard axis
    // Assumes this is not empty!
    // Returns total size of start & end.
    T getMin(const int c) const;

    // Get maximum coordinate for given standard axis
    // Assumes this is not empty!
    T getMax(const int c) const;

    // Get theh speed the box is moving in this coordinate.
    T getVel(const int c) const;

    // Expand the box just enough so that it contains a box b
    void absorbBox(const UT_VelBoxT<T>& b);

    // Return whether the closed set *this intersects the closed set b
    bool intersects(const UT_VelBoxT<T>& b) const;

    // Return the axis in which the box is the widest.
    // For the empty set, this always returns axis 0
    int getLargestAxis() const;

private:
    // This represents set of all points x
    // with myMin[c] <= x[c] <= myMax[c] for each c = 0, 1, 2
    // Because we commonly perform isect tests, and those should early
    // exit, we rotate the storage so
    // x() - min
    // y() - max
    // z() - vel
    UT_Vector3T<T>	myBounds[3];
};



// UT_Sphere represents a simple sphere or the empty set.
template< typename T >
class UT_SphereT
{
public:
    typedef T Scalar;

    // The constructor must take a centre point and a radius

    template< typename U >
    explicit UT_SphereT(const U p[3], const U r) 
	: myRadius(r)
    {
	myCentre[0] = p[0];
	myCentre[1] = p[1];
	myCentre[2] = p[2];
    };

    // Return whether the closed set *this intersects the closed set b
    bool intersects(const UT_BoxT<T>& b) const;

private:
    T myCentre[3];
    T myRadius;
};

template< int MAX_ORDER >
class UT_RTreeGeneric;

// This is an assigment of a box to each item stored in the R-tree.
// It is passed an argument to queries on the R-tree.
// This way, an R-tree can be re-used efficiently if the item boxes change.
// This will only remain efficient as long as the configuration of boxes
// doesn't change dramatically.
template< typename BOXTYPE>
class UT_RTreeAssignmentT
{
public:
    UT_RTreeAssignmentT() {}
    ~UT_RTreeAssignmentT() {}

    exint		getMemoryUsage() const
    {
	// Do not include ourselves.
	return myBoxesForSharedNodes.capacity() * sizeof(BOXTYPE);
    }

private:
    typedef BOXTYPE Box;
    std::vector<BOXTYPE> myBoxesForSharedNodes;

    template< int MAX_ORDER >
    friend class UT_RTreeGeneric;
};


template< typename T >
using UT_RTreeBoxAssignmentT = UT_RTreeAssignmentT<UT_BoxT<T> >;

template< int MAX_ORDER >
class UT_RNode;

// The order of the R-Tree is the number of children that each
// internal node in the tree has. Picking a higher value of
// MAX_ORDER will reduce the height of the R-tree.
// R-tree stores a containment structure for the boxes that are
// passed to its contructor, but doesn't store any actual boxes.
// That's why UT_RTreeGeneric doesn't take the number type T as a template parameter.
template< int MAX_ORDER >
class UT_RTreeGeneric
{
public:
    // Construct R-tree that contains the range of items [0, size),
    // During the construction, item_boxes[i] is used as the bounding box
    // for item #i. These boxes determine the initial structure of the R-tree.
    // However, the box positions and dimensions are not stored in the R-tree.
    template< typename BOXTYPE >
    UT_RTreeGeneric(const BOXTYPE item_boxes[], const int size);

    ~UT_RTreeGeneric();

    // The number of items stored in the tree
    int getNumItems() const;

    exint		getMemoryUsage() const
    {
	return sizeof(*this) + sizeof(Node) * mySharedNodesSize;
    }

    // Create an assignment of a box to each item in the tree.
    // That is, item i is assigned box item_boxes[i].
    // This box assignment can be used in intersection queries.
    template< typename T >
    void createBoxAssignment(
	UT_RTreeBoxAssignmentT<T>& assignment, 
	const UT_BoxT<T> item_boxes[]
    ) const
    {
	createAssignment<UT_BoxT<T> >(assignment, item_boxes);
    }

    template< typename BOXTYPE>
    void createAssignment(
	UT_RTreeAssignmentT<BOXTYPE>& assignment, 
	const BOXTYPE item_boxes[]
    ) const;

    /// For each item i for which item_boxes[i] intersects query_box,
    /// call result_acceptor(i).
    /// This assumes that RESULT_ACCEPTOR is a function or a functor.
    /// A further generlaization of getIntersectingItems that takes
    /// a queryshape.  It is assumed to have an .intersects() method
    /// which takes a UT_Box<T>
    /// It is very important that the intersects() method will
    /// return false for empty nodes.
    template< typename QUERY_SHAPE, typename RESULT_ACCEPTOR, typename BOXTYPE >
    void getIntersectingItems(
        RESULT_ACCEPTOR& result_acceptor,
	const QUERY_SHAPE& query_box, 
	const UT_RTreeAssignmentT<BOXTYPE>& assignment
    ) const;

    template< typename QUERY_SHAPE, typename RESULT_ACCEPTOR, typename BOXTYPE, int BATCH_SIZE >
    void getIntersectingItemsBatch(
        RESULT_ACCEPTOR &batch_result_acceptor,
	const QUERY_SHAPE *query_box, 
	const UT_RTreeAssignmentT<BOXTYPE>& assignment
    ) const;

    template< typename BOXTYPE >
    BOXTYPE boundingBox(const UT_RTreeAssignmentT<BOXTYPE>& assignment) const;

    /// Returns the number of nodes in the tree.
    /// NOTE: This does not count the items themselves as nodes,
    ///       because they don't have nodes allocated for them.
    int getNumNodes() const
    { return mySharedNodesSize; }

    /// For each node, this effectively does:
    /// LOCAL_DATA local_data[MAX_ORDER];
    /// bool descend = pre_functor(nodei, parent_data);
    /// if (!descend)
    ///     return;
    /// for each child {
    ///     if (isitem(child))
    ///         item_functor(getitemi(child), nodei, local_data[child]);
    ///     else if (isnode(child))
    ///         recurse(getnodei(child), local_data);
    /// }
    /// post_functor(nodei, parent_nodei, data_for_parent, num_children, local_data);
    template<typename LOCAL_DATA,typename PRE_FUNCTOR,typename ITEM_FUNCTOR,typename POST_FUNCTOR>
    void traverse(
        PRE_FUNCTOR &pre_functor,
        ITEM_FUNCTOR &item_functor,
        POST_FUNCTOR &post_functor,
        LOCAL_DATA *data_for_parent=nullptr) const;

private:
    template<typename LOCAL_DATA,typename PRE_FUNCTOR,typename ITEM_FUNCTOR,typename POST_FUNCTOR>
    void traverseHelper(
        int nodei,
        int parent_nodei,
        PRE_FUNCTOR &pre_functor,
        ITEM_FUNCTOR &item_functor,
        POST_FUNCTOR &post_functor,
        LOCAL_DATA *data_for_parent=nullptr) const;

private:
    typedef UT_RNode<MAX_ORDER> Node;

    int mySharedNodesSize;
    Node* mySharedNodes;
    Node* myRoot;
    int myNumItems;

    // Disallow
    UT_RTreeGeneric();
    UT_RTreeGeneric(const UT_RTreeGeneric&);
    UT_RTreeGeneric& operator=(const UT_RTreeGeneric&);
};

//
// Helpers for storing query results in containers
//

// On return, "results" consists of all items i for which item_boxes[i] 
// intersects query_box.
// The contents of "results" from before the call are cleared, so it
// is not neccessary to call results.clear() before calling this function.
template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE >
inline void UTgetIntersectingItems(
    UT_Array<int>& results,
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE& query_box, 
    const UT_RTreeAssignmentT<BOXTYPE>& assignment
);

// Similar to UTgetIntersectingItems, but does not invoke clear()
// allowing you to join multiple searches together.
// The new items will all have baseindex added to them.
template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE >
inline void UTappendIntersectingItems(
    UT_Array<int>& results,
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE& query_box, 
    const UT_RTreeAssignmentT<BOXTYPE>& assignment,
    exint baseindex
);

// Does not clear the various isect lists.  Will simultaneously
// walk the tree for all the batched values.
template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE, int BATCHSIZE >
inline void UTappendIntersectingItemsBatch(
    UT_Array<int> *results,
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE *query_box, 
    const UT_RTreeAssignmentT<BOXTYPE>& assignment
);


// Return all items i for which item_boxes[i] intersects query_box.
// The resulting items will be stored in the range [items_begin, items_end),
// where items_end is the return value.
// It is assumed that the array starting at items_begin is large enough
// to contain the results (at most the total number of items).
template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE >
inline int* UTgetIntersectingItems(
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE& query_box, 
    const UT_RTreeAssignmentT<BOXTYPE>& assignment,
    int*const items_begin
);

#if defined( WIN32 ) || defined( LINUX ) || defined( MBSD ) || defined(GAMEOS)
    #include "UT_RTree.C"
#endif

// R-tree boxes for various float types
typedef UT_BoxT<fpreal32> UT_BoxF;
typedef UT_BoxT<fpreal64> UT_BoxD;
typedef UT_BoxT<fpreal64> UT_Box;

// R-tree spheres for float types
typedef UT_SphereT<fpreal32> UT_SphereF;
typedef UT_SphereT<fpreal64> UT_SphereD;
typedef UT_SphereT<fpreal64> UT_Sphere;

// R-tree box assignments for various float types
typedef UT_RTreeBoxAssignmentT<fpreal32> UT_RTreeBoxAssignmentF;
typedef UT_RTreeBoxAssignmentT<fpreal64> UT_RTreeBoxAssignmentD;
typedef UT_RTreeBoxAssignmentT<fpreal64> UT_RTreeBoxAssignment;

// Tree of various orders
typedef UT_RTreeGeneric<2> UT_RTree2;
typedef UT_RTreeGeneric<16> UT_RTree16;
typedef UT_RTreeGeneric<2> UT_RTree;

#endif

