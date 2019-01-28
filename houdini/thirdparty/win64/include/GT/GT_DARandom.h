/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DARandom.h ( GT Library, C++)
 *
 * COMMENTS:	An example of a non-data driven array.  This array computes
 *		random values based on a random number seed.
 */

#ifndef __GT_DARandom__
#define __GT_DARandom__

#include "GT_API.h"
#include "GT_DataArray.h"

/// @brief An array of random values
///
/// This array has minimal storage and represents an array of random values.
/// @note The array has issues with more than 2^31 entries
class GT_API GT_DARandom : public GT_DataArray
{
public:
    /// @{ Constructors/Destructors
	     GT_DARandom(GT_Size array_size, int tuple_size=1,
			GT_Type tinfo=GT_TYPE_NONE,
			uint seed=0, fpreal min=0, fpreal max=1);
	     GT_DARandom(const GT_DARandom &src);
    virtual ~GT_DARandom();
    /// @}

    /// @{
    /// Methods defined on GT_DataArray
    virtual const char	*className() const	{ return "GT_DARandom"; }
    virtual GT_Storage	getStorage() const	{ return GT_STORE_REAL32; }
    virtual GT_Size	entries() const		{ return myEntries; }
    virtual GT_Size	getTupleSize() const	{ return myTupleSize; }
    virtual int64	getMemoryUsage() const	{ return sizeof(*this); }
    virtual GT_Type	getTypeInfo() const	{ return myType; }
    
    virtual fpreal32	getF32(GT_Offset offset, int idx) const;
    virtual uint8	getU8(GT_Offset offset, int idx) const;
    virtual int32	getI32(GT_Offset offset, int idx) const;
    virtual GT_String	getS(GT_Offset offset, int idx) const;
    virtual GT_Size	getStringIndexCount() const		{ return -1; }
    virtual GT_Offset	getStringIndex(GT_Offset, int) const	{ return -1; }
    virtual void	getIndexedStrings(UT_StringArray &,
				    UT_IntArray &) const {}
    virtual GT_DataArrayHandle	harden() const
			    { return GT_DataArrayHandle(SYSconst_cast(this)); }
    /// @}

    /// The random numbers generated will be fit to this range.
    void	setRange(fpreal min=0, fpreal max=1)
		    { myMin = min; myMax = max; }

private:
    fpreal32	myMin, myMax;
    GT_Size	myEntries;
    int		myTupleSize;
    uint	mySeed;
    GT_Type	myType;
};

#endif
