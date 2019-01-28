/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_DataArray.h ( GA Library, C++)
 *
 * COMMENTS:	A very simple array used to store attribute data.  Some areas
 *		of the array may be "vacant".
 */

#pragma once

#ifndef __GA_DataArray__
#define __GA_DataArray__

#include "GA_API.h"
#include "GA_Types.h"
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Types.h>


class GA_Attribute;
class GA_Defragment;
class GA_LoadMap;
class GA_MergeMap;
class GA_MergeOffsetMap;
class GA_Range;
class UT_BitArray;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
template <typename T> class UT_Array;
template <typename T> class UT_ValArray;

class ga_PageTableBase;
namespace GA_Private { class ga_PageTableBaseImpl; }


/// @brief An array of numbers with various storage types.
///
/// GA_DataArray provides a way of storing arrays of scalar values.  The array
/// can have various storage types from 8-bit integer to 64-bit reals.
///
/// There are basic operations on the array.
///
/// See also: @ref JSON-GA_DataArray
///
class GA_API GA_DataArray 
{
public:
     GA_DataArray(GA_Storage storage, GA_Offset size, int64 value);
     GA_DataArray(GA_Storage storage, GA_Offset size, fpreal value);
     /// Creates a shared array if slice is other than 0.  You must
     /// invoke shareData() on the resulting data to steal the 0th entries
     /// data tables.
     GA_DataArray(GA_Storage storage, GA_Offset size, int slice, UT_Vector3D value);
     GA_DataArray(const GA_DataArray &src);
    ~GA_DataArray();

    GA_DataArray	&operator=(const GA_DataArray &src);

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
    /// If the array is packed, this must only be invoked on the 0th
    /// array, and the other shared arrays must be updated by calling
    /// shareData()
    void	 setArraySize(GA_Offset size);

    /// Query the size of the array
    GA_Offset	 getArraySize() const		{ return mySize; }

    /// Query the storage used for the array data.
    GA_Storage	 getStorage() const		{ return myStorage; }

    /// Changes all values back to their defaults
    /// Ignored for shared arrays.
    void	 clear();

    /// Clears a specific value to the default
    void	 clearOffset(GA_Offset off, GA_Offset num);

    /// Makes the array constant.
    void	 makeConstant(int32 val);
    void	 makeConstant(int64 val);
    void	 makeConstant(fpreal32 val);
    void	 makeConstant(fpreal64 val);
    void	 makeVector3Constant(const int32 *val);
    void	 makeVector3Constant(const int64 *val);
    void	 makeVector3Constant(const fpreal32 *val);
    void	 makeVector3Constant(const fpreal64 *val);

    /// We can have multiple data arrays refer to the same structure
    /// with different slice/vectorsizes.  The 0th is always the true owner
    /// the rest need to do a shareData to track any changes.
    void	 shareData(GA_DataArray *src);

    /// mergeGrowArrayAndCopy() is called to grow the data array while copying
    /// data from the source.
    /// If the array is packed, this must only be invoked on the 0th
    /// array, and the other shared arrays must be updated by calling
    /// shareData()
    void	mergeGrowArrayAndCopy(const GA_MergeMap &map,
				      GA_AttributeOwner owner,
				      const GA_DataArray &src);

    /// @{
    /// Access data from the array
    int32	 getI32(GA_Offset idx) const;
    int64	 getI64(GA_Offset idx) const;
    fpreal16	 getF16(GA_Offset idx) const;
    fpreal32	 getF32(GA_Offset idx) const;
    fpreal64	 getF64(GA_Offset idx) const;
    fpreal	 getF(GA_Offset idx) const
		     { return getF64(idx); }
    void	 get(GA_Offset idx, int32 &v) const	{ v = getI32(idx); }
    void	 get(GA_Offset idx, int64 &v) const	{ v = getI64(idx); }
    void	 get(GA_Offset idx, fpreal32 &v) const	{ v = getF32(idx); }
    void	 get(GA_Offset idx, fpreal64 &v) const	{ v = getF64(idx); }
    /// @}

    // Access a range of values from the array
    void	 get(GA_Offset start, GA_Offset end, int32 *v, int step=1) const;
    void	 get(GA_Offset start, GA_Offset end, int64 *v, int step=1) const;
    void	 get(GA_Offset start, GA_Offset end, fpreal32 *v, int step=1) const;
    void	 get(GA_Offset start, GA_Offset end, fpreal64 *v, int step=1) const;

    /// Set the value at a particular index
    void	 set(GA_Offset di, int32 val);
    void	 set(GA_Offset di, int64 val);
    void	 set(GA_Offset di, fpreal32 val);
    void	 set(GA_Offset di, fpreal64 val);
    void	 set(const GA_Range &di, int32 val);
    void	 set(const GA_Range &di, int64 val);
    void	 set(const GA_Range &di, fpreal32 val);
    void	 set(const GA_Range &di, fpreal64 val);

    // Only to be used on vector3 arrays!
    void	 setVector3(const GA_Range &di, const int32 *val);
    void	 setVector3(const GA_Range &di, const int64 *val);
    void	 setVector3(const GA_Range &di, const fpreal32 *val);
    void	 setVector3(const GA_Range &di, const fpreal64 *val);

    void	 set(GA_Offset di, const GA_DataArray &src, GA_Offset si);
    void	 set(const GA_Range &di, const GA_DataArray &src, GA_Offset si);
    void	 set(const GA_Range &di, const GA_DataArray &src);
    void	 set(const GA_Range &di, const GA_DataArray &src, const GA_Range &si);

    /// Set a range of values in the array
    void	 set(GA_Offset start, GA_Offset end, const int32 *v, int step=1);
    void	 set(GA_Offset start, GA_Offset end, const int64 *v, int step=1);
    void	 set(GA_Offset start, GA_Offset end, const fpreal32 *v, int step=1);
    void	 set(GA_Offset start, GA_Offset end, const fpreal64 *v, int step=1);

    /// Set the value for a tuple of data array from a source tuple.
    static  void	 setTuple(GA_DataArray *const* dest, const GA_Range &di,
				  const GA_DataArray *const* src,
				  int tuple_size);
    static  void	 setTuple(GA_DataArray *const* dest, const GA_Range &di,
				  const GA_DataArray *const* src,
				  const GA_Range &si, int tuple_size);
    static  void	 setTuple(GA_DataArray *const* dest,
				  const GA_DataArray *const* src,
				  const GA_Range &si,
				  const GA_MergeOffsetMap &map, int tuple_size);

    /// Reset the value at a particular index to its default.
    void	 reset(GA_Offset di);
    void	 reset(const GA_Range &di);

    /// @{
    /// Interface for defragmentation
    void	swapRange(GA_Offset a, GA_Offset b, GA_Size n);
    void	moveRange(GA_Offset src, GA_Offset dst, GA_Size n);
    void	defragment(const GA_Defragment &defrag);
    /// @}

    /// @{
    /// Test for NAN's
    bool	isNan(GA_Offset ai) const;
    bool	isNan(const GA_Range &di) const;
    /// @}

    /// Integer values in the array are re-mapped to the values stored in the
    /// map.
    /// @code
    ///   for (GA_Iterator it(di); !it.atEnd(); ++it) {
    ///       idx = array[it];
    ///       array[it] = idx < 0 || idx >= map.entries() ? invalid : map(idx);
    ///   }
    /// @endcode
    void	xlate(const GA_Range &di, const UT_IntArray &map, int64 invalid=-1);
    /// Save data to a JSON stream.
    /// @section JSON-GA_DataArray JSON Schema: GA_DataArray
    /// @code
    /// {
    ///     "name"          : "GA_DataArray",
    ///     "description"   : "An array of numbers",
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
    inline bool	isSameType(const GA_DataArray &b) const
		{
		    return myStorage == b.myStorage && mySize == b.mySize;
		}

    /// Marshall a bit array indicating which output pages are constant given
    /// the internal page mapping.  The mapping consists of a list of internal
    /// pages with the start of each output page indicated by a negative value,
    /// (-(internal_page + 1)).
    ///
    /// This method considers any internal pages not already marked constant as
    /// varying.
    ///
    /// Returns the total number of constant output pages.
    GA_Size	 marshallConstantFlagsForOutputPagesFast(
			    const UT_ValArray<GA_PageNum> &internal_page_map,
			    UT_BitArray &constant_flags) const;

    /// Marshall a bit array indicating which output pages are constant given
    /// the internal page mapping.  The mapping consists of a list of internal
    /// sub-page blocks with the start of each output page indicated by a
    /// negative page number (-(internal_page + 1)).
    ///
    /// This method traverses any varying internal pages to detect whether an
    /// output page will be constant.
    ///
    /// Returns the total number of constant output pages.
    class ga_SubPageBlock
    {
	public:
	    ga_SubPageBlock() {}
	    ga_SubPageBlock(GA_PageNum page, GA_PageOff start, GA_PageOff end)
		: myPage(page), myStartOffset(start), myEndOffset(end) {}

	    GA_PageNum	 myPage;
	    GA_PageOff	 myStartOffset;
	    GA_PageOff	 myEndOffset;
    };
    GA_Size	 marshallConstantFlagsForOutputPagesSlow(
			const UT_Array<ga_SubPageBlock> &internal_page_map,
			UT_BitArray &constant_flags) const;


    /// @cond INTERNAL
    const ga_PageTableBase	*getPageTable() const { return myPageTable; }
    ga_PageTableBase		*getPageTable() { return myPageTable; }
    GA_Private::ga_PageTableBaseImpl
				*getPageTableImpl();
    /// @endcond

    /// @{
    /// Direct access to compound objects.
    template <typename DESTTYPE, typename MYTYPE>
	static DESTTYPE directGetterScalar(const GA_DataArray *const* data,
					   GA_Offset off, int comp);
    template <typename SRCTYPE, typename MYTYPE>
	static void directSetterScalar(GA_DataArray *const* data,
				       GA_Offset off, int comp, SRCTYPE val);
    template <typename SRCTYPE, typename MYTYPE>
	static void directAdderScalar(GA_DataArray *const* data,
				      GA_Offset off, int comp, SRCTYPE val);
    template <typename DESTTYPE, typename MYTYPE>
	static void directGetterVector(DESTTYPE &value,
				       const GA_DataArray *const* data,
				       GA_Offset off);
    template <typename SRCTYPE, typename MYTYPE>
	static void directSetterVector(GA_DataArray *const* data,
				       GA_Offset off, const SRCTYPE &val);
    template <typename SRCTYPE, typename MYTYPE>
	static void directAdderVector(GA_DataArray *const* data,
				      GA_Offset off, const SRCTYPE &val);

    template <typename DESTTYPE, typename MYTYPE>
	static DESTTYPE directVector3GetterScalar(const GA_DataArray *const *d,
						  GA_Offset off, int comp);
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3SetterScalar(GA_DataArray *const* data,
					      GA_Offset off, int comp,
					      SRCTYPE val);
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3AdderScalar(GA_DataArray *const *data,
					     GA_Offset off, int comp,
					     SRCTYPE val);
    template <typename DESTTYPE, typename MYTYPE>
	static void directVector3GetterVector(DESTTYPE &value,
					      const GA_DataArray *const* data,
					      GA_Offset off);
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3SetterVector(GA_DataArray *const* data,
					      GA_Offset off,const SRCTYPE &val);
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3AdderVector(GA_DataArray *const* data,
					     GA_Offset off, const SRCTYPE &val);

    // Page access methods
    template <typename T, typename BASETYPE, typename MYTYPE, bool READONLY>
	static T *directPageGetterVary(
			GA_DataArray *const* data, T *marshall, GA_Offset off, int comp, bool &isconstant);
    template <typename T, typename BASETYPE, int TUPLE_SIZE, typename MYTYPE, bool READONLY>
	static T *directVector3PageGetterVary(
			GA_DataArray *const* data, T *marshall, GA_Offset off, int comp, bool &isconstant);
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directPageGetter(
			GA_DataArray *const* data, T *marshall, GA_Offset off, int comp, bool &isconstant);
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directVector3PageGetter(
			GA_DataArray *const* data, T *marshall, GA_Offset off, int comp, bool &isconstant);
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directPageGetterRO(
			const GA_DataArray *const* data, T *marshall, GA_Offset off, int comp, bool &isconstant);
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directVector3PageGetterRO(
			const GA_DataArray *const* data, T *marshall, GA_Offset off, int comp, bool &isconstant);
    template <typename T, typename BASETYPE, int TUPLE_SIZE, typename MYTYPE>
	static void directVector3PageSetterVary(GA_DataArray *const* data, const T *srcdata, GA_Offset off, int comp);
    template <typename T, typename BASETYPE, typename MYTYPE>
	static void directPageSetter(GA_DataArray *const* data, const T *srcdata, GA_Offset off, int comp);
    template <typename T, typename BASETYPE, typename MYTYPE>
	static void directVector3PageSetter(GA_DataArray *const* data, const T *srcdata, GA_Offset off, int comp);
    /// @}

    /// @{
    /// Helper class for wrapping all direct access methods in a version that
    /// takes an attribute pointer.  The provided base class must provide two
    /// static conversion methos:
    ///   static const GA_DataArray *const* getData(const GA_Attribute *);
    ///   static       GA_DataArray *const* getData(GA_Attribute *);
    template <typename BASE>
    class directAttributeAdapter : public BASE
    {
    public:
    /// Direct access to compound objects.
    template <typename DESTTYPE, typename MYTYPE>
	static DESTTYPE directGetterScalar(const GA_Attribute *atr,
					   GA_Offset off, int comp)
	{
	    return GA_DataArray::directGetterScalar<DESTTYPE, MYTYPE>(
					    BASE::getData(atr), off, comp);
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directSetterScalar(GA_Attribute *atr,
				       GA_Offset off, int comp, SRCTYPE val)
	{
	    GA_DataArray::directSetterScalar<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, comp, val);
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directAdderScalar(GA_Attribute *atr,
				      GA_Offset off, int comp, SRCTYPE val)
	{
	    GA_DataArray::directAdderScalar<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, comp, val);
	}
    template <typename DESTTYPE, typename MYTYPE>
	static void directGetterVector(DESTTYPE &value, const GA_Attribute *atr,
				       GA_Offset off)
	{
	    GA_DataArray::directGetterVector<DESTTYPE, MYTYPE>(
					    value, BASE::getData(atr), off);
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directSetterVector(GA_Attribute *atr, 
				       GA_Offset off, const SRCTYPE &val)
	{
	    GA_DataArray::directSetterVector<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, val);
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directAdderVector(GA_Attribute *atr,
				      GA_Offset off, const SRCTYPE &val)
	{
	    GA_DataArray::directAdderVector<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, val);
	}

    template <typename DESTTYPE, typename MYTYPE>
	static DESTTYPE directVector3GetterScalar(const GA_Attribute *atr,
						  GA_Offset off, int comp)
	{
	    return GA_DataArray::directVector3GetterScalar<DESTTYPE, MYTYPE>
					    (BASE::getData(atr), off, comp);
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3SetterScalar(GA_Attribute *atr,
					      GA_Offset off, int comp,
					      SRCTYPE val)
	{
	    GA_DataArray::directVector3SetterScalar<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, comp, val); 
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3AdderScalar(GA_Attribute *atr,
					     GA_Offset off, int comp,
					     SRCTYPE val)
	{
	    GA_DataArray::directVector3AdderScalar<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, comp, val);
	}
    template <typename DESTTYPE, typename MYTYPE>
	static void directVector3GetterVector(DESTTYPE &value,
					      const GA_Attribute *atr,
					      GA_Offset off)
	{
	    GA_DataArray::directVector3GetterVector<DESTTYPE, MYTYPE>(
					    value, BASE::getData(atr), off);
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3SetterVector(GA_Attribute *atr,
					      GA_Offset off,const SRCTYPE &val)
	{
	    GA_DataArray::directVector3SetterVector<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, val);
	}
    template <typename SRCTYPE, typename MYTYPE>
	static void directVector3AdderVector(GA_Attribute *atr,
					     GA_Offset off, const SRCTYPE &val)
	{
	    GA_DataArray::directVector3AdderVector<SRCTYPE, MYTYPE>(
					    BASE::getData(atr), off, val);
	}
    // Page access methods
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directPageGetter(
			GA_Attribute *atr, T *marshall, GA_Offset off, int comp, bool &isconstant)
	{
	    return GA_DataArray::directPageGetter<T, BASETYPE, MYTYPE>(
				    BASE::getData(atr), marshall, off, comp, isconstant);
	}
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directVector3PageGetter(
			GA_Attribute *atr, T *marshall, GA_Offset off, int comp, bool &isconstant)
	{
	    return GA_DataArray::directVector3PageGetter<T, BASETYPE, MYTYPE>(
				    BASE::getData(atr), marshall, off, comp, isconstant);
	}
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directPageGetterRO(
			const GA_Attribute *atr, T *marshall,
			GA_Offset off, int comp, bool &isconstant)
	{
	    return GA_DataArray::directPageGetterRO<T, BASETYPE, MYTYPE>(
				    BASE::getData(atr), marshall, off, comp, isconstant);
	}
    template <typename T, typename BASETYPE, typename MYTYPE>
	static T *directVector3PageGetterRO(
			const GA_Attribute *atr, T *marshall,
			GA_Offset off, int comp, bool &isconstant)
	{
	    return GA_DataArray::directVector3PageGetterRO<T, BASETYPE, MYTYPE>(
				    BASE::getData(atr), marshall, off, comp, isconstant);
	}
    template <typename T, typename BASETYPE, typename MYTYPE>
	static void directPageSetter(GA_Attribute *atr,
				     const T *srcdata, GA_Offset off, int comp)
	{
	    GA_DataArray::directPageSetter<T, BASETYPE, MYTYPE>(
				    BASE::getData(atr), srcdata, off, comp);
	}
    template <typename T, typename BASETYPE, typename MYTYPE>
	static void directVector3PageSetter(GA_Attribute *atr,
					    const T *srcdata, GA_Offset off,
					    int comp)
	{
	    GA_DataArray::directVector3PageSetter<T, BASETYPE, MYTYPE>(
				    BASE::getData(atr), srcdata, off, comp);
	}
    };
    /// @}

    // Sets bits for all pages with differing values.
    void comparePages(UT_BitArray &pages, const GA_DataArray &other) const;

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
    template <typename T, typename TB>
	static bool nativeSetFullTuple(GA_DataArray *const* dest,
				       const GA_Range &di,
				       const GA_DataArray *const* src,
				       int tuple_size);
    template <typename T, typename TB>
	static bool nativeSetFullTuple(GA_DataArray *const* dest,
				       const GA_Range &di,
				       const GA_DataArray *const* src,
				       const GA_Range &si,
				       int tuple_size);
    template <typename T, typename TB, int VECTORSIZE>
	bool nativeSetDispatch(const GA_Range &di, const GA_DataArray &src,
			       const GA_Range &si);
    template <typename T, typename TB, int VECTORSIZE,
	      typename S_T, typename S_TB, int S_VECTORSIZE>
	bool nativeSet(const GA_Range &di, const GA_DataArray &src,
		       const GA_Range &si);

private:
    ga_PageTableBase		*myPageTable;
    GA_Offset			 mySize;
    GA_Storage			 myStorage;
    int				 mySlice, myVectorSize;
};

#endif
