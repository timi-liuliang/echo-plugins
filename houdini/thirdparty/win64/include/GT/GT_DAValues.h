/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAValues.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAValue__
#define __GT_DAValue__

#include "GT_API.h"
#include "GT_DataArray.h"
#include <UT/UT_Assert.h>

/// @brief An array of const values.
///
/// The data in the array is @b not owned by the array, but needs to exist
/// while the array is in use.
template <typename T>
class GT_DAValues : public GT_DataArray {
public:
    GT_DAValues(const T *values, GT_Size array_size,
		    int tuple_size, GT_Type type=GT_TYPE_NONE)
	: myData(values)
	, mySize(array_size)
	, myTupleSize(tuple_size)
	, myType(type)
    {
    }
    virtual ~GT_DAValues() {}

    const T	*getData() const	{ return myData; }

    virtual const char *className() const { return "GT_DAValues"; }
    virtual bool	getPointerAliasing(const void *data) const
				    { return myData == data; }

    /// @{
    /// Methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const	{ return GTstorage<T>(); }
    virtual GT_Size	getTupleSize() const	{ return myTupleSize; }
    virtual int64	getMemoryUsage() const	{ return sizeof(*this); }
    virtual GT_Size	entries() const		{ return mySize; }
    virtual GT_Type	getTypeInfo() const	{ return myType; }

    virtual uint8	getU8(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return myData[offset];
			}
    virtual int32	getI32(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return myData[offset];
			}
    virtual int64	getI64(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return myData[offset];
			}
    virtual fpreal16	getF16(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return myData[offset];
			}
    virtual fpreal32	getF32(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return myData[offset];
			}
    virtual fpreal64	getF64(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return myData[offset];
			}
    virtual GT_String	getS(GT_Offset, int) const		{ return NULL; }
    virtual GT_Size	getStringIndexCount() const		{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				    UT_IntArray &) const {}
    /// @}

    virtual const int32	*get(GT_Offset offset, int32 *buf, int size) const
			{
			    const T	*data = myData + offset*myTupleSize;
			    UT_ASSERT_P(offset < mySize && size <= myTupleSize);
			    for (int i = 0; i < size; i++)
				buf[i] = data[i];
			    return buf;
			}
    virtual const int64 *get(GT_Offset offset, int64 *buf, int size) const
			{
			    const T	*data = myData + offset*myTupleSize;
			    UT_ASSERT_P(offset < mySize && size <= myTupleSize);
			    for (int i = 0; i < size; i++)
				buf[i] = data[i];
			    return buf;
			}
    virtual const fpreal16 *get(GT_Offset offset, fpreal16 *buf, int size) const
			{
			    const T	*data = myData + offset*myTupleSize;
			    UT_ASSERT_P(offset < mySize && size <= myTupleSize);
			    for (int i = 0; i < size; i++)
				buf[i] = data[i];
			    return buf;
			}
    virtual const fpreal32 *get(GT_Offset offset, fpreal32 *buf, int size) const
			{
			    const T	*data = myData + offset*myTupleSize;
			    UT_ASSERT_P(offset < mySize && size <= myTupleSize);
			    for (int i = 0; i < size; i++)
				buf[i] = data[i];
			    return buf;
			}
    virtual const fpreal64 *get(GT_Offset offset, fpreal64 *buf, int size) const
			{
			    const T	*data = myData + offset*myTupleSize;
			    UT_ASSERT_P(offset < mySize && size <= myTupleSize);
			    for (int i = 0; i < size; i++)
				buf[i] = data[i];
			    return buf;
			}
private:
    const T	*myData;
    GT_Size	 myTupleSize;
    GT_Size	 mySize;
    GT_Type	 myType;
};

typedef GT_DAValues<int32>	GT_I32Values;
typedef GT_DAValues<int64>	GT_I64Values;
typedef GT_DAValues<fpreal16>	GT_F16Values;
typedef GT_DAValues<fpreal32>	GT_F32Values;
typedef GT_DAValues<fpreal64>	GT_F64Values;

#endif
