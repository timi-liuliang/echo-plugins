/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimRTree.h
 *
 * COMMENTS:    Convenience wrapper for RTree used with GA Primitives
 *
 */

#pragma once

#include "GU_API.h"
#include <GEO/GEO_Detail.h>
#include <UT/UT_Array.h>
#include <UT/UT_RTree.h>
#include <utility>

class GU_API GU_PrimRTree
{

public:
    /// Construct an RTree of primiotives in the given group.
    /// padding is relative to the bounding box radius of each box in the rtree
    /// to ensure there is always enough precision to represent a non-zero
    /// padding even for large models
    explicit	    GU_PrimRTree(
			    const GEO_Detail& gdp,
			    const GA_PrimitiveGroup *prim_grp = 0,
			    fpreal relative_padding = SYS_FTOLERANCE_D,
			    fpreal absolute_padding = 0);

    explicit	    GU_PrimRTree(
			    const GEO_Detail& gdp,
			    const GA_Range&   prim_range,
			    fpreal relative_padding = SYS_FTOLERANCE_D,
			    fpreal absolute_padding = 0);

    /// Returns the bounding box of this tree
    UT_Box	    boundingBox() const;

    // WARNING: These two functions are non re-entrant, please use the following
    // functions to get thread safe behaviour
    void	    getIntersectingPrimOffsetsUnsafe(
			    const UT_Box   &query_box,
			    GA_OffsetArray &intersecting_prims) const;
    void	    getIntersectingPrimIndicesUnsafe(
			    const UT_Box   &query_box,
			    GA_IndexArray &intersecting_prims) const;

    void	    getIntersectingPrimOffsets(
			    const UT_Box   &query_box,
			    UT_IntArray	   &box_set,
			    GA_OffsetArray &intersecting_prims) const;
    void	    getIntersectingPrimIndices(
			    const UT_Box   &query_box,
			    UT_IntArray	   &box_set,
			    GA_IndexArray &intersecting_prims) const;
    void	    getIntersectingBoxIndices(
			    const UT_Box   &query_box,
			    UT_IntArray	   &box_set) const;

    static UT_Box   getPrimBoundingBox(
			    const GEO_Detail& gdp,
			    GA_Offset prim_off,
			    fpreal relative_padding = SYS_FTOLERANCE_D,
			    fpreal absolute_padding = 0);

    /// Returns the bounding box of a given segment with some padding.
    /// This method is exposed for convenience
    static UT_Box   getSegmentBoundingBox(
			    const UT_Vector3F& a,
			    const UT_Vector3F& b,
			    fpreal relative_padding = SYS_FTOLERANCE_D,
			    fpreal absolute_padding = 0);

    /// Returns the bounding box of a given segment with some padding.
    /// This method is exposed for convenience
    static UT_Box   getTriangleBoundingBox(
			    const UT_Vector3F& a,
			    const UT_Vector3F& b,
			    const UT_Vector3F& c,
			    fpreal relative_padding = SYS_FTOLERANCE_D,
			    fpreal absolute_padding = 0);

    // Permissive member accessors. These are exposed for inspection
    const UT_RTree&		    getTree() const { return myTree; }
    const UT_RTreeBoxAssignment&    getBoxAssignment() const { return myBoxAssignment; }
    const UT_Array<UT_Box>&	    getBoxArray() const { return myBoxes.getBoxArray(); }
    const GA_OffsetArray&	    getBoxOffsetMap() const { return myBoxes.getBoxOffsetMap(); }
    const UT_Box&		    getBox(GA_Size i) const { return myBoxes.getBox(i); }
    GA_Offset			    boxOffset(GA_Size i) const { return myBoxes.boxOffset(i); }
    GA_Index			    boxIndex(GA_Size i) const { return myBoxes.boxIndex(i); }
    
    class GU_API PrimBoundingBoxes
    {
    public:
		 PrimBoundingBoxes();
	explicit PrimBoundingBoxes(const GEO_Detail& gdp,
				   const GA_PrimitiveGroup *grp,
				   fpreal relative_padding = SYS_FTOLERANCE_D,
				   fpreal absolute_padding = 0);
	explicit PrimBoundingBoxes(const GEO_Detail& gdp,
				   const GA_Range&   prim_range,
				   fpreal relative_padding = SYS_FTOLERANCE_D,
				   fpreal absolute_padding = 0);

	void	 build(const GEO_Detail& gdp,
		       const GA_PrimitiveGroup *grp,
		       fpreal relative_padding = SYS_FTOLERANCE_D,
		       fpreal absolute_padding = 0);
	void	 build(const GEO_Detail& gdp,
		       const GA_Range&   prim_range,
		       fpreal relative_padding = SYS_FTOLERANCE_D,
		       fpreal absolute_padding = 0);

	const UT_Array<UT_Box>&	    getBoxArray() const { return myBoxArray; }
	const GA_OffsetArray&	    getBoxOffsetMap() const { return myOffsetArray; }
	const UT_Box&		    getBox(GA_Size i) const { return myBoxArray[i]; }
	GA_Offset		    boxOffset(GA_Size i) const { return myOffsetArray[i]; }
	GA_Index		    boxIndex(GA_Size i) const
	{ 
	    return myPrimitiveMap ? myPrimitiveMap->indexFromOffset(boxOffset(i)) : GA_INVALID_INDEX;
       	}

    private:
	GA_OffsetArray	    myOffsetArray;
	UT_Array<UT_Box>    myBoxArray;
	const GA_IndexMap*  myPrimitiveMap;
    };

private:
    mutable UT_IntArray	    myBoxSet; // working box set
    
    // WARNING: Don't change the order of these since construction of myTree
    //		depends on myBoxes
    PrimBoundingBoxes	    myBoxes;
    UT_RTree		    myTree;
    UT_RTreeBoxAssignment   myBoxAssignment;

    // Disallow as in UT_RTree
    GU_PrimRTree();
    GU_PrimRTree(const GU_PrimRTree&);
    GU_PrimRTree& operator=(const GU_PrimRTree&);
};
