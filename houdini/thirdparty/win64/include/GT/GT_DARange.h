/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DARange.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DARange__
#define __GT_DARange__

#include "GT_API.h"
#include "GT_DataArray.h"
#include <SYS/SYS_Math.h>

/// @brief A range of values similar to the python range function
///
/// This is a light-weight way of creating a range of integers.  The class uses
/// the same semantics as the Python range() function.
/// - GT_DARange(4)	@n Create a sequence of integers [0,1,2,3]
/// - GT_DARange(3,6)	@n Create the sequence [3,4,5]
/// - GT_DARange(4,0,-1) @n Create the sequence [4,3,2,1]
class GT_API GT_DARange : public GT_DataArray
{
public:
    /// Default constructor (empty range)
    GT_DARange(GT_Type type=GT_TYPE_NONE)
	: GT_DataArray()
	, myType(type)
    {
	init(0, 0, 1);
    }
    /// Initialize [0..stop)
    GT_DARange(GT_Offset stop, GT_Type type=GT_TYPE_NONE)
	: GT_DataArray()
	, myType(type)
    {
	init(0, stop, 1);
    }
    /// Initialize [start..stop)
    GT_DARange(GT_Offset start, GT_Offset stop, GT_Type type=GT_TYPE_NONE)
	: GT_DataArray()
	, myType(type)
    {
	init(start, stop, 1);
    }
    /// Initialize [start..stop, by step)
    GT_DARange(GT_Offset start, GT_Offset stop, GT_Offset step, GT_Type type=GT_TYPE_NONE)
	: GT_DataArray()
	, myType(type)
    {
	init(start, stop, step);
    }
    /// Copy constructor
    GT_DARange(const GT_DARange &src)
	: myStart(src.myStart)
	, myEnd(src.myEnd)
	, myStep(src.myStep)
	, myType(src.myType)
	, GT_DataArray()
    {
    }

    // Destructor
    virtual ~GT_DARange();

    virtual const char *className() const { return "GT_DARange"; }
    
    /// Initialize with given data
    void	init(GT_Offset start, GT_Offset end, GT_Size step=1)
		{
		    myStart = start;
		    myEnd = end;
		    myStep = (step == 0) ? 1 : step;
		}

    /// @{
    /// Methods defined on GT_DataArray
    virtual GT_Storage	getStorage() const
			    { return isBig(myStart) || isBig(myEnd)
					? GT_STORE_INT64
					: GT_STORE_INT32;
			    }
    virtual GT_Size	getTupleSize() const
			    { return 1; }
    virtual int64	getMemoryUsage() const
			    { return sizeof(*this); }
    virtual GT_Type	getTypeInfo() const
			    { return myType; }
    virtual GT_Size	entries() const
			{
			    GT_Size	n;
			    if (myStep > 0)
				n = (myEnd - myStart) + (myStep-1);
			    else
				n = (myEnd - myStart) + (myStep+1);
			    return SYSmax(n/myStep, 0);
			}
    virtual fpreal16	getF16(GT_Offset offset, int) const
			    { return myStart + offset*myStep; }
    virtual fpreal32	getF32(GT_Offset offset, int) const
			    { return myStart + offset*myStep; }
    virtual fpreal64	getF64(GT_Offset offset, int) const
			    { return myStart + offset*myStep; }
    virtual uint8	getU8(GT_Offset offset, int) const
			    { return myStart + offset*myStep; }
    virtual int32	getI32(GT_Offset offset, int) const
			    { return myStart + offset*myStep; }
    virtual int64	getI64(GT_Offset offset, int) const
			    { return myStart + offset*myStep; }
    virtual GT_String	getS(GT_Offset, int) const		{ return NULL; }
    virtual GT_Size	getStringIndexCount() const		{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				    UT_IntArray &) const {}
    /// @}

private:
    bool	isBig(GT_Offset v) const
		{
		    return v < -0x7ffffff || v > 0x7ffffff;
		}
    GT_Offset	myStart, myEnd;
    GT_Size	myStep;
    GT_Type	myType;
};

#endif

