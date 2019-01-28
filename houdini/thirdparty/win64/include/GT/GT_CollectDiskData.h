/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_CollectDiskData.h ( GT Library, C++)
 *
 * COMMENTS:
 */
#ifndef GT_CollectDiskData_h
#define GT_CollectDiskData_h

#include "GT_GEODetailList.h"
#include "GT_GEOPrimCollect.h"
#include "GT_GEOSupport.h"
#include "GT_RefineParms.h"

#include <GEO/GEO_PackedTypes.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_VectorTypes.h>

class GU_PackedDiskSequence;
class GU_PackedDisk;
class GU_PrimPacked;

class GT_CollectDiskData : public GT_GEOPrimCollectData
{
public:
    GT_CollectDiskData(const GT_GEODetailListHandle &geometry,
		       const GT_RefineParms *parms)
	: GT_GEOPrimCollectData()
	, myGeometry(geometry)
	, myRefineParms(parms)
	{}

    void		append(const GU_ConstDetailHandle &prim_gdh,
			       const GU_PrimPacked &prim);
    GT_PrimitiveHandle	finish();
    
private:
    GT_GEODetailListHandle	myGeometry;
    const GT_RefineParms       *myRefineParms;
    
    class Item
    {
    public:
	Item()
	    : myPrimSeq(nullptr), myPrimDisk(nullptr), myLODBits(0), myIndex(0)
	    {}
	
	UT_Matrix4DArray	 myTransforms;
	GT_GEOOffsetList	 myPrimOffsets;
	GT_GEOOffsetList	 myVertexOffsets;
	UT_Array<GEO_ViewportLOD> myLODs;
	const GU_PackedDiskSequence *myPrimSeq;
	const GU_PackedDisk	*myPrimDisk;
	exint			 myIndex;
	int			 myLODBits;
    };
    UT_StringMap<Item>		myFiles;
};

#endif
