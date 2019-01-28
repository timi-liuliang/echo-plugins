/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimMeshSplit.h ( GT Library, C++)
 *
 * COMMENTS:	Mesh splitting class
 */

#ifndef __GT_PrimMeshSplit__
#define __GT_PrimMeshSplit__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_DataArray.h"
#include <UT/UT_BoundingBox.h>
#include <UT/UT_ParallelUtil.h>		// For UT_BlockedRange

typedef UT_BlockedRange<GT_Offset>	GT_PrimMeshSplitRange;

/// Parent class to split a mesh.
class GT_API GT_PrimMeshSplitParent : public GT_Primitive
{
public:
    /// Create a split parent for the mesh given N elemental primitives
    GT_PrimMeshSplitParent(GT_Primitive *mesh, GT_Size nelements);
    GT_PrimMeshSplitParent(const GT_PrimMeshSplitParent &src);
    /// d-tor
    virtual ~GT_PrimMeshSplitParent();

    virtual const char *className() const { return "GT_PrimMeshSplitParent"; }
    
    /// @{
    /// Methods from GT_Primitive
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;
    virtual void	enlargeBounds(UT_BoundingBox boxes[], int nsegs) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    /// @}

    /// The range refine method should refine a range of elements into
    /// individual elements.   If the method returns false, the mesh will be
    /// split into sub-meshes with smaller ranges.
    virtual bool	rangeRefine(const GT_PrimMeshSplitRange &range,
				GT_Refine &refiner,
				const GT_RefineParms *parms) const = 0;

    /// Enlarge bounding boxes for each segment over the range of elements
    /// There is be a bounding box for each segment.
    virtual void	rangeEnlargeBounds(const GT_PrimMeshSplitRange &range,
				UT_BoundingBox &full,
				UT_BoundingBox boxes[], int nsegs) const = 0;

    /// Enlarge the bounding boxes for each element in the range.  There is a
    /// bounding box for each element.  Only the motion segment given should be
    /// considered.
    virtual void	rangeElementBounds(const GT_PrimMeshSplitRange &range,
				UT_BoundingBox boxes[], int segment) const = 0;

    /// Given a bounding box for the range of primitives, partition
    /// the range of primitives by pivoting the indices.
    /// The default behaviour relies on the bounding boxes of primtives at
    /// segment 0.
    virtual bool	partitionRange(const GT_PrimMeshSplitRange &range,
				    const UT_BoundingBox &full_box,
				    GT_PrimMeshSplitRange &left,
				    GT_PrimMeshSplitRange &right,
				    int start_segment=0, int end_segment=0);
    /// Lookup an index
    GT_Offset		getIndex(GT_Offset index) const
			{
			    myIndices->import(index, &index, 1);
			    return index;
			}

    /// Access parent primitive
    const GT_PrimitiveHandle	&getBaseMesh() const	{ return myMesh; }
protected:
    /// Determine which elements should be on the left side and which should be
    /// on the right side.
    /// By default, this will compute bounding boxes for the elements and split
    /// based on the largest box dimension.
    virtual void	partitionElements(const GT_PrimMeshSplitRange &range,
				    const UT_BoundingBox &full, bool *left,
				    int start_seg=0, int end_seg=0) const;


    GT_PrimitiveHandle	myMesh;
    GT_DataArrayHandle	myIndices;
};

class GT_API GT_PrimMeshSplitChild : public GT_Primitive
{
public:
    /// Create a split primitive for the given mesh
    GT_PrimMeshSplitChild(GT_PrimMeshSplitParent *parent,
			const GT_PrimMeshSplitRange &range);
    GT_PrimMeshSplitChild(const GT_PrimMeshSplitChild &src);
    /// d-tor
    virtual ~GT_PrimMeshSplitChild();

    virtual const char *className() const { return "GT_PrimMeshSplitChild"; }
    
    /// It's up the subclass to perform refinement.  But, to implement a simple
    /// split into further children, you might do something like: @code
    /// GT_Offset       l0, l1, r0, r1;
    /// if (getParent()->rangePartition(myBox, myRange, l0, l1, r0, r1))
    /// {
    ///     if (l1 >= l0)
    ///         refiner.addPrimitive(new SplitPrimitive(getParent(), l0, l1))
    ///     if (r1 >= r0)
    ///         refiner.addPrimitive(new SplitPrimitive(getParent(), r0, r1))
    /// }
    /// @endcode
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;

    /// @{
    /// Methods from GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[], int nsegs) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimMeshSplitChild(*this); }
    /// @}

    /// Convenience method to get the entries
    GT_Size		entries() const
			    { return myRange.size(); }
    /// Convenience method to map from the index in this list to the parent's
    /// index.
    GT_Offset		getIndex(GT_Offset index) const
			{
			    return getParent()->getIndex(myRange.begin()+index);
			}

    /// Get bounding box.
    /// @note This is only valid after @c enlargeBounds() has been called.
    const UT_BoundingBox	getBox() const	{ return myBox; }

protected:
    const GT_PrimMeshSplitParent *getParent() const
				{
				    return static_cast
						<const GT_PrimMeshSplitParent *>
						(myParent.get());
				}
    GT_PrimMeshSplitParent	*getParent()
				{
				    return static_cast
						<GT_PrimMeshSplitParent *>
						(myParent.get());
				}

    GT_PrimitiveHandle		myParent;
    GT_PrimMeshSplitRange	myRange;
    UT_BoundingBox		myBox;
};

#endif
