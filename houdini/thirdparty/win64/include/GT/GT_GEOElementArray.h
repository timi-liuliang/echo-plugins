/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOElementArray.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOElementArray__
#define __GT_GEOElementArray__

#include "GT_API.h"
#include <UT/UT_Assert.h>
#include "GT_DataArray.h"
#include "GT_GEOSupport.h"
#include <GA/GA_AIFTuple.h>
#include <GA/GA_AIFStringTuple.h>
#include <GU/GU_Detail.h>
#include <GA/GA_Handle.h>

class GA_AIFSharedStringTuple;
class GA_Attribute;

/// @brief A GT Data Array to extract values from a point/primitive attribute.
class GT_API GT_GEOElementArray : public GT_DataArray
{
public:
    enum GT_GEOAttributeOwner
    {
	GT_GEOATTRIB_INVALID = -1,

	GT_GEOATTRIB_VERTEX = GA_ATTRIB_VERTEX,
	GT_GEOATTRIB_POINT = GA_ATTRIB_POINT,
	GT_GEOATTRIB_PRIMITIVE = GA_ATTRIB_PRIMITIVE,
	GT_GEOATTRIB_GLOBAL = GA_ATTRIB_GLOBAL,
	GT_GEOATTRIB_DETAIL = GA_ATTRIB_DETAIL,

	// Point attribute indexed by vertex
	GT_GEOATTRIB_VERTEX_POINT,
	// Primitive attribute index by vertex
	GT_GEOATTRIB_VERTEX_PRIMITIVE,
	// Detail attribute indexed by vertex
	GT_GEOATTRIB_VERTEX_DETAIL,

	// Detail attribute indexed by primitive
	GT_GEOATTRIB_PRIMITIVE_DETAIL,
    };
    /// The @c index_mode is used to lookup values.  This does @b not
    /// necessarily have to match the attribute owner, but it must be one of
    /// the following.
    /// - @c index_mode := point, @c attribute := point @n
    /// - @c index_mode := vertex, @c attribute := point @n
    /// - @c index_mode := vertex, @c attribute := primitive @n
    /// - @c index_mode := vertex, @c attribute := vertex @n
    /// - @c index_mode := primitive, @c attribute := primitive @n
    /// - @c index_mode := detail, @c attribute := detail @n
    GT_GEOElementArray(const GU_ConstDetailHandle &gdp,
                        const GA_Attribute *attrib,
			const GT_GEOOffsetList &offsets,
			GA_AttributeOwner offset_owner);
    virtual ~GT_GEOElementArray();

    virtual const char *className() const { return "GT_GEOElementArray"; }

    /// @{
    /// Query methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const
			    { return myStorage; }
    virtual GT_Size	entries() const
			{
			    return myOffsets.entries();
#if 0
			    switch (myOffsetOwner)
			    {
				case GA_ATTRIB_VERTEX:
				    return myGdp->getNumVertexOffsets();
				case GA_ATTRIB_POINT:
				    return myGdp->getNumPointOffsets();
				case GA_ATTRIB_PRIMITIVE:
				    return myGdp->getNumPrimitiveOffsets();
				case GA_ATTRIB_DETAIL:
				    return 1;
				default:
				    UT_ASSERT(0);
			    }
#endif
			    return 0;
			}
    virtual GT_Size	getTupleSize() const
			{
			    return myTupleSize;
			}
    virtual GT_Type	getTypeInfo() const
			{
			    GA_TypeInfo info = myAttribute->getTypeInfo();
			    // avoid transforming attributes like "rest"
			    switch(info)
			    {
				case GA_TYPE_POINT:
				case GA_TYPE_HPOINT:
				case GA_TYPE_NORMAL:
				case GA_TYPE_VECTOR:
				    if(!myAttribute->needsTransform())
					return GT_TYPE_NONE;
				    break;

				default:
				    break;
			    }
			    return mapTypeInfo(info, myTupleSize);
			}
    virtual int64	getMemoryUsage() const
			    { return sizeof(*this); }
    virtual bool	isValid() const { return myAttribute != NULL; }
    virtual bool	hasArrayEntries() const 
			{ 
			    return myFA.isValid() || 
				   myIA.isValid() ||
				   mySSArray; 
			}
    virtual void	updateGeoDetail(const GU_ConstDetailHandle &dtl,
					const char *name,
					GT_Owner attrib_scope,
					const int expected_size);

    /// @}

    /// @{
    /// Access methods defined on GT_DataArray
    virtual fpreal16	getF16(GT_Offset offset, int idx) const
			    { return getValue_t<fpreal32>(offset, idx); }
    virtual fpreal32	getF32(GT_Offset offset, int idx) const
			    { return getValue_t<fpreal32>(offset, idx); }
    virtual fpreal64	getF64(GT_Offset offset, int idx) const
			    { return getValue_t<fpreal64>(offset, idx); }
    virtual uint8	getU8(GT_Offset offset, int idx) const
			    { return getValue_t<int32>(offset, idx); }
    virtual int32	getI32(GT_Offset offset, int idx) const
			    { return getValue_t<int32>(offset, idx); }
    virtual int64	getI64(GT_Offset offset, int idx) const
			    { return getValue_t<int64>(offset, idx); }
    virtual GT_String	getS(GT_Offset offset, int idx) const
			{
			    if (mySTuple)
			    {
				GA_Offset	 si = getDataOffset(offset);
				return mySTuple->getString(myAttribute, si,idx);
			    }
			    return NULL;
			}
    virtual bool	getSA(UT_StringArray &a, GT_Offset offset) const
			{
			    if (mySSArray)
			    {
				GA_Offset	si = getDataOffset(offset);
				mySSArray->getString(myAttribute, si, a);
				return true;
			    }
			    return false;
			}
    virtual GT_Size	getStringIndexCount() const;
    virtual GT_Offset	getStringIndex(GT_Offset offset, int idx=0) const;
    virtual void	getIndexedStrings(UT_StringArray &,
				    UT_IntArray &) const;

    virtual bool	getFA32(UT_ValArray<fpreal32> &a, GT_Offset offset) const
			{ return getArrayValue_t<fpreal32>(a,offset); }
    virtual bool	getIA32(UT_ValArray<int32> &a, GT_Offset offset) const
			{ return getArrayValue_t<int32>(a,offset); }
    /// @}

    virtual const uint8		*get(GT_Offset i, uint8 *store, int sz) const
				    { return GT_DataArray::get(i, store, sz); }
    virtual const int8		*get(GT_Offset i, int8 *store, int sz) const
				    { return GT_DataArray::get(i, store, sz); }
    virtual const int16		*get(GT_Offset i, int16 *store, int sz) const
				    { return GT_DataArray::get(i, store, sz); }
    virtual const int32		*get(GT_Offset i, int32 *store, int sz) const
				    { return getTuple_t(i, store, sz); }
    virtual const int64		*get(GT_Offset i, int64 *store, int sz) const
				    { return getTuple_t(i, store, sz); }
    virtual const fpreal16	*get(GT_Offset i, fpreal16 *store, int z) const
				    { return GT_DataArray::get(i, store, z); }
    virtual const fpreal64	*get(GT_Offset i, fpreal64 *store, int z) const
				    { return getTuple_t(i, store, z); }
    virtual const fpreal32	*get(GT_Offset idx, fpreal32 *store, int z) const
    {
	if (z == 3 && myV3.isValid())
	{
	    UT_Vector3	v = myV3.get(getDataOffset(idx));
	    store[0] = v.x();
	    store[1] = v.y();
	    store[2] = v.z();
	    return store;
	}
	if (myF.isValid())
	{
	    GA_Offset	o = getDataOffset(idx);
	    for (int i = 0; i < z; ++i)
		store[i] = myF.get(o, i);
	    return store;
	}
	return getTuple_t(idx, store, z);
    }

    /// @{
    /// Optimized filling methods
    virtual void doImport(GT_Offset idx, uint8 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int8 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int16 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int32 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int64 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, fpreal16 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, fpreal32 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, fpreal64 *data, GT_Size size) const;
    virtual void doImportArray(GT_Offset idx, UT_ValArray<fpreal32> &data) const;
    virtual void doImportArray(GT_Offset idx, UT_ValArray<int32> &data) const;
    virtual void doFillArray(uint8 *data, GT_Offset start, GT_Size length,
			     int tuple_size, int stride) const;
    virtual void doFillArray(int8 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(int16 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(int32 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(int64 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(fpreal16 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(fpreal32 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillArray(fpreal64 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const;
    virtual void doFillVec3BBox(fpreal32 *data, GT_Offset start, GT_Size length,
				UT_BoundingBoxF &bbox, int tsize, int stride);
    virtual void doFillVec3BBox(fpreal64 *data, GT_Offset start, GT_Size length,
				UT_BoundingBoxD &bbox, int tsize, int stride);
    virtual void doFillQuantizedArray(uint8 *data,
			GT_Offset start, GT_Size length,
			int tuple_size, int stride,
			fpreal black, fpreal white) const;
    virtual void doExtendedQuantizedFill(uint8 *data,
			GT_Offset start, GT_Size length,
			int tuple_size, int nrepeats, int stride,
			fpreal black, fpreal white) const;
    /// @}
protected:
    /// GT_GEOAutoNormalArray has no attribute yet when calling constructor.
    GT_GEOElementArray(const GU_ConstDetailHandle &gdp,
        const GT_GEOOffsetList &offsets,
        GA_AttributeOwner offset_owner);

    void	initAttribute(const GA_Attribute *attrib);
    void	makeInvalid();
    
    template <typename T> inline T
    getValue_t(GT_Offset offset, int idx) const
		{
		    GA_Offset		 si = getDataOffset(offset);
		    T			 val;

		    if (myTuple && myTuple->get(myAttribute, si, val, idx))
			return val;
		    return 0;
		}

    template <typename T> inline const T *
    getTuple_t(GT_Offset offset, T *store, int size) const
    {
	GA_Offset	si = getDataOffset(offset);
	if (myTuple && myTuple->get(myAttribute, si, store, size))
	    return store;
	return NULL;
    }

    template <typename T> inline bool
    getArrayValue_t( UT_ValArray<T> &val, GT_Offset offset) const
		{
		    GA_Offset		 si = getDataOffset(offset);

		    if (myNumericArray && 
			    myNumericArray->get(myAttribute, si, val))
			return true;
		    return false;
		}


    GA_Offset		getDataOffset(GT_Offset offset) const
			{
			    if (myAttributeOwner == GA_ATTRIB_DETAIL)
				return GA_Offset(0);

			    GA_Offset gaoff = myOffsets(offset);
			    if (myOffsetOwner == GA_ATTRIB_VERTEX &&
				    myOffsetOwner != myAttributeOwner)
			    {
				    if (myAttributeOwner == GA_ATTRIB_POINT)
				    {
					gaoff = myGdp->vertexPoint(gaoff);
				    }
				    else
				    {
					UT_ASSERT(myAttributeOwner ==
						GA_ATTRIB_PRIMITIVE);
					gaoff = myGdp->vertexPrimitive(gaoff);
				    }
			    }
			    return gaoff;
			}

    GT_Type		mapTypeInfo(GA_TypeInfo type, int tsize) const;

    GU_ConstDetailHandle		 myGdh;
    const GU_Detail			*myGdp;
    GT_GEOOffsetList			 myOffsets;
    const GA_Attribute			*myAttribute;
    const GA_AIFTuple			*myTuple;
    const GA_AIFStringTuple		*mySTuple;
    const GA_AIFSharedStringTuple	*mySSTuple;
    const GA_AIFNumericArray		*myNumericArray;
    const GA_AIFSharedStringArray	*mySSArray;
    GA_AttributeOwner			 myOffsetOwner;	// What type of offsets
    GA_AttributeOwner			 myAttributeOwner;	// Type of data
    GA_ROHandleF			 myF;
    GA_ROHandleI			 myI;
    GA_ROHandleV3			 myV3;
    GA_ROHandleFA			 myFA;
    GA_ROHandleIA			 myIA;
    GT_Storage				 myStorage;
    int					 myTupleSize;
};

/// This class is specific to managing a detached, auto-generated
/// normal attribute, when no point, vertex, attrib_owner, or offset_owner
/// normals are present on the original detail.
class GT_API GT_GEOAutoNormalArray : public GT_GEOElementArray
{
public:
    GT_GEOAutoNormalArray(
        const GU_ConstDetailHandle &gdp,
        GA_AttributeOwner attrib_owner,
        const GT_GEOOffsetList &offsets,
        GA_AttributeOwner offset_owner,
        float cusp_angle);

    virtual ~GT_GEOAutoNormalArray();

    virtual int64 getMemoryUsage() const
    { return sizeof(*this); }


    virtual void updateGeoDetail(
        const GU_ConstDetailHandle &dtl,
        const char *name,
        GT_Owner attrib_scope,
        const int expected_size);
private:
    void createAttribute();

    int64 myPDataId;
    int64 myTopologyDataId;
    int64 myPrimitiveListDataId;
    const float myCuspAngle;
    const GA_AttributeOwner myAttribOwner;
};

#endif
