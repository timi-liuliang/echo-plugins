/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_DataArrayTuple.h ( GA Library, C++)
 *
 * COMMENTS:	Maintains an tuple of GA_DataArray objects.
 * 		This is a tuple of arrays.
 */

#ifndef __GA_DataArrayTuple__
#define __GA_DataArrayTuple__

#include "GA_API.h"
#include "GA_DataArrayPageTableImpl.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>

class GA_DataArray;
class GA_Defaults;
class GA_Range;
class GA_MergeMap;
class GA_LoadMap;
class GA_Defragment;
class UT_BitArray;
class UT_JSONWriter;
class UT_JSONParser;
class UT_MemoryCounter;
class UT_Options;
class GA_SaveOptions;

/// @brief A tuple of scalar arrays
///
/// This class maintains a tuple of arrays.  For example storing a position is
/// kept as three distinct arrays (one for x, y and z).  The storage for all
/// arrays is uniform.
///
/// See also: @ref JSON-GA_DataArrayTuple
///
/// @see GA_DataArray

class GA_API GA_DataArrayTuple
{
public:
    /// Regardless of packing, we always have one GA_DataArray for
    /// each of our tuple_size components.  Packing causes the first
    /// set of these data arrays to share data tables and store their
    /// actual data in an interleaved format.  If tuplesize is larger
    /// than the packing size, the extra components are normal arrays,
    /// so P + Pw works as expected with a PACK_VECTOR3
    enum ga_Packing
    {
	PACK_NONE,
	PACK_VECTOR3,
    };

     GA_DataArrayTuple(GA_Storage storage);
     GA_DataArrayTuple(const GA_DataArrayTuple &src);
    ~GA_DataArrayTuple();

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

    GA_DataArrayTuple	&operator=(const GA_DataArrayTuple &src);

    /// Changing the storage requires a copy iterator which accesses each and
    /// every element of the array and the defaults
    /// The @c allow_packing parameter should be set if the attribute allows it.
    void	setStorage(GA_Storage storage,
				const GA_Range &copy_iterator,
				const GA_Defaults &defs,
				bool allow_packing=false);

    /// Setting the tuple size requires the array_size and the defaults
    void	setTupleSize(int tuple_size,
				GA_Offset array_size, const GA_Defaults &defs,
				ga_Packing packing = PACK_NONE);

    /// This method grows each array of the tuple
    void	setArraySize(GA_Offset size);

    /// This method queries the (common) size of the arrays.
    GA_Offset	getArraySize() const;

    /// Try to compress data pages
    /// Will compress *all* pages overlapping the specified offset range.
    void	tryCompressAllPages(
			GA_Offset start_offset = GA_Offset(0),
			GA_Offset end_offset = GA_INVALID_OFFSET);

    /// Harden data pages
    /// Will harden *all* pages overlapping the specified offset range.
    void	hardenAllPages(
			GA_Offset start_offset = GA_Offset(0),
			GA_Offset end_offset = GA_INVALID_OFFSET);

    void	mergeGrowArrayAndCopy(const GA_MergeMap &map,
				      GA_AttributeOwner owner,
				      const GA_DataArrayTuple &src);

    GA_Storage		 getStorage() const { return myStorage; }
    int			 getTupleSize() const	{ return myTupleSize; }
    ga_Packing		 getPacking() const { return myPacking; }
    GA_DataArray	*getData(int tuple_index)
			    {
				UT_ASSERT_P(tuple_index >= 0);
				UT_ASSERT_P(tuple_index < myTupleSize);
				return myData[tuple_index];
			    }
    const GA_DataArray	*getData(int tuple_index) const
			    {
				UT_ASSERT_P(tuple_index >= 0);
				UT_ASSERT_P(tuple_index < myTupleSize);
				return myData[tuple_index];
			    }

    /// @{
    /// Interface for defragmentation
    void	defragment(const GA_Defragment &defrag);
    /// @}

    /// @{
    /// Access to data as arrays of tuples
    const GA_DataArray	*const*getDataPtr() const	{ return myData; }
    GA_DataArray	*const*getDataPtr()		{ return myData; }
    /// @}

    /// @cond INTERNAL
    GA_Private::ga_DataArrayPageTableImplV3 *	getPageTableImplV3();
    /// @endcond

    /// Save data to JSON schema.
    /// @section JSON-GA_DataArrayTuple JSON Schema: GA_DataArrayTuple
    /// The data values are stored as an array of structs.  That is, if the
    /// DataArrayTuple is a 2-vector (tuple size 2) containing 4 elements, the
    /// value array will be an array of 4 2-vectors (i.e. [ [0,0], [1,1],
    /// [2,2], [3,3] ]).  For single valued (tuple size == 1), a single array
    /// of values is saved out.  The keyword for this is "arrays" vs. "tuples".
    /// @code
    /// {
    ///     "name"          : "GA_DataArrayTuple",
    ///     "description    : "A tuple of scalar arrays",
    ///     "type"          : "orderedmap",
    ///     "properties"    :
    ///     {
    ///         "size": {
    ///             "type"          : "integer",
    ///             "minimum"       : 1,
    ///         },
    ///         "storage": {
    ///             "type"          : "string",
    ///             "enum"          : [ "uint8","int8","int16","int32","int64",
    ///                                 "fpreal16", "fpreal32", "fpreal64" ],
    ///         },
    ///         "tuples": {
    ///             "type"          : "array",
    ///             "description"   :
    ///                 "Saved as an array of structs (not a struct of arrays)."
    ///             "items"         : {
    ///                 "description"   :
    ///                     "Each item is a number array for each tuple",
    ///                 "type"  : "array"
    ///                 "items" : "number",
    ///             }
    ///         }
    ///         "arrays": {
    ///             "type"          : "array",
    ///             "description"   :
    ///                 "For scalars, just save out the single GA_DataArray",
    ///             "items"         : { "$ref":"GA_DataArray" }
    ///        }
    ///     }
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool	jsonSave(UT_JSONWriter &w, const GA_Range &it,
			 const GA_SaveOptions *options) const;

    static bool	jsonSave(UT_JSONWriter &w, const GA_Range &it,
			 const GA_SaveOptions *options,
			 const GA_DataArray * const *data, int data_tuple_size,
			 GA_Storage data_storage, ga_Packing data_packing);

    /// Load from a JSON stream
    bool	jsonLoad(UT_JSONParser &w,
			const GA_LoadMap &map, GA_AttributeOwner owner);

    static bool	jsonLoad(UT_JSONParser &w,
			 const GA_LoadMap &map, GA_AttributeOwner owner,
			 GA_DataArray * const *data, int data_tuple_size,
			 GA_Storage data_storage, ga_Packing data_packing);

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
    /// @see GA_DataArray::jsonSave()
    bool	jsonSave(UT_JSONWriter &w, const GA_Range &range,
			 const GA_SaveOptions *options,
			 const UT_IntArray *map, int defvalue=-1) const;

    static bool	jsonSave(UT_JSONWriter &w, const GA_Range &range,
			 const GA_SaveOptions *options,
			 const UT_IntArray *map, int defvalue,
			 const GA_DataArray * const *data, int data_tuple_size,
			 GA_Storage data_storage, ga_Packing data_packing);

    // Sets bits for all pages with differing values.
    void comparePages(UT_BitArray &pages, const GA_DataArrayTuple &other) const;

protected:
    GA_DataArray	 *buildDataArray(int idx, GA_Offset length, const GA_Defaults &defs);
    bool		  isSharedArray(int idx) const;

private:
    typedef UT_UniquePtr<UT_BitArray>	 ga_BitArrayPtr;

    static bool		jsonLoadSOA(UT_JSONParser &w, const GA_LoadMap &map,
				    GA_AttributeOwner owner,
				    GA_DataArray * const *data,
				    int data_tuple_size);
    static bool		jsonLoadAOS(UT_JSONParser &w, const GA_LoadMap &map,
				    GA_AttributeOwner owner,
				    GA_DataArray *const *data,
				    int data_tuple_size,
				    GA_Storage data_storage);
    static bool		jsonSaveRawPageData(UT_JSONWriter &w,
				    const GA_Range &range,
				    GA_Size output_page_size,
				    const int *packing,
				    int n_packing_entries,
				    const ga_BitArrayPtr *const_output_pages,
				    const UT_IntArray *map,
				    int defvalue,
				    const GA_DataArray * const *data,
				    int data_tuple_size,
				    GA_Storage data_storage);

    static bool		jsonLoadRawPageData(UT_JSONParser &p,
				    const GA_LoadMap &map,
				    GA_AttributeOwner owner,
				    GA_Size input_page_size,
				    const int *packing,
				    int n_packing_entries,
				    ga_BitArrayPtr *const_input_pages,
				    GA_DataArray * const *data,
				    int data_tuple_size,
				    GA_Storage data_storage,
				    ga_Packing data_packing);

    GA_DataArray	**myData;
    int			  myTupleSize;
    GA_Storage		  myStorage;
    ga_Packing		  myPacking;
};

#endif
