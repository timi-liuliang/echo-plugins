/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ArrayDataArray.h ( GA Library, C++)
 *
 * COMMENTS:	A very simple array of arrays used to store attribute data.  
 */

#ifndef __GA_ArrayDataArray__
#define __GA_ArrayDataArray__

#include "GA_API.h"
#include "GA_Types.h"
#include <UT/UT_VectorTypes.h>
#include <UT/UT_PackedArrayOfArrays.h>
#include <SYS/SYS_Types.h>

class GA_Attribute;
class GA_Defragment;
class GA_LoadMap;
class GA_MergeMap;
class GA_MergeOffsetMap;
class GA_Range;
class GA_IndexMap;
class UT_BitArray;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
template <typename T> class UT_Array;
template <typename T> class UT_ValArray;

class ga_PageArrayTable;


/// @brief An array of array of numbers with various storage types.
///
/// GA_ArrayDataArray provides a way of storing arrays of tuples of scalar
/// values.  
/// The array can have various storage types from 8-bit integer to 64-bit
/// reals.
///
class GA_API GA_ArrayDataArray 
{
public:
     GA_ArrayDataArray(GA_Storage storage, int tuplesize);

     GA_ArrayDataArray(const GA_ArrayDataArray &src);
    ~GA_ArrayDataArray();

    GA_ArrayDataArray	&operator=(const GA_ArrayDataArray &src);

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

    /// Change the size of the array
    void	 setArraySize(GA_Offset size);

    /// Query the size of the array
    GA_Offset	 getArraySize() const		{ return mySize; }

    /// The maximum length of any of our sub-arrays, requries running
    /// over the entire array
    exint	 findMaximumArrayLength() const;

    /// Query the storage used for the array data.
    GA_Storage	 getStorage() const;

    /// Changes the storage, copying the data to the new format.
    void	 setStorage(GA_Storage newstorage);

    /// Queries the size of each tuple stored in the arrays.
    GA_Size	 getTupleSize() const;

    /// Adjusts the tuple size, note this does not affect the
    /// actual data layout!
    void	 setTupleSize(GA_Size size);

    /// Empties the array, creating an empty array.
    void	 clear();

    /// Empties a specific subset of the array, [off, off+num)
    void	 clearOffset(GA_Offset off, GA_Size num);

    /// mergeGrowArrayAndCopy() is called to grow the data array while copying
    /// data from the source.
    void	mergeGrowArrayAndCopy(const GA_MergeMap &map,
				      GA_AttributeOwner owner,
				      const GA_ArrayDataArray &src);

    /// Clear the particular indices to empty arrays.
    void	 reset(GA_Offset di);
    void	 reset(const GA_Range &di);

    /// Copies
    void	 copy(GA_Offset di, const GA_ArrayDataArray *src, GA_Offset si);
    void	 copy(GA_Range di, const GA_ArrayDataArray *src, GA_Range si);

    /// Return the array size for a given offset
    exint	 arraySize(GA_Offset offset) const;

    /// Read/Write individual elements
    /// If you have tuplesize 3, the values will be multiple of 3!
    void 	 get(GA_Offset off, UT_Array<fpreal32> &values) const;
    void 	 get(GA_Offset off, UT_Array<fpreal64> &values) const;
    void 	 get(GA_Offset off, UT_Array<int32> &values) const;
    void 	 get(GA_Offset off, UT_Array<int64> &values) const;

    void 	 append(GA_Offset off, UT_PackedArrayOfArrays<fpreal32> &values) const;
    void 	 append(GA_Offset off, UT_PackedArrayOfArrays<fpreal64> &values) const;
    void 	 append(GA_Offset off, UT_PackedArrayOfArrays<int32> &values) const;
    void 	 append(GA_Offset off, UT_PackedArrayOfArrays<int64> &values) const;

#define GETBLOCKFROMINDEX(ITYPE, DTYPE) \
    void	 getBlockFromIndices(const GA_IndexMap &map, \
			    GA_Index start, GA_Size nelem, \
			    UT_Array<ITYPE> &index, \
			    UT_Array<DTYPE> &data) const;
    GETBLOCKFROMINDEX(int32, int32);
    GETBLOCKFROMINDEX(int32, int64);
    GETBLOCKFROMINDEX(int32, fpreal32);
    GETBLOCKFROMINDEX(int32, fpreal64);
    GETBLOCKFROMINDEX(int64, int32);
    GETBLOCKFROMINDEX(int64, int64);
    GETBLOCKFROMINDEX(int64, fpreal32);
    GETBLOCKFROMINDEX(int64, fpreal64);
#undef GETBLOCKFROMINDEX

    /// For efficiency, should already have set the length.  If a resize
    /// is forced, will convert the touched page to POINTERS
    void 	 set(GA_Offset off, const UT_Array<fpreal32> &values);
    void 	 set(GA_Offset off, const UT_Array<fpreal64> &values);
    void 	 set(GA_Offset off, const UT_Array<int32> &values);
    void 	 set(GA_Offset off, const UT_Array<int64> &values);

    /// @{
    /// Interface for defragmentation
    void	swapRange(GA_Offset a, GA_Offset b, GA_Size n);
    void	moveRange(GA_Offset src, GA_Offset dest, GA_Size n);
    void	defragment(const GA_Defragment &defrag);
    /// @}

    /// Save data to a JSON stream.
    /// @section JSON-GA_DataArray JSON Schema: GA_DataArray
    /// @code
    /// {
    ///     "name"          : "GA_ArrayDataArray",
    ///     "description"   : "An array of arrays of numbers",
    ///     "type"          : "array",
    ///     "items"         : "number",
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool	jsonSave(UT_JSONWriter &w, const GA_Range &it) const;

    /// Load from a JSON stream.
    bool	jsonLoad(UT_JSONParser &p,
			const GA_LoadMap &map, GA_AttributeOwner owner);

    /// Save with an integer translation.  This method is just like the
    /// standard jsonSave() method.  However each value is used as an index
    /// into the integer array.  The value stored in the array is written
    /// instead of the raw value.
    /// @code
    ///     for (GA_Iterator it(range); !it.atEnd(); ++it) {
    ///         idx = getInteger(it.getOffset());
    ///         json.write( idx < 0 || idx >= map.entries() ? -1 : map(idx) );
    ///     }
    /// @endcode
    bool	jsonSave(UT_JSONWriter &w, const GA_Range &range,
			const UT_IntArray *map, int defvalue=-1) const;

    // Return whether the attribute storage & size match
    inline bool	isSameType(const GA_ArrayDataArray &b) const
		{
		    return myStorage == b.myStorage && mySize == b.mySize
			    && getTupleSize() == b.getTupleSize();
		}

    /// Try to compress data pages
    /// Will try to compress *all* pages overlapping the specified offset range.
    void	tryCompressAllPages(GA_Offset start_offset = GA_Offset(0),
                                    GA_Offset end_offset = GA_INVALID_OFFSET);
    /// Compresses a specific page.
    /// Pass in the offset whose page you wish to compress, *not*
    /// the page id.
    void	tryCompressSinglePage(GA_Offset pageoffset);

    /// Harden data pages
    /// Will harden *all* pages overlapping the specified offset range.
    void	hardenAllPages(GA_Offset start_offset = GA_Offset(0),
                               GA_Offset end_offset = GA_INVALID_OFFSET);

    /// Remaps all numbers through the lookup table.  Negative numbers
    /// get mapped to -1.  Counts the number of occurrences of each
    /// number in the counter, which should be pre-allocated to your
    /// maximum size.
    /// Inclusive
    /// count measures the *source* indices, not destination.
    void	remapAndCount(GA_Offset start_offset,
			      GA_Offset end_offset,
			      const UT_IntArray &old2new,
			      UT_IntArray &count);

private:

private:
    ga_PageArrayTable		*myPageTable;
    GA_Offset			 mySize;
    GA_Storage			 myStorage;
};

#endif

