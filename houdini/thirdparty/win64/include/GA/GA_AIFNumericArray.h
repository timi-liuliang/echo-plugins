/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFNumericArray.h ( GA Library, C++)
 *
 * COMMENTS:	Array Data Attribute Interface
 */

#ifndef __GA_AIFNumericArrayTuple__
#define __GA_AIFNumericArrayTuple__

#include "GA_API.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>
#include <UT/UT_PackedArrayOfArrays.h>


class GA_Attribute;
class GA_Range;

/// @brief Generic Attribute Interface class to access an attribute as a
/// array.
///
/// This class provides the interface to access attribute data.  Each
/// attribute type may provide this interface if it makes sense.
class GA_API GA_AIFNumericArray 
{
public:
	     GA_AIFNumericArray();
    virtual ~GA_AIFNumericArray();

    /// @{
    /// Query and set some tuple intrisics.
    virtual int		getTupleSize(const GA_Attribute *attrib) const = 0;
    virtual bool	setTupleSize(GA_Attribute *attrib, int size) const = 0;

    virtual GA_Storage	getStorage(const GA_Attribute *attrib) const = 0;
    virtual bool	setStorage(GA_Attribute *attrib,
				   GA_Storage storage) const = 0;
    /// @}

    /// @{
    /// Erase all arrays
    virtual bool	clear(GA_Attribute *attrib) const = 0;
    /// @}

    /// @{
    /// Return the length of longest array
    virtual exint	findMaximumArrayLength(const GA_Attribute *attrib) const = 0;
    /// @}

    /// @{
    /// Get a single value from a single offset in an attribute array.
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, UT_Array<fpreal32> &data) const = 0;
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, UT_Array<fpreal64> &data) const = 0;
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, UT_Array<int32> &data) const = 0;
    virtual bool	get(const GA_Attribute *attrib,
			    GA_Offset ai, UT_Array<int64> &data) const = 0;
    /// @}

    /// @{
    /// Appends a value from a single offset into a packed array.
    virtual bool	append(const GA_Attribute *attrib,
			    GA_Offset ai, UT_PackedArrayOfArrays<fpreal32> &data) const = 0;
    virtual bool	append(const GA_Attribute *attrib,
			    GA_Offset ai, UT_PackedArrayOfArrays<fpreal64> &data) const = 0;
    virtual bool	append(const GA_Attribute *attrib,
			    GA_Offset ai, UT_PackedArrayOfArrays<int32> &data) const = 0;
    virtual bool	append(const GA_Attribute *attrib,
			    GA_Offset ai, UT_PackedArrayOfArrays<int64> &data) const = 0;
    /// @}

    /// @{
    /// Stream in a block representation of the array.
    /// The index array will have nelements+1.  Each array will go
    /// from [index[i] .. index[i+1]) in the data array.
#define GETBLOCKFROMINDEX(ITYPE, DTYPE) \
    virtual bool	getBlockFromIndices(const GA_Attribute *attrib, \
			    GA_Index start, GA_Size nelem,  \
			    UT_Array<ITYPE> &index,  \
			    UT_Array<DTYPE> &data) const = 0;

    GETBLOCKFROMINDEX(int32, int32);
    GETBLOCKFROMINDEX(int32, int64);
    GETBLOCKFROMINDEX(int32, fpreal32);
    GETBLOCKFROMINDEX(int32, fpreal64);
    GETBLOCKFROMINDEX(int64, int32);
    GETBLOCKFROMINDEX(int64, int64);
    GETBLOCKFROMINDEX(int64, fpreal32);
    GETBLOCKFROMINDEX(int64, fpreal64);
#undef GETBLOCKFROMINDEX
    /// @}

    /// @{
    /// Stream in a packed array representation of the array.
#define GETBLOCKFROMINDEX(DTYPE) \
    bool		getPackedArrayFromIndices(const GA_Attribute *attrib, \
			    GA_Index start, GA_Size nelem, \
			    UT_PackedArrayOfArrays<DTYPE> &array) const \
    { return getBlockFromIndices(attrib, start, nelem, array.rawOffsets(), array.rawData()); }

    GETBLOCKFROMINDEX(int32);
    GETBLOCKFROMINDEX(int64);
    GETBLOCKFROMINDEX(fpreal32);
    GETBLOCKFROMINDEX(fpreal64);
#undef GETBLOCKFROMINDEX

    /// @}

    /// @{
    /// Test if any elements are NAN's
    virtual bool	isNan(const GA_Attribute *attrib, GA_Offset ai) const=0;
    virtual bool	isNan(const GA_Attribute *attrib,
				const GA_Range &it) const = 0;
    /// @}

    /// Return the size of the array for the GA_Offset
    virtual exint	arraySize(const GA_Attribute *atr, GA_Offset off) const = 0;

    // ----------------------------------------------------------------------

    /// @{
    /// Set a single array of the array.
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				const UT_Array<fpreal32> &data) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				const UT_Array<fpreal64> &data) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				const UT_Array<int32> &data) const = 0;
    virtual bool	set(GA_Attribute *attrib, GA_Offset ai,
				const UT_Array<int64> &data) const = 0;
    /// @}
};

#endif
