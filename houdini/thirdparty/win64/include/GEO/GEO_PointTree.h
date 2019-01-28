/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PointTree.h ( GEO Library, C++)
 *
 * COMMENTS:  	This provides a method to index points inside of a
 *		geometry.
 */

#ifndef __GEO_PointTree__
#define __GEO_PointTree__

#include "GEO_API.h"
#include "GEO_Detail.h"
#include <GA/GA_Types.h>
#include <UT/UT_Assert.h>
#include <UT/UT_DMatrix4.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_KDTree.h>
#include <UT/UT_Vector3Array.h>
#include <stddef.h>

class GA_PointGroup;


///////////////////////////////////////////////////////////////////////////////
// GEO_PointTreeT<IDX_T>
//

/// GEO_PointTreeT is a position tree used to accelerate lookups based on
/// the index type IDX_T.
template <typename IDX_T>
class GEO_PointTreeT : protected UT_KDTree
{
public:
    typedef IDX_T		    IdxType;
    typedef UT_ValArray<IdxType>    IdxArrayType;

	     GEO_PointTreeT();
    virtual ~GEO_PointTreeT();

    /// Rebuilds the point tree as a pure index based tree using the
    /// given vectors.
    void		 build(const UT_Vector3Array &pts,
                            bool enable_multithreading = true);

    /// Rebuilds the point tree as a pure index based tree using the
    /// given vector/index lookup.
    void		 build(
			    const UT_Vector3Array &pts,
			    const IdxArrayType &idx,
                            bool enable_multithreading = true);

    /// This will slowly build the tree in place.  Balancing isn't done
    /// until the next lookup.  Thus, it can provide a nicer way to
    /// initialize a tree.  When auto_rebalance is true, the tree may choose
    /// not to rebalance on the next lookup, and will rebalance after a
    /// sufficient number of points have been added.  This can be useful in
    /// cases where lookups and appends are interleaved.  When it is false,
    /// the tree will be rebalanced on the next query.
    /// @{
    void		 append(
			    const UT_Vector3 &pt,
			    IdxType idx,
			    bool auto_rebalance=false);
    void		 append(const UT_Vector3 &pt);
    /// @}

    /// This builds a tree which has a per point radius.
    /// Note that this does not work with the tube testing code.
    /// Note that there is a large performance penalty for the
    /// findNearest methods, but no real penalty for the findAllClose
    /// methods
    /// If any points are added in radius mode, ALL must be added in
    /// radius mode.
    /// NOTE: If the tree is queried or built with zero entries, it
    /// will be stuck with non-radius mode.  So if you are using the
    /// auto_rebalance, make sure the first point is *not* auto_rebalance
    /// and you call buildIfNeeded() after adding it.
    /// @{
    void		 appendPtRadius(
			    const UT_Vector3 &pt,
			    fpreal radius, 
			    IdxType idx,
			    bool auto_rebalance=false);
    void		 appendPtRadius(
			    const UT_Vector3 &pt,
			    fpreal radius);
    /// @}

    /// Clears out the tree, leaving it with no entries.
    virtual void	 clear();

    /// Returns the number of actual points in the tree.
    int			 entries() const;

    /// Returns the amount of memory used by this tree.
    virtual int64	 getMemoryUsage(bool inclusive=true) const;

    /// Finds the nearest index to the given value.  Returns -1 on no
    /// such point.
    IdxType		 findNearestIdx(const UT_Vector3 &pt);

    /// Finds the nearest index to the given value.  Returns -1 on no
    /// such point.  The point must be within the given range.
    /// NOTE: findNearestPt() will fail if it has not been built with a gdp
    IdxType		 findNearestIdx(const UT_Vector3 &pt, fpreal maxdist);

    /// This uses a pre-built queue to reduce memory allocation
    /// You must call UT_KDTree::destroyQueue() when you are done with
    /// the queue.
    IdxType		 findNearestIdxQueue(
			    const UT_Vector3 &pt,
			    ut_KDPQueue &q,
			    fpreal maxdist = 1e18f,
                            bool wrapunitcube = false);

    /// Find the nearest [groupsize] points not more than maxdist
    /// away.
    /// Returns the number of points found.  Found points will be
    /// stored in the group array.
    /// The resulting points will be sorted by their distance.
    int			 findNearestGroupIdx(
			    const UT_Vector3 &pt,
			    fpreal maxdist,
			    int groupsize,
			    IdxArrayType &group,
			    UT_FloatArray &groupdist);

    /// This uses a pre-built queue to reduce memory allocation
    /// You must call UT_KDTree::destroyQueue() when you are done with
    /// the queue.
    int			 findNearestGroupIdxQueue(
			    const UT_Vector3 &pt,
			    fpreal maxdist,
			    int groupsize,
			    IdxArrayType &group,
			    UT_FloatArray &groupdist,
			    ut_KDPQueue &q,
                            bool wrapunitcube = false);

    /// Finds all the points within the given radius of a point.
    /// The resulting points will *not* be sorted.
    int			 findAllCloseIdx(
			    const UT_Vector3 &pt,
			    fpreal maxdist,
			    IdxArrayType &list);

    /// Creates a search queue useful for findAll queries.  You
    /// must destroy it with UT_KDTree::destroyQueue(q) when done.
    int			 findAllCloseIdxQueue(
			    const UT_Vector3 &pt,
			    ut_KDPQueue &queue,
			    fpreal maxdist,
			    IdxArrayType &list);

    /// Find all of the points inside the given tube.
    int			 findAllInTubeIdx(
			    const UT_Vector3 &pt,
			    const UT_Vector3 &dir,
			    fpreal radius,
			    IdxArrayType &list);

    /// Sets the myPointTransform member so that we can build our tree from
    /// a piece of geometry with an implicit transform on it.
    void		 setPointTransform(const UT_DMatrix4 &xform);

    /// Ensures the KD tree has been fully built.  If a KD tree
    /// is being shared among multiple threads, it is important it
    /// has first been compiled on one thread to avoid race conditions
    /// (GEO_PointTree is thread safe on read provided it has been built
    /// and provided no new points are added to it)
    SYS_DEPRECATED_REPLACE(16.0,buildIfNeeded)
    void		 ensureTreeBuilt() { updateKDTree(); }

    /// This must be called before querying, to build and balance the tree.
    virtual void         buildIfNeeded(bool enable_multithreading = true)
    {
        updateKDTree(enable_multithreading);
    }

    /// Sets the KD-tree balancing algorithm.
    /// - UT_KD_MEDIAN: splits at the median point
    /// - UT_KD_CENTROID: splits at the spatial centroid
    /// - UT_KD_SAH: splits at SAH optimal splitting point
    /// The UT_KD_SAH (surface area heuristic) algorithm builds more
    /// efficient trees but they are slower to construct.  The default is
    /// UT_KD_MEDIAN.
    void		 setBalancer(ut_KDBalancer balance)
    { UT_KDTree::setBalancer(balance); }

    /// Sets the maximum number of nodes to be stored in a leaf.  Smaller
    /// values will produce deeper and more memory-hungry trees.
    void		 setMaxLeafNodes(int max_leaf_nodes)
    { UT_KDTree::setMaxLeafNodes(max_leaf_nodes); }

protected:
    /// These are used by KDTree:
    virtual int		 comparePosition(int idx0, int idx1, int dim) const;
    virtual const float	*getP(int idx) const;

    /// These are used if the user adds points with radii.
    virtual bool	 pointsHaveRadius() const;
    virtual fpreal	 getRadius(int idx) const;


    /// Should be called prior to any invocation of KD Tree methods
    /// as it ensures the KDTree knows about the current number of
    /// entries.
    void		 updateKDTree(bool enablemultithread=true);

    /// Marks the kd tree as out of date.
    void		 dirtyKDTree() { myIsKDTreeDirty = true; }

protected:
    /// Clears the myPointTransform member data.
    void		 clearPointTransform();

    /// If this pointer is set, then before adding any GEO_Points to
    /// our point list, we transform their position with this matrix.
    UT_DMatrix4		*myPointTransform;
    
    /// This is the mapping from the KD entries into the GDP numbers.
    IdxArrayType	 myIndexList;
    
    /// The list of all the points.
    UT_Vector3Array	 myPointList;
    
    /// List of radii for each pont.
    UT_FloatArray	 myRadii;

    /// Tracks if the underlying KDtree has knowledge of our current
    /// size.
    bool		 myIsKDTreeDirty;

};


///////////////////////////////////////////////////////////////////////////////
//
// GEO_PointTreeInt
//

/// For basic opaque integer id trees, use GEO_PointTreeInt
typedef GEO_PointTreeT<int>	    GEO_PointTreeInt;


///////////////////////////////////////////////////////////////////////////////
//
// GEO_PointTreeGAOffset
//

/// GEO_PointTreeGAOffset is mostly a pure GEO_PointTreeT<GA_Offset>.
/// It additionally provides a build() method from a GA_PointGroup.
class GEO_API GEO_PointTreeGAOffset : public GEO_PointTreeT<GA_Offset>
{
public:
    typedef GEO_PointTreeT<GA_Offset> Super;

    /// Rebuilds the PointTree with the given detail and point group. If
    /// ptgroup is NULL, then all points are used.
    /// It will make its own internal vector3 array of the points and
    /// original indices.
    void build(const GEO_Detail *gdp, const GA_PointGroup *ptgroup = NULL,
        bool enable_multithreading = true);
    void build(const GEO_Detail *gdp, const GA_PointGroup *ptgroup,
		const char *attrib, bool enable_multithreading = true);

    /// Rebuilds the PointTree given a list of point offsets.
    /// It will make its own internal vector3 array of the points and
    /// original indices.
    void build(const GEO_Detail *gdp, const GA_OffsetArray &ptoffsets,
        bool enable_multithreading = true);
};


///////////////////////////////////////////////////////////////////////////////
//
// GEO_PointTree
//

/// GEO_PointTree is a position tree use to accelerate lookups based by
/// point number (GA_Index).
class GEO_API GEO_PointTree : public GEO_PointTreeT<GA_Index>
{
public:
    typedef GEO_PointTreeT<GA_Index> Super;

    /// Rebuilds the PointTree with the given detail and values.
    /// It will make its own internal vector3 array of the points and
    /// original indices.
    void build(const GEO_Detail *gdp, const GA_PointGroup *ptgroup = NULL,
        bool enable_multithreading = true);
    void build(const GEO_Detail *gdp, const GA_PointGroup *ptgroup,
		const char *attrib, bool enable_multithreading = true);
    void build(const GEO_Detail *gdp, const GA_PointGroup *ptgroup,
		const char *attrib, const char *radattrib, fpreal radscale,
                bool enable_multithreading = true);

    /// Methods that must delegate to base class due to overloads
    /// @{
    void build(const UT_Vector3Array &pts, bool enable_multithreading = true)
    { Super::build(pts, enable_multithreading); }
    void build(const UT_Vector3Array &pts, const GA_IndexArray &idx, bool enable_multithreading = true)
    { Super::build(pts, idx, enable_multithreading); }
    /// @}
};


////////////////////////////////////////////////////////////////////////////
//
// GEO_PointTreeT<IDX_T> Implementation
//

template <typename IDX_T>
GEO_PointTreeT<IDX_T>::GEO_PointTreeT()
    : UT_KDTree()
    , myPointTransform(NULL)
    , myIsKDTreeDirty(false)
{
    // Our entries by default as both are zero.
}

template <typename IDX_T>
GEO_PointTreeT<IDX_T>::~GEO_PointTreeT()
{
    clearPointTransform();
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::build(const UT_Vector3Array &pts, bool enable_multithreading)
{
    // Empty the kd tree.
    setEntries(0);

    myPointList = pts;
    exint n = myPointList.size();
    if (myPointTransform)
    {
        for (exint ptnum = 0; ptnum < n; ++ptnum)
            myPointList(ptnum) *= *myPointTransform;
    }

    // This has a very simple lookup relationship.
    myIndexList.setSize(n);

    for (exint i = 0; i < n; i++)
	myIndexList(i) = IdxType(i);

    // Build the tree.
    dirtyKDTree();
    buildIfNeeded(enable_multithreading);
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::build(
    const UT_Vector3Array &pts,
    const IdxArrayType &idx,
    bool enable_multithreading)
{
    clear();

    UT_ASSERT(myPointList.size() == myIndexList.size());

    myPointList = pts;
    if (myPointTransform)
    {
        exint n = myPointList.size();
        for (exint ptnum = 0; ptnum < n; ++ptnum)
            myPointList(ptnum) *= *myPointTransform;
    }
    myIndexList = idx;

    // Build the tree.
    dirtyKDTree();
    buildIfNeeded(enable_multithreading);
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::append(
	const UT_Vector3 &pt,
	IdxType idx,
	bool auto_rebalance)
{
    // If we want to take advantage of auto balancing, we want
    // to first ensure the KD tree is up to date.
    if (auto_rebalance)
	updateKDTree();

    if( myPointTransform )
	myPointList.append(pt * (*myPointTransform));
    else
	myPointList.append(pt);
    myIndexList.append(idx);

    // GrowEntries will only trigger a rebalance after a critical
    // number is added.
    // Tests with RBD shows setEntries is still faster.
    if (auto_rebalance)
	growEntries(1);
    else
	dirtyKDTree();
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::append(const UT_Vector3 &pt)
{
    append(pt, IdxType(myPointList.size()), false);
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::appendPtRadius(
	const UT_Vector3 &pt,
	fpreal radius,
	IdxType idx,
	bool auto_rebalance)
{
    // Verify we are in radius mode.
    UT_ASSERT(myRadii.size() == myPointList.size());

    // Ignore the points if we have mixed points.
    if (myRadii.size() != myPointList.size())
    {
	UT_ASSERT(!"Adding radius points to a non-radius based point tree");
	return;
    }

    myRadii.append(radius);
    append(pt, idx, auto_rebalance);
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::appendPtRadius(const UT_Vector3 &pt, fpreal radius)
{
    appendPtRadius(pt, radius, IdxType(myPointList.size()), false);
}

template <typename IDX_T>
IDX_T
GEO_PointTreeT<IDX_T>::findNearestIdx(const UT_Vector3 &pt)
{
    int			idx;
    UT_KDQueryPt	qpt(pt.vec);

    updateKDTree();

    idx = findClosest(qpt, 1e37F);

    if (idx < 0)
	return IdxType(-1);

    return myIndexList(idx);
}

template <typename IDX_T>
IDX_T
GEO_PointTreeT<IDX_T>::findNearestIdx(const UT_Vector3 &pt, fpreal maxdist)
{
    UT_IntArray idxlist;
    UT_KDQueryPt qpt(pt.vec);

    updateKDTree();

    (void) findClosest(idxlist, qpt, maxdist*maxdist, 1);
    if (idxlist.size())
	return myIndexList(idxlist(0));
    else
	return IdxType(-1);
}

template <typename IDX_T>
IDX_T
GEO_PointTreeT<IDX_T>::findNearestIdxQueue(
	const UT_Vector3 &pt, ut_KDPQueue &q, fpreal maxdist,
        bool wrapunitcube)
{

    updateKDTree();

    int i;
    if (!wrapunitcube)
    {
        UT_KDQueryPt qpt(pt.vec);
        i = findClosestQueue(qpt, q, maxdist * maxdist);
    }
    else
    {
        UT_KDQueryPtUnitWrap qpt(pt.vec,3);
        i = findClosestQueue(qpt, q, maxdist * maxdist);
    }

    if (i < 0)
	return IdxType(-1);
    else
	return myIndexList(i);
}

template <typename IDX_T>
int
GEO_PointTreeT<IDX_T>::findNearestGroupIdx(
	const UT_Vector3 &pt,
	fpreal maxdist,
	int groupsize,
	IdxArrayType &list,
	UT_FloatArray &groupdist)
{
    UT_KDQueryPt	qpt(pt.vec);
    UT_IntArray		group;

    updateKDTree();

    int numnear = findClosest(group, groupdist, qpt,
			      maxdist * maxdist,
			      groupsize);

    list.setSize(numnear);
    for (int i = 0; i < numnear; i++)
    {
	list(i) = myIndexList(group(i));
    }

    return numnear;
}

template <typename IDX_T>
int
GEO_PointTreeT<IDX_T>::findNearestGroupIdxQueue(
	const UT_Vector3 &pt,
	fpreal maxdist,
	int groupsize,
	IdxArrayType &list,
	UT_FloatArray &groupdist,
	ut_KDPQueue &q,
        bool wrapunitcube)
{

    updateKDTree();

    UT_IntArray group;
    exint numnear;
    if (!wrapunitcube)
    {
        UT_KDQueryPt qpt(pt.vec);
        numnear = findClosestQueue(
            group, groupdist, qpt, q, maxdist * maxdist, groupsize);
    }
    else
    {
        UT_KDQueryPtUnitWrap qpt(pt.vec,3);
        numnear = findClosestQueue(
            group, groupdist, qpt, q, maxdist * maxdist, groupsize);
    }

    list.setSize(numnear);
    for (exint i = 0; i < numnear; i++)
    {
	list(i) = myIndexList(group(i));
    }

    return numnear;
}

template <typename IDX_T>
int
GEO_PointTreeT<IDX_T>::findAllCloseIdx(
	const UT_Vector3 &pt,
	fpreal maxdist,
	IdxArrayType &list)
{
    updateKDTree();

    UT_IntArray idxlist;
    exint numnear = findAllClosest(idxlist, UT_KDQueryPt(pt.data()),
	    maxdist * maxdist);

    list.setSize(numnear);
    for (exint i = 0; i < numnear; i++)
    {
	list(i) = myIndexList(idxlist(i));
    }

    return numnear;
}

template <typename IDX_T>
int
GEO_PointTreeT<IDX_T>::findAllCloseIdxQueue(
	const UT_Vector3 &pt,
	ut_KDPQueue &queue,
	fpreal maxdist,
	IdxArrayType &list)
{
    updateKDTree();

    UT_IntArray idxlist;
    exint numnear = findClosestQueue(idxlist, UT_KDQueryPt(pt.data()),
	    queue, maxdist * maxdist, getEntries());

    list.setSize(numnear);
    for (exint i = 0; i < numnear; i++)
    {
	list(i) = myIndexList(idxlist(i));
    }

    return numnear;
}

template <typename IDX_T>
int			 
GEO_PointTreeT<IDX_T>::findAllInTubeIdx(
	const UT_Vector3 &orig,
	const UT_Vector3 &dir,
	fpreal radius,
	IdxArrayType &list)
{
    updateKDTree();

    UT_IntArray idxlist;
    UT_KDLineQuery pt(orig, dir);
    exint numnear = findAllClosest(idxlist, pt, radius*radius);

    list.setSize(numnear);
    for (exint i = 0; i < numnear; i++)
    {
	list(i) = myIndexList(idxlist(i));
    }

    return numnear;
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::clear()
{
    myPointList.setSize(0);
    myIndexList.setSize(0);
    myRadii.setSize(0);

    // Empty the KD tree.  We want to free the memory
    // quickly so update immediately.
    dirtyKDTree();
    updateKDTree();
}

template <typename IDX_T>
int
GEO_PointTreeT<IDX_T>::entries() const
{
    return myPointList.size();
}

template <typename IDX_T>
int64
GEO_PointTreeT<IDX_T>::getMemoryUsage(bool inclusive) const
{
    int64 mem = inclusive ? sizeof(*this) : false;
    mem += UT_KDTree::getMemoryUsage(false);
    if (myPointTransform)
        mem += sizeof(*myPointTransform);
    mem += myPointList.getMemoryUsage(false);
    mem += myIndexList.getMemoryUsage(false);
    mem += myRadii.getMemoryUsage(false);
    return mem;
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::clearPointTransform()
{
    delete myPointTransform;
    myPointTransform = 0;
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::setPointTransform(const UT_DMatrix4 &xform)
{
    clearPointTransform();
    if( !xform.isIdentity() )
	myPointTransform = new UT_DMatrix4(xform);
}

template <typename IDX_T>
void
GEO_PointTreeT<IDX_T>::updateKDTree(bool enable_multithreading)
{
    if (myIsKDTreeDirty)
    {
	// Size the kd tree.
	setEntries(myPointList.size());

	// Override the balancer to be centroid, it should behave nicer
	// for very large data sets.  (Mantra uses centroid for photons already)
	setBalancer(UT_KD_CENTROID);

	// Doing this prior to forking to multithreaded code
	// avoids extra locks.
	balance(enable_multithreading);

	myIsKDTreeDirty = false;
    }
}

template <typename IDX_T>
int
GEO_PointTreeT<IDX_T>::comparePosition(int idx0, int idx1, int dim) const
{
    float delta = myPointList(idx0)(dim) - myPointList(idx1)(dim);
    if (delta < 0)
	return -1;
    else if (delta > 0)
	return 1;
    else
	return 0;
}

template <typename IDX_T>
const float *
GEO_PointTreeT<IDX_T>::getP(int idx) const
{
    return myPointList(idx).data();
}

template <typename IDX_T>
bool
GEO_PointTreeT<IDX_T>::pointsHaveRadius() const
{
    if (myRadii.size())
	return true;

    return false;
}

template <typename IDX_T>
fpreal
GEO_PointTreeT<IDX_T>::getRadius(int idx) const
{
    return myRadii(idx);
}


#endif
