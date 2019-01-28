/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DASubArray.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DASubArray__
#define __GT_DASubArray__

#include "GT_API.h"
#include "GT_DAInherit.h"

/// @brief A slice of a different data array
class GT_API GT_DASubArray : public GT_DAInherit {
public:
    /// Default constructor
    GT_DASubArray()
	: mySize(0)
	, myStart(0)
	, GT_DAInherit()
    {
    }
    /// Useful constructor
    GT_DASubArray(const GT_DataArrayHandle &data, GT_Offset start, GT_Size size)
	: mySize(size)
	, myStart(start)
	, GT_DAInherit(data)
    {
    }
    /// Copy constructor
    GT_DASubArray(const GT_DASubArray &src)
	: mySize(src.mySize)
	, myStart(src.myStart)
	, GT_DAInherit(src)
    {
    }
    /// Destructor
    virtual ~GT_DASubArray();

    virtual const char *className() const { return "GT_DASubArray"; }
    
    /// Initialize with given data
    void		init(const GT_DataArrayHandle &data,
			    GT_Offset start, GT_Size size)
			{
			    GT_DAInherit::init(data);
			    myStart = start;
			    mySize = size;
			}

    GT_IMPL_INHERIT_ARRAY(getIndirect, mySize)

private:
    /// Map the requested offset to the offset in my data
    inline GT_Offset	getIndirect(GT_Offset offset) const
			    { return myStart + offset; }

    GT_Offset		myStart;
    GT_Size		mySize;
};

#endif
