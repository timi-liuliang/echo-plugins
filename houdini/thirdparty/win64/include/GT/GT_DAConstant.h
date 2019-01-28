/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAConstant.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAConstant__
#define __GT_DAConstant__

#include "GT_API.h"
#include "GT_DAInherit.h"

/// @brief Extend a single element from a source array to emulate a larger array
///
/// Given a source data array, this class will emulate a larger array by
/// extending a single element from the source array over a given size.
/// For example: @code
///	GT_DataArray	&src;
///	GT_DAConstant	 varying;
///	varying.init(32, 100);
/// @endcode
/// Creates an array of 100 entries referencing element 32 from the source
/// array.
class GT_API GT_DAConstant : public GT_DAInherit {
public:
    /// Default constructor
    GT_DAConstant()
	: mySize(0)
	, myOffset(0)
	, GT_DAInherit()
    {
    }
    /// Useful constructor
    GT_DAConstant(const GT_DataArrayHandle &data,
		    GT_Offset offset, GT_Size size)
	: myOffset(offset)
	, mySize(size)
	, GT_DAInherit(data)
    {
    }
    /// Copy constructor
    GT_DAConstant(const GT_DAConstant &src)
	: myOffset(src.myOffset),
	  mySize(src.mySize),
	  GT_DAInherit(src)
    {
    }
    virtual ~GT_DAConstant();

    virtual const char *className() const { return "GT_DAConstant"; }
    
    /// Create a constant data array from the existing data array.
    static const GT_DataArrayHandle	createConstant(
						const GT_DataArrayHandle &h,
						GT_Offset offset,
						GT_Size size);

    /// Initialize with given data
    void		init(const GT_DataArrayHandle &data,
			    GT_Offset offset, GT_Size size)
			{
			    GT_DAInherit::init(data);
			    myOffset = offset;
			    mySize = size;
			}

    GT_IMPL_INHERIT_ARRAY(getIndirect, mySize)

private:
    /// Map the requested offset to the offset in my data
    inline GT_Offset	getIndirect(GT_Offset) const
			    { return myOffset; }

    GT_Offset		myOffset;
    GT_Size		mySize;
};

#endif
