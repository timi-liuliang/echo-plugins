/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DAIndirect.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DAIndirect__
#define __GT_DAIndirect__

#include "GT_API.h"
#include "GT_DAInherit.h"

/// @brief An array to handle indirect references (i.e. shared items)
///
/// This class takes two GT_DataArrays.  The indirect array should be an array
/// of integers which are used to perform indirect lookup into the shared data
/// array.
/// For example: @code
///	GT_DataArray	VertexList =  [ 1, 4, 2, 3 ]
///	GT_DataArray	&PointAttributes;
///	GT_DAIndirect	attribs(VertexList, PointAttributes);
/// @endcode
/// This allows the point attributes to be referenced by vertex number.
class GT_API GT_DAIndirect : public GT_DAInherit {
public:
    /// Default constructor
    GT_DAIndirect()
	: myIndirect()
	, GT_DAInherit()
    { }
    /// Convenience constructor
    GT_DAIndirect(const GT_DataArrayHandle &indirect,
		  const GT_DataArrayHandle &data,
		  bool flatten_nested = false)
	: myIndirect()
	, GT_DAInherit()
    {
	init(indirect, data, flatten_nested);
    }
    /// Copy constructor
    GT_DAIndirect(const GT_DAIndirect &src)
	: myIndirect()
	, GT_DAInherit()
    {
	init(src.myIndirect, src.myData, true);
    }
    /// Destructor
    virtual ~GT_DAIndirect();

    virtual const char *className() const { return "GT_DAIndirect"; }

    virtual bool	isValid() const
		    { return myIndirect && myData &&
		 	     myIndirect->isValid() && myData->isValid(); }
    virtual void	updateGeoDetail(const GU_ConstDetailHandle &dtl,
					const char *attrib_name,
					GT_Owner attrib_owner,
					const int expected_size);

    
    /// Initialize data from an indirect array of indices and a data array
    void		init(const GT_DataArrayHandle &indirect,
			     const GT_DataArrayHandle &data,
			     bool flatten_nested);

    GT_DataArrayHandle	getIndirect() { return myIndirect; }
    virtual int64	getDataId() const;

    static int getUnmapThreshold();

    GT_IMPL_INHERIT_ARRAY(getIndirect, myIndirect->entries())

private:
    /// Map the requested offset to the offset in my data
    inline GT_Offset	getIndirect(GT_Offset offset) const
			    { return myIndirect->getI64(offset); }

    GT_DataArrayHandle		myIndirect;
};

#endif

