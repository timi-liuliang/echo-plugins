/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_BlobContainer.h ( GA Library, C++)
 *
 */

#ifndef __GA_BlobContainer__
#define __GA_BlobContainer__

#include "GA_API.h"
#include "GA_BlobData.h"

#include <UT/UT_IndexedHashMapT.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>


template <typename T> class UT_Array;
template <typename T> class UT_ValArray;
class UT_MemoryCounter;


typedef UT_IndexedHashMapItemId		GA_BlobIndex;
#define GA_INVALID_BLOB_INDEX		GA_BlobIndex(-1)


/// @brief Container to store blobs of arbitrary data for attributes
///
/// The geometry library can store blobs of binary data for each object
/// element.  It does this by storing a shared reference count.  Blobs are
/// added to the blob container and accessed by integer handle (index).
///
/// As a developer, the semantics of: @code
///	handle = container.addBlob(blob);
/// @endcode
/// are that the container will create a @b copy of the blob.  It will
/// associate an integer index with that blob and return the index.
///
/// To test whether a blob is in the container, you can call:  @code
///	handle = container.getIndex(blob);
/// @endcode
/// This will not add a blob to the container.
///
/// You get the blob back out of the container by calling: @code
///	blob = container.getBlob(handle);
/// @endcode
/// It's important to realize that the blob you get back from the container is
/// immutable.  If you need to change the blob, you need to make a copy and
/// store the modifed version in the container.
///
/// Blobs are deleted from the container by calling: @code
///	container.freeBlob(handle);
/// @endcode
/// You should call @c freeBlob() for each blob you stored calling @addBlob()
///
/// @note
/// Blobs are stored using UT_COW.  This enforces the fact that the blobs are
/// immutable once they have been stored in the container.  The container only
/// provides read-pointers when you get blobs out of the container.
///
/// An example of how you might store a blob: @code
///	GA_BlobRef	bref(new MyBlobSubClass());
///	handle = container.addBlob( bref );
/// @endcode
///	

class GA_API GA_BlobContainer
{
public:
     GA_BlobContainer();
    ~GA_BlobContainer();

    /// Report approximate memory usage (*including* storage for blobs)
    /// @note Not thread safe
    int64	 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    /// @note Not thread safe
    void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Allocated size of the container
    /// @note Not thread safe
    int		 capacity() const
		    { return myMap.getItemIdUpperBound()+1; }
    /// Occupied size of the container
    /// @note Not thread safe
    int		 entries() const
		    { return myMap.entries(); }

    /// Get occupancy of the map which can be used to determine whether
    /// compaction is required.
    /// @note Not thread safe
    fpreal	getOccupancy() const
		{
		    return myMap.getOccupancy();
		}

    /// Return the blob for a given handle.  If the handle isn't valid,
    /// a NULL ptr will be returned.
    /// @note This method is thread safe
    SYS_FORCE_INLINE
    GA_BlobRef getBlob(GA_BlobIndex handle) const
		{
		    const MapItem *item = myMap[handle];
		    return item ? item->getKey().getBlob() : GA_BlobRef();
		}

    /// Return the blob for a given handle.  If the handle isn't valid,
    /// a NULL ptr will be returned.
    /// @note This method is thread safe
    SYS_FORCE_INLINE
    const GA_BlobData *getRawBlob(GA_BlobIndex handle) const
    {
        const MapItem *item = myMap[handle];
        return item ? item->getKey().getBlob().get() : nullptr;
    }

    /// Return the n'th blob given in an ordered list of blobs.  This method
    /// may be significantly more expensive than looking up by @c GA_BlobIndex
    /// or extracting all items at one time.  For example: @code
    ///   for (exint i = 0; ; ++i)
    ///	  {
    ///       GA_BlobRef blob = blobcontainer.getOrderedBlob(i);
    ///       if (!blob)
    ///            break;
    ///   }
    /// @note This method is @b not thread safe
    GA_BlobRef		getOrderedBlob(exint index) const
			{
			    const MapItem	*item;
			    item = myMap.getOrderedItem(index);
			    return item ? item->getKey().getBlob()
					: GA_BlobRef();
			}

    /// @{
    /// Look up an index for a given blob.  If the blob isn't in the container,
    /// the return code will be -1.
    /// @note This method is thread safe
    GA_BlobIndex	 getIndex(const GA_BlobRef &blob) const
			    { return myMap.findId(blob); }
    /// @}

    /// Blobs are stored in a sparse array.  When looping over all valid
    /// offsets, this method will "validate" the handle.  That is, if there's
    /// no blob stored at the location, it will map it to -1.
    /// @note This method is thread safe
    GA_BlobIndex	 validateIndex(GA_BlobIndex index) const;

    /// @{
    /// Replace the blob for the given index with a new blob.  The new index
    /// will be returned.  This is equivalent to: @code
    ///		freeBlob(prevhandle);
    ///		return addBlob(blob);
    /// @endcode
    /// @note This method is thread safe
    GA_BlobIndex	 replaceBlob(GA_BlobIndex prevhandle,
				     const GA_BlobRef &blob)
			    { return myMap.replaceItem(prevhandle, blob); }
    /// @}

#if 0
    /// Replace the blob for the given index with a new blob.  The new index
    /// will be returned.  This is equivalent to: @code
    ///	   return replaceBlob(prevhandle, GA_BlobRef(getBlob(newhandle)));
    /// @endcode
    /// @note This method is thread safe
    GA_BlobIndex	 replaceBlob(GA_BlobIndex prevhandle,
				     GA_BlobIndex newhandle);
#endif

    /// @{
    /// Allocate a new blob and return the index to the blob.
    /// @note This method is thread safe
    GA_BlobIndex	 addBlob(const GA_BlobRef &blob);
    /// @}

    /// Add a reference to the blob given by the index.
    ///	This is equivalent to: @code
    ///		addBlob( getBlob(handle) );
    /// @endcode
    /// @note This method is thread safe
    /// @note inc must be positive.
    /// @{
    void		 addIndexReference(GA_BlobIndex handle, int inc)
			    { myMap.addReference(handle, inc); }
    void		 addIndexReference(GA_BlobIndex handle)
			    { addIndexReference(handle, 1); }
    /// @}

    /// For debugging purposes, this returns the number of references
    /// to this blob index in this map.
    exint                getIndexReferenceCount(GA_BlobIndex handle) const
    {
        return myMap.getReferenceCount(handle);
    }

    /// Free the blob (de-reference) given by the handle
    /// @note This method is thread safe
    bool		 freeBlob(GA_BlobIndex handle)
			    { return myMap.remove(handle); }

    /// @{
    /// Free the given blob (de-reference) by blob pointer.  This is equivalent
    /// to: @code
    ///		freeBlob( getIndex(blob) );
    /// @endcode
    /// @note This method is thread safe
    bool		 freeBlob(const GA_BlobRef &blob)
			    { return myMap.remove(MapKey(blob)); }
    /// @}

    /// This will forcibly clear out all blobs, regardless of whether they have
    /// references.  Use this with caution.
    /// @note Not thread safe
    void		 clear()	{ myMap.clear(); }

    /// Extract the blobs into two arrays.
    /// - The array of blobs is the list of unique blobs in the container.
    /// - The handles are the corresponding integer handles
    /// The return code is the largest handle in the list (or -1 if there are
    /// no blobs in the container).
    /// @warning The maximum handle value may be is @b less than the number of
    /// blobs stored in the container.  There may also be integer handles which
    /// aren't valid handles between 0 and the maximum.
    /// @note Not thread safe
    GA_BlobIndex	 extractBlobs(UT_Array<GA_BlobRef> &array,
				      UT_IntArray &handles) const;
    GA_BlobIndex	 extractBlobs(UT_Array<GA_BlobRef> &array,
				  UT_IntArray &handles, exint maxblobs) const;

    /// Return the maximum index number used.  If the maximum index is less
    /// than zero, there are no blobs in the container.
    /// @warning The maximum index value may be is @b greater than the number of
    /// blobs stored in the container.  There may also be values which aren't
    /// bound to a blob between 0 and the maximum.
    /// @note Not thread safe
    GA_BlobIndex	 getMaximumIndex() const
			    { return myMap.getItemIdUpperBound(); }

    /// Compact blobs can be called to "shrink" the index list (i.e. remove
    /// all vacancies in the index list).  Since this will change the index
    /// values, a mapping array is returned which can be used to map the
    /// existing handle value to the new handle value.  For example: @code
    ///   UT_ValArray<GA_BlobIndex> map;
    ///	  GA_Range	range(...);
    ///   if (myBlobData.compactStrings(map)) {
    ///       for (GA_Iterator it(range); !it.atEnd(); ++it)
    ///           attribute_data[it.getOffset()] =
    ///                            map(attribute_data[it.getOffset()]);
    ///   }
    /// @endcode
    /// @note Not thread safe
    bool	compactBlobs(UT_ValArray<GA_BlobIndex> &map);

    /// Replaces the content of this with the content of src.
    void replace(const GA_BlobContainer &src);

private:
    class MapKey
    {
    public:
	MapKey(const GA_BlobRef &blob)
	    : myBlob(blob)
	{}
	MapKey(const MapKey &src)
	    : myBlob(src.myBlob)
	{
	}
	uint	hash() const
        {
            return myBlob ? myBlob->hash() : 123456789;
        }
	bool	isEqual(const MapKey &src) const
	{
            if (!myBlob || !src.myBlob)
                return (myBlob && src.myBlob);
            return myBlob->isEqual(*src.myBlob);
        }

        SYS_FORCE_INLINE
	const GA_BlobRef &getBlob() const
        { return myBlob; }
	bool operator<(const MapKey &src) const
	{ return myBlob.get() < src.myBlob.get(); }

        int64 getMemoryUsage(bool inclusive) const
        {
            int64 mem = inclusive ? sizeof(*this) : 0;
            mem += myBlob->getMemoryUsage(true);
            return mem;
        }

        void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    private:
	GA_BlobRef	myBlob;
    };
    typedef UT_IndexedHashMapItemKeyContainer<MapKey>		MapItem;
    typedef ut_IndexedHashMapDeferItemAlloc<MapKey,MapItem>	MapAlloc;

    UT_IndexedHashMapT<MapKey, MapItem, MapAlloc>	myMap;
};

#endif
