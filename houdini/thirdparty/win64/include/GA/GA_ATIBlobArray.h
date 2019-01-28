/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATIBlobArray.h ( GA Library, C++)
 *
 * COMMENTS:	Blob Array ATI (Attribute Type Implementation)
 */

#pragma once

#ifndef __GA_ATIBlobArray__
#define __GA_ATIBlobArray__

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_BlobContainer.h"
#include "GA_ATIBlob.h"
#include "GA_BlobData.h"
#include "GA_ArrayDataArray.h"
#include "GA_Types.h"

#include <UT/UT_StringHolder.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>


class GA_AIFBlobArray;
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
class GA_API GA_ATIBlobArray : public GA_Attribute
{
public:
    static void registerType();
    SYS_FORCE_INLINE
    static const UT_StringHolder &getTypeName()
    { return theAttributeType->getTypeName(); }
    SYS_FORCE_INLINE
    static const GA_AttributeType &getType() { return *theAttributeType; }

    SYS_FORCE_INLINE
    static bool isType(const GA_Attribute *attrib);
    SYS_FORCE_INLINE
    static GA_ATIBlobArray *cast(GA_Attribute *attrib);
    SYS_FORCE_INLINE
    static const GA_ATIBlobArray *cast(const GA_Attribute *attrib);

    static GA_Attribute *create(const GA_IndexMap &index_map,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				const GA_AttributeOptions *attribute_options=NULL);
    static GA_Attribute *create(const GA_IndexMap &index_map,
				const UT_StringHolder &name)
			{
			    return create(index_map, GA_SCOPE_PUBLIC, name);
			}

    GA_ATIBlobArray(const GA_AttributeType &type,
			const GA_IndexMap &index_map,
			GA_AttributeScope scope,
			const UT_StringHolder &name,
                        int tuple_size);
    virtual ~GA_ATIBlobArray();

    /// Report approximate memory usage
    virtual int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// @{
    /// Interface for defragmentation
    virtual void	defragment(const GA_Defragment &defrag)
			    { myHandles.defragment(defrag); }
    /// @}

    exint		 findMaximumArrayLength() const
    { return myHandles.findMaximumArrayLength(); }

    /// Get the tuple size
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
    void		getBlobIndex(UT_Array<GA_BlobIndex> &indices, GA_Offset offset) const;

    /// Store a new blob_index at the given offset
    bool		setBlobIndex(const UT_Array<GA_BlobIndex> &indices, 
				     GA_Offset offset);

    /// Get the blob associated with a given offset into the array
    void		getBlob(UT_Array<GA_BlobRef> &blobs, 
				GA_Offset offset) const
			{
			    UT_Array<GA_BlobIndex>	indices;
			    
			    getBlobIndex(indices, offset);
			    blobs.clear();
			    for (int i = 0; i < indices.entries(); i++)
				blobs.append(lookupBlob(indices(i)));
			}

    /// Store a new blob at the given offset
    bool		setBlob(const UT_Array<GA_BlobRef> &blobs, 
				GA_Offset offset);

    /// Replace a blob in the blob container with a new blob value
    bool		replaceBlob(GA_BlobIndex handle,
				const GA_BlobRef &blob);


    /// Look up a blob given its handle
    GA_BlobRef		lookupBlob(GA_BlobIndex handle) const
			    { return myBlobs.getBlob(handle); }

    /// Lookup a blob given an ordered index
    GA_BlobRef		getOrderedBlob(exint idx) const
			    { return myBlobs.getOrderedBlob(idx); }

    /// This method will "compact" the attribute container, possibly changing
    /// all the handles in the attribute data.
    virtual void	compactStorage();

    /// Get a measure of the vacancy entropy of the storage container.  This
    fpreal		getStorageOccupancy()
			    { return myBlobs.getOccupancy(); }

    /// Adding blobs is thread-safe, so we're only subject to GA_DataArray
    /// limitations.
    virtual WriteConcurrence	 getSupportedWriteConcurrence() const
				    { return WRITE_CONCURRENCE_PAGE; }

    virtual const GA_AIFBlobArray	*getAIFBlobArray() const	{ return myAIFBlobArray; }
    virtual const GA_AIFMerge	*getAIFMerge() const	{ return myAIFMerge; }
    virtual const GA_AIFCompare	*getAIFCompare() const	{ return myAIFCompare;}
    virtual const GA_AIFCopyData *getAIFCopyData() const{ return myAIFCopyData;}
    virtual const GA_AIFInterp	*getAIFInterp() const	{ return 0; }

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

    /// Grow or shrink the array size
    virtual bool	setArraySize(GA_Offset new_size);

    /// Return the array size for a given offset
    virtual exint	arraySize(GA_Offset item) const;

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
        const GA_ATIBlobArray *thatn = UTverify_cast<const GA_ATIBlobArray *>(that);
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

protected:	// Methods
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
    void		mergeGrowArray(const GA_MergeMap &map, const GA_ATIBlobArray &s);

    /// Base class implementation of GA_AIFMerge::copyArray()
    bool		 mergeAppendData(const GA_MergeMap &map,
					const GA_Attribute *sattrib);
    /// @}

    /// @{
    /// Methods to implement copying of data for AIFCopyData
    bool	copyData(GA_Offset di,
				const GA_ATIBlobArray *s,
				GA_Offset si);
    bool	copyData(const GA_Range &di,
				const GA_ATIBlobArray *s,
				const GA_Range &si);
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
			    const GA_ATIBlobArray &b, GA_Offset b_offset) const;
    virtual bool	isEqual(const GA_Range & range,
			    const GA_ATIBlobArray &b, const GA_Range &b_range) const;
    /// @}

protected:	// Data
    /// Array of handles.  The default value is []
    /// This is protected for convenience to sub-classes.
    GA_ArrayDataArray		 myHandles;

    /// Blob references.  This is protected for convenience to sub-classes.
    GA_BlobContainer		 myBlobs;

private:
    /// Load up an array of blob
    bool		jsonLoadBlobs(UT_JSONParser &p,
				const GA_LoadMap &load,
				UT_Array<GA_BlobRef> &blobs,
				const GA_BlobDataLoader &blobloader);

    /// AIF Merge interface
    static GA_AIFBlobArray	*myAIFBlobArray;
    static GA_AIFMerge		*myAIFMerge;
    static GA_AIFCopyData	*myAIFCopyData;
    static GA_AIFCompare	*myAIFCompare;

    static const GA_AttributeType *theAttributeType;

    /// @cond INTERNAL_DOX
    friend class ga_BlobArrayBlob;
    friend class ga_BlobArrayMerge;
    friend class ga_BlobArrayCopyData;
    friend class ga_BlobArrayCompare;
    /// @endcond
};

// NOTE: It may seem strange to include GA_ATIStringArray.h here,
//       but GA_ATIBlobArray::isType() and cast() need GA_ATIStringArray::isType(),
//       and GA_ATIStringArray inherits from GA_ATIBlobArray, so
//       GA_ATIBlobArray::isType() and cast() are defined in GA_ATIStringArray.h
//       below the class.
#include "GA_ATIStringArray.h"

#endif

