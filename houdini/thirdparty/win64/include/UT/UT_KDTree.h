/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_KDTree.h ( UT Library, C++)
 *
 * COMMENTS:	KD Tree Implementation
 *
 * The template class representing the nodes must have the following methods
 * defined:
 *	const UT_Vector3/UT_Vector4	getP(int idx) const;
 *	bool				isValid(int idx) const;
 * It's ok for the area function to return 0.
 * The valid method is used when searching for valid nodes.
 * The splits are the dimension used which the node is split in.  Thus, the
 *    split direction can be encoded using a minimal 2 bits.
 *
 * TODO: We may want to convert this to a template class
 */

#ifndef __UT_KDTree__
#define __UT_KDTree__

#include "UT_API.h"
#include "UT_BoundingBox.h"
#include "UT_BoundingRect.h"
#include "UT_IntArray.h"
#include "UT_Array.h"
#include "UT_Lock.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Math.h>


class ut_KDPQueue;
class UT_Filter;

// The maximum dimension must be less than 255 (unless you change the splits
// variable to a ushort)
#define UT_KD_MAXDIM	254

/// Lookup point information to be passed to the query functions
struct UT_KDQueryPt {
    UT_KDQueryPt(const float *p)
    { P = p; myData = 0; }
    UT_KDQueryPt(const float *p, const void *data)
    { P = p; myData = data; }

    float boxDist(int axis, float bmin, float bmax) const
    {
	return SYSmax(P[axis] - bmax, bmin - P[axis], 0.0F);
    }
    float dist(int axis, float off) const
    {
	return P[axis] - off;
    }
    float dist(const float *P, int dim) const
    {
	float	d2 = 0;
	float	d;

	switch (dim) {
	    default:
		for (int i = dim; i-- > 4; ) {
		    d = dist(i, P[i]); d2 += d*d;
		}
                SYS_FALLTHROUGH;
	    case 4: d = dist(3, P[3]); d2 += d*d;
                SYS_FALLTHROUGH;
	    case 3: d = dist(2, P[2]); d2 += d*d;
                SYS_FALLTHROUGH;
	    case 2: d = dist(1, P[1]); d2 += d*d;
                SYS_FALLTHROUGH;
	    case 1: d = dist(0, P[0]); d2 += d*d;
	}
	return d2;
    }

    const float		*P;
    const void		*myData;
};

/// This query point considers space to wrap between 0 and 1
/// in all dimensions.  It only supports up to 3 dimensions
/// due to UT_BoundingBox only having 3 dimensions.
struct UT_KDQueryPtUnitWrap {
    UT_KDQueryPtUnitWrap(const float *p, int ndims=3)
        : myP(p)
        , myNDims(ndims)
    {}

    /// This can be an underestimate, but not an overestimate
    /// of the distance squared.
    float boxDist(const UT_BoundingBox &box) const
    {
        float maxdist = 0;
        for (int axis = 0; axis < myNDims; ++axis)
        {
            float p = myP[axis];
            float c = box.centerAxis(axis);
            float centredist;
            if (c < p)
                centredist = SYSmin(p - c, 1 - p + c);
            else
                centredist = SYSmin(c - p, 1 - c + p);
            float axisdist = centredist - 0.5f*box.sizeAxis(axis);
            maxdist = SYSmax(maxdist, axisdist);
        }
        return ((maxdist >= 0) ? 1 : -1) * maxdist*maxdist;
    }

    /// This distance squared needs to be exact.
    float dist(const float *P, int) const
    {
        float d2 = 0;
        for (int axis = 0; axis < myNDims; ++axis)
        {
            float p = myP[axis];
            float q = P[axis];
            float axisdist;
            if (q < p)
                axisdist = SYSmin(p - q, 1 - p + q);
            else
                axisdist = SYSmin(q - p, 1 - q + p);

            d2 += axisdist*axisdist;
        }
        return d2;
    }

    const float *myP;
    const int myNDims;
};

/// Lookup information for 2D-tree triangle queries
/// NOTE: Distance squared here is not Euclidean distance squared, but
///       edge-perpendicular distance squared, i.e. distance is from
///       the incentre out, perpendicular to one of the edges, minus the
///       incircle's radius.  This avoids the need to have special
///       cases for what would be the circular sections around each vertex.
///       It basically indicates how far the triangle would have to be
///       expanded (or contracted) relative to the incentre in order to
///       hit the query point.
class UT_KDTriQuery {
public:
    UT_KDTriQuery(
        const UT_Vector2 &a,
        const UT_Vector2 &b,
        const UT_Vector2 &c)
    {
        UT_Vector2 sidea = c-b;
        UT_Vector2 sideb = a-c;
        UT_Vector2 sidec = b-a;
        float la = sidea.length();
        float lb = sideb.length();
        float lc = sidec.length();
        float p = la + lb + lc;
        myIncentre = (la*a + lb*b + lc*c)/p;
        float s = 0.5f*p;
        myDist = sqrt((s-la)*(s-lb)*(s-lc)/s);
        myDirs[0] = UT_Vector2(sidea.y(), -sidea.x());
        myDirs[1] = UT_Vector2(sideb.y(), -sideb.x());
        myDirs[2] = UT_Vector2(sidec.y(), -sidec.x());
        myDirs[0].normalize();
        myDirs[1].normalize();
        myDirs[2].normalize();
        // Winding order matters for myDirs.
        // We need to make sure that they point toward their corresponding
        // sides, instead of away from them, else we'll get the triangle
        // rotated a half turn.  Either point on side a works for checking
        // dir 0, and either all of the dirs are inverted, or none of them are.
        if (dot(myDirs[0], b - myIncentre) < 0)
        {
            myDirs[0] = -myDirs[0];
            myDirs[1] = -myDirs[1];
            myDirs[2] = -myDirs[2];
        }
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[0], c - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[1], c - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[1], a - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[2], a - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[2], b - myIncentre), 0));
    }

    /// This can be an underestimate, but not an overestimate
    /// of the distance squared.
    float boxDist(const UT_BoundingRect &box) const
    {
        // Expand the box to the minimum circle fully containing it
        // and then compute the maximum of the 3 distances.
        float boxradius = 0.5f*SYSsqrt(box.sizeX()*box.sizeX()
                                     + box.sizeY()*box.sizeY());
        UT_Vector2 boxcentre(box.centerX(), box.centerY());
        UT_Vector2 dir = boxcentre - myIncentre;
        float d0 = dot(dir, myDirs[0]);
        float d1 = dot(dir, myDirs[1]);
        float d2 = dot(dir, myDirs[2]);
        float d = SYSmax(d0, d1, d2) - myDist - boxradius;
        float dsquared = d*d;
        return (d < 0) ? -dsquared : dsquared;
    }
    /// This distance squared needs to be exact.
    float dist(const float *P, int) const
    {
        // Compute the maximum of the 3 distances.
        UT_Vector2 dir = UT_Vector2(P[0], P[1]) - myIncentre;
        float d0 = dot(dir, myDirs[0]);
        float d1 = dot(dir, myDirs[1]);
        float d2 = dot(dir, myDirs[2]);
        float d = SYSmax(d0, d1, d2) - myDist;
        float dsquared = d*d;
        return (d < 0) ? -dsquared : dsquared;
    }

private:
    UT_Vector2 myDirs[3];
    UT_Vector2 myIncentre;
    float myDist;
};

/// Lookup information for 3D-tree tetrahedron queries
/// NOTE: Distance squared here is not Euclidean distance squared, but
///       face-perpendicular distance squared, i.e. distance is from
///       the incentre out, perpendicular to one of the faces, minus the
///       insphere's radius.  This avoids the need to have special
///       cases for what would be the spherical sections around each vertex.
///       It basically indicates how far the tetrahedron would have to be
///       expanded (or contracted) relative to the incentre in order to
///       hit the query point.
class UT_KDTetQuery {
public:
    UT_KDTetQuery(
        const UT_Vector3 &a,
        const UT_Vector3 &b,
        const UT_Vector3 &c,
        const UT_Vector3 &d)
    {
        UT_Vector3 edgeda = a-d;
        UT_Vector3 edgedb = b-d;
        UT_Vector3 edgedc = c-d;
        UT_Vector3 edgeab = b-a;
        UT_Vector3 edgebc = c-b;
        myDirs[0] = cross(edgedb, edgedc);
        float volume = SYSabs(dot(edgeda, myDirs[0]))/6;
        myDirs[1] = cross(edgedc, edgeda);
        myDirs[2] = cross(edgeda, edgedb);
        myDirs[3] = cross(edgebc, edgeab);
        float areaa = myDirs[0].normalize()/2;
        float areab = myDirs[1].normalize()/2;
        float areac = myDirs[2].normalize()/2;
        float aread = myDirs[3].normalize()/2;
        float area = areaa + areab + areac + aread;
        myDist = (3*volume)/area;
        myIncentre = (areaa*a + areab*b + areac*c + aread*d)/area;

        // Winding order matters for myDirs.
        // We need to make sure that they point toward their corresponding
        // sides, instead of away from them, else we'll get the tetrahedron
        // inverted through the incentre. Any point on side a works for checking
        // dir 0, and either all of the dirs are inverted, or none of them are.
        if (dot(myDirs[0], b - myIncentre) < 0)
        {
            myDirs[0] = -myDirs[0];
            myDirs[1] = -myDirs[1];
            myDirs[2] = -myDirs[2];
            myDirs[3] = -myDirs[3];
        }
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[0], c - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[0], d - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[1], c - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[1], d - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[1], a - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[2], d - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[2], a - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[2], b - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[3], a - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[3], b - myIncentre), 0));
        UT_ASSERT_P(SYSisGreaterOrEqual(dot(myDirs[3], c - myIncentre), 0));
    }

    /// This can be an underestimate, but not an overestimate
    /// of the distance squared.
    float boxDist(const UT_BoundingBox &box) const
    {
        // Expand the box to the minimum sphere fully containing it
        // and then compute the maximum of the 4 distances.
        float boxradius = box.getRadius();
        UT_Vector3 boxcentre = box.center();
        UT_Vector3 dir = boxcentre - myIncentre;
        float d0 = dot(dir, myDirs[0]);
        float d1 = dot(dir, myDirs[1]);
        float d2 = dot(dir, myDirs[2]);
        float d3 = dot(dir, myDirs[3]);
        float d = SYSmax(d0, d1, d2, d3) - myDist - boxradius;
        float dsquared = d*d;
        return (d < 0) ? -dsquared : dsquared;
    }
    /// This distance squared needs to be exact.
    float dist(const float *P, int) const
    {
        // Compute the maximum of the 4 distances.
        UT_Vector3 dir = UT_Vector3(P[0], P[1], P[2]) - myIncentre;
        float d0 = dot(dir, myDirs[0]);
        float d1 = dot(dir, myDirs[1]);
        float d2 = dot(dir, myDirs[2]);
        float d3 = dot(dir, myDirs[3]);
        float d = SYSmax(d0, d1, d2, d3) - myDist;
        float dsquared = d*d;
        return (d < 0) ? -dsquared : dsquared;
    }

private:
    UT_Vector3 myDirs[4];
    UT_Vector3 myIncentre;
    float myDist;
};

/// Lookup information for line queries
class UT_KDTubeQuery {
public:
    UT_KDTubeQuery(const UT_Vector3 &orig,
		   const UT_Vector3 &dir)
    {
	myP0 = orig;
	myP1 = orig+dir;
	myDir = dir;
    }

    /// This can be an underestimate, but not an overestimate
    /// of the distance squared.
    float boxDist(const UT_BoundingBox &box) const
    {
	return box.approxLineDist2(myP0, myDir);
    }
    /// This distance squared needs to be exact.
    float dist(const float *P, int) const
    {
	return segmentPointDist2(UT_Vector3(P), myP0, myP1);
    }

private:
    UT_Vector3		myP0;
    UT_Vector3		myP1;
    UT_Vector3		myDir;
};

/// Queries for infinite lines (infinite tubes)
class UT_KDLineQuery {
public:
    UT_KDLineQuery(const UT_Vector3 &orig,
		   const UT_Vector3 &dir)
    {
	myP0 = orig;
	myDir = dir;
    }

    /// This can be an underestimate, but not an overestimate
    /// of the distance squared.
    float boxDist(const UT_BoundingBox &box) const
    {
	UT_Vector3 pos = box.center();
	pos -= myP0;
	pos -= myDir.project(pos);
	float dist = SYSmax(pos.length() - box.getRadius(), 0.0);
	return dist*dist;
    }
    /// This distance squared needs to be exact.
    float dist(const float *P, int) const
    {
	UT_Vector3 pos(P);
	pos -= myP0;
	pos -= myDir.project(pos);
	return pos.length2();
    }

private:
    UT_Vector3		myP0;
    UT_Vector3		myDir;
};

#if 0
/// Queries for spherical cones
class UT_KDConeQuery {
public:
    UT_KDConeQuery(
        const UT_Vector3 &orig,
        const UT_Vector3 &dir,
        const float dotthreshold,
        const float maxdistancesquared = 1e37)
    {
        myP0 = orig;
        myDir = dir;
        myDotThreshold = dotthreshold;
        mySine = SYSsqrt(1-myDotThreshold*myDotThreshold);
        myMaxDistaceSquared = maxdistancesquared;
        myMaxDistace = SYSsqrt(myMaxDistaceSquared);
    }

    /// This can be an underestimate, but not an overestimate
    /// of the distance squared.
    float boxDist(const UT_BoundingBox &box) const
    {
        if (box.isInside(myP0))
            return 0;
        UT_ASSERT_MSG(0, "Unimplemented!!!  Note: if box is completely outside cone, should return something strictly > myMaxDistanceSquared");
        return 2*myMaxDistaceSquared;
    }
    /// This distance squared needs to be exact.
    float dist(const float *P, int) const
    {
        UT_Vector3 pos(P);
        pos -= myP0;

        // Counterintuitively, this just uses regular distance squared.
        // Points outside the cone are rejected by isValid().
        float actualdist2 = pos.length2();

        return actualdist2;
    }
    float distOutsideCone(const float *P) const
    {
        UT_Vector3 pos(P);
        pos -= myP0;
        UT_Vector3 dir = pos;
        dir.normalize();

        float actualdist2 = pos.length2();

        // If inside cone, return -1
        float dt = dir.dot(myDir);
        if (dt >= myDotThreshold)
            return -1;

        // If outside, use max of actualdist2 and (maxdist + dist to cone side)^2

        UT_Vector3 planex = pos - myDir.project(pos);
        UT_Vector3 sideraydir = myDotThreshold*myDir + mySine*planex;
        float raydist = sideraydir.dot(pos);
        float disttoconeside;
        if (raydist <= 0)
            disttoconeside = SYSsqrt(actualdist2);
        else
            disttoconeside = (pos - raydist*sideraydir).length();

        return disttoconeside;
    }

private:
    UT_Vector3 myP0;
    UT_Vector3 myDir;
    float myDotThreshold;
    float mySine;
    float myMaxDistace;
    float myMaxDistaceSquared;
};
#endif

class UT_API UT_KDTree
{
public:
    /// KD Tree balancing algorithms.  See setBalancer.
    typedef enum {
	UT_KD_MEDIAN,
	UT_KD_SAH,
	UT_KD_CENTROID
    } ut_KDBalancer;

public:
    UT_KDTree(int dim=3, size_t size=0)
    {
	myDim = dim;
	myList = 0;
	myMaxLeafNodes = 25;
	myRebalanceCount = 128;
	myBalancer = UT_KD_MEDIAN;
	myHasRadius = false;
	setEntries(size);
    }
    virtual ~UT_KDTree();

    /// This must be called before querying, to build and balance the tree.
    virtual void buildIfNeeded(bool enable_multithreading = true)
    {
        balance(enable_multithreading);
    }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += mySplits.getMemoryUsage(false);
        mem += myLock.getMemoryUsage(false);
        if (myList)
            mem += sizeof(myList[0])*myEntries;
        return mem;
    }

    /// setEntries instructs the KD Tree about the number of points
    /// that it should read back from its callbacks.
    /// Note that each call to setEntries has O(size) cost!  Thus,
    ///   for (i = 0; i < n; i++) tree.setEntries(i);
    /// is O(n^2).  There isn't a good reason for this (as one could instead
    /// just mark the tree as dirty until the next query, much like
    /// already happens with rebalancing) but it is the way things are so be
    // /careful.
    void		 setEntries(size_t size);
    size_t		 getEntries() const	{ return myFullEntries; }

    /// Sometimes, a tree might have new points added since it's construction.
    /// Rather than re-balancing the tree (which can be relatively expensive),
    /// we have allow N points to be added to the tree without re-balancing.
    /// To add entries to the tree, simply call "growEntries" with the number
    /// of points being added.  If the size of the unsorted pool exceeds the
    /// re-balance count, then the tree will be re-balanced.
    ///
    /// The rebalance count is automatically scaled as the tree is constructed,
    /// so it shouldn't be required to set the count (unless you know what
    /// you're doing).
    ///
    /// To force a re-build of the tree, simply call setEntries(), or flag the
    /// tree as dirty.
    void		 growEntries(size_t amount);
    size_t		 getRebalanceCount() const
			 {
			     return myRebalanceCount;
			 }
    void		 setRebalanceCount(size_t count);

    /// Sets the KD-tree balancing algorithm.
    /// - UT_KD_MEDIAN: splits at the median point
    /// - UT_KD_CENTROID: splits at the spatial centroid
    /// - UT_KD_SAH: splits at SAH optimal splitting point
    /// The UT_KD_SAH (surface area heuristic) algorithm builds more
    /// efficient trees but they are slower to construct.  The default is
    /// UT_KD_MEDIAN.
    void		 setBalancer(ut_KDBalancer balance)
			 {
			     myBalancer = balance;
			 }
    /// Sets the maximum number of nodes to be stored in a leaf.  Smaller
    /// values will produce deeper and more memory-hungry trees.
    void		 setMaxLeafNodes(int max_leaf_nodes)
			 {
			     UT_ASSERT_P(max_leaf_nodes >= 3);
			     myMaxLeafNodes = SYSmax(3, max_leaf_nodes);
			 }

    /// Marks the tree as dirty.  Note, however, that this has
    /// O(size) time cost.
    void		 flagDirty()
			 {
			     // Re-build the lists
			     setEntries(myFullEntries);
			 }

    /// The compare function should compare the distance between two points
    /// (given by idx0 and idx1) in the dimension specified.  The dimension will
    /// be between 0 and 3.  For example,
    ///		comparePosition(...) {
    ///		   fpreal delta = myP[idx0](dim) - myP[idx1](dim);
    ///		   return delta < 0 ? -1 : (delta > 0 ? 1 : 0);
    ///		}
    virtual int			 comparePosition(int idx0,
						 int idx1, int dim) const = 0;
    /// Return the position associated with the given point
    virtual const float		*getP(int idx) const = 0;

    /// If each point in the KD Tree has a radius associated with it, then this
    /// method should return true.  Adding a per-point radius can affect
    /// performance adversely.
    virtual bool		 pointsHaveRadius() const { return false; }
    /// Return the radius associated with the point in question
    virtual fpreal		 getRadius(int /*idx*/) const { return 0; }

    /// Returns whether the given index should be considered in searches
    virtual bool	 isValid(int /*idx*/) const
			 { return true; }
    virtual bool	 isValid(int idx, const UT_KDQueryPt & /*pt*/) const
			 { return isValid(idx); }

    /// Return the maximum number of invalid indices that should be
    /// considered before bailing.  Return -1 for no limit.
    ///		maxn - the search limit
    virtual int		 getInvalidLimit(int maxn) const { return -1; }

    /// Find the closest node to the position specified.  This method
    /// returns -1 if no photon is found.
    /// NOTE THAT max_distance_squared IS SQUARED DISTANCE.
    template <typename QueryPoint>
    int			 findClosest(const QueryPoint &pt,
				     fpreal max_distance_squared);
    template <typename QueryPoint>
    int			 findClosestQueue(const QueryPoint &pt,
				     ut_KDPQueue &queue,
				     fpreal max_distance_squared);

    /// Find the closest N (max_nodes) nodes to the position given.  Only points
    /// within the sphere defined by max_distance_squared will be considered.
    /// NOTE THAT max_distance_squared IS SQUARED DISTANCE.
    template <typename QueryPoint>
    int			 findClosest(UT_IntArray &list,
				     const QueryPoint &pt,
				     fpreal max_distance_squared,
				     int    max_nodes,
				     bool   sorted = true);
    template <typename QueryPoint>
    int			 findClosestQueue(UT_IntArray &list,
				     const  QueryPoint &pt,
				     ut_KDPQueue &q,
				     fpreal max_distance_squared,
				     int    max_nodes,
				     bool   sorted = true);

    /// Find the closest N (max_nodes) nodes to the position given.  Only points
    /// within the sphere defined by max_distance_squared will be considered.
    /// NOTE THAT max_distance_squared IS SQUARED DISTANCE.
    template <typename QueryPoint>
    int			 findClosest(UT_IntArray &list,
				     UT_FloatArray &dist,
				     const  QueryPoint &pt,
				     fpreal max_distance_squared,
				     int    max_nodes,
				     bool   sorted = true);
    template <typename QueryPoint>
    int			 findClosestQueue(UT_IntArray &list,
				     UT_FloatArray &dist,
				     const  QueryPoint &pt,
				     ut_KDPQueue &q,
				     fpreal max_distance_squared,
				     int    max_nodes,
				     bool   sorted = true);
    template <typename QueryPoint>
    int			 findNClosest(UT_IntArray &list,
				     const	  QueryPoint &pt,
				     int	  max_nodes)
			 {
			     return findClosest(list, pt, 1e37, max_nodes);
			 }
    template <typename QueryPoint>
    int			 findAllClosest(UT_IntArray &list,
				     const QueryPoint &pt,
				     fpreal max_distance_squared)
			 {
			     return findClosest(list, pt, max_distance_squared,
						myFullEntries, false);
			 }

    /// Interface for aggregate data used by volume filtering
    class VolumeData {
    public:
	virtual ~VolumeData() {}
	virtual void sum(const VolumeData &data, float weight) = 0;
	virtual void scale(float scale) = 0;
    };

    /// Interface for tree traversals
    class Visitor {
    public:
	virtual ~Visitor() {}

	/// Before traversal, this method is called to indicate the total
	/// number of nodes (internal + leaf) that will be traversed.
	/// nodeid passed to the visit functions will take on values in the
	/// range [0, num_nodes-1].
	virtual void	setNumNodes(int num_nodes)
	{
	}

	/// Traverses a leaf node in the tree.  Leaf nodes contain a list
	/// of points (specified by point_list and size) that is disjoint
	/// from other leaf nodes.
	virtual void	visitLeaf(int nodeid,
				  const int *point_list, int size,
				  const UT_BoundingBox &box) = 0;

	/// Post-traverses an internal node (left and right are guaranteed
	/// to have been traversed before this method is called)
	virtual void	postVisitInternal(int nodeid, int leftid, int rightid,
					  const UT_BoundingBox &box,
					  int axis, float split)
	{
	}
    };

    /// Traverse the KD Tree in depth first order.  This routine only works
    /// on 3D trees.
    /// NOTE: This will call balance() if the tree isn't balanced.
    void		 traverse(Visitor &visitor);

    /// This class knows how to compute aggregate volume data (using a tree
    /// traversal) and also provides an accessor to the computed data.
    class AggregateVolume : public Visitor {
    public:
	virtual const VolumeData	&getData(int nodeid) const = 0;
    };

    ///
    /// Filtered evaluation for 3D volumes.  This method interprets the
    /// KD-Tree as a volume, and filters aggregate point data using the
    /// volumetric coverage for aggregate nodes in the tree.  The max_nodes
    /// parameter controls the approximate number of points that should be
    /// averaged to produce the filtered result.  The actual filter radius
    /// used will depend on max_nodes and on the size of KD-Tree nodes
    /// close to the query point.  Since this is an approximate query, the
    /// actual number of points that contribute to the result may be larger
    /// than max_nodes.
    ///
    /// This method relies on implemented subclasses for AggregateVolume
    /// and for VolumeData.  You must compute aggregate volume data before
    /// calling this method.
    ///
    /// UT_KDTree			tree;
    /// AggregateVolumeSubclass		aggdata;
    /// VolumeDataSubclass		data;
    ///
    /// // Store aggregate values (do this once)
    /// kdtree.traverse(aggdata);
    ///
    /// // Filter aggregate data (do this many times)
    /// kdtree.filterVolume(data, pos, filter, aggdata, max_nodes);
    ///
    void		 filterVolume(VolumeData &data,
				      const UT_Vector3 &pos,
				      const UT_Filter &filter,
				      const AggregateVolume &aggdata,
				      int max_nodes);

    const fpreal	*getBoxMin();
    const fpreal	*getBoxMax();

    /// This allows you to create a search queue so you can maintain
    /// it over time, avoiding the cost of reallocing for each search.
    /// NOTE THAT max_distance_squared IS SQUARED DISTANCE.
    static ut_KDPQueue	*createQueue();
    static void		 destroyQueue(ut_KDPQueue *q);

protected:
    //
    // KD tree splits
    //

    class KDSplit {
    public:
	KDSplit()
	{
	    myLeft = myRight = -1;
	    mySplitIdx = 0;
	    myAxis = 0;
	}
	void init(int left, int right,
		  fpreal off, fpreal radius, int mid, int axis)
	{
	    myLeft = left;
	    myRight = right;
	    myOffset = off;
	    myRadius = radius;
	    mySplitIdx = mid;
	    myAxis = axis;
	}

	int	 left() const		{ return myLeft; }
	int	 right() const		{ return myRight; }
	fpreal	 offset() const		{ return myOffset; }
	fpreal	 radius() const		{ return myRadius; }
	int	 splitIdx() const	{ return mySplitIdx; }
	int	 axis() const		{ return myAxis; }

    private:
	int		 myLeft;
	int		 myRight;
	fpreal		 myOffset;

	/// This stores the maximum radius of any children of this node.  The
	/// idea is that this is the maximum distance you can be on the wrong
	/// side of a splitting plane and still be legal.
	fpreal		 myRadius;
	int		 mySplitIdx;
	int		 myAxis;
    };

    int			 getHead() const { return mySplits.entries() ? 0 : -1; }

    // Dummy definition required for tube, triangle, and tetrahedron searches
    bool		 isValid(int node, const UT_KDTubeQuery &) const
			 { return true; }
    bool		 isValid(int node, const UT_KDLineQuery &) const
			 { return true; }
    bool		 isValid(int node, const UT_KDTriQuery &) const
			 { return true; }
    bool		 isValid(int node, const UT_KDTetQuery &) const
			 { return true; }
    bool		 isValid(int node, const UT_KDQueryPtUnitWrap &) const
			 { return true; }
#if 0
    bool		 isValid(int node, const UT_KDConeQuery &q) const
    {
        if (!isValid(node))
            return false;

        float dist = q.distOutsideCone(getP(node));
        if (dist <= 0)
            return true;
        if (!myHasRadius)
            return false;
        return getRadius(node) > dist;
    }
#endif

    template <typename QueryPoint>
    int			 findClosest(ut_KDPQueue &list, const QueryPoint &pt);
    template <typename QueryPoint>
    void		 recurseFind(ut_KDPQueue &list, const QueryPoint &pt,
				     int split, int lo, int hi) const;
    template <typename QueryPoint>
    void		 recurseFindTube(
				     ut_KDPQueue &list, const QueryPoint &pt,
				     int split, int lo, int hi) const;
    template <typename QueryPoint>
    void		 recurseFindTri(
				     ut_KDPQueue &list, const QueryPoint &pt,
				     int split, int lo, int hi) const;
    template <typename QueryPoint>
    void		 findInLeaf(ut_KDPQueue &list, const QueryPoint &pt,
				     int lo, int hi, int invalid_limit,
				     int &invalid) const;

    bool		 isBalanced() const	{ return myBalanced; }

    void		 traverseRecursive(Visitor &visitor,
					   int split, int nodeid,
					   UT_BoundingBox &box,
					   int lo, int hi);

    void		 computeBox(int start_index=0);
    bool		 isBoxClose(const fpreal *P, fpreal qd, fpreal r) const;

    /// Creates a KD tree from an unsorted set of points.
    /// Given a list of points, we:
    /// 1) Find the dimension in which there is most change.
    /// 2) Use the balancing algorithm to choose a splitting plane and
    ///    partition myList into left and right portions.
    /// 3) Store the splitting information in mySplits
    /// 4) Recurse on the two halves.
    /// This is repeated until myMaxLeafNodes is reached where we just
    /// leave it as a linear list.
    void		 balance(bool enable_multithreading=true);
    /// Balances a subset, returning the maximum radius of that subset.
    /// If splitlock is NULL, the balancing will all be done serially
    /// instead of in parallel.
    void balanceSet(int &split, fpreal &radius, int *list, int entries,
                    fpreal* boxmin, fpreal* boxmax, UT_Lock* splitlock);

    /// Stores the bounding box of all of the points in the KD tree, including
    /// their individual radii, in each of the possible dimensions.
    /// Note that during balanceSet these values are temporarily altered
    /// before recursion, so don't be too shocked.
    fpreal		 myBMin[UT_KD_MAXDIM];
    fpreal		 myBMax[UT_KD_MAXDIM];

    /// Nodes in a KD tree have two indices.  One is the index that
    /// they were added in.  This is the index used in getP, for example.
    /// The other is there location in the binary heap.  myList takes
    /// a binary heap location and returns the index useable for
    /// getP().
    int			*myList;

    /// Split information stored as a tree structure.  This stores the
    /// midpoints, splitting offset, axis, and max radius.
    UT_Array<KDSplit>	mySplits;

    /// For protecting tree balancing and bounding box computation
    UT_Lock		 myLock;

    /// This marks the number of entries that have been added to our tree
    size_t		 myEntries;

    /// This marks the total number of entries in this data structure.
    /// The difference between this and myEntries is the unsorted
    /// chaff at the end.
    size_t		 myFullEntries;
    size_t		 myRebalanceCount;
    int			 myDim;
    int			 myMaxLeafNodes;
    bool		 myBalanced;
    bool		 myBoxComputed;

    /// A faster way to evaluate pointsHaveRadius() at runtime.
    bool		 myHasRadius;
    ut_KDBalancer	 myBalancer;
};

#endif
