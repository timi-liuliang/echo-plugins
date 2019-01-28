/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DataArray.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DataArray__
#define __GT_DataArray__

#include "GT_API.h"
#include "GT_Types.h"
#include "GT_Handles.h"
#include <UT/UT_Assert.h>
#include <UT/UT_ParallelUtil.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_BoundingBox.h>

class UT_StringArray;
class UT_JSONWriter;
class GU_ConstDetailHandle;

class GT_DataArray;
typedef UT_IntrusivePtr<GT_DataArray>	GT_DataArrayHandle;

/// @brief Abstract data class for an array of float, int or string data
///
/// This is an abstract class.  The data does not have to be owned by this
/// array.  The data can be generated on the fly by the class.  Or the data
/// might be flat numeric data.
class GT_API GT_DataArray
    : public UT_IntrusiveRefCounter<GT_DataArray>
{
public:
	     GT_DataArray();
	     GT_DataArray(const GT_DataArray &src);
    virtual ~GT_DataArray();

    virtual const char *className() const = 0;
    
    /// Create a "hardened" version of the array
    virtual GT_DataArrayHandle	harden() const;

    /// Type of data stored in the array
    virtual GT_Storage		getStorage() const = 0;

    /// Number of entries in the array
    virtual GT_Size		entries() const = 0;

    /// Number of elements for each array element
    virtual GT_Size		getTupleSize() const = 0;

    /// Get an approximation of the memory usage.  Since data is shared, this
    /// may be an over-estimation of the memory used.
    virtual int64		getMemoryUsage() const = 0;

    /// Return "type" information for the data.  This defaults to GT_TYPE_NONE
    virtual GT_Type		getTypeInfo() const;

    /// Returns "true" if each entry is an array.
    virtual bool		hasArrayEntries() const { return false; }

    /// Return "true" if there's pointer aliasing
    virtual bool		getPointerAliasing(const void *data) const
				    { return false; }

    /// Data array is valid; can be sampled from.
    virtual bool		isValid() const { return true; }

    /// Return the "data id" associated with the data.  The data ID will be
    /// incremented if the data is modified.  An ID less than 0 indicates that
    /// the data ID should always be considered unique and will never be the
    /// same across evaluations.  This data ID can be used to detect whether
    /// attribute data has changed in similar tesselations.
    virtual int64	getDataId() const { return myDataId; }

    /// @{
    /// Get data out of the data array
    virtual uint8	getU8(GT_Offset offset, int idx=0) const = 0;
    virtual int8	getI8(GT_Offset offset, int idx=0) const
			    { return getI32(offset, idx); }
    virtual int16	getI16(GT_Offset offset, int idx=0) const
			    { return getI32(offset, idx); }
    virtual int32	getI32(GT_Offset offset, int idx=0) const = 0;
    virtual int64	getI64(GT_Offset offset, int idx=0) const
			    { return getI32(offset, idx); }

    virtual fpreal16	getF16(GT_Offset offset, int idx=0) const
			    { return getF32(offset, idx); }
    virtual fpreal32	getF32(GT_Offset offset, int idx=0) const = 0;
    virtual fpreal64	getF64(GT_Offset offset, int idx=0) const
			    { return getF32(offset, idx); }

    virtual GT_String	getS(GT_Offset offset, int idx=0) const = 0;
    virtual bool	getSA(UT_StringArray &a, GT_Offset offset) const
			    { return false; }

    virtual bool	getFA32(UT_ValArray<fpreal32> &a, GT_Offset offset) const 
			    { return false; }
    virtual bool	getIA32(UT_ValArray<int32> &a, GT_Offset offset) const 
			    { return false; }
    /// @}

    /// @{
    /// Get raw access to the data buffer (or fill the data array passed in)
    /// If the array is unable to provide a raw pointer to the underlying data,
    /// the GT_DataArrayHandle will be allocated to store the data, and it's
    /// raw data will be returned.
    ///
    /// This means that the data returned will be destructed when the buffer is
    /// destructed.  Beware.
    virtual const uint8		*getU8Array(GT_DataArrayHandle &buffer) const;
    virtual const int8		*getI8Array(GT_DataArrayHandle &buffer) const;
    virtual const int16		*getI16Array(GT_DataArrayHandle &buffer) const;
    virtual const int32		*getI32Array(GT_DataArrayHandle &buffer) const;
    virtual const int64		*getI64Array(GT_DataArrayHandle &buffer) const;
    virtual const fpreal16	*getF16Array(GT_DataArrayHandle &buffer) const;
    virtual const fpreal32	*getF32Array(GT_DataArrayHandle &buffer) const;
    virtual const fpreal64	*getF64Array(GT_DataArrayHandle &buffer) const;
    const fpreal		*getRealArray(GT_DataArrayHandle &buffer) const;
    /// @}

    /// When an array of strings is based on an indexed list of strings, this
    /// method can be used to query the maximum number of indices.  If the
    /// strings are not indexed, the method should return -1.
    virtual GT_Size	getStringIndexCount() const = 0;

    /// When an array of strings is based on an indexed list of strings, this
    /// method can be used to query each element's index.  If the strings are
    /// not indexed, the method should return -1.
    virtual GT_Offset	getStringIndex(GT_Offset offset, int idx=0) const = 0;

    /// When an array of strings is based on an indexed list of strings, this
    /// method can be used to extract all the strings and their corresponding
    /// indicies.  It's possible that the indices may not be contiguous or may
    /// even be out of order.
    virtual void	getIndexedStrings(UT_StringArray &strings,
					UT_IntArray &indices) const = 0;
    /// When an array of strings is based on an indexed list of strings, this
    /// method can be used to extract all the strings.  If an index is not
    /// used, the string array may contain a NULL pointer.  When you call @c
    /// getStringIndex(), you can look up directly into the @c strings array
    /// returned here.
    ///
    /// Note, if the strings are not stored as an indexed list, this method
    /// will return an empty array.
    virtual void	getStrings(UT_StringArray &strings) const;

    /// Fill an array of std::string with the strings corresponding to each
    /// entry.  There must be @c entries() strings in the array.  Only a single
    /// tuple index is extracted.
    void		fillStrings(std::string *strings,
				int tuple_idx=0) const;

    /// @{
    /// Extract data out of the array in a template friendly fashion.
    /// Unlike the @c get() methods, these methods will @b always copy data
    /// into the buffers.  If the @c size is 0, the tuple size will be used.
    /// The data buffer must be big enough.
    ///
    /// If the size given is larger than getTupleSize(), only the first
    /// @c getTupleSize() elements will be filled out (leaving the others
    /// untouched).
    void import(GT_Offset idx, int8 *data, GT_Size size=0) const
	    { doImport(idx, data, size); }
    void import(GT_Offset idx, int16 *data, GT_Size size=0) const
	    { doImport(idx, data, size); }
    void import(GT_Offset idx, int32 *data, GT_Size size=0) const
	    { doImport(idx, data, size); }
    void import(GT_Offset idx, int64 *data, GT_Size size=0) const
	    { doImport(idx, data, size); }
    void import(GT_Offset idx, fpreal16 *data, GT_Size size=0) const
	    { doImport(idx, data, size); }
    void import(GT_Offset idx, fpreal32 *data, GT_Size size=0) const
	    { doImport(idx, data, size); }
    void import(GT_Offset idx, fpreal64 *data, GT_Size size=0) const
	    { doImport(idx, data, size); }
    void import(GT_Offset idx, UT_ValArray<fpreal32> &data) const
	    { doImportArray(idx, data); }
    void import(GT_Offset idx, UT_ValArray<int32> &data) const
	    { doImportArray(idx, data); }
    void import(GT_Offset idx, uint8 *data, GT_Size size=0,
		    fpreal black=0, fpreal white=1) const
	    {
		if (GTisFloat(getStorage()))
		    doImportQuantized(idx, data, size, black, white);
		else
		    doImport(idx, data, size);
	    }
    /// @}

    /// @{
    /// Extract data for the entire array into a flat data array.
    /// By default, this is implemented as: @code
    ///	    if (tuple_size <= 0)
    ///		tuple_size = getTupleSize();
    ///	    for (GT_Offset idx = 0; idx < length; ++idx, data += tuple_size)
    ///		import(idx+start, data, tuple_size);
    /// @endcode
    /// Obviously, the @c data array should be big enough to hold @c
    /// length*tuple_size elements.
    ///
    /// The tuple size (@c tsize) specifies how many elements to extract from
    /// each element of this array.  The @c stride specifies how many items to
    /// skip over for each element.  With a stride of -1, the stride will be
    /// set to the @c tsize.  Otherwise the stride will be set to the maximum
    /// of @c tsize and @c stride.
    ///
    /// The stride can be useful when trying to interleave data.  For example:
    /// @code
    ///	   float uvbuf[ucoord->entries()*2];
    ///	   ucoord->fillArray(&uvbuf[0], 0, ucoord->entries(), 1, 2);
    ///	   vcoord->fillArray(&uvbuf[1], 0, vcoord->entries(), 1, 2);
    /// @endcode
    void fillArray(int8 *data, GT_Offset start, GT_Size length,
			int tsize, int stride=-1) const
		    { doFillArray(data, start, length, tsize, stride); }
    void fillArray(int16 *data, GT_Offset start, GT_Size length,
			int tsize, int stride=-1) const
		    { doFillArray(data, start, length, tsize, stride); }
    void fillArray(int32 *data, GT_Offset start, GT_Size length,
			int tsize, int stride=-1) const
		    { doFillArray(data, start, length, tsize, stride); }
    void fillArray(int64 *data, GT_Offset start, GT_Size length,
			int tsize, int stride=-1) const
		    { doFillArray(data, start, length, tsize, stride); }
    void fillArray(fpreal16 *data, GT_Offset start, GT_Size length,
			int tsize, int stride=-1) const
		    { doFillArray(data, start, length, tsize, stride); }
    void fillArray(fpreal32 *data, GT_Offset start, GT_Size length,
			int tsize, int stride=-1) const
		    { doFillArray(data, start, length, tsize, stride); }
    void fillArray(fpreal64 *data, GT_Offset start, GT_Size length,
			int tsize, int stride=-1) const
		    { doFillArray(data, start, length, tsize, stride); }
    void fillArray(uint8 *data,
			GT_Offset start, GT_Size length,
			int tsize, int stride=-1,
			fpreal black=0, fpreal white=1) const
		    {
			if(GTisFloat(getStorage()))
			    doFillQuantizedArray(data, start, length, tsize,
						 stride, black, white);
			else
			    doFillArray(data, start, length, tsize, stride);
		    }

    void fillVec3BBox(fpreal32 *dest, GT_Offset start, GT_Size length,
		      UT_BoundingBoxF &bbox, int tsize, int stride=-1)
		    {
			UT_ASSERT_P(tsize==3);
			doFillVec3BBox(dest, start, length, bbox,
				       tsize, stride);
		    }
    void fillVec3BBox(fpreal64 *dest, GT_Offset start, GT_Size length,
		      UT_BoundingBoxD &bbox, int tsize, int stride=-1)
		    {
			UT_ASSERT_P(tsize==3);
			doFillVec3BBox(dest, start, length, bbox,
				       tsize, stride);
		    }
    /// @}

    virtual void extendedFillArray(uint8 *data,
			GT_Offset start, GT_Size length,
			int tsize, int nrepeats,
			int stride=-1,
			fpreal black=0, fpreal white=1) const
		    {
			doExtendedQuantizedFill(data, start, length, tsize,
					nrepeats, stride, black, white);
		    }

    /// @{
    /// Extract an element, but making @c nrepeats copies.  The buffer
    /// should be able to contain @c length*nrepeats*tsize elements
    /// When filling an RGB color, with a repeat count of 4, the resulting
    /// buffer will look like: @code
    ///  [ RGB0 RGB0 RGB0 RGB0 RGB1 RGB1 RGB1 RGB1 ... ]
    /// @endcode
    /// With a repeat count of 1, this is equivalent to @c fillArray()
    virtual void extendedFill(uint8 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    virtual void extendedFill(int8 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    virtual void extendedFill(int16 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    virtual void extendedFill(int32 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    virtual void extendedFill(int64 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    virtual void extendedFill(fpreal16 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    virtual void extendedFill(fpreal32 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    virtual void extendedFill(fpreal64 *data, GT_Offset start, GT_Size length,
			int tsize, int nrepeats, int stride=-1) const
		    {
			t_extendedFill(data, start, length, tsize,
					nrepeats, stride);
		    }
    /// @}

    /// @{
    /// Copy the data out of the array as a tuple of @em size entries
    /// The data may be copied, or the array may return a pointer to raw data.
    /// That is, the sub-class may @b not copy the data into the storage.
    virtual const uint8		*get(GT_Offset i, uint8 *store, int sz) const;
    virtual const int8		*get(GT_Offset i, int8 *store, int sz) const;
    virtual const int16		*get(GT_Offset i, int16 *store, int sz) const;
    virtual const int32		*get(GT_Offset i, int32 *store, int sz) const;
    virtual const int64		*get(GT_Offset i, int64 *store, int sz) const;
    virtual const fpreal16	*get(GT_Offset i, fpreal16 *store, int z) const;
    virtual const fpreal32	*get(GT_Offset i, fpreal32 *store, int z) const;
    virtual const fpreal64	*get(GT_Offset i, fpreal64 *store, int z) const;
    /// @}

    /// @{
    /// Get the range of values in the array
    virtual void	getRange(exint &lo, exint &hi, int tuple_idx=0) const;
    virtual void	getRange(fpreal &lo, fpreal &hi, int tidx=0) const;
    /// @}

    /// Compare whether two data arrays are equal.
    bool		isEqual(const GT_DataArray &src) const;

    /// Test to see whether the data array requires int32 or int64 indexing.
    GT_Storage	checkIndexing(GT_IndexingMode mode) const;

    /// Quick & dirty test to see if a size is bigger than a 32 bit int
    static bool	isBigInteger(GT_Size size)
		    { return size >= (((int64)1) << 31); }

    /// For debugging, print values to stdout
    void	dumpValues(const char *msg=NULL) const;

    /// Save array to a JSON stream. The 'flat' determines how to output
    /// arrays with a tuple size greater than 1. If 'flat' is true, the
    /// output will be an uninterrupted stream of values. If 'flat' is
    /// false, then the output will look something like [[1,2,3],[4,5,6], ... ]
    bool	save(UT_JSONWriter &w, bool flat = true) const;

    /// @{
    /// For memory tracking, we override the new/delete operators
    static void	*operator	new(size_t size);
    static void	*operator	new(size_t size, void *p);
    static void	 operator	delete(void *p, size_t size);
    /// @}

    /// Forcibly set the data id for the array.  You take responsibility for
    /// all artifacts and crashes if you call this indiscriminately.
    void	copyDataId(const GT_DataArray &src)
		    { setDataId(src.getDataId()); }

    /// Set the data id - This is used by geometry arrays to copy over the data
    /// id from GA. As above, you take responsibility for all artifacts and
    /// crashes if you set this incorrectly. Do not set this if you don't
    /// know how data IDs work.
    void	setDataId(int64 id)	{ myDataId = id; }
    
    /// Update cached data, in case the underlying attribute changed.
    virtual void	updateGeoDetail(const GU_ConstDetailHandle &dtl,
					const char *attrib_name,
					GT_Owner attrib_owner,
					const int expected_size) {}

protected:
    /// Ensure the size is set properly when performing an import.  If the size
    /// isn't specified, we use the array's tuple size, otherwise we make sure
    /// to clamp the tuple size.
    SYS_STATIC_FORCE_INLINE GT_Size
    fixImportTupleSize(GT_Size size, GT_Size tuple_size)
    {
	return size == 0 ? tuple_size : SYSmin(size, tuple_size);
    }

    /// @{
    /// Virtual implementation for inline methods.
    /// Note that @c doImport is for tuple data, while @c doImportArray
    /// is for tuple array data.
    virtual void doImport(GT_Offset idx, uint8 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int8 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int16 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int32 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, int64 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, fpreal16 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, fpreal32 *data, GT_Size size) const;
    virtual void doImport(GT_Offset idx, fpreal64 *data, GT_Size size) const;
    virtual void doImportArray(GT_Offset idx, UT_ValArray<fpreal32> &data) const;
    virtual void doImportArray(GT_Offset idx, UT_ValArray<int32> &data) const;
    virtual void doImportQuantized(GT_Offset idx, uint8 *data, GT_Size size,
				fpreal black, fpreal white) const;
    
    virtual void doFillArray(uint8 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }

    virtual void doFillArray(int8 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }
    virtual void doFillArray(int16 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }
    virtual void doFillArray(int32 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }
    virtual void doFillArray(int64 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }
    virtual void doFillArray(fpreal16 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }
    virtual void doFillArray(fpreal32 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }
    virtual void doFillArray(fpreal64 *data, GT_Offset start, GT_Size length,
			int tuple_size, int stride) const
		 { t_extendedFill(data, start, length, tuple_size, 1, stride); }
    
    virtual void doFillQuantizedArray(uint8 *data,
			GT_Offset start, GT_Size length,
			int tuple_size, int stride,
			fpreal black, fpreal white) const
		    {
			doExtendedQuantizedFill(data, start, length, tuple_size,
						1, stride, black, white);
		    }
    virtual void doExtendedQuantizedFill(uint8 *data,
			GT_Offset start, GT_Size length,
			int tuple_size, int nrepeats, int stride,
			fpreal black, fpreal white) const;

    /// Nested class to perform filling for a POD array
    template <typename T_POD>
    class fillV3BoxTask
    {
    public:
	fillV3BoxTask(const GT_DataArray &array, T_POD *data,
		GT_Offset start, int stride)
	    : myArray(array)
	    , myData(data)
	    , myStart(start)
	    , myStride(stride)
	{
	    myBox.initBounds();
	}
	fillV3BoxTask(fillV3BoxTask &src, UT_Split)
	    : myArray(src.myArray)
	    , myBox(src.myBox)
	    , myData(src.myData)
	    , myStart(src.myStart)
	    , myStride(src.myStride)
	{
	}
	void	operator()(const UT_BlockedRange<exint> &range)
	{
	    exint	 i = range.begin();
	    T_POD	*dest = myData + i*myStride;
	    for (; i != range.end(); ++i, dest += myStride)
	    {
		myArray.import(i+myStart, dest, 3);
		myBox.enlargeBounds(dest[0], dest[1], dest[2]);
	    }
	}
	void	join(const fillV3BoxTask<T_POD> &src)
	{
	    myBox.enlargeBounds(src.myBox);
	}
	const UT_BoundingBox	&box() const	{ return myBox; }
    private:
	const GT_DataArray	&myArray;
	UT_BoundingBox		 myBox;
	T_POD			*myData;
	GT_Offset		 myStart;
	int			 myStride;
    };

    virtual void doFillVec3BBox(fpreal32 *dest, GT_Offset start, GT_Size length,
				UT_BoundingBoxF &bbox, int, int stride)
    {
	stride = SYSmax(stride, 3);
	fillV3BoxTask<fpreal32>	task(*this, dest, start, stride);
	UTparallelReduceLightItems(UT_BlockedRange<exint>(0, length), task);
	bbox = task.box();
    }
    virtual void doFillVec3BBox(fpreal64 *dest, GT_Offset start, GT_Size length,
				UT_BoundingBoxD &bbox, int, int stride)
    {
	stride = SYSmax(stride, 3);
	fillV3BoxTask<fpreal64>	task(*this, dest, start, stride);
	UTparallelReduceLightItems(UT_BlockedRange<exint>(0, length), task);
	bbox = task.box();
    }
    /// @}

    /// Templated method to fill an array
    template <typename T_POD> void
    t_extendedFill(T_POD *dest, GT_Offset start, GT_Size length,
		    int tsize, int nrepeats, int stride) const;

private:
    /// @{
    /// @private
    /// Get more complicated POD data types out of the array
    template <typename T, int tuplesize> class VectorPODAccessor
    {
	public:
	    static T	getValue(const GT_DataArray &array, GT_Offset offset)
			{
			    T	result;
			    UT_ASSERT_P(array.getTupleSize() >= tuplesize);
			    array.get(offset, result.data(), tuplesize);
			    return result;
			}
    };
    class I32Accessor {
	public:
	    static int32 getValue(const GT_DataArray &a, GT_Offset i)
			    { return a.getI32(i); }
    };
    class I64Accessor {
	public:
	    static int64 getValue(const GT_DataArray &a, GT_Offset i)
			    { return a.getI64(i); }
    };
    class F16Accessor {
	public:
	    static fpreal16 getValue(const GT_DataArray &a, GT_Offset i)
			    { return a.getF16(i); }
    };
    class F32Accessor {
	public:
	    static fpreal32 getValue(const GT_DataArray &a, GT_Offset i)
			    { return a.getF32(i); }
    };
    class F64Accessor {
	public:
	    static fpreal64 getValue(const GT_DataArray &a, GT_Offset i)
			    { return a.getF64(i); }
    };
    /// @}

    /// @{
    /// @private
    /// Private template class to register POD accessors
    template <typename T, int FAKE> class TypeInfo {};

    /// @{
    /// @private
    /// Simple scalar and vector accessors
    template <int FAKE>	class TypeInfo<int32, FAKE>
	{ public: typedef I32Accessor Accessors; };
    template <int FAKE>	class TypeInfo<int64, FAKE>
	{ public: typedef I64Accessor Accessors; };
    template <int FAKE>	class TypeInfo<fpreal16, FAKE>
	{ public: typedef F16Accessor Accessors; };
    template <int FAKE>	class TypeInfo<fpreal32, FAKE>
	{ public: typedef F32Accessor Accessors; };
    template <int FAKE>	class TypeInfo<fpreal64, FAKE>
	{ public: typedef F64Accessor Accessors; };

    template <int FAKE>	class TypeInfo<UT_Vector3F, FAKE>
	{ public: typedef VectorPODAccessor<UT_Vector3F, 3> Accessors; };
    template <int FAKE>	class TypeInfo<UT_Vector3D, FAKE>
	{ public: typedef VectorPODAccessor<UT_Vector3D, 3> Accessors; };
    template <int FAKE>	class TypeInfo<UT_Vector4F, FAKE>
	{ public: typedef VectorPODAccessor<UT_Vector4F, 4> Accessors; };
    template <int FAKE>	class TypeInfo<UT_Vector4D, FAKE>
	{ public: typedef VectorPODAccessor<UT_Vector4D, 4> Accessors; };
    template <int FAKE>	class TypeInfo<UT_Matrix3F, FAKE>
	{ public: typedef VectorPODAccessor<UT_Matrix3F, 9> Accessors; };
    template <int FAKE>	class TypeInfo<UT_Matrix3D, FAKE>
	{ public: typedef VectorPODAccessor<UT_Matrix3D, 9> Accessors; };
    template <int FAKE>	class TypeInfo<UT_Matrix4F, FAKE>
	{ public: typedef VectorPODAccessor<UT_Matrix4F, 16> Accessors; };
    template <int FAKE>	class TypeInfo<UT_Matrix4D, FAKE>
	{ public: typedef VectorPODAccessor<UT_Matrix4D, 16> Accessors; };
    /// @}
public:
    /// Public accessor for POD types
    template <typename T>	T
	getValue(GT_Offset index) const
	    { return TypeInfo<T, 0>::Accessors::getValue(*this, index); }

    template <typename T>	T
	lerpValue(GT_Offset i0, GT_Offset i1, fpreal t) const
	{
	    return (1-t)*getValue<T>(i0) + t*getValue<T>(i1);
	}

    template <typename T>	T
	bilerpValue( GT_Offset u0v0, GT_Offset u1v0,
		GT_Offset u0v1, GT_Offset u1v1, fpreal u, fpreal v)
	{
	    T	l = (1-v)*getValue<T>(u0v0) + v*getValue<T>(u0v1);
	    T	r = (1-v)*getValue<T>(u1v0) + v*getValue<T>(u1v1);
	    return (1-u)*l + u*r;
	}

private:
    int64		myDataId;
};

#endif
