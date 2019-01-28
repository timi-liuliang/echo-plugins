/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SpatialTree.h (UT Library, C++)
 *
 * COMMENTS:
 * 	An n-dimensional tree implementation (e.g. quadtree, octree, etc.)
 * 	that stores arbitrary data and supports arbitrary bounding regions
 * 	and subdivision strategies.
 *
 *	A bounding region provider must implement getDistance2(), getMin(),
 *	and getMax() as follows:
 *
 *	/// Returns the distance (squared) from the bounding region
 *	/// corresponding to the supplied object to the specified point.
 *	fpreal getDistance2(T object, const fpreal *pt) const;
 *
 *	/// Returns the maximum coordinates of the bounding box for the
 *	/// bounding region corresponding to the supplied object.  The
 *	/// supplied buffer may be used to return the result if neccessary.
 *	/// Its size is equal to sizeof(fpreal) * ndimensions, where
 *	/// ndimensions is the dimension of the tree.
 *	const fpreal *getMax(T object, fpreal *buf) const;
 *
 *	/// Returns the mnimum coordinates of the bounding box for the
 *	/// bounding region corresponding to the supplied object.  The
 *	/// supplied buffer may be used to return the result if neccessary.
 *	/// Its size is equal to sizeof(fpreal) * ndimensions, where
 *	/// ndimensions is the dimension of the tree.
 *	const fpreal *getMin(T object, fpreal *buf) const;
 *
 * 	A subdivision strategy must implement subdivide() as follows:
 *
 *	/// Returns true if the node at the specified depth with the
 *	/// specified bounds (min/max), containing the specified
 *	/// objects, whose bounds are given, should be subdivided.
 * 	template<class T, class B>
 * 	bool subdivide(
 *		const B *bounds_provider,
 *		const fpreal *min,
 *		const fpreal *max,
 * 		const UT_ValArray<T> &objects,
 * 		int depth,
 *		int ndimensions);
 */

#ifndef __UT_SpatialTree__
#define __UT_SpatialTree__

#include "UT_API.h"

#include "UT_RingBuffer.h"
#include "UT_SmallObject.h"
#include "UT_StackAlloc.h"
#include "UT_ValArray.h"

#include <iostream>
#include <string.h>
#include <stack>

#define MIN_NODE_SIZE	1e-3

class UT_API UT_STAccepter
{
public:
		 UT_STAccepter(bool accept) : myAccept(accept) {}
    bool	 accept(int objectno) const { return myAccept; }

private:
    bool	 myAccept;
};

template<class T, class S, class B, int N>
class UT_SpatialTree
{
    enum { BranchFactor = 1 << N };
public:
    /// Construct a tree with the specified subdivision strategy
    /// (subd_strategy must not be null), the bounds provider (must not
    /// be null), the specified bounds (min/max), and with the specified
    /// dimensions.
			 UT_SpatialTree(
				S *subd_strategy,
				B *bounds_provider,
				const fpreal *min,
				const fpreal *max);

    /// Construct a tree with the specified subdivision strategy
    /// (subd_strategy must not be null), the specified bounds provider
    /// (must not be null), and with the specified dimensions.
			 UT_SpatialTree(
				S *subd_strategy,
				B *bounds_provider);

    /// Destructor.
			~UT_SpatialTree();

    /// Add the specified object to the tree.
    void		 addObject(T object);

    /// Returns the bounds provider used by this tree.
    const B		*getBoundsProvider() const { return myBoundsProvider; }

    /// Return the maximum number of dimensions allowed.
    static int		 getMaxDimensions() { return 31; }

    /// Return the objects in the tree whose bounds contain the specified
    /// point.  Objects are returned in the supplied array.
    template<class A>
    void		 getObjects(
				const fpreal *pt,
				UT_ValArray<T> &objects,
				const A &accepter) const;

    void		 getObjects(
				const fpreal *pt,
				UT_ValArray<T> &objects) const
			 {
			     getObjects(pt, objects, UT_STAccepter(true));
			 }

    /// Return the objects in the tree whose bounds are withing the
    /// specified radius of the specified point.
    template<class A>
    int			 getObjects(
				const fpreal *pt,
				UT_ValArray<T> &objects,
				UT_FloatArray &distances2,
				fpreal radius,
				int max_objects,
				const A &accepter) const;

    int			 getObjects(
				const fpreal *pt,
				UT_ValArray<T> &objects,
				UT_FloatArray &distances2,
				fpreal radius,
				int max_objects) const
			 {
			     return getObjects(
				 pt, objects, distances2, radius, max_objects,
				 UT_STAccepter(true));
			 }

    /// Returns the subdivision strategy used by this tree.
    const S		*getSubdivisionStrategy() const
			 { return mySubDStrategy; }

private:
    class Node : public UT_SmallObject<Node, UT_SMALLOBJECT_CLEANPAGES_ON, 1024>
    {
    public:
			 Node();
			~Node();

	bool		 isLeaf() const { return !myChildren; }
	void		 steal(Node &node);

	fpreal		 myCenter[N];
	UT_ValArray<T>	 myObjects;
	Node		*myChildren;
    };

    struct NodeChildPointer
    {
	NodeChildPointer(const Node &node) :
	    myNode(node), myChildIndex(0)
	{}

	const Node	&myNode;
	int		 myChildIndex;
    };

public:
    void dump(std::ostream &os)
    {
	os << "UT_SpatialTree:\n";
	if (!myRoot)
	{
	    os << "<<Empty>>\n";
	    return;
	}
	std::stack<NodeChildPointer>	nodes;
	nodes.push(NodeChildPointer(*myRoot));
	while(!nodes.empty())
	{
	    const Node	&node = nodes.top().myNode;
	    int	&child_idx = nodes.top().myChildIndex;
	    if (child_idx == 0)
	    {
		for (size_t i = 0; i < nodes.size(); i++) os << " ";
		os << "Node [" << &node << "] - Object count: "
		   << node.myObjects.entries() << "\n";
	    }
	    if (child_idx < BranchFactor && !node.isLeaf())
		nodes.push(NodeChildPointer(node.myChildren[child_idx++]));
	    else
		nodes.pop();
	}
    }

    class NodeIterator
    {
    public:
	bool atEnd() const { return myNodeStack.empty(); }
	void advance()
	{
	    // Advance, depth-first
	    while(!myNodeStack.empty())
	    {
		NodeChildPointer &top = myNodeStack.top();
		if (top.myNode.isLeaf() ||
		    top.myChildIndex >= BranchFactor)
		{
		    myNodeStack.pop();
		}
		else
		{
		    myNodeStack.push(NodeChildPointer(
			top.myNode.myChildren[top.myChildIndex++]));
		    break;
		}

	    }
	}

	const UT_ValArray<T> &objects() const
	{
	    static UT_ValArray<T> empty;
	    if (myNodeStack.empty())
		return empty;

	    const NodeChildPointer &top = myNodeStack.top();
	    return top.myNode.myObjects;
	}

	NodeIterator	&operator++() { advance(); return *this; }

	bool operator==(const NodeIterator &other)
	{
	    if (myNodeStack.empty() || other.myNodeStack.empty())
		return myNodeStack.empty() && other.myNodeStack.empty();

	    NodeChildPointer &top = myNodeStack.top();
	    NodeChildPointer &other_top = other.myNodeStack.top();
	    return top.myNode == other_top.myNode &&
		   top.myChildIndex == other_top.myChildIndex;
	}

	bool operator!=(const NodeIterator &other)
	{
	    return !(*this == other);
	}

    protected:
	friend class UT_SpatialTree;

	NodeIterator(const Node *root)
	{
	    if (root)
		myNodeStack.push(NodeChildPointer(*root));
	}

    private:
	std::stack<NodeChildPointer> myNodeStack;
    };

    NodeIterator begin() const
    {
	return NodeIterator(myRoot);
    }

    NodeIterator leaf_end() const
    {
	return NodeIterator(NULL);
    }


private:
    /// Expand the tree so that it contains the specified bounds.
    void		 expand(const fpreal *min, const fpreal *max);

    /// Compute the union of the two bounds, storing the result in
    /// min1/max1.
    void		 expand(
				fpreal *min1,
				fpreal *max1,
				const fpreal *min2,
				const fpreal *max2) const;

    /// Return the center coordinates, in center, of the specified bounds.
    void		 getCenter(
				const fpreal *min,
				const fpreal *max,
				fpreal *center) const;

    /// Return the bounding rectangle of the specified quadrant (childno)
    /// of the specified bounds.
    void		 getChildBounds(
				int childno,
				const fpreal *min,
				const fpreal *max,
				const fpreal *center,
				fpreal *child_min,
				fpreal *child_max) const;

    /// Return the index of the child node that contains the specified
    /// point.  min/max specify the bounds of the parent node.
    int			 getChildContainer(
				const fpreal *center,
				const fpreal *pt) const;

    /// Given two masks, less and more, that indicate which children
    /// of a node we would like to iterate over, return the first such
    /// child.  Use this, along with getNextChild(), to iterate over
    /// all children indicated by the masks.  Each bit of the masks
    /// possibly filters out half of a certain dimension.  For example,
    /// if bit 0 of less is set to 0, the first half of dimension 0
    /// is filtered out; otherwise, it is included in the iteration.
    /// If a certain bit of less and the same bit of more are both 1,
    /// no filtering happens for that dimension.
    int			 getFirstChild(int less, int more) const;

    /// Returns the next child index in the supplied childno parameter
    /// and returns true if the end of the iteration has not yet been
    /// reached.
    bool		 getNextChild(int &childno, int less, int more) const;

    /// Initialize the children of the specified node, computing their
    /// center coordinates from the supplied bounds (min/max).
    void		 initChildren(
				Node *node,
				const fpreal *min,
				const fpreal *max) const;

    /// Returns true if the bounding region specified by min1/max1 contains
    /// the bounding region specified by min2/max2.
    bool		 isInside(
				const fpreal *min1,
				const fpreal *max1,
				const fpreal *pt) const;

    /// Returns true if the bounding region specified by min1/max1 contains
    /// the bounding region specified by min2/max2.
    bool		 isInside(
				const fpreal *min1,
				const fpreal *max1,
				const fpreal *min2,
				const fpreal *max2) const;

    /// Store the ordering (ascending) of the n specified values in indices.
    /// For example, values[indices[0]] will be the smallest value.  This
    /// function does not move elements in values.
    static void		 sort(const fpreal *values, int n, int *indices);

    /// Subdivide the specified node whose depth and bounds are given.
    void		 subdivide(
				Node *node,
				int depth,
				const fpreal *min,
				const fpreal *max);

    S			*mySubDStrategy;
    B			*myBoundsProvider;
    Node		*myRoot;
    fpreal		 myMin[N];
    fpreal		 myMax[N];
    int			 myDepth;
};

/// A basic subdivision strategy for use with UT_SpatialTree that subdivides
/// based on three criteria: node size, node object count, and node depth.
class UT_API UT_STBasicSubD
{
public:
    /// Construct an instance of UT_STBasicSubD that subdivides a node
    /// when all of the following are true:
    /// * the node's width and height are greater than min_size
    /// * the node has more than min_objects children
    /// * the node's depth is less than max_depth
		 UT_STBasicSubD(
			fpreal min_size,
			int min_objects,
			int max_depth)
		     : myMinSize(min_size)
		     , myMinObjects(min_objects)
		     , myMaxDepth(max_depth)
		 {}

    /// Returns true if the node at the specified depth with the
    /// specified bounds provider, containing the specified
    /// objects, whose bounds are given, should be subdivided.
    template<class T, class B>
    bool	 subdivide(
			B *bounds_provider,
			const fpreal *min,
			const fpreal *max,
			const UT_ValArray<T> &objects,
			int depth,
			int ndimensions)
		 {
		     int idimension;

		     if (depth >= myMaxDepth ||
			 objects.entries() <= myMinObjects)
			 return false;

		     for (idimension = 0; idimension < ndimensions;++idimension)
		     {
			 if (max[idimension] - min[idimension] < myMinSize)
			     return false;
		     }

		     return true;
		 }

private:
    fpreal	 myMinSize;
    int		 myMinObjects;
    int		 myMaxDepth;
};

template<class T, class S, class B, int N>
UT_SpatialTree<T, S, B, N>::Node::Node()
    : myChildren(0)
{
}

template<class T, class S, class B, int N>
UT_SpatialTree<T, S, B, N>::Node::~Node()
{
    delete[] myChildren;
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::Node::steal(Node &node)
{
    if (this != &node)
    {
	int	 iobject;

	delete[] myChildren;
	myObjects.entries(0);

	::memcpy(myCenter, node.myCenter, N * sizeof(fpreal));

	myChildren = node.myChildren;
	node.myChildren = 0;

	for (iobject = 0; iobject < node.myObjects.entries(); ++iobject)
	    myObjects.append(node.myObjects(iobject));
    }
}

template<class T, class S, class B, int N>
UT_SpatialTree<T, S, B, N>::UT_SpatialTree(S *subd_strategy,
					   B *bounds_provider,
					   const fpreal *min,
					   const fpreal *max)
    : myRoot(0)
    , mySubDStrategy(subd_strategy)
    , myBoundsProvider(bounds_provider)
    , myDepth(0)
{
    UT_ASSERT(subd_strategy);
    UT_ASSERT(0 < N && N <= getMaxDimensions());

    ::memcpy(myMin, min, N * sizeof(fpreal));
    ::memcpy(myMax, max, N * sizeof(fpreal));
}

template<class T, class S, class B, int N>
UT_SpatialTree<T, S, B, N>::UT_SpatialTree(S *subd_strategy, B *bounds_provider)
    : myRoot(0)
    , mySubDStrategy(subd_strategy)
    , myBoundsProvider(bounds_provider)
    , myDepth(0)
{
    UT_ASSERT(subd_strategy);
    UT_ASSERT(0 < N && N <= getMaxDimensions());

    ::memset(myMin, 0, N * sizeof(fpreal));
    ::memset(myMax, 0, N * sizeof(fpreal));
}

template<class T, class S, class B, int N>
UT_SpatialTree<T, S, B, N>::~UT_SpatialTree()
{
    delete myRoot;
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::addObject(T object)
{
    Node		*node;
    const fpreal	*bounds_min, *bounds_max;
    fpreal		 bounds_min_buf[N], bounds_max_buf[N];
    fpreal		 min[N], max[N];
    fpreal		 child_min[N], child_max[N];
    int			 childno, depth;

    // Get the object's bounds.
    bounds_min = myBoundsProvider->getMin(object, bounds_min_buf);
    bounds_max = myBoundsProvider->getMax(object, bounds_max_buf);

    // Expand the octree until it contains the bounds.
    if (!isInside(myMin, myMax, bounds_min, bounds_max))
	expand(bounds_min, bounds_max);

    UT_ASSERT(isInside(myMin, myMax, bounds_min, bounds_max));

    // Initialize min/max with the octree's bounds.
    ::memcpy(min, myMin, N * sizeof(fpreal));
    ::memcpy(max, myMax, N * sizeof(fpreal));

    depth = 1;

    // If we don't have a root node yet, create one.
    if (!myRoot)
    {
	myRoot = node = new Node();
	getCenter(myMin, myMax, myRoot->myCenter);
	myDepth = 1;
    }
    else
    {
	// Search the tree until we find a leaf node or a non-leaf node
	// that contains the entire bounds but has no children that
	// contain the entire bounds.
	node = myRoot;

	while (!node->isLeaf())
	{
	    childno = getChildContainer(node->myCenter, bounds_min);
	    if (childno != getChildContainer(node->myCenter, bounds_max))
		break;

	    getChildBounds(
		childno, min, max, node->myCenter, child_min, child_max);
	    node = &node->myChildren[childno];
	    UT_ASSERT(isInside(child_min,child_max,bounds_min,bounds_max));
	    ::memcpy(min, child_min, N * sizeof(fpreal));
	    ::memcpy(max, child_max, N * sizeof(fpreal));
	    depth++;
	}
    }

    UT_ASSERT(node);

    // Add the object and its bounds to the node.
    node->myObjects.append(object);

    // Possibly subdivide the node.
    if (node->isLeaf())
	subdivide(node, depth, min, max);
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::expand(const fpreal *min, const fpreal *max)
{
    Node	*root;
    fpreal	 center[2];
    fpreal	 size;
    int		 childno, idimension;

    // If we haven't created a tree yet, simply expand myMin/myMax until
    // they contain min/max.
    if (!myRoot)
    {
	::memcpy(myMin, min, N * sizeof(fpreal));
	::memcpy(myMax, max, N * sizeof(fpreal));
    }
    else if (myRoot->isLeaf())
    {
	expand(myMin, myMax, min, max);
	getCenter(myMin, myMax, myRoot->myCenter);
    }
    else
    {
	// Add one level at a time to our octree until it contains min/max.
	while (!isInside(myMin, myMax, min, max))
	{
	    root = new Node();
	    childno = 0;

	    for (idimension = 0; idimension < N; ++idimension)
	    {
		// Compute the center of myMin/myMax and min/max for the
		// current dimension so that we can determine which direction
		// to expand the octree in.
		size = myMax[idimension] - myMin[idimension];
		center[0] = (myMin[idimension] + myMax[idimension]) / 2.0;
		center[1] = (min[idimension] + max[idimension]) / 2.0;

		if (center[0] < center[1])
		{
		    // Expand to the right.
		    childno |= 1 << idimension;
		    root->myCenter[idimension] = myMax[idimension];
		    myMax[idimension] += size;
		}
		else
		{
		    // Expand to the left.
		    root->myCenter[idimension] = myMin[idimension];
		    myMin[idimension] -= size;
		}
	    }

	    // Initialize the children of the new root node and assign
	    // the data from the old root node to the appropriate child.
	    initChildren(root, myMin, myMax);
	    root->myChildren[childno].steal(*myRoot);
	    delete myRoot;
	    myRoot = root;
	    myDepth++;
	}
    }
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::expand(fpreal *min1,
				   fpreal *max1,
				   const fpreal *min2,
				   const fpreal *max2) const
{
    int		 idimension;

    for (idimension = 0; idimension < N; ++idimension)
    {
	min1[idimension] = SYSmin(min1[idimension], min2[idimension]);
	max1[idimension] = SYSmax(max1[idimension], max2[idimension]);
    }
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::getCenter(const fpreal *min,
				      const fpreal *max,
				      fpreal *center) const
{
    int		 idimension;

    for (idimension = 0; idimension < N; ++idimension)
	center[idimension] = (min[idimension] + max[idimension]) / 2.0;
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::getChildBounds(int childno,
					   const fpreal *min,
					   const fpreal *max,
					   const fpreal *center,
					   fpreal *child_min,
					   fpreal *child_max) const
{
    int		 idimension;

    for (idimension = 0; idimension < N; ++idimension)
    {
	if ((childno & (1 << idimension)) != 0)
	{
	    child_min[idimension] = min[idimension];
	    child_max[idimension] = center[idimension];
	}
	else
	{
	    child_min[idimension] = center[idimension];
	    child_max[idimension] = max[idimension];
	}
    }
}

template<class T, class S, class B, int N>
int
UT_SpatialTree<T, S, B, N>::getChildContainer(const fpreal *center,
					      const fpreal *pt) const
{
    int		 childno, idimension;

    childno = 0;

    for (idimension = 0; idimension < N; ++idimension)
    {
	if (pt[idimension] < center[idimension])
	    childno |= 1 << idimension;
    }

    UT_ASSERT(0 <= childno && childno < BranchFactor);

    return childno;
}

template<class T, class S, class B, int N>
int
UT_SpatialTree<T, S, B, N>::getFirstChild(int less, int more) const
{
    int	 free, childno;

    // Find all dimensions that may be less or more.  Every other dimension
    // is either less or more (but must be at least one).
    free = less & more;

    // Remove the free dimensions from both less and more.
    less &= ~free;
    more &= ~free;

    // Ensure that the non-free dimensions are set correctly.  Set all the
    // free dimensions to 0.
    childno = 0;
    childno |= less;
    childno &= ~more;

    UT_ASSERT(childno < BranchFactor);

    return childno;
}

template<class T, class S, class B, int N>
bool
UT_SpatialTree<T, S, B, N>::getNextChild(int &childno, int less, int more) const
{
    int	 free, mask, idimension;

    // Find all dimensions that may be less or more.  Every other dimension
    // is either less or more (but must be at least one).
    free = less & more;

    // Find the first free dimension that is set to 0 and flip it.  Flip
    // all free dimensions occuring before this back to 0.
    for (idimension = 0; idimension < N; ++idimension)
    {
	mask = 1 << idimension;
	UT_ASSERT((less & mask) != 0 || (more & mask) != 0);

	if ((free & mask) != 0)
	{
	    childno ^= mask;
	    if ((childno & mask) != 0)
		break;
	}
    }

    UT_ASSERT(childno < BranchFactor || idimension == N);

    return idimension < N;
}

template<class T, class S, class B, int N>
template<class A>
void
UT_SpatialTree<T, S, B, N>::getObjects(const fpreal *pt,
				       UT_ValArray<T> &objects,
				       const A &accepter) const
{
    Node		*node;
    const fpreal	*bounds_min, *bounds_max;
    fpreal		 bounds_min_buf[N], bounds_max_buf[N];
    int			 iobject, childno;

    // Search the tree for objects containing the specified point.
    if (!myRoot)
	return;

    node = myRoot;

    while (node)
    {
	// Check each object in the current node to see if it contains
	// the query position.
	for (iobject = 0; iobject < node->myObjects.entries(); ++iobject)
	{
	    // Get the object's bounds.
	    bounds_min = myBoundsProvider->getMin(
		node->myObjects(iobject), bounds_min_buf);
	    bounds_max = myBoundsProvider->getMax(
		node->myObjects(iobject), bounds_max_buf);

	    if (isInside(bounds_min, bounds_max, pt) &&
		accepter.accept(node->myObjects(iobject)))
		objects.append(node->myObjects(iobject));
	}

	if (node->isLeaf())
	    break;

	// Get the next node.
	childno = getChildContainer(node->myCenter, pt);
	node = &node->myChildren[childno];
    }
}

template<class T, class S, class B, int N>
template<class A>
int
UT_SpatialTree<T, S, B, N>::getObjects(const fpreal *pt,
				       UT_ValArray<T> &objects,
				       UT_FloatArray &distances2,
				       fpreal radius,
				       int max_objects,
				       const A &accepter) const
{
    Node		**node_stack;
    fpreal		 *dist_stack;
    int			 *childnos;
    fpreal		 *child_dists;
    int			 *indices;
    Node		 *node;
    const fpreal	 *center;
    fpreal		  dim_dists2[N];
    fpreal		  radius2, dist2;
    int			  iobject, idimension, ichild;
    int			  childno, ptchildno, index, nchildren;
    int			  max_stack_size, stack_size, less, more, diff;

    objects.entries(0);
    distances2.entries(0);

    // If the caller requested 0 (or fewer) objects, we are done.
    if (!myRoot || max_objects <= 0)
	return 0;

    // Allocate stack buffers.
    max_stack_size = (myDepth - 1) * BranchFactor + 1;
    node_stack = (Node **) UTstackAlloc(max_stack_size * sizeof(Node *));
    dist_stack = (fpreal *) UTstackAlloc(max_stack_size * sizeof(fpreal));
    child_dists = (fpreal *) UTstackAlloc(BranchFactor * sizeof(fpreal));
    childnos = (int *) UTstackAlloc(2 * BranchFactor * sizeof(int));
    indices = childnos + BranchFactor;

    // Validate the radius and compute the squared radius.
    radius = SYSmin(SYSmax(0.0, radius), 1e18);
    radius2 = radius * radius;

    // Push the root node onto the node stack.
    node_stack[0] = myRoot;
    dist_stack[0] = 0.0;
    stack_size = 1;

    while (stack_size > 0)
    {
	// Retrieve the next node.  We perform a distance check again
	// (we did this previously when we inserted the node into the
	// stack) because our radius may have shrunk.  This can drastically
	// improve performance because we can avoid touching many nodes.
	while (stack_size > 0 && dist_stack[--stack_size] >= radius2)
	    ;

	if (stack_size < 0)
	    break;

	node = node_stack[stack_size];
	UT_ASSERT(node);
	center = node->myCenter;

	// Check to see if the objects in the current node are within
	// the specified radius.
	for (iobject = 0; iobject < node->myObjects.entries(); ++iobject)
	{
	    dist2 = myBoundsProvider->getDistance2(
		node->myObjects(iobject), pt);

	    if (dist2 < radius2 && accepter.accept(node->myObjects(iobject)))
	    {
		// Determine the position of the object in the object list.
		index = distances2.sortedInsert(dist2);
		UT_ASSERT(index >= 0);
		objects.insert(node->myObjects(iobject), index);
		objects.truncate(max_objects);
		distances2.truncate(max_objects);

		// If we already have the number of requested objects
		// (max_objects), then we can come up with a new, lower bound
		// for the maximum distance that an object can be from
		// the query position.
		if (objects.entries() == max_objects)
		    radius2 = distances2.last();
	    }
	}

	if (!node->isLeaf())
	{
	    // Determine which child nodes may possibly be within the
	    // specified radius of the specified point.
	    less = more = 0;

	    for (idimension = 0; idimension < N; ++idimension)
	    {
		dim_dists2[idimension] = pt[idimension] - center[idimension];
		dim_dists2[idimension] *= dim_dists2[idimension];

		if (dim_dists2[idimension] <= radius2)
		{
		    less |= 1 << idimension;
		    more |= 1 << idimension;
		}
		else if (pt[idimension] < center[idimension])
		    less |= 1 << idimension;
		else
		    more |= 1 << idimension;
	    }

	    // Sort the children by their distance to the supplied point
	    // (measured from the center of the child).
	    ptchildno = getChildContainer(center, pt);
	    childno = getFirstChild(less, more);
	    nchildren = 0;

	    do
	    {
		diff = ptchildno ^ childno;
		dist2 = 0.0;

		for (idimension = 0; idimension < N; ++idimension)
		{
		    if ((diff & (1 << idimension)) != 0)
			dist2 += dim_dists2[idimension];
		}

		childnos[nchildren] = childno;
		child_dists[nchildren] = dist2;
		nchildren++;
	    }
	    while (getNextChild(childno, less, more));

	    sort(child_dists, nchildren, indices);

	    // Add the children to the stack starting with the ones furthest
	    // from the supplied point.
	    for (ichild = nchildren - 1; ichild >= 0; --ichild)
	    {
		childno = childnos[indices[ichild]];
		node_stack[stack_size] = &node->myChildren[childno];
		dist_stack[stack_size] = child_dists[indices[ichild]];
		stack_size++;
		UT_ASSERT(stack_size <= max_stack_size);
	    }
	}
    }

    if (node_stack)
    {
	UTstackFree(node_stack);
	UTstackFree(dist_stack);
	UTstackFree(child_dists);
	UTstackFree(childnos);
    }

    return objects.entries();
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::initChildren(Node *node,
					 const fpreal *min,
					 const fpreal *max) const
{
    fpreal	 child_min[N], child_max[N];
    int		 ichild;

    UT_ASSERT(node);
    UT_ASSERT(!node->myChildren);
    UT_ASSERT(min);
    UT_ASSERT(max);

    node->myChildren = new Node[BranchFactor];

    // Compute the center coordinates for each child.
    for (ichild = 0; ichild < BranchFactor; ++ichild)
    {
	getChildBounds(ichild, min, max, node->myCenter, child_min, child_max);
	getCenter(child_min, child_max, node->myChildren[ichild].myCenter);
    }
}

template<class T, class S, class B, int N>
bool
UT_SpatialTree<T, S, B, N>::isInside(const fpreal *min,
				     const fpreal *max,
				     const fpreal *pt) const
{
    int idimension;

    for (idimension = 0; idimension < N; ++idimension)
    {
	if (pt[idimension] < min[idimension] ||
	    pt[idimension] > max[idimension])
	    return false;
    }

    return true;
}

template<class T, class S, class B, int N>
bool
UT_SpatialTree<T, S, B, N>::isInside(const fpreal *min1,
				     const fpreal *max1,
				     const fpreal *min2,
				     const fpreal *max2) const
{
    int idimension;

    for (idimension = 0; idimension < N; ++idimension)
    {
	if (min2[idimension] < min1[idimension] ||
	    max2[idimension] > max1[idimension])
	    return false;
    }

    return true;
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::sort(const fpreal *values, int n, int *indices)
{
    int	 i, j, tmp;

    for (i = 0; i < n; ++i)
	indices[i] = i;

    for (i = 0; i < n; ++i)
    {
	for (j = i + 1; j < n; ++j)
	{
	    if (values[indices[j]] < values[indices[i]])
	    {
		tmp = indices[i];
		indices[i] = indices[j];
		indices[j] = tmp;
	    }
	}
    }
}

template<class T, class S, class B, int N>
void
UT_SpatialTree<T, S, B, N>::subdivide(Node *node, int depth,
				      const fpreal *min, const fpreal *max)
{
    UT_ASSERT(node);
    UT_ASSERT(node->isLeaf());

    Node		*child;
    const fpreal	*bounds_min, *bounds_max;
    fpreal		 bounds_min_buf[N], bounds_max_buf[N];
    fpreal		 child_min[N], child_max[N];
    int			 childno, ichild, iobject;
    bool		 subd;

    // Query the subdivision strategy to determine if the node should
    // be subdivided.
    UT_ASSERT(mySubDStrategy);
    subd = mySubDStrategy->subdivide(
	myBoundsProvider,
	min,
	max,
	(const UT_ValArray<T> &) node->myObjects,
	depth,
	N);

    if (!subd)
	return;

    // Increase the tree's depth.
    myDepth = SYSmax(myDepth, depth + 1);

    // Initialize the node's children.
    initChildren(node, min, max);

    // Test each of the node's objects to see if they can be placed
    // in one of its children.
    for (iobject = 0; iobject < node->myObjects.entries(); )
    {
	// Get the object's bounds.
	bounds_min = myBoundsProvider->getMin(
	    node->myObjects(iobject), bounds_min_buf);
	bounds_max = myBoundsProvider->getMax(
	    node->myObjects(iobject), bounds_max_buf);

	// Test for containment.
	childno = getChildContainer(node->myCenter, bounds_min);
	if (childno == getChildContainer(node->myCenter, bounds_max))
	{
	    child = &node->myChildren[childno];
	    child->myObjects.append(node->myObjects(iobject));
	    node->myObjects.removeIndex(iobject);
	}
	else
	    iobject++;
    }

    // Possibly subdivide each of the children.
    for (ichild = 0; ichild < BranchFactor; ++ichild)
    {
	getChildBounds(ichild, min, max, node->myCenter, child_min, child_max);
	subdivide(&node->myChildren[ichild], depth + 1, child_min, child_max);
    }
}

#undef MIN_NODE_SIZE

#endif

