/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAInherit.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAInherit__
#define __GT_DAInherit__

#include "GT_API.h"
#include "GT_DataArray.h"

/// @brief Base class for a data array which references another data array
class GT_API GT_DAInherit : public GT_DataArray {
public:
    /// Default constructor
    GT_DAInherit() : myData() {}
    /// Convenience constructor
    GT_DAInherit(const GT_DataArrayHandle &data) : myData(data) { }
    /// Copy constructor
    GT_DAInherit(const GT_DAInherit &src)
	: myData(src.myData)
	, GT_DataArray()
    {
    }
    virtual ~GT_DAInherit();

    void	init(const GT_DataArrayHandle &data)
		{
		    myData = data;
		}

    /// @{
    /// Methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const
			    { return myData->getStorage(); }
    virtual GT_Size	getTupleSize() const
			    { return myData->getTupleSize(); }
    virtual GT_Type	getTypeInfo() const
			    { return myData->getTypeInfo(); }
    virtual int64	getMemoryUsage() const
			    { return myData->getMemoryUsage() + sizeof(*this); }

    /// Return "true" if there's pointer aliasing
    virtual bool	getPointerAliasing(const void *data) const
			    { return myData->getPointerAliasing(data); }
    virtual bool	isValid() const { return myData && myData->isValid(); }
    virtual int64	getDataId() const { return myData->getDataId(); }
    virtual void	updateGeoDetail(const GU_ConstDetailHandle &dtl,
					const char *attrib_name,
					GT_Owner attrib_owner,
					const int expected_size)
			{
			    myData->updateGeoDetail(dtl, attrib_name,
						    attrib_owner,
						    expected_size);
			}
    /// @}

    /// @{
    /// Implement the specific methods of the inherited array.
    /// @param MAP_INDEX @n
    ///		A function which takes an offset an maps it to the index in the
    ///		source array
    /// @param SIZE @n
    ///		The size of this array
#define GT_IMPL_INHERIT_ARRAY(MAP_INDEX, SIZE)	\
    virtual GT_Size	entries() const	{ return SIZE; } \
    virtual fpreal16	getF16(GT_Offset offset, int idx=0) const \
			    { return myData->getF16(MAP_INDEX(offset), idx); } \
    virtual fpreal32	getF32(GT_Offset offset, int idx=0) const \
			    { return myData->getF32(MAP_INDEX(offset), idx); } \
    virtual fpreal64	getF64(GT_Offset offset, int idx=0) const \
			    { return myData->getF64(MAP_INDEX(offset), idx); } \
    virtual uint8	getU8(GT_Offset offset, int idx=0) const \
			    { return myData->getU8(MAP_INDEX(offset), idx); } \
    virtual int32	getI32(GT_Offset offset, int idx=0) const \
			    { return myData->getI32(MAP_INDEX(offset), idx); } \
    virtual int64	getI64(GT_Offset offset, int idx=0) const \
			    { return myData->getI64(MAP_INDEX(offset), idx); } \
    virtual GT_String	getS(GT_Offset offset, int idx=0) const \
			    { return myData->getS(MAP_INDEX(offset), idx); } \
    virtual GT_Size	getStringIndexCount() const \
			    { return myData->getStringIndexCount(); } \
    virtual GT_Offset	getStringIndex(GT_Offset offset, int idx) const \
			    { return myData->getStringIndex(MAP_INDEX(offset), idx); } \
    virtual void	getIndexedStrings(UT_StringArray &strings, \
				    UT_IntArray &indices) const \
			    { myData->getIndexedStrings(strings, indices); } \
    virtual void	doImport(GT_Offset off, uint8 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); } \
    virtual void	doImport(GT_Offset off, int8 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); } \
    virtual void	doImport(GT_Offset off, int16 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); } \
    virtual void	doImport(GT_Offset off, int32 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); } \
    virtual void	doImport(GT_Offset off, int64 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); } \
    virtual void	doImport(GT_Offset off, fpreal16 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); } \
    virtual void	doImport(GT_Offset off, fpreal32 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); } \
    virtual void	doImport(GT_Offset off, fpreal64 *d, GT_Size sz) const \
			    { myData->import(MAP_INDEX(off), d, sz); }
    /// @}

protected:
    
    GT_DataArrayHandle	myData;
};

#endif

