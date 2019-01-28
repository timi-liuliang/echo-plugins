/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_DataBitArray.h ( GA Library, C++)
 *
 * COMMENTS:	A very simple array used to store attribute data.  Some areas
 *		of the array may be "vacant".
 */

#ifndef __GA_DataBitArray__
#define __GA_DataBitArray__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_Array.h>

#include <SYS/SYS_Types.h>

#include <iosfwd>
#include <stddef.h>


class GA_Defragment;
class GA_LoadMap;
class GA_MergeMap;
class GA_MergeOffsetMap;
class GA_Range;
class UT_IStream;
class UT_JSONWriter;
class UT_JSONParser;
class UT_JSONValue;
class UT_MemoryCounter;
class UT_Options;

class ga_DataBitArrayPage;

// We want a bit pattern the size of a pointer whose every bit
// but the last is set.
#define GA_BIT_PAGE_PTR_MASK ( ~((size_t)1) )

/// @brief An array of bits.
///
/// GA_DataBitArray provides an efficient way of storing bit arrays.
///
/// There are basic operations on the array.
///
/// See also: @ref JSON-GA_DataBitArray
///
class GA_API GA_DataBitArray 
{
public:
     GA_DataBitArray(GA_Offset size, bool value);
    ~GA_DataBitArray();

    /// Change the size of the array
    void	 setArraySize(GA_Offset size);

    /// Query the size of the array
    GA_Offset	 getArraySize() const { return mySize; }

    /// Clear all entries to the default value
    void	 clear();

    /// Clear a specific offset to the default
    void	 clearOffset(GA_Offset off, GA_Offset num);

    /// Set all entries to the given value
    void	 makeConstant(bool value);

    /// Try to compress all pages overlapping the specified offset range.
    void	tryCompressAllPages(GA_Offset start_offset = GA_Offset(0),
                                    GA_Offset end_offset = GA_INVALID_OFFSET);

    /// Harden all pages overlapping the specified offset range.
    void	hardenAllPages(GA_Offset start_offset = GA_Offset(0),
                               GA_Offset end_offset = GA_INVALID_OFFSET);

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

    /// Copy bit array from the source.  This will change the size
    /// of the array if the source size is different.
    bool	copyFrom(const GA_DataBitArray &src);

    /// mergeGrowArrayAndCopy() is called to grow the data array while
    /// appending or copying data from the source (non-interleaved merge).
    /// The array is resized to the new destination capacity (as defined by
    /// the map) and the data from the source array is copied over into the
    /// newly allocated area.
    void	 mergeGrowArrayAndCopy(const GA_MergeMap &map,
				       GA_AttributeOwner owner,
				       const GA_DataBitArray &src);

    GA_Size	 countSetBits(GA_Offset start, GA_Offset end) const;
    /// Query the length of repeated bit values starting from 'start' until
    /// 'end' or a change in bit values.
    void	 getConstantSpan(GA_Offset start, GA_Offset end, GA_Size &size, bool &value) const;

    /// Access data from the array
    bool	 get(GA_Offset idx) const;
    /// Test if any bit in the range is set
    bool	 isAnySet(const GA_Range &range) const;

    /// Set the value at a particular index
    void	 set(GA_Offset di, bool val);
    void	 set(GA_Offset di, const GA_DataBitArray &src, GA_Offset si)
		    { set(di, src.get(si)); }
    void	 set(const GA_Range &it, bool val);
    void	 set(const GA_Range &it, const GA_DataBitArray &src);
    void	 set(const GA_Range &it, const GA_DataBitArray &src, const GA_Range &sit);
    void	 set(const GA_DataBitArray &src, const GA_Range &sit,
		     const GA_MergeOffsetMap &map);

    /// @{
    /// Interface for defragmentation
    void	defragment(const GA_Defragment &defrag);
    /// @}

    /// Toggle the value at a particular index
    bool	 toggle(GA_Offset idx);
    void	 toggle(const GA_Range &it);

    /// Page accelerated full group operations
    void	 andEqual(const GA_DataBitArray &src);
    void	 xorEqual(const GA_DataBitArray &src);
    void	 orEqual(const GA_DataBitArray &src);
    void	 subEqual(const GA_DataBitArray &src);
    void	 toggleAll(GA_Size numelements);

    /// A special method for loading group data from geo files.
    bool	loadGroupBitArrayH9(UT_IStream &is);
    bool	saveGroupBitArrayH9(std::ostream &os, int binary,
			const GA_Range &it) const;

    /// @section JSON-GA_DataBitArray JSON Schema: GA_DataBitArray
    /// jsonSave() will save the array to a JSON stream.  There are multiple
    /// ways to store a bit-array.
    /// @code
    /// {
    ///     "name"          : "GA_DataBitArray",
    ///     "description"   : "A array bits",
    ///     "type"          : "orderedmap",
    ///     "properties"          : [
    ///         "boolRLE": {
    ///             "type"     : "array",
    ///             "items"    : [ "integer", "boolean" ]
    ///             "optional" : true,
    ///             "description": "A run length encoded bit array stored as a
    ///                        count/bool pair where the 'bool' value is
    ///                        repeated 'count' times.",
    ///         }
    ///         "i8": {
    ///             "type"        : "array",
    ///             "items"       : "integer",
    ///             "description" : "A array of 0/1, not very efficient",
    ///             "optional"    : true,
    ///         }
    ///     ]
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool	jsonSave(UT_JSONWriter &w, const GA_Range &it,
			    const UT_Options *options) const;

    /// Load from a JSON stream
    bool	jsonLoad(UT_JSONParser &p, const GA_LoadMap &map,
			GA_AttributeOwner owner);
    /// Load from a JSON value
    bool	jsonLoad(UT_JSONParser &p, const UT_JSONValue &v,
			const GA_LoadMap &map, GA_AttributeOwner owner);

    SYS_FORCE_INLINE bool isPageConstant(GA_PageNum pageid) const
    { return !(((size_t)myPages(pageid)) & GA_BIT_PAGE_PTR_MASK); }

    SYS_FORCE_INLINE bool getPageCVal(GA_PageNum pageid) const
    {
        UT_ASSERT_P(isPageConstant(pageid));
        return (bool) (myPages(pageid));
    }

    void tryCompressPage(GA_PageNum pageid);
    void makePageConstant(GA_PageNum pageid, bool value);

private:
    void	reallocPageTable(GA_Offset size);
    ga_DataBitArrayPage	*harden(GA_PageNum pageid);

    bool		 myDefault;
    UT_Array<ga_DataBitArrayPage *> myPages;
    GA_Offset		 mySize;
};

#endif
