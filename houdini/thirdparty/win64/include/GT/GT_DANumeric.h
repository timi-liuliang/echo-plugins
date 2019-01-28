/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DANumeric.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DANumeric__
#define __GT_DANumeric__

#include "GT_API.h"
#include "GT_DataArray.h"
#include "GT_Memory.h"
#include <UT/UT_Assert.h>
#include <UT/UT_ArrayHelp.h>
#include <string.h>

/// @brief An array of numeric values (int32, int64, fpreal16, fpreal32, fpreal64)
template <typename T>
class GT_DANumeric : public GT_DataArray
{
public:
    typedef T   data_type;

    /// Create a numeric array
    GT_DANumeric(GT_Size array_size, int tuple_size, GT_Type type=GT_TYPE_NONE)
	: myData(NULL)
	, myTupleSize(tuple_size)
	, myType(type)
	, myCapacity(0)
	, mySize(0)
    {
	resize(array_size);
    }
    /// Create a numeric array, copying values from the source data.  The data
    /// array does @b not hold a reference to the source data, it's copied.
    GT_DANumeric(const T *data, GT_Size array_size, int tuple_size,
	    GT_Type type=GT_TYPE_NONE)
	: myData(NULL)
	, myTupleSize(tuple_size)
	, myType(type)
	, myCapacity(0)
	, mySize(0)
    {
	resize(array_size);
	copyFrom(data);
    }
    virtual ~GT_DANumeric()
    {
	resize(0);
    }

    virtual const char *className() const { return "GT_DANumeric"; }
    
    /// A numeric array is hard to begin with
    virtual GT_DataArrayHandle	harden() const
				{
				    GT_DataArray	*me;
				    me = const_cast<
					    GT_DANumeric<T> *>(this);
				    return GT_DataArrayHandle(me);
				}

    void	truncate()
		{
		    if (mySize != myCapacity)
			resize(mySize);
		}

    void	resize(GT_Size size, GT_Size capacity = -1)
		{
                    if (capacity < 0)
                        capacity = size;

		    if (!capacity)
		    {
			GT_Size	bytes = myCapacity * myTupleSize * sizeof(T);
			GT_Memory::Free(GT_MEM_DATA, myData, bytes);
			myData = NULL;
		    }
		    else
		    {
			GT_Size	obytes = myCapacity * myTupleSize * sizeof(T);
			GT_Size	nbytes = capacity * myTupleSize * sizeof(T);
			myData = (T *)GT_Memory::Realloc(GT_MEM_DATA,
				    myData, obytes, nbytes);
		    }
		    mySize = size;
		    myCapacity = capacity;
		}

    /// Append a scalar value to the array
    void	append(T value)
		{
		    if (mySize == myCapacity)
			grow();
		    mySize++;
		    set(value, mySize-1);
		}
    void	append(const T *value)
		{
		    if (mySize == myCapacity)
			grow();
		    mySize++;
		    setTuple(value, mySize-1);
		}
    void	concat(const GT_DANumeric<T> &src)
		{
		    UT_ASSERT_P(src.myTupleSize == myTupleSize);
		    UT_ASSERT_P(src.myType == myType);
		    if (src.mySize)
		    {
			// Ensure we have enough room
			if (mySize + src.mySize > myCapacity)
			    grow(mySize + src.mySize);
			memcpy(myData + mySize*myTupleSize,
				src.myData,
				src.mySize*src.myTupleSize*sizeof(T));
			mySize += src.mySize;
		    }
		}

    /// Raw access to the data array
    T		*data() const { return myData; }
    /// Raw pointer access to a tuple
    T		*getData(GT_Offset offset)
		{
		    UT_ASSERT_P(offset >= 0 && offset < mySize);
		    offset *= myTupleSize;
		    return myData + offset;
		}
    /// Raw pointer access to a tuple
    const T	*getData(GT_Offset offset) const
		{
		    UT_ASSERT_P(offset >= 0 && offset < mySize);
		    offset *= myTupleSize;
		    return myData + offset;
		}
    /// Set a component of the tuple
    void	set(T value, GT_Offset offset, int index=0)
		{
		    offset = offset * myTupleSize + index;
		    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
		    myData[offset] = value;
		}
    /// Set an entire tuple
    void	setTuple(const T *value, GT_Offset offset)
		{
		    UT_ASSERT_P(offset>=0 && offset<mySize);
		    offset = offset * myTupleSize;
		    memcpy(myData + offset, value, myTupleSize*sizeof(T));
		}
    /// Set a block of entire tuples
    void	setTupleBlock(const GT_DANumeric<T> *values, GT_Offset offset)
		{
                    if (values->mySize)
                    {
                        UT_ASSERT_P(offset>=0 && offset<mySize);
                        offset = offset * myTupleSize;
                        memcpy(myData + offset, values->myData, values->mySize*myTupleSize*sizeof(T));
                    }
		}
    /// Set a block of entire tuples
    void	setTupleBlock(const T *values, GT_Size n, GT_Offset offset)
		{
                    if (n)
                    {
                        UT_ASSERT_P(offset>=0 && offset<mySize);
                        offset = offset * myTupleSize;
                        memcpy(myData + offset, values, n*myTupleSize*sizeof(T));
                    }
		}
    /// Copy an entire data from a flat array
    void	copyFrom(const T *src)
		{
		    if (src)
		    {
			memcpy(myData, src, myTupleSize*mySize*sizeof(T));
		    }
		    else UT_ASSERT_P(mySize == 0);
		}

    /// @{
    /// Methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const	{ return GTstorage<T>(); }
    virtual GT_Size	getTupleSize() const	{ return myTupleSize; }
    virtual GT_Size	entries() const		{ return mySize; }
    virtual GT_Type	getTypeInfo() const	{ return myType; }
    virtual int64	getMemoryUsage() const
			    { return sizeof(T)*mySize*myTupleSize; }

    virtual uint8	getU8(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return (uint8)myData[offset];
			}
    virtual int32	getI32(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return (int32)myData[offset];
			}
    virtual int64	getI64(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return (int64)myData[offset];
			}
    virtual fpreal16	getF16(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return (fpreal16)myData[offset];
			}
    virtual fpreal32	getF32(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return (fpreal32)myData[offset];
			}
    virtual fpreal64	getF64(GT_Offset offset, int index=0) const
			{
			    offset = offset * myTupleSize + index;
			    UT_ASSERT_P(offset>=0 && offset<mySize*myTupleSize);
			    return (fpreal64)myData[offset];
			}
    virtual GT_String	getS(GT_Offset, int) const	{ return NULL; }
    virtual GT_Size	getStringIndexCount() const		{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				    UT_IntArray &) const {}

    virtual const uint8		*getU8Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, uint8>())
			return reinterpret_cast<const uint8 *>(data());
		    return GT_DataArray::getU8Array(buffer);
		}
    virtual const int8		*getI8Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, int8>())
			return reinterpret_cast<const int8 *>(data());
		    return GT_DataArray::getI8Array(buffer);
		}
    virtual const int16		*getI16Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, int16>())
			return reinterpret_cast<const int16 *>(data());
		    return GT_DataArray::getI16Array(buffer);
		}
    virtual const int32		*getI32Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, int32>())
			return reinterpret_cast<const int32 *>(data());
		    return GT_DataArray::getI32Array(buffer);
		}
    virtual const int64		*getI64Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, int64>())
			return reinterpret_cast<const int64 *>(data());
		    return GT_DataArray::getI64Array(buffer);
		}
    virtual const fpreal16	*getF16Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, fpreal16>())
			return reinterpret_cast<const fpreal16 *>(data());
		    return GT_DataArray::getF16Array(buffer);
		}
    virtual const fpreal32	*getF32Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, fpreal32>())
			return reinterpret_cast<const fpreal32 *>(data());
		    return GT_DataArray::getF32Array(buffer);
		}
    virtual const fpreal64	*getF64Array(GT_DataArrayHandle &buffer) const
		{
		    if (SYSisSame<T, fpreal64>())
			return reinterpret_cast<const fpreal64 *>(data());
		    return GT_DataArray::getF64Array(buffer);
		}

    virtual void doImport(GT_Offset idx, uint8 *data, GT_Size size) const
			{ importTuple<uint8>(idx, data, size); }
    virtual void doImport(GT_Offset idx, int8 *data, GT_Size size) const
			{ importTuple<int8>(idx, data, size); }
    virtual void doImport(GT_Offset idx, int16 *data, GT_Size size) const
			{ importTuple<int16>(idx, data, size); }
    virtual void doImport(GT_Offset idx, int32 *data, GT_Size size) const
			{ importTuple<int32>(idx, data, size); }
    virtual void doImport(GT_Offset idx, int64 *data, GT_Size size) const
			{ importTuple<int64>(idx, data, size); }
    virtual void doImport(GT_Offset idx, fpreal16 *data, GT_Size size) const
			{ importTuple<fpreal16>(idx, data, size); }
    virtual void doImport(GT_Offset idx, fpreal32 *data, GT_Size size) const
			{ importTuple<fpreal32>(idx, data, size); }
    virtual void doImport(GT_Offset idx, fpreal64 *data, GT_Size size) const
			{ importTuple<fpreal64>(idx, data, size); }

    virtual void doFillArray(uint8 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    virtual void doFillArray(int8 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    virtual void doFillArray(int16 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    virtual void doFillArray(int32 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    virtual void doFillArray(int64 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    virtual void doFillArray(fpreal16 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    virtual void doFillArray(fpreal32 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    virtual void doFillArray(fpreal64 *data, GT_Offset start, GT_Size length,
			int tsize, int stride) const
		 { t_NumericFill(data, start, length, tsize, stride); }
    /// @}

private:
    void	grow(exint sz=-1)
		{
		    exint	obytes = myCapacity * myTupleSize * sizeof(T);
		    myCapacity = sz < 0 ? UTbumpAlloc(myCapacity+1) : sz;
		    exint	nbytes = myCapacity * myTupleSize * sizeof(T);
		    myData = (T *)GT_Memory::Realloc(GT_MEM_DATA,
				    myData, obytes, nbytes);
		}
    template <typename T_POD> inline void
    importTuple(GT_Offset idx, T_POD *data, GT_Size tsize) const
		{
		    if (tsize < 1)
			tsize = myTupleSize;
		    else
			tsize = SYSmin(tsize, myTupleSize);
		    const T	*src = myData + idx*myTupleSize;
		    for (int i = 0; i < tsize; ++i)
			data[i] = (T_POD)src[i];
		}

    inline void
    t_NumericFill(T *dest, GT_Offset start, GT_Size length,
		    int tsize, int stride) const
		{
		    if (tsize < 1)
			tsize = myTupleSize;
		    stride = SYSmax(stride, tsize);
		    int n = SYSmin(tsize, myTupleSize);
		    if (n == myTupleSize && stride == myTupleSize)
		    {
			memcpy(dest, myData+start*myTupleSize,
				    length*n*sizeof(T));
		    }
		    else
		    {
			const T *src = myData+start*myTupleSize;
			for (GT_Offset i = 0; i < length; ++i,
					src += myTupleSize, dest += stride)
			{
			    for (int j = 0; j < n; ++j)
				dest[j] = src[j];
			}
		    }
		}

    template <typename T_POD> inline void
    t_NumericFill(T_POD *dest, GT_Offset start, GT_Size length,
		    int tsize, int stride) const
		{
		    if (tsize < 1)
			tsize = myTupleSize;
		    stride = SYSmax(stride, tsize);
		    int n = SYSmin(tsize, myTupleSize);
		    const T *src = myData+start*myTupleSize;
		    for (GT_Offset i = 0; i < length; ++i,
					src += myTupleSize, dest += stride)
		    {
			for (int j = 0; j < n; ++j)
			    dest[j] = (T_POD)src[j];
		    }
		}

    T		*myData;
    GT_Size	 mySize;
    GT_Size	 myCapacity;
    int		 myTupleSize;
    GT_Type	 myType;
};
using GT_Unsigned8Array = GT_DANumeric<uint8>;
using GT_UInt8Array = GT_Unsigned8Array;
using GT_Int8Array = GT_DANumeric<int8>;
using GT_Int16Array = GT_DANumeric<int16>;
using GT_Int32Array = GT_DANumeric<int32>;
using GT_Int64Array = GT_DANumeric<int64>;
using GT_Real16Array = GT_DANumeric<fpreal16>;
using GT_Real32Array = GT_DANumeric<fpreal32>;
using GT_Real64Array = GT_DANumeric<fpreal64>;
using GT_RealArray = GT_DANumeric<fpreal>;

/// Return either an int64 or int32 array based on the contents of the
/// array.  That is, only allocate a 64 bit integer array if it's needed.
/// For tuple sizes > 1, this function assumes that the array is an array
/// of structs, rather than a struct of arrays.
GT_API extern GT_DataArray	*GTallocateIntArray(const int64 *array,
					GT_Size size, int tuple_size=1);
GT_API extern GT_DataArray	*GTallocateIntArray(const int32 *array,
					GT_Size size, int tuple_size=1);

#endif
