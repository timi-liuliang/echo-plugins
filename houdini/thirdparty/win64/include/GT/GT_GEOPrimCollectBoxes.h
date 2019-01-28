/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimCollectBoxes.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimCollectBoxes__
#define __GT_GEOPrimCollectBoxes__

#include "GT_API.h"
#include "GT_GEOPrimCollect.h"

/// Class to collect boxes and centroids of boxes.
class GT_API GT_GEOPrimCollectBoxes
{
public:
    GT_GEOPrimCollectBoxes(const GT_GEODetailListHandle &geometry,
			   bool add__primitive_id);
    ~GT_GEOPrimCollectBoxes();

    void	appendBox(const UT_BoundingBox &box,
			const UT_Matrix4F &xform,
			GA_Offset prim_offset=GA_INVALID_OFFSET,
			GA_Offset vertex_offset=GA_INVALID_OFFSET,
			GA_Offset point_offset=GA_INVALID_OFFSET);
    void	appendBox(const UT_BoundingBox &box,
			const UT_Matrix4D &xform,
			GA_Offset prim_offset=GA_INVALID_OFFSET,
			GA_Offset vertex_offset=GA_INVALID_OFFSET,
			GA_Offset point_offset=GA_INVALID_OFFSET);
    void	appendCentroid(const UT_BoundingBox &box,
			const UT_Matrix4F &xform,
			GA_Offset prim_offset=GA_INVALID_OFFSET,
			GA_Offset vertex_offset=GA_INVALID_OFFSET,
			GA_Offset point_offset=GA_INVALID_OFFSET);
    void	appendCentroid(const UT_BoundingBox &box,
			const UT_Matrix4D &xform,
			GA_Offset prim_offset=GA_INVALID_OFFSET,
			GA_Offset vertex_offset=GA_INVALID_OFFSET,
			GA_Offset point_offset=GA_INVALID_OFFSET);

    GT_PrimitiveHandle	getBoxes() const;
    GT_PrimitiveHandle	getCentroids() const;
    GT_PrimitiveHandle	getPrimitive() const;

private:
    const GT_GEODetailListHandle myGeometry;
    UT_Array<UT_Matrix4F>	myBoxTransforms;
    GT_GEOOffsetList		myBoxOffsets;
    GT_GEOOffsetList		myBoxVertices;
    GT_GEOOffsetList		myBoxPoints;
    GT_DataArrayHandle		myP;
    UT_Array<UT_Matrix4F>	myCentroidTransforms;
    GT_GEOOffsetList		myCentroidOffsets;
    GT_GEOOffsetList		myCentroidVertices;
    GT_GEOOffsetList		myCentroidPoints;
    bool			myPrimitiveIds;
};

#endif
