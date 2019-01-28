/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATIBlob.h ( GA Library, C++)
 *
 * COMMENTS:	Blob ATI (Attribute Type Implementation)
 */

#pragma once

#ifndef __GA_ATIBlob__
#define __GA_ATIBlob__

#define GA_NEW_STRING_DATA_STRUCTURE 1

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_BlobContainer.h"
#include "GA_BlobData.h"
#include "GA_PageArray.h"
#include "GA_Types.h"

#include <UT/UT_StringHolder.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>


class GA_AIFBlob;
class GA_AIFCompare;
class GA_AIFCopyData;
class GA_AIFInterp;
class GA_AIFMerge;
class GA_AttributeType;
class GA_Defragment;
class GA_IndexMap;
class GA_LoadMap;
class GA_MergeMap;
class GA_SaveMap;
class GA_Range;

class UT_JSONParser;
class UT_JSONWriter;
class UT_Options;
template <typename T> class UT_Array;


/// @brief Utility class to load blob data from a JSON stream
///
/// Since blobs are opaque to the blob container, it's impossible to create
/// blobs when loading.  If blobs need to be saved/loaded, a blob loader class
/// will have to be used to create blob data from the stream.
/// This class should load the data stored by the BlobData::jsonSave() method.
class GA_API GA_BlobDataLoader
{
public:
    virtual ~GA_BlobDataLoader() {}

    /// Load the data from the JSON parser and assign the blob to the handle
    /// passed in.  If there's an error loading the blob, then, return false.
    virtual bool	jsonLoad(UT_JSONParser &p,
				const GA_LoadMap &load,
				GA_BlobRef &handle) const = 0;
};

/// @brief A simple ATI to store aribtrary "blobs" of data in an attribute
///
/// The blob attribute type stores arbitrary blobs (GA_Blob) of data for each
/// element in the attribute array.  The blobs are stored as reference counted
/// shared objects, meaning the blobs may be shared between multiple elements
/// of the array.  Each blob is referenced by a unique integer handle.
///
/// It's also possible to get a list of all the blobs stored by the attribute
///
/// By default, the array is filled with NULL pointers.
///
/// This class is very simple and only provides a minimal interface.  Blob
/// attributes are not saved/loaded, and there is minimal access to the blobs.
///
/// Users may sub-class from this ATI to create more complicated classes which
/// provide alternate interfaces.
///
/// This attribute looks for options (GA_Attribute::getOptions())
/// - bool blob:stringset (default: true)@n
///   This option is queried when merging detail attributes.  When true,
///   the @b unique blobs from both details will be merged into the resulting
///   detail.  When false, the resulting detail will only have the blobs from
///   the first detail in the merge.
///
class GA_API GA_ATIBlob : public GA_Attribute
{
public:
    static void registerType();
    SYS_FORCE_INLINE
    static const UT_StringHolder &getTypeName()
    { return theAttributeType->getTypeName(); }
    SYS_FORCE_INLINE
    static const GA_AttributeType &getType() { return *theAttributeType; }

#if GA_NEW_STRING_DATA_STRUCTURE
    SYS_FORCE_INLINE
    static bool isType(const GA_Attribute *attrib)
    {
        return attrib && (&attrib->getType() == theAttributeType);
    }
    SYS_FORCE_INLINE
    static GA_ATIBlob *cast(GA_Attribute *attrib)
    {
        if (attrib && (&attrib->getType() == theAttributeType))
            return static_cast<GA_ATIBlob *>(attrib);
        return nullptr;
    }
    SYS_FORCE_INLINE
    static const GA_ATIBlob *cast(const GA_Attribute *attrib)
    {
        if (attrib && (&attrib->getType() == theAttributeType))
            return static_cast<const GA_ATIBlob *>(attrib);
        return nullptr;
    }
#else
    SYS_FORCE_INLINE
    static bool isType(const GA_Attribute *attrib);
    SYS_FORCE_INLINE
    static GA_ATIBlob *cast(GA_Attribute *attrib);
    SYS_FORCE_INLINE
    static const GA_ATIBlob *cast(const GA_Attribute *attrib);
#endif

    static GA_Attribute *create(const GA_IndexMap &index_map,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				int tuple_size,
				const GA_AttributeOptions *attribute_options=NULL);
    static GA_Attribute *create(const GA_IndexMap &index_map,
				const UT_StringHolder &name,
				int tuple_size)
			{
			    return create(index_map, GA_SCOPE_PUBLIC, name,
					  tuple_size);
			}

    GA_ATIBlob(const GA_AttributeType &type,
			const GA_IndexMap &index_map,
			GA_AttributeScope scope,
			const char *name,
			int tuple_size);
    virtual ~GA_ATIBlob();

    /// Report approximate memory usage
    virtual int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// @{
    /// Interface for defragmentation
    virtual void	defragment(const GA_Defragment &defrag);
    /// @}

    virtual void        reconstructElementBlock(GA_Offset offset, GA_Offset nelements);

    /// Get the tuple size
    SYS_FORCE_INLINE
    int			getTupleSize() const
			    { return myHandles.getTupleSize(); }

    /// Return the entries in the blob container
    int			entries() const
			    { return myBlobs.entries(); }

    /// Return the maximum index of any blob in the container.  This may be
    /// more than the number of blobs in the container.  If the maximum index
    /// is less than 0, there are no blobs in the container.
    GA_BlobIndex	getMaximumIndex() const
			    { return myBlobs.getMaximumIndex(); }

    /// Return the capacity of the blob container
    int			capacity() const
			    { return myBlobs.capacity(); }

    /// Look up a blob handle by offset.
    SYS_FORCE_INLINE
    GA_BlobIndex        getBlobIndex(GA_Offset offset, int tuple_index=0) const
    {
        if (tuple_index >= 0 && tuple_index < getTupleSize())
            return GA_BlobIndex(myHandles.get(offset, tuple_index));
        return GA_BlobIndex(GA_INVALID_BLOB_INDEX);
    }

    /// Store a new blob_index at the given offset
    bool		setBlobIndex(GA_BlobIndex blob_index, GA_Offset offset,
				     int tuple_idx=0);

    /// Store a new blob_index at the given range of offsets
    bool		setBlobIndex(GA_BlobIndex blob_i,
				     const GA_Range &di,
				     int tuple_index = 0);

    /// Get the blob associated with a given offset into the array
    /// @{
    SYS_FORCE_INLINE
    GA_BlobRef          getBlob(GA_Offset offset, int tuple_idx = 0) const
			{
			    return lookupBlob(getBlobIndex(offset, tuple_idx));
			}
    SYS_FORCE_INLINE
    const GA_BlobData  *getRawBlob(GA_Offset offset, int tuple_idx = 0) const
    {
        return lookupRawBlob(getBlobIndex(offset, tuple_idx));
    }
    /// @}

    /// Store a new blob at the given offset
    bool		setBlob(const GA_BlobRef &blob, GA_Offset offset,
				int tuple_index = 0);

    /// Replace a blob in the blob container with a new blob value
    bool		replaceBlob(GA_BlobIndex handle,
				const GA_BlobRef &blob);


    /// Look up a blob given its handle
    /// @{
    SYS_FORCE_INLINE
    GA_BlobRef		lookupBlob(GA_BlobIndex handle) const
			    { return myBlobs.getBlob(handle); }

    SYS_FORCE_INLINE
    const GA_BlobData *lookupRawBlob(GA_BlobIndex handle) const
    { return myBlobs.getRawBlob(handle); }
    /// @}

    /// Lookup a blob given an ordered index
    GA_BlobRef		getOrderedBlob(exint idx) const
			    { return myBlobs.getOrderedBlob(idx); }

    /// This method will "compact" the attribute container, possibly changing
    /// all the handles in the attribute data.
    virtual void	compactStorage();

    /// Get a measure of the vacancy entropy of the storage container.  This
    fpreal		getStorageOccupancy()
			    { return myBlobs.getOccupancy(); }

    /// Adding blobs is thread-safe, so we're only subject to GA_PageArray
    /// limitations.
    virtual WriteConcurrence	 getSupportedWriteConcurrence() const
				    { return WRITE_CONCURRENCE_PAGE; }

    virtual const GA_AIFBlob	*getAIFBlob() const	{ return myAIFBlob; }
    virtual const GA_AIFMerge	*getAIFMerge() const	{ return myAIFMerge; }
    virtual const GA_AIFCompare	*getAIFCompare() const	{ return myAIFCompare;}
    virtual const GA_AIFCopyData *getAIFCopyData() const{ return myAIFCopyData;}
    virtual const GA_AIFInterp	*getAIFInterp() const	{ return myAIFInterp; }

    /// Save blobs to a JSON stream.
    /// This method can be called by sub-classes to save blob data to a JSON
    /// stream.  Since the GA_ATIBlob class doesn't provide an GA_AIFJSON
    /// interface, data is not saved/loaded by default.
    ///
    /// @param w  The JSON writer
    /// @param s  The save map options
    /// @param blobtoken The token used to identify the blob data
    bool	jsonSave(UT_JSONWriter &w,
			const GA_SaveMap &s,
			const char *blobtoken = "data") const;

    /// Load blobs from a JSON stream.  This method can be called by
    /// sub-classes to load their data from a JSON stream.  The class must
    /// provide a GA_BlobDataLoader class which is used to create and load new
    /// blob data.
    ///
    /// @param p  The JSON parser
    /// @param blobloader  A class to create and load blobs
    /// @param load Load options
    /// @param blobtoken The token used to identify the blob data
    bool	jsonLoad(UT_JSONParser &p,
			const GA_BlobDataLoader &blobloader,
			const GA_LoadMap &load,
			const char *blobtoken = "data");

    /// Convenience function to extract all the blobs (and their handles)
    /// The string handles are guaranteed to be in ascending order, but
    /// may or may not be contiguous.
    int		extractBlobs(UT_Array<GA_BlobRef> &blobs,
			     UT_IntArray &handles) const
		{
		    return myBlobs.extractBlobs(blobs, handles);
		}
    int		extractBlobs(UT_Array<GA_BlobRef> &blobs,
			     UT_IntArray &handles, exint maxblobs) const
		{
		    return myBlobs.extractBlobs(blobs, handles, maxblobs);
		}

    /// Grow or shrink the array size
    virtual bool	setArraySize(GA_Offset new_size);

    /// Try to compress data pages
    virtual void	tryCompressAllPages(
				GA_Offset start_offset = GA_Offset(0),
				GA_Offset end_offset = GA_INVALID_OFFSET);
    /// Harden data pages
    virtual void	hardenAllPages(
				GA_Offset start_offset = GA_Offset(0),
				GA_Offset end_offset = GA_INVALID_OFFSET);

    /// Returns true iff that is an attribute whose content can be copied
    /// from this without any type conversions.  This is important to
    /// avoid reallocation of an attribute if its storage type,
    /// including tuple size, matches the source attribute exactly.
    virtual bool matchesStorage(const GA_Attribute *that) const
    {
        if (!GA_Attribute::matchesStorage(that))
            return false;
        const GA_ATIBlob *thatn = UTverify_cast<const GA_ATIBlob *>(that);
        if (getTupleSize() != thatn->getTupleSize())
            return false;
        return true;
    }

    /// This replaces the entirety of this attribute's content and non-
    /// storage metadata (except the name) with that of the src attribute.
    /// matchesStorage(src) should already return true.
    /// This is primarily for use by GA_AttributeSet::replace().
    /// NOTE: The internal content sizes may not match exactly if the
    ///       attribute type may overallocate, but the sizes should be such
    ///       that any real data will fit in the destination, so be careful
    ///       and deal with the myTailInitialize flag appropriately if
    ///       any extra elements aren't equal to the default.
    virtual void replace(const GA_Attribute &src);

    /// Copy attribute values for a single element.
    /// @{
    virtual bool copy(GA_Offset desti, GA_Offset srci) final
    {
        copyData(desti, this, srci);
        return true;
    }
    virtual bool copy(GA_Offset desti, const GA_Attribute &src, GA_Offset srci) final
    {
        if (!GA_ATIBlob::isType(&src))
            return false;
        copyData(desti, UTverify_cast<const GA_ATIBlob *>(&src), srci);
        return true;
    }
    bool copy(GA_Offset desti, const GA_ATIBlob &src, GA_Offset srci)
    {
        copyData(desti, &src, srci);
        return true;
    }
    /// @}

    /// Copy attribute values for a range of elements.
    /// @{
    virtual bool copy(const GA_Range &destrange, const GA_Range &srcrange) final
    {
        return copyData(destrange, this, srcrange);
    }
    virtual bool copy(const GA_Range &destrange, const GA_Attribute &src, const GA_Range &srcrange) final
    {
        if (!GA_ATIBlob::isType(&src))
            return false;
        return copyData(destrange, UTverify_cast<const GA_ATIBlob*>(&src), srcrange);
    }
    bool copy(const GA_Range &destrange, const GA_ATIBlob &src, const GA_Range &srcrange)
    {
        return copyData(destrange, &src, srcrange);
    }
    /// @}

    /// Assign all elements of a range from a single attribute value.
    /// @{
    virtual bool fill(const GA_Range &destrange, GA_Offset srci) final
    {
        fillData(destrange, this, srci);
        return true;
    }
    virtual bool fill(const GA_Range &destrange, const GA_Attribute &src, GA_Offset srci) final
    {
        if (!GA_ATIBlob::isType(&src))
            return false;
        fillData(destrange, UTverify_cast<const GA_ATIBlob *>(&src), srci);
        return true;
    }
    bool fill(const GA_Range &destrange, const GA_ATIBlob &src, GA_Offset srci)
    {
        fillData(destrange, &src, srci);
        return true;
    }
    /// @}

    /// Validates the internal structure for debugging purposes.
    bool validate() const;

    typedef GA_PageArray<int32> HandleArrayType;

protected:	// Methods
    /// Grow or shrink the tuple size
    virtual bool	setTupleSize(int size);

    /// Blob attributes need each element to properly destruct for accurate
    /// reference counting.
    virtual bool	needDestruction() const;

    /// Callback method to destruct an offset.
    virtual void	destructElement(GA_Offset offset);

    /// Create a new ATIBlob attribute.  Sub-classes must implement this.
    virtual GA_Attribute *doClone(const GA_IndexMap &index_map,
				  const UT_StringHolder &name) const;

    /// @{ GA_AIFMerge
    /// Base class implementation of GA_AIFMerge::destroyDestination()
    void		 mergeDestroyDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    /// Base class implementation of GA_AIFMerge::addDestination()
    GA_Attribute	*mergeAddDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    /// Base class implementation of GA_AIFMerge::growArray()
    void		mergeGrowArray(const GA_MergeMap &map, const GA_ATIBlob &s);

    /// Base class implementation of GA_AIFMerge::copyArray()
    bool		 mergeAppendData(const GA_MergeMap &map,
					const GA_Attribute *sattrib);
    /// @}

    /// @{
    /// Methods to implement copying of data for AIFCopyData
    bool	copyData(GA_Offset di,
				const GA_ATIBlob *s,
				GA_Offset si);
    bool	copyData(const GA_Range &di,
				const GA_ATIBlob *s,
				const GA_Range &si);
    bool	fillData(const GA_Range &di,
				const GA_ATIBlob *s,
				GA_Offset si);
    /// @}

    /// @{
    /// The addBlobBuffer()/delBlobBuffer() methods are added solely for the
    /// corresponding methods in GA_AIFBlob.
    GA_BlobIndex	addBlobReference(const GA_BlobRef &blob);
    void		delBlobReference(GA_BlobIndex handle);
    /// @}

    /// @{
    /// Interface used by AIFCompare.  By default, this will use the blob key
    /// comparison to determine equality.
    virtual bool	isEqual(GA_Offset offset,
			    const GA_ATIBlob &b, GA_Offset b_offset) const;
    virtual bool	isEqual(const GA_Range & range,
			    const GA_ATIBlob &b, const GA_Range &b_range) const;
    /// @}

protected:	// Data
    /// Array of handles.  The default value is -1.
    /// This is protected for convenience to sub-classes.
    HandleArrayType myHandles;

    /// Blob references.  This is protected for convenience to sub-classes.
    GA_BlobContainer myBlobs;

private:
    /// Load up an array of blob
    bool		jsonLoadBlobs(UT_JSONParser &p,
				const GA_LoadMap &load,
				UT_Array<GA_BlobRef> &blobs,
				const GA_BlobDataLoader &blobloader);
#if 0
    /// Add references for the indices referenced by the elements in the
    /// range optimized for the native format of myHandles.
    template <typename T>
	void		addReferencesNativeImpl(const GA_Range &range);
#endif

    /// AIF Merge interface
    static GA_AIFBlob		*myAIFBlob;
    static GA_AIFMerge		*myAIFMerge;
    static GA_AIFCopyData	*myAIFCopyData;
    static GA_AIFCompare	*myAIFCompare;
    static GA_AIFInterp		*myAIFInterp;

    static const GA_AttributeType *theAttributeType;

    /// @cond INTERNAL_DOX
    friend class ga_BlobBlob;
    friend class ga_BlobMerge;
    friend class ga_BlobCopyData;
    friend class ga_BlobCompare;
    /// @endcond
};

#if !GA_NEW_STRING_DATA_STRUCTURE
// NOTE: It may seem strange to include GA_ATIString.h here,
//       but GA_ATIBlob::isType() and cast() need GA_ATIString::isType(),
//       and GA_ATIString inherits from GA_ATIBlob, so
//       GA_ATIBlob::isType() and cast() are defined in GA_ATIString.h
//       below the class.
#include "GA_ATIString.h"
#endif

#endif
