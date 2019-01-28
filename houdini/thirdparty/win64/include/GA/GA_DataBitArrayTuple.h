/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_DataBitArrayTuple.h ( GA Library, C++)
 *
 * COMMENTS:	Maintains an tuple of GA_DataBitArray objects.
 * 		This is a tuple of arrays.
 */

#ifndef __GA_DataBitArrayTuple__
#define __GA_DataBitArrayTuple__

#include "GA_API.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>


class GA_DataBitArray;
class GA_Defaults;
class GA_Defragment;
class GA_MergeMap;

class UT_MemoryCounter;

/// @brief A tuple of bit arrays
///
/// This class maintains a tuple of bit arrays.
///
/// @see GA_DataBitArray, GA_DataArrayTuple

class GA_API GA_DataBitArrayTuple
{
public:
     GA_DataBitArrayTuple();
    ~GA_DataBitArrayTuple();

    /// Report memory usage
    int64	getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Setting the tuple size requires the array_size and the defaults
    void	setTupleSize(int tuple_size,
				GA_Offset array_size, const GA_Defaults &defs);

    /// This method grows each array of the tuple
    void	setArraySize(GA_Offset size);

    /// This method queries the (common) size of the arrays.
    GA_Offset	getArraySize() const;

    void	 mergeGrowArrayAndCopy(const GA_MergeMap &map,
				       GA_AttributeOwner owner,
				       const GA_DataBitArrayTuple &src);

    int			 getTupleSize() const	{ return myTupleSize; }
    GA_DataBitArray	*getData(int tuple_index)
			    { return myData[tuple_index]; }
    const GA_DataBitArray	*getData(int tuple_index) const
			    { return myData[tuple_index]; }

    /// @{
    /// Interface for defragmentation
    void	defragment(const GA_Defragment &defrag);
    /// @}

#if 0
    /// Save data to a JSON stream.
    /// @section JSON-GA_DataBitArrayTuple JSON Schema: GA_DataBitArrayTuple
    /// @code
    /// {
    ///     "name"          : "GA_DataBitArrayTuple",
    ///     "description    : "A tuple of bit arrays",
    ///     "properties"    :
    ///     {
    ///         "size": {
    ///             "type"          : "integer",
    ///             "minimum"       : 1,
    ///         },
    ///         "data": {
    ///             "type"          : "array",
    ///             "items"         : { "$ref":"GA_DataBitArray" }
    ///        }
    ///     }
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool	jsonSave(UT_JSONWriter &w, const GA_Range &it,
			    const UT_Options *options) const;
#endif
private:
    GA_DataBitArray	**myData;
    int			  myTupleSize;
};

#endif
