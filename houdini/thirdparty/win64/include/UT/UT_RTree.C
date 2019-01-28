/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_RTree.C (UT library, C++)
 *
 * COMMENTS:
 */

#include "UT_RTree.h"
#include "UT_Assert.h"
#include <SYS/SYS_Math.h>
#include <SYS/SYS_StaticAssert.h>
#include <iostream>
#include <algorithm>
#include <VM/VM_SIMD.h>

template< typename T >
inline
UT_BoxT<T>::UT_BoxT()
{
    myMin[0] = std::numeric_limits<T>::max();
    myMin[1] = std::numeric_limits<T>::max();
    myMin[2] = std::numeric_limits<T>::max();

    myMax[0] = -std::numeric_limits<T>::max();
    myMax[1] = -std::numeric_limits<T>::max();
    myMax[2] = -std::numeric_limits<T>::max();
}

template< typename T >
template< typename U >
inline
UT_BoxT<T>::UT_BoxT(const U p[3])
{
    myMin[0] = myMax[0] = p[0];
    myMin[1] = myMax[1] = p[1];
    myMin[2] = myMax[2] = p[2];
}

template< typename T >
template< typename U >
inline
UT_BoxT<T>::UT_BoxT(UT_Vector3T<U> p)
{
    myMin[0] = myMax[0] = p.x();
    myMin[1] = myMax[1] = p.y();
    myMin[2] = myMax[2] = p.z();
}

template< typename T >
inline
UT_BoxT<T>::UT_BoxT(const UT_BoundingBox &bbox)
{
    myMin[0] = bbox.xmin();
    myMax[0] = bbox.xmax();
    myMin[1] = bbox.ymin();
    myMax[1] = bbox.ymax();
    myMin[2] = bbox.zmin();
    myMax[2] = bbox.zmax();
}

template< typename T >
inline bool
UT_BoxT<T>::isEmpty() const
{
    return(
        (myMin[0] > myMax[0]) ||
        (myMin[1] > myMax[1]) ||
        (myMin[2] > myMax[2])
    );
}

template< typename T >
inline T
UT_BoxT<T>::getMin(const int c) const
{
    UT_ASSERT_P( (0 <= c) && (c < 3) );
    UT_ASSERT_P( !isEmpty() );

    return myMin[c];
}

template< typename T >
inline T
UT_BoxT<T>::getMax(const int c) const
{
    UT_ASSERT_P( (0 <= c) && (c < 3) );
    UT_ASSERT_P( !isEmpty() );

    return myMax[c];
}

template< typename T >
template< typename U >
inline void
UT_BoxT<T>::assignPoint(const U p[3])
{
    myMin[0] = p[0];
    myMin[1] = p[1];
    myMin[2] = p[2];

    myMax[0] = p[0];
    myMax[1] = p[1];
    myMax[2] = p[2];
}

template< typename T >
template< typename U >
inline void
UT_BoxT<T>::absorbPoint(const U p[3])
{
    myMin[0] = (myMin[0] > p[0]) ? p[0] : myMin[0];
    myMin[1] = (myMin[1] > p[1]) ? p[1] : myMin[1];
    myMin[2] = (myMin[2] > p[2]) ? p[2] : myMin[2];

    myMax[0] = (myMax[0] < p[0]) ? p[0] : myMax[0];
    myMax[1] = (myMax[1] < p[1]) ? p[1] : myMax[1];
    myMax[2] = (myMax[2] < p[2]) ? p[2] : myMax[2];
}

template< typename T >
inline void
UT_BoxT<T>::absorbBox(const UT_BoxT<T>& b)
{
    myMin[0] = (myMin[0] > b.myMin[0]) ? b.myMin[0] : myMin[0];
    myMin[1] = (myMin[1] > b.myMin[1]) ? b.myMin[1] : myMin[1];
    myMin[2] = (myMin[2] > b.myMin[2]) ? b.myMin[2] : myMin[2];

    myMax[0] = (myMax[0] < b.myMax[0]) ? b.myMax[0] : myMax[0];
    myMax[1] = (myMax[1] < b.myMax[1]) ? b.myMax[1] : myMax[1];
    myMax[2] = (myMax[2] < b.myMax[2]) ? b.myMax[2] : myMax[2];
}

template< typename T >
inline void
UT_BoxT<T>::absorbBox(const UT_BoundingBox &b)
{
    myMin[0] = (myMin[0] > b.xmin()) ? b.xmin() : myMin[0];
    myMin[1] = (myMin[1] > b.ymin()) ? b.ymin() : myMin[1];
    myMin[2] = (myMin[2] > b.zmin()) ? b.zmin() : myMin[2];

    myMax[0] = (myMax[0] < b.xmax()) ? b.xmax() : myMax[0];
    myMax[1] = (myMax[1] < b.ymax()) ? b.ymax() : myMax[1];
    myMax[2] = (myMax[2] < b.zmax()) ? b.zmax() : myMax[2];
}

template< typename T >
inline void
UT_BoxT<T>::expandDistance(const T l)
{
    if( isEmpty() )
        return;

    myMin[0] -= l;
    myMin[1] -= l;
    myMin[2] -= l;

    myMax[0] += l;
    myMax[1] += l;
    myMax[2] += l;
}

template< typename T >
inline void
UT_BoxT<T>::expandDistance(const T l, int axis)
{
    if( isEmpty() )
        return;

    myMin[axis] -= l;
    myMax[axis] += l;
}

template< typename T >
inline bool
UT_BoxT<T>::contains(const T*const p) const
{
    return ( (myMin[0] <= p[0]) && (p[0] <= myMax[0]) &&
             (myMin[1] <= p[1]) && (p[1] <= myMax[1]) &&
             (myMin[2] <= p[2]) && (p[2] <= myMax[2]) );
}

template< typename T >
inline bool
UT_BoxT<T>::intersects(const UT_BoxT<T>& b) const
{
    return (
	(SYSmax(myMin[0], b.myMin[0]) <= SYSmin(myMax[0], b.myMax[0])) &&
	(SYSmax(myMin[1], b.myMin[1]) <= SYSmin(myMax[1], b.myMax[1])) &&
	(SYSmax(myMin[2], b.myMin[2]) <= SYSmin(myMax[2], b.myMax[2]))
    );
}

template< >
inline bool
UT_BoxT<float>::intersects(const UT_BoxT<float>& b) const
{
#if 1
    // We have to load from the wrong vector & swizzle or
    // we risk reading past a page boundary, triggering a fault,
    // even though we don't use the results of the w() component.
    v4uf	tmax( &myMin[2] );
    v4uf	tmin( &myMin[0] );
    tmax = tmax.swizzle<1, 2, 3, 0>();

    v4uf	bmax( &b.myMin[2] );
    v4uf	bmin( &b.myMin[0] );
    bmax = bmax.swizzle<1, 2, 3, 0>();

    tmin = vmax(tmin, bmin);
    tmax = vmin(tmax, bmax);
    v4uu	valid = tmin <= tmax;

    int		validmask;
    validmask = signbits(valid);

    return ((validmask & 0x7) == 0x7);
#else
    return (
	(SYSmax(myMin[0], b.myMin[0]) <= SYSmin(myMax[0], b.myMax[0])) &&
	(SYSmax(myMin[1], b.myMin[1]) <= SYSmin(myMax[1], b.myMax[1])) &&
	(SYSmax(myMin[2], b.myMin[2]) <= SYSmin(myMax[2], b.myMax[2]))
    );
#endif
}


template< typename T >
inline int
UT_BoxT<T>::getLargestAxis() const
{
    if( isEmpty() )
        return 0;

    int max_axis(0);
    T max_length(myMax[0] - myMin[0]);

    T length(0);

    length = myMax[1] - myMin[1];
    if( length > max_length )
    {
        max_length = length;
        max_axis = 1;
    }

    length = myMax[2] - myMin[2];
    if( length > max_length )
    {
        max_length = length;
        max_axis = 2;
    }

    return max_axis;
}

template< typename T >
inline std::ostream& operator<<(std::ostream& os, const UT_BoxT<T>& b)
{
    os << "Box{ min = ";
    os << b.getMin(0) << ", " << b.getMin(1) << ", " << b.getMin(2);
    os << ", max = ";
    os << b.getMax(0) << ", " << b.getMax(1) << ", " << b.getMax(2);
    os << " }";
    return os;
}

template< typename T >
inline
UT_VelBoxT<T>::UT_VelBoxT()
{
    myBounds[0].assign(std::numeric_limits<T>::max(),
		       -std::numeric_limits<T>::max(),
		       0);
    myBounds[1].assign(std::numeric_limits<T>::max(),
		       -std::numeric_limits<T>::max(),
		       0);
    myBounds[2].assign(std::numeric_limits<T>::max(),
		       -std::numeric_limits<T>::max(),
		       0);
}

template< typename T >
inline
UT_VelBoxT<T>::UT_VelBoxT(const UT_BoundingBox &bbox)
{
    myBounds[0].x() = bbox.xmin();
    myBounds[0].y() = bbox.xmax();
    myBounds[0].z() = 0;
    myBounds[1].x() = bbox.ymin();
    myBounds[1].y() = bbox.ymax();
    myBounds[1].z() = 0;
    myBounds[2].x() = bbox.zmin();
    myBounds[2].y() = bbox.zmax();
    myBounds[2].z() = 0;
}

template< typename T >
inline
UT_VelBoxT<T>::UT_VelBoxT(const UT_BoundingBox &start, const UT_BoundingBox &end)
{
    for (int i = 0; i < 3; i++)
    {
	float startmin = start(i, 0);
	float startmax = start(i, 1);

	float endmin = end(i, 0);
	float endmax = end(i, 1);

	// Solve for minimal velocity.
	myBounds[i].x() = startmin;
	myBounds[i].y() = startmin + SYSmax(startmax - startmin, endmax - endmin);
	myBounds[i].z() = endmin - startmin;
    }
}

template< typename T >
inline bool
UT_VelBoxT<T>::isEmpty() const
{
    return(
        (myBounds[0].x() > myBounds[0].y()) ||
        (myBounds[1].x() > myBounds[1].y()) ||
        (myBounds[2].x() > myBounds[2].y()) 
    );
}

template< typename T >
inline T
UT_VelBoxT<T>::getMin(const int c) const
{
    UT_ASSERT_P( (0 <= c) && (c < 3) );
    UT_ASSERT_P( !isEmpty() );

    return myBounds[c].x() + SYSmin(myBounds[c].z(), (T)0);
}

template< typename T >
inline T
UT_VelBoxT<T>::getMax(const int c) const
{
    UT_ASSERT_P( (0 <= c) && (c < 3) );
    UT_ASSERT_P( !isEmpty() );

    return myBounds[c].y() + SYSmax(myBounds[c].z(), (T)0);
}

template< typename T >
inline void
UT_VelBoxT<T>::absorbBox(const UT_VelBoxT<T>& b)
{
    for (int i = 0; i < 3; i++)
    {
	float startmin = SYSmin(myBounds[i].x(), b.myBounds[i].x());
	float startmax = SYSmax(myBounds[i].y(), b.myBounds[i].y());

	float endmin = SYSmin(myBounds[i].x() + myBounds[i].z(), b.myBounds[i].x() + b.myBounds[i].z());
	float endmax = SYSmax(myBounds[i].y() + myBounds[i].z(), b.myBounds[i].y() + b.myBounds[i].z());

	// Solve for minimal velocity.
	myBounds[i].x() = startmin;
	myBounds[i].y() = startmin + SYSmax(startmax - startmin, endmax - endmin);
	myBounds[i].z() = endmin - startmin;
    }
}

template< typename T >
inline bool
UT_VelBoxT<T>::intersects(const UT_VelBoxT<T>& b) const
{
    for (int i = 0; i < 3; i++)
    {
	T relvel = b.myBounds[i].z() - myBounds[i].z();

	// b is moving at the speed myVel[i]
	T	bmin = b.myBounds[i].x() + SYSmin(relvel, (T)0);
	T	bmax = b.myBounds[i].y() + SYSmax(relvel, (T)0);
	if (SYSmax(myBounds[i].x(), bmin) > SYSmin(myBounds[i].y(), bmax))
	    return false;
    }
    return true;
}

template< typename T >
inline int
UT_VelBoxT<T>::getLargestAxis() const
{
    if( isEmpty() )
        return 0;

    int max_axis(0);
    T max_length(myBounds[0].y() - myBounds[0].x());

    T length(0);

    length = myBounds[1].y() - myBounds[1].x();
    if( length > max_length )
    {
        max_length = length;
        max_axis = 1;
    }

    length = myBounds[2].y() - myBounds[2].x();
    if( length > max_length )
    {
        max_length = length;
        max_axis = 2;
    }

    return max_axis;
}

template< typename T >
inline std::ostream& operator<<(std::ostream& os, const UT_VelBoxT<T>& b)
{
    os << "Box{ min = ";
    os << b.getMin(0) << ", " << b.getMin(1) << ", " << b.getMin(2);
    os << ", max = ";
    os << b.getMax(0) << ", " << b.getMax(1) << ", " << b.getMax(2);
    os << ", vel = ";
    os << b.getVel(0) << ", " << b.getVel(1) << ", " << b.getVel(2);
    os << " }";
    return os;
}

template< typename T >
inline bool
UT_SphereT<T>::intersects(const UT_BoxT<T>& b) const
{
    // We calculate whether the box intersects the sphere by
    // calculating the distance from the sphere center to the
    // minimum / maximum axis coordinate (for each axis), squaring,
    // and adding together to compare to the sphere radius
    T min_distance = 0;
    for( int axis = 0; axis < 3; axis++ )
    {
	if( myCentre[axis] < b.getMin(axis) )
	{
	    T difference = myCentre[axis] - b.getMin(axis);
	    min_distance += difference * difference;
	}
	else if( myCentre[axis] > b.getMax(axis) )
	{
	    T difference = myCentre[axis] - b.getMax(axis);
	    min_distance += difference * difference;
	}
    }
    return min_distance <= myRadius*myRadius;
}

template< typename BOXTYPE>
struct UT_BoxedItemT
{
    typedef BOXTYPE Box;

    Box myBox;
    int myItem;

    UT_BoxedItemT() :
	myBox(),
	myItem(-1)
    {
    }
};

template< typename T >
inline std::ostream& operator<<(std::ostream& os, const UT_BoxedItemT<T>& bf)
{
    os << "BoxedItem{ ";
    os << "box = ";
    os << bf.myBox << ", ";
    os << "item = ";
    os << bf.myItem;
    os << " }";
    return os;
}

// Compute the bounding box for a range of boxes [begin, end)
template< typename BOXTYPE>
inline void
computeBoundingBox(
    BOXTYPE& bounding_box,
    const BOXTYPE*const begin, const BOXTYPE*const end
)
{
    UT_ASSERT_P( end != begin );

    bounding_box = *begin;

    for( const BOXTYPE* i = begin + 1; i != end; ++i )
    {
        bounding_box.absorbBox(*i);
    }
}

// Compute the bounding box for all the boxes in
// a range of boxed items [begin, end)
template< typename BOXTYPE >
inline void
computeBoundingBoxItem(
    BOXTYPE& bounding_box,
    const UT_BoxedItemT<BOXTYPE>*const begin, const UT_BoxedItemT<BOXTYPE>*const end
)
{
    UT_ASSERT_P( end != begin );

    bounding_box = begin->myBox;

    for( const UT_BoxedItemT<BOXTYPE>* i = begin + 1; i != end; ++i )
    {
        bounding_box.absorbBox(i->myBox);
    }
}

// Sort boxed items in the direction of an axis, by their centers
template< typename T >
struct UT_BoxedItemAxisOrderT
{
    int myAxis;

    explicit UT_BoxedItemAxisOrderT(const int axis) :
	myAxis(axis)
    {
    }

    ~UT_BoxedItemAxisOrderT()
    {
    }

    bool operator()(const UT_BoxedItemT<T>& a, const UT_BoxedItemT<T>& b)
    {
	typename T::Scalar amid = a.myBox.getMin(myAxis) + a.myBox.getMax(myAxis);
	typename T::Scalar bmid = b.myBox.getMin(myAxis) + b.myBox.getMax(myAxis);

	return amid < bmid;
    }
};

template< int MAX_ORDER >
class UT_RNode
{
public:
    typedef uint32 Index;

    UT_RNode()
    {
	clear();
    }

    ~UT_RNode()
    {
    }

    void clear()
    {
	setInternal(false);
	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    mySlots[s] = SLOT_VALUE_EMPTY;
	}
    }

    void setInternal(const bool isInternal)
    {
	mySlots[0] = ( (mySlots[0] & SLOT_MASK_INDEX) |
		       (isInternal ? SLOT_FLAG_INTERNAL : 0) );
    }

    // Assuming this is an internal node, assign a node index to slot s
    void assignSubtree(const int s, const int index_root_subtree)
    {
	UT_ASSERT_P( isInternal() );
	UT_ASSERT_P( (0 <= s) && (s < MAX_ORDER) );
	UT_ASSERT_P( 0 <= index_root_subtree );
	UT_ASSERT_P(
	    (static_cast<uint32>(index_root_subtree) & SLOT_MASK_INDEX) ==
	    static_cast<uint32>(index_root_subtree)
	);

	mySlots[s] = index_root_subtree | SLOT_FLAG_INTERNAL;
    }

    // Assign an item to slot s
    void assignItem(const int s, const int item)
    {
	UT_ASSERT_P( !isInternal() );
	UT_ASSERT_P( (0 <= s) && (s < MAX_ORDER) );
	UT_ASSERT_P( 0 <= item );
	UT_ASSERT_P(
	    (static_cast<uint32>(item) & SLOT_MASK_INDEX) ==
	    static_cast<uint32>(item)
	);

	mySlots[s] = item;
    }

    bool isInternal() const
    {
	return ((mySlots[0] & SLOT_FLAG_INTERNAL) != 0);
    }

    // Get node stored in slot.
    // This assumes that the last assignment to this slot was an node!
    // (not an item)
    int getSubtree(const int s) const
    {
	UT_ASSERT_P( isInternal() );
	UT_ASSERT_P( (0 <= s) && (s < MAX_ORDER) );
	UT_ASSERT_P( (mySlots[s] & SLOT_MASK_INDEX) != SLOT_VALUE_EMPTY );

	return (mySlots[s] & SLOT_MASK_INDEX);
    }

    // For internal nodes, count the number of children
    // For leaf nodes, count the number of items
    int countNumItems() const
    {
	int num_items(0);
	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    if( (mySlots[s] & SLOT_MASK_INDEX) == SLOT_VALUE_EMPTY )
		break;

	    ++num_items;
	}

	return num_items;
    }

    // Get item stored in slot.
    // This assumes that the last assignment to this slot was an item!
    // (not a node)
    int getItem(const int s) const
    {
	UT_ASSERT_P( !isInternal() );
	UT_ASSERT_P( (0 <= s) && (s < MAX_ORDER) );
	UT_ASSERT_P( (mySlots[s] & SLOT_MASK_INDEX) != SLOT_VALUE_EMPTY );

	return (mySlots[s] & SLOT_MASK_INDEX);
    }

    typedef Index Slot;
    static const Slot SLOT_FLAG_INTERNAL = 0x80000000;
    static const Slot SLOT_MASK_INDEX    = 0x7fffffff;
    static const Slot SLOT_VALUE_EMPTY   = SLOT_MASK_INDEX;

    //
    // mySlots[0] has a bit that defines whether the node is an internal
    // node. This is the SLOT_FLAG_INTERNAL bit.
    // In an internal node, each non-empty slot has the index of a child
    // node.
    // In a leaf node, each non-empty slot has the index of an item.
    //
    // A node (internal or leaf) may not always be full
    // (fewer than MAX_ORDER children for internal node case, or fewer than
    // MAX_ORDER items for leaf case).
    // When the node is not full, then the empty slots form
    // a contiguous range after all the nonempty slots.
    // The full range consists of the slots s in the half-open range
    // [ 0, getNumItems() )
    //

    Slot mySlots[MAX_ORDER];
};

// Determine the exact number of nodes needed to store an R-tree of MAX_ORDER
// with "size" elements. The O(log n) cost of this calculation is insignificant compared to
// the time it takes to construct the tree.
inline size_t
subtreeDetermineNumNodes(
    const int MAX_ORDER,
    const size_t size
)
{
    int num_nodes(0);

    if( size > MAX_ORDER )
    {
        // Internal node

        ++num_nodes;

        size_t m(MAX_ORDER);
        while( m * MAX_ORDER < size )
            m *= MAX_ORDER;

        UT_ASSERT_P( MAX_ORDER <= m );
        UT_ASSERT_P( m < size );

        // Count number of nodes for the at most MAX_ORDER subtrees that
        // have exactly m elements

        const int num_subtrees_size_m( size / m );

        UT_ASSERT_P( num_subtrees_size_m <= MAX_ORDER );

        const size_t num_nodes_for_m( subtreeDetermineNumNodes(MAX_ORDER, m) );

        num_nodes += num_subtrees_size_m * num_nodes_for_m;

        // Count number of nodes for the subtree with the less than m
        // remaining elements (if this exists)

        const size_t num_remaining_items( size % m );
        if( num_remaining_items > 0 )
        {
            num_nodes += subtreeDetermineNumNodes(MAX_ORDER, num_remaining_items);
        }
    }
    else
    {
        // Leaf node

        ++num_nodes;
    }

    return num_nodes;
}

/// Partition the range of boxed items [begin, end) according to the given
/// comparison function such that all items in the range [0, m-1] are less than
/// or equal to all items in [m, 2m-1], which are less than or equal to all
/// items in [2m, 3m-1], etc, where m is the subtree size.
template< typename BOXTYPE >
inline void
partitionForSubtrees(
    UT_BoxedItemT<BOXTYPE>*const begin, UT_BoxedItemT<BOXTYPE>*const end,
    const UT_BoxedItemAxisOrderT<BOXTYPE> &order,
    int subtree_size, int num_subtrees
)
{
    if (num_subtrees <= 1)
        return;

    // Choose a multiple of the subtree size that's close to the middle, and
    // partition so that all items in [begin, begin + i) are less than or equal
    // to all items in [begin + i, end).
    const int mid = num_subtrees / 2;
    const int i = mid * subtree_size;
    UTnth_element(begin, begin + i, end, order);

    // Recursively partition each side.
    partitionForSubtrees(begin, begin + i, order, subtree_size, mid);
    partitionForSubtrees(begin + i, end, order, subtree_size,
                         num_subtrees - mid);
}

// Create a subtree for the range of boxed items [begin, end).
// Return a pointer to the subtree root (this points into in shared_nodes)
// In addition, bounding_box will be a bounding box for the subtree.
template< int MAX_ORDER, typename BOXTYPE >
inline UT_RNode<MAX_ORDER>*
subtreeCreate(
    BOXTYPE& bounding_box,
    BOXTYPE shared_boxes[],
    UT_BoxedItemT<BOXTYPE>*const begin, UT_BoxedItemT<BOXTYPE>*const end,
    UT_RNode<MAX_ORDER>*const shared_nodes,
    UT_RNode<MAX_ORDER>*& shared_nodes_end
)
{
    SYS_STATIC_ASSERT( MAX_ORDER >= 2 );

    UT_RNode<MAX_ORDER>* node(shared_nodes_end++);
    node->clear();

    // node_boxes is the sub-array that contains the at most MAX_ORDER boxes
    // for the node that we're building
    BOXTYPE*const node_boxes(
	shared_boxes +
	((node - shared_nodes) * MAX_ORDER)
    );

    const size_t size(end - begin);

    UT_ASSERT_P( size > 0 );

    computeBoundingBoxItem(bounding_box, begin, end);

    if( size > MAX_ORDER )
    {
	// Create an internal node at most MAX_ORDER nonempty slots.
	// slot s will have the index of the root of a subtree,
	// which is contained in node_boxes[s]

	node->setInternal(true);

        // Partition in a way that is balanced but that aims
        // to keep the number of unused slots in leaves very small
        // Pick "m" to be the ideal number of items per subtree.
        size_t m(MAX_ORDER);
        while( m * MAX_ORDER < size )
            m *= MAX_ORDER;

        UT_ASSERT_P( MAX_ORDER <= m );
        UT_ASSERT_P( m < size );

        size_t offset(0);

        // Create at most MAX_ORDER subtrees, each of which has exactly m elements
        const int num_subtrees_size_m( size / m );

        UT_ASSERT_P( num_subtrees_size_m <= MAX_ORDER );

	// TODO: perhaps splitting along a single axis is not the best
        UT_BoxedItemAxisOrderT<BOXTYPE> order(bounding_box.getLargestAxis());
        // Partition the elements for each subtree.
        partitionForSubtrees(begin, end, order, m, num_subtrees_size_m);

        for( int s = 0; s < num_subtrees_size_m; ++s )
	{
            if( size - offset < m )
                break;

            const size_t next_offset( offset + m );

	    node->assignSubtree(
		s,
		subtreeCreate(
		    node_boxes[s],
		    shared_boxes,
                    begin + offset, begin + next_offset,
		    shared_nodes,
		    shared_nodes_end
	        ) - shared_nodes //FIXME: ugly pointer arithmetic
	    );

            offset = next_offset;
	}

        // If needed, create a subtree for any remaining elements (strictly less than m)
        const size_t num_remaining_items( size % m );
        if( num_remaining_items > 0 )
        {
            UT_ASSERT_P( num_subtrees_size_m < MAX_ORDER );
            UT_ASSERT_P( num_remaining_items == size - offset );

            const size_t next_offset( offset + num_remaining_items );

            node->assignSubtree(
		num_subtrees_size_m,
		subtreeCreate(
		    node_boxes[num_subtrees_size_m],
		    shared_boxes,
                    begin + offset, begin + next_offset,
		    shared_nodes,
		    shared_nodes_end
	        ) - shared_nodes //FIXME: ugly pointer arithmetic
	    );

            offset = next_offset;
        }

        UT_ASSERT_P( offset == size );

        UT_ASSERT_P( node->countNumItems() > 0 );
    }
    else
    {
	// Create a leaf with size nonempty slots.
	// Each slot has the index of a shared node box.
	node->setInternal(false);

	int s(0);
	for( const UT_BoxedItemT<BOXTYPE>* i = begin; i != end; ++i )
	{
            node_boxes[s] = i->myBox;
	    node->assignItem(s, i->myItem);
	    ++s;
	}
    }

    return node;
}

template< int MAX_ORDER >
inline int
UT_RTreeGeneric<MAX_ORDER>::getNumItems() const
{
    return myNumItems;
}

template< int MAX_ORDER, typename BOXTYPE>
inline void
subtreeCreateAssignment(
    BOXTYPE& bounding_box,
    BOXTYPE*const shared_boxes,
    const UT_RNode<MAX_ORDER>*const shared_nodes,
    const UT_RNode<MAX_ORDER>*const node,
    const BOXTYPE item_boxes[]
)
{
    using RNode = UT_RNode<MAX_ORDER>;

    UT_ASSERT_P( node != 0 );

    // node_boxes is the sub-array that contains the MAX_ORDER boxes
    // for the node that we're visiting
    BOXTYPE*const node_boxes(
	shared_boxes +
	((node - shared_nodes) * MAX_ORDER)
    );

    if( node->isInternal() )
    {
	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;

	    subtreeCreateAssignment(
	        node_boxes[s],
	        shared_boxes,
	        shared_nodes,
	        shared_nodes + node->getSubtree(s),
	        item_boxes
	    );
	}
    }
    else
    {
        int s = 0;
	for( ; s < MAX_ORDER; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;
	    
	    node_boxes[s] = item_boxes[node->getItem(s)];
	}
        for( ; s < MAX_ORDER; ++s )
	{
	    node_boxes[s] = BOXTYPE(); // Empty set
	}
    }

    computeBoundingBox(bounding_box, node_boxes, node_boxes + MAX_ORDER);
}

template< typename QUERY_SHAPE, typename RESULT_ACCEPTOR, int MAX_ORDER, typename BOXTYPE >
inline void
subtreeGetIntersectingItems(
    RESULT_ACCEPTOR& acceptor,
    const UT_RNode<MAX_ORDER>*const shared_nodes,
    const BOXTYPE*const shared_boxes,
    const UT_RNode<MAX_ORDER>*const node,
    const QUERY_SHAPE& query_box
)
{
    using RNode = UT_RNode<MAX_ORDER>;
    
    UT_ASSERT_P( node != 0 );

    // node_boxes is the sub-array that contains the MAX_ORDER boxes
    // for the node that we're visiting
    const BOXTYPE*const node_boxes(
	shared_boxes +
	((node - shared_nodes) * MAX_ORDER)
    );

    if( node->isInternal() )
    {
	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;
		
	    if( query_box.intersects(node_boxes[s]) )
            {
		subtreeGetIntersectingItems(
			acceptor,
			shared_nodes, shared_boxes,
			shared_nodes + node->getSubtree(s),
			query_box
		    );
	    }
	}
    }
    else
    {
	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;

	    if( query_box.intersects(node_boxes[s]) )
            {
                acceptor(node->getItem(s));
	    }
	}
    }
}

template< typename QUERY_SHAPE, typename RESULT_ACCEPTOR, int MAX_ORDER, typename BOXTYPE, int BATCH_SIZE >
inline void
subtreeGetIntersectingItemsBatch(
    RESULT_ACCEPTOR& acceptor,
    const UT_RNode<MAX_ORDER>*const shared_nodes,
    const BOXTYPE*const shared_boxes,
    const UT_RNode<MAX_ORDER>*const node,
    const QUERY_SHAPE *query_boxes
)
{
    using RNode = UT_RNode<MAX_ORDER>;
    
    UT_ASSERT_P( node != 0 );

    // node_boxes is the sub-array that contains the MAX_ORDER boxes
    // for the node that we're visiting
    const BOXTYPE*const node_boxes(
	shared_boxes +
	((node - shared_nodes) * MAX_ORDER)
    );

    if( node->isInternal() )
    {
	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;
		
	    for (int i = 0; i < BATCH_SIZE; i++)
	    {
		if( query_boxes[i].intersects(node_boxes[s]) )
		{
		    subtreeGetIntersectingItemsBatch<QUERY_SHAPE, RESULT_ACCEPTOR, MAX_ORDER, BOXTYPE, BATCH_SIZE>(
			acceptor,
			shared_nodes, shared_boxes,
			shared_nodes + node->getSubtree(s),
			query_boxes
		    );
		    break;
		}
	    }
	}
    }
    else
    {
	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;

	    for (int i = 0; i < BATCH_SIZE; i++)
	    {
		if( query_boxes[i].intersects(node_boxes[s]) )
		{
		    acceptor(i, node->getItem(s));
		}
	    }
	}
    }
}

template< int MAX_ORDER >
inline int
UTsubtreeComputeMaxDepth(
    const UT_RNode<MAX_ORDER> shared_nodes[],
    const UT_RNode<MAX_ORDER>*const node
)
{
    using RNode = UT_RNode<MAX_ORDER>;
    
    UT_ASSERT_P( node != 0 );

    if( node->isInternal() )
    {
        int max_depth_children(0);


	for( int s = 0; s < MAX_ORDER; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;
		
            max_depth_children = std::max(
                max_depth_children,
                UTsubtreeComputeMaxDepth(shared_nodes, shared_nodes + node->getSubtree(s))
	    );
	}

        return 1 + max_depth_children;
    }
    else
    {
        return 0;
    }
}


template< int MAX_ORDER >
template< typename BOXTYPE >
inline UT_RTreeGeneric<MAX_ORDER>::UT_RTreeGeneric(
    const BOXTYPE item_boxes[], const int size
) :
    mySharedNodesSize(0),
    mySharedNodes(0),
    myRoot(0),
    myNumItems(size)
{
    SYS_STATIC_ASSERT( MAX_ORDER >= 2 );

    UT_ASSERT_P( size >= 0 );

    if( size <= 0 )
        return;

    mySharedNodesSize = subtreeDetermineNumNodes(MAX_ORDER, size);
    mySharedNodes = new UT_RNode<MAX_ORDER>[mySharedNodesSize];

    UT_RNode<MAX_ORDER>* shared_nodes_end(mySharedNodes);

    // Put item #i in box[i], resulting in boxes_items
    std::vector< UT_BoxedItemT<BOXTYPE> > boxed_items;
    boxed_items.resize(size);
    for( int i = 0; i < size; ++i )
    {
        boxed_items[i].myBox = item_boxes[i];
	boxed_items[i].myItem = i;
    }

    // Create the R-tree nodes, based on boxed_items
    BOXTYPE bounding_box_root;
    std::vector< BOXTYPE > shared_boxes( MAX_ORDER * mySharedNodesSize );
    myRoot = subtreeCreate(
	bounding_box_root,
	&(shared_boxes[0]),
	&(boxed_items[0]), &(boxed_items[0]) + boxed_items.size(),
	mySharedNodes,
	shared_nodes_end
    );

    UT_ASSERT_P( shared_nodes_end - mySharedNodes == mySharedNodesSize );

#if 0
    std::cout << "RTree stats: " << std::endl;
    std::cout << "Number of items: " << size << std::endl;
    std::cout << "Tree size (in node): " << mySharedNodesSize << std::endl;
    std::cout << "MAX_ORDER (max children/items per node) = " << MAX_ORDER << std::endl;
    std::cout << "Number of items per node: " << fpreal64(size) / fpreal64(mySharedNodesSize) << std::endl;
    std::cout << "Max tree depth: " << UTsubtreeComputeMaxDepth(mySharedNodes, myRoot) << std::endl;
#endif // 0
}

template< int MAX_ORDER >
inline UT_RTreeGeneric<MAX_ORDER>::~UT_RTreeGeneric()
{
    delete[] mySharedNodes;
    mySharedNodes = 0;
}

template< int MAX_ORDER >
template< typename BOXTYPE>
inline void
UT_RTreeGeneric<MAX_ORDER>::createAssignment(
    UT_RTreeAssignmentT<BOXTYPE>& assignment,
    const BOXTYPE item_boxes[]
) const
{
    if( myRoot == 0 )
        return;

    assignment.myBoxesForSharedNodes.reserve(
	MAX_ORDER * mySharedNodesSize
    );

    // Allocate room for MAX_ORDER boxes for each shared node
    // Box c of node n is stored in slot (MAX_ORDER * n) + c
    assignment.myBoxesForSharedNodes.resize(
	MAX_ORDER * mySharedNodesSize
    );

    BOXTYPE bounding_box_root;
    subtreeCreateAssignment(
	bounding_box_root,
	&(assignment.myBoxesForSharedNodes[0]),
	mySharedNodes,
	myRoot,
	item_boxes
    );

#if 0
    std::cout << "reserved: " << MAX_ORDER * mySharedNodesSize << std::endl;
    std::cout << "capacity: " << assignment.myBoxesForSharedNodes.capacity() << std::endl;
#endif // 0
}

template< int MAX_ORDER >
template< typename QUERY_SHAPE, typename RESULT_ACCEPTOR, typename BOXTYPE >
void
UT_RTreeGeneric<MAX_ORDER>::getIntersectingItems(
    RESULT_ACCEPTOR& result_acceptor,
    const QUERY_SHAPE& query_box,
    const UT_RTreeAssignmentT<BOXTYPE>& assignment
) const
{
    if( myRoot == 0 )
        return;

    subtreeGetIntersectingItems(
        result_acceptor,
	mySharedNodes,
	&(assignment.myBoxesForSharedNodes[0]),
	myRoot, query_box
    );
}

template< int MAX_ORDER >
template< typename QUERY_SHAPE, typename RESULT_ACCEPTOR, typename BOXTYPE, int BATCH_SIZE >
void
UT_RTreeGeneric<MAX_ORDER>::getIntersectingItemsBatch(
    RESULT_ACCEPTOR& result_acceptor,
    const QUERY_SHAPE *query_boxes,
    const UT_RTreeAssignmentT<BOXTYPE>& assignment
) const
{
    if( myRoot == 0 )
        return;

    subtreeGetIntersectingItemsBatch< QUERY_SHAPE, RESULT_ACCEPTOR, MAX_ORDER, BOXTYPE, BATCH_SIZE> (
        result_acceptor,
	mySharedNodes,
	&(assignment.myBoxesForSharedNodes[0]),
	myRoot, query_boxes
    );
}

class UT_RTreeResultAppenderIntArray
{
public:
    UT_RTreeResultAppenderIntArray(UT_Array<int>& results, exint baseindex) 
	: myResults(results)
	, myBaseIndex(baseindex)
    {
    }

    inline void operator()(const int i)
    {
        myResults.append(i + myBaseIndex);
    }

private:
    UT_Array<int>& myResults;
    exint	 myBaseIndex;

    // Disallow
    UT_RTreeResultAppenderIntArray();
    UT_RTreeResultAppenderIntArray(const UT_RTreeResultAppenderIntArray&);
    UT_RTreeResultAppenderIntArray& operator=(
        const UT_RTreeResultAppenderIntArray&
    );
};

class UT_RTreeResultAppenderBatchIntArray
{
public:
    UT_RTreeResultAppenderBatchIntArray(UT_Array<int> *results, exint baseindex) 
	: myResults(results)
	, myBaseIndex(baseindex)
    {
    }

    inline void operator()(const int batch, const int i)
    {
        myResults[batch].append(i + myBaseIndex);
    }

private:
    UT_Array<int> *myResults;
    exint	 myBaseIndex;

    // Disallow
    UT_RTreeResultAppenderBatchIntArray();
    UT_RTreeResultAppenderBatchIntArray(const UT_RTreeResultAppenderBatchIntArray&);
    UT_RTreeResultAppenderBatchIntArray& operator=(
        const UT_RTreeResultAppenderBatchIntArray&
    );
};

template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE >
inline void UTgetIntersectingItems(
    UT_Array<int>& results,
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE& query_box,
    const UT_RTreeAssignmentT<BOXTYPE>& assignment
)
{
    results.clear();
    UT_RTreeResultAppenderIntArray appender(results, 0);
    tree.getIntersectingItems(appender, query_box, assignment);
}

template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE, int BATCH_SIZE >
inline void UTappendIntersectingItemsBatch(
    UT_Array<int> *results,
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE *query_boxes,
    const UT_RTreeAssignmentT<BOXTYPE>& assignment
)
{
    UT_RTreeResultAppenderBatchIntArray	appender(results, 0);
    tree.template getIntersectingItemsBatch<QUERY_SHAPE, UT_RTreeResultAppenderBatchIntArray, BOXTYPE, BATCH_SIZE>(appender, query_boxes, assignment);
}

template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE >
inline void UTappendIntersectingItems(
    UT_Array<int>& results,
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE& query_box,
    const UT_RTreeAssignmentT<BOXTYPE>& assignment,
    exint baseindex
)
{
    UT_RTreeResultAppenderIntArray appender(results, baseindex);
    tree.getIntersectingItems(appender, query_box, assignment);
}

class UT_RTreeResultAcceptorRawIntIterator
{
public:
    UT_RTreeResultAcceptorRawIntIterator(int* it) :
        myIt(it)
    {
    }

    inline void operator()(const int i)
    {
        *myIt++ = i;
    }

    int* getIterator() const
    {
        return myIt;
    }

private:
    int* myIt;

    // Disallow
    UT_RTreeResultAcceptorRawIntIterator();
    UT_RTreeResultAcceptorRawIntIterator(const UT_RTreeResultAcceptorRawIntIterator&);
    UT_RTreeResultAcceptorRawIntIterator& operator=(
        const UT_RTreeResultAcceptorRawIntIterator&
        );
};

template< typename QUERY_SHAPE, int MAX_ORDER, typename BOXTYPE >
inline int* UTgetIntersectingItems(
    const UT_RTreeGeneric<MAX_ORDER>& tree,
    const QUERY_SHAPE& query_box,
    const UT_RTreeAssignmentT<BOXTYPE>& assignment,
    int*const items_begin
)
{
    UT_RTreeResultAcceptorRawIntIterator acceptor(items_begin);

    tree.getIntersectingItems(acceptor, query_box, assignment);

    return acceptor.getIterator();
}


template< int MAX_ORDER >
template< typename BOXTYPE >
BOXTYPE UT_RTreeGeneric<MAX_ORDER>::boundingBox(
    const UT_RTreeAssignmentT<BOXTYPE>& assignment) const
{
    using RNode = UT_RNode<MAX_ORDER>;

    BOXTYPE box;

    if (myRoot == 0)
        return box;

    const BOXTYPE*const root_boxes =
        assignment.myBoxesForSharedNodes.data() + (myRoot - mySharedNodes)*MAX_ORDER;

    for (int s = 0; s < MAX_ORDER; ++s)
    {
        if ((myRoot->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY)
            break;

        box.absorbBox(root_boxes[s]);
    }

    return box;
}


template< int MAX_ORDER >
template<typename LOCAL_DATA,typename PRE_FUNCTOR,typename ITEM_FUNCTOR,typename POST_FUNCTOR>
void
UT_RTreeGeneric<MAX_ORDER>::traverse(
    PRE_FUNCTOR &pre_functor,
    ITEM_FUNCTOR &item_functor,
    POST_FUNCTOR &post_functor,
    LOCAL_DATA *data_for_parent) const
{
    if (!myRoot)
        return;

    traverseHelper(myRoot-mySharedNodes, -1, pre_functor, item_functor, post_functor, data_for_parent);
}
template< int MAX_ORDER >
template<typename LOCAL_DATA,typename PRE_FUNCTOR,typename ITEM_FUNCTOR,typename POST_FUNCTOR>
void
UT_RTreeGeneric<MAX_ORDER>::traverseHelper(
    int nodei,
    int parent_nodei,
    PRE_FUNCTOR &pre_functor,
    ITEM_FUNCTOR &item_functor,
    POST_FUNCTOR &post_functor,
    LOCAL_DATA *data_for_parent) const
{
    using RNode = UT_RNode<MAX_ORDER>;
    const RNode &node = mySharedNodes[nodei];
    LOCAL_DATA local_data[MAX_ORDER];
    bool descend = pre_functor(nodei, data_for_parent);
    if (!descend)
        return;
    const int nsub = node.countNumItems();
    if (node.isInternal())
    {
        for (int s = 0; s < nsub; ++s)
            traverseHelper(node.getSubtree(s), nodei, pre_functor, item_functor, post_functor, &local_data[s]);
    }
    else
    {
        for (int s = 0; s < nsub; ++s)
            item_functor(node.getItem(s), nodei, local_data[s]);
    }
    post_functor(nodei, parent_nodei, data_for_parent, nsub, local_data);
}

template <>
inline void
subtreeGetIntersectingItemsBatch<UT_BoxT<float>,
				UT_RTreeResultAppenderBatchIntArray,
				4,
				UT_BoxT<float>,
				4>
	    (
    UT_RTreeResultAppenderBatchIntArray& acceptor,
    const UT_RNode<4>*const shared_nodes,
    const UT_BoxT<float>*const shared_boxes,
    const UT_RNode<4>*const node,
    const UT_BoxT<float> *query_boxes
)
{
    using RNode = UT_RNode<4>;

    UT_ASSERT_P( node != 0 );

    // node_boxes is the sub-array that contains the MAX_ORDER boxes
    // for the node that we're visiting
    const UT_BoxT<float>*const node_boxes(
	shared_boxes +
	((node - shared_nodes) * 4)
    );

    v4uf	qmin[3];
    v4uf	qmax[3];
    
    for (int i = 0; i < 3; i++)
    {
	qmin[i] = v4uf(query_boxes[0].getMin(i),
		       query_boxes[1].getMin(i),
		       query_boxes[2].getMin(i),
		       query_boxes[3].getMin(i));
	qmax[i] = v4uf(query_boxes[0].getMax(i),
		       query_boxes[1].getMax(i),
		       query_boxes[2].getMax(i),
		       query_boxes[3].getMax(i));
    }


    if( node->isInternal() )
    {
	int			liveslots = 0;
	const UT_RNode<4>*	childnodes[4];

	for( int s = 0; s < 4; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;
		
	    v4uu		valid(0);
	    v4uf		tmax, tmin;
	    int			validmask = 0;

	    for (int i = 0; i < 3; i++)
	    {
		// Extract node box & splat.
		tmin = v4uf(node_boxes[s].getMin(i));
		tmax = v4uf(node_boxes[s].getMax(i));

		// Do intersection of 4 query boxes with node box.
		tmin = vmax(tmin, qmin[i]);
		tmax = vmin(tmax, qmax[i]);
		valid |= tmin > tmax;

		validmask = _mm_movemask_ps(V4SF(valid.vector));
		if (validmask == 0xf)
		    break;
	    }

	    if (validmask != 0xf)
	    {
		childnodes[liveslots++] = shared_nodes + node->getSubtree(s);
	    }
	}

	for (int s = 0; s < liveslots; s++)
	{
	    subtreeGetIntersectingItemsBatch<UT_BoxT<float>, UT_RTreeResultAppenderBatchIntArray, 4, UT_BoxT<float>, 4>(
		    acceptor,
		    shared_nodes, shared_boxes,
		    childnodes[s],
		    query_boxes
		);
	}
    }
    else
    {
	for( int s = 0; s < 4; ++s )
	{
	    if( (node->mySlots[s] & RNode::SLOT_MASK_INDEX) == RNode::SLOT_VALUE_EMPTY )
		break;

	    v4uu		valid(0);
	    int			validmask = 0;
	    v4uf		tmax, tmin;

	    for (int i = 0; i < 3; i++)
	    {
		// Extract node box & splat.
		tmin = v4uf(node_boxes[s].getMin(i));
		tmax = v4uf(node_boxes[s].getMax(i));

		// Do intersection of 4 query boxes with node box.
		tmin = vmax(tmin, qmin[i]);
		tmax = vmin(tmax, qmax[i]);
		valid |= tmin > tmax;

		validmask = _mm_movemask_ps(V4SF(valid.vector));
		if (validmask == 0xf)
		    break;
	    }

	    if (validmask != 0xf)
	    {
		for (int i = 0; i < 4; i++)
		{
		    if (! (valid[i] & (1 << i)) )
		    {
			acceptor(i, node->getItem(s));
		    }
		}
	    }
	}
    }
}
