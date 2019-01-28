/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAConstantValue.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAConstantValue__
#define __GT_DAConstantValue__

#include "GT_API.h"
#include <UT/UT_Assert.h>
#include <SYS/SYS_Math.h>
#include "GT_DataArray.h"

/// @brief A data array for a constant value
template <typename T_POD>
class GT_DAConstantValue : public GT_DataArray
{
public:
    GT_DAConstantValue(GT_Size array_size, T_POD value=0,
			int tuple_size=1, GT_Type typeinfo=GT_TYPE_NONE)
	: mySize(array_size)
	, myTupleSize(tuple_size)
	, myTypeInfo(typeinfo)
    {
	myData = new T_POD[tuple_size];
	set(value);
    }
    GT_DAConstantValue(GT_Size array_size, const T_POD *value,
			int tuple_size, GT_Type typeinfo=GT_TYPE_NONE)
	: mySize(array_size)
	, myTupleSize(tuple_size)
	, myTypeInfo(typeinfo)
    {
	myData = new T_POD[tuple_size];
	set(value, tuple_size);
    }
    virtual ~GT_DAConstantValue()
    {
	delete [] myData;
    }

    virtual const char *className() const { return "GT_DAConstantValue"; }
    
    static bool isConstantValue(GT_Size a_size, const T_POD *data, int t_size)
		{
		    // Check to see if an array of tuples is a constant value
		    for (GT_Offset i = 1; i < a_size; ++i)
		    {
			for (int j = 0; j < t_size; ++j)
			{
			    if (data[j] != data[i*t_size + j])
				return false;
			}
		    }
		    return true;
		}

    /// Set constant value
    void	set(T_POD value)
		{
		    int		i;
		    for (i = 0; i < myTupleSize; i++)
			myData[i] = value;
		}
    /// Set from a tuple
    void	set(const T_POD *value, int size)
		{
		    int		i;
		    size = SYSmin(size, myTupleSize);
		    for (i = 0; i < size; i++)
			    myData[i] = value[i];
		}
    /// Assign a 3-tuple
    void	assign(T_POD v0, T_POD v1, T_POD v2)
		{
		    myData[0] = v0;
		    if (myTupleSize > 0)
			myData[1] = v1;
		    if (myTupleSize > 1)
			myData[2] = v2;
		}

    /// @{
    /// Methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const	{ return GTstorage<T_POD>(); }
    virtual GT_Size	getTupleSize() const	{ return myTupleSize; }
    virtual GT_Type	getTypeInfo() const	{ return myTypeInfo; }
    virtual GT_Size	entries() const		{ return mySize; }
    virtual int64	getMemoryUsage() const
			    { return sizeof(T_POD)*myTupleSize; }

    virtual fpreal16	getF16(GT_Offset, int index) const
			{
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    return (fpreal16)myData[index];
			}
    virtual fpreal32	getF32(GT_Offset, int index) const
			{
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    return (fpreal32)myData[index];
			}
    virtual fpreal64	getF64(GT_Offset, int index) const
			{
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    return (fpreal64)myData[index];
			}
    virtual uint8	getU8(GT_Offset, int index) const
			{
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    return (uint8)myData[index];
			}
    virtual int32	getI32(GT_Offset, int index) const
			{
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    return (int32)myData[index];
			}
    virtual int64	getI64(GT_Offset, int index) const
			{
			    UT_ASSERT_P(index >= 0 && index < myTupleSize);
			    return (int64)myData[index];
			}
    virtual GT_String	getS(GT_Offset, int) const	{ return NULL; }
    virtual GT_Size	getStringIndexCount() const		{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				    UT_IntArray &) const {}

    virtual GT_DataArrayHandle	harden() const
			    { return GT_DataArrayHandle(SYSconst_cast(this)); }
    /// @}
private:
    T_POD	*myData;
    GT_Size	 mySize;
    int		 myTupleSize;
    GT_Type	 myTypeInfo;
};

typedef GT_DAConstantValue<int64>	GT_IntConstant;
typedef GT_DAConstantValue<fpreal64>	GT_RealConstant;

#endif
