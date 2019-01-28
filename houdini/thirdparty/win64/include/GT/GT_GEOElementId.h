/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOElementId.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOElementId__
#define __GT_GEOElementId__

#include "GT_API.h"
#include "GT_DataArray.h"
#include "GT_GEOSupport.h"
#include <GU/GU_DetailHandle.h>
#include <UT/UT_Assert.h>

/// A representation of an offsets list as a GT data array.  If we're given a
/// list of vertex offsets, we can also provide the primitive/point id's.  This
/// is done for primitives like quadrics.
class GT_API GT_GEOElementId : public GT_DataArray
{
public:
    GT_GEOElementId(const GU_ConstDetailHandle &gdp,
		    const GT_GEOOffsetList &offsets,
		    GA_AttributeOwner index_type,
		    bool from_vertex);
    virtual ~GT_GEOElementId();

    virtual const char *className() const { return "GT_GEOElementId"; }
    
    /// @{
    /// Query methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const
			{
			    return myOffsets.is32Bit() ? GT_STORE_INT32
							: GT_STORE_INT64;
			}
    virtual GT_Size	getTupleSize() const	{ return 1; }
    virtual GT_Type	getTypeInfo() const	{ return GT_TYPE_HIDDEN; }
    virtual GT_Size	entries() const		{ return myOffsets.entries(); }
    virtual int64	getMemoryUsage() const;
    /// @}

    /// @{
    /// Access methods
    virtual int64	getI64(GT_Offset offset, int =0) const
			    { return getOffset(offset); }
    virtual uint8	getU8(GT_Offset offset, int idx) const
			    { return getOffset(offset); }
    virtual int32	getI32(GT_Offset offset, int idx) const
			    { return getOffset(offset); }
    virtual fpreal16	getF16(GT_Offset offset, int idx=0) const
			    { return getOffset(offset); }
    virtual fpreal32	getF32(GT_Offset offset, int idx=0) const
			    { return getOffset(offset); }
    virtual fpreal64	getF64(GT_Offset offset, int idx=0) const
			    { return getOffset(offset); }

    virtual GT_String	getS(GT_Offset, int) const		{ return NULL; }
    virtual GT_Size	getStringIndexCount() const		{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				UT_IntArray &) const	{ }
    /// @}

protected:
    // specialization for filling int32/int64
    virtual void doFillArray(int8 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(int16 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(int32 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(int64 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;

    /// @{
    /// Call base class to handle other data
    virtual void doFillArray(uint8 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 {
		     GT_DataArray::doFillArray(data, start, length,
			     tuple_size, stride);
		 }
    virtual void doFillArray(fpreal16 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 {
		     GT_DataArray::doFillArray(data, start, length,
			     tuple_size, stride);
		 }
    virtual void doFillArray(fpreal32 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 {
		     GT_DataArray::doFillArray(data, start, length,
			     tuple_size, stride);
		 }
    virtual void doFillArray(fpreal64 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 {
		     GT_DataArray::doFillArray(data, start, length,
			     tuple_size, stride);
		 }
    /// @}

private:
    int64		getOffset(GT_Offset offset) const
			{
			    GA_Offset	gaoff = myOffsets(offset);
			    if (myVertexMap)
			    {
				if (myOwner == GA_ATTRIB_POINT)
				    gaoff = myGdp->vertexPoint(gaoff);
				else
				    gaoff = myGdp->vertexPrimitive(gaoff);
			    }
			    return gaoff;
			}
    GU_ConstDetailHandle	 myGdh;
    const GU_Detail		*myGdp;
    GT_GEOOffsetList		 myOffsets;
    GA_AttributeOwner		 myOwner;
    bool			 myVertexMap;
};

#endif

