/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFBlob.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_AIFBlob__
#define __GA_AIFBlob__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_BlobData.h"
#include "GA_BlobContainer.h"

#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>


class GA_Range;
class GA_Attribute;


typedef UT_ValArray<GA_BlobIndex>	GA_BlobIndexList;
typedef UT_Array<GA_BlobRef>		GA_BlobRefList;


/// @brief Attribute Interface for accessing generic blob data
///
/// This attribute interface allows access to blob data on an attribute.
/// This is provided automatically if you inherit from GA_ATIBlob.
class GA_API GA_AIFBlob 
{
public:
	     GA_AIFBlob();
    virtual ~GA_AIFBlob();

    /// Return the number of blobs per element
    virtual int		getTupleSize(const GA_Attribute *attrib) const = 0;

    /// Set the tuple size
    virtual bool	setTupleSize(GA_Attribute *attrib, int size) const = 0;

    /// Return the total number of unique blobs stored in the attribute
    virtual GA_Size	getBlobCount(const GA_Attribute *attrib) const = 0;

    /// Given a blob integer identifier, return a handle to the actual blob data
    virtual GA_BlobRef		lookupBlob(const GA_Attribute *attrib,
					GA_BlobIndex handle) const = 0;

    /// Blob handles (GA_BlobIndex) may not be contiguous.
    /// This may be an expensive operation.
    virtual GA_BlobRef		lookupOrderedBlob(const GA_Attribute *attrib,
					exint index) const = 0;

    /// Get the integer identifier for the blob associated with the tuple index
    /// of the given element.  An element with no blob data will have an index
    /// less than 0.
    virtual GA_BlobIndex	getBlobIndex(const GA_Attribute *atr,
					GA_Offset offset,
					int tuple_index=0) const = 0;
    /// Get the blob data associated with the tuple index of the given element.
    virtual GA_BlobRef		getBlob(const GA_Attribute *attrib,
					GA_Offset offset,
					int tuple_index=0) const = 0;
    /// Store a blob on the given element
    virtual bool		setBlob(GA_Attribute *attrib,
					const GA_BlobRef &blob,
					GA_Offset offset,
					int tuple_index=0) const = 0;

    /// Replace the value of the blob at the given index with a new value.
    /// Implementors should be careful to check whether the @c new_blob exists
    /// (i.e. renaming an existing blob to another existing blob).
    /// All blobs which reference the original handle will now reference the
    /// new blob.
    virtual bool		replaceBlob(GA_Attribute *attrib,
					GA_BlobIndex handle,
					const GA_BlobRef &new_blob) const =0;

    /// Compact the storage by removing empty indexes
    virtual void		compactStorage(GA_Attribute *attrib) const = 0;

    /// @brief Class which holds temporary references to blobs
    /// 
    /// In some cases, it's expedient to add multiple blobs to the attribute
    /// and then assign the handle values after the fact.  This class is
    /// similar to the GA_AIFBlob::StringBuffer class.
    class BlobBuffer
    {
    public:
	BlobBuffer(GA_Attribute *attribute = NULL,
		    const GA_AIFBlob *aif=NULL)
	    : myAttribute(attribute)
	    , myAIFBlob(aif)
	{
	}
	BlobBuffer(const BlobBuffer &src)
	    : myAttribute(NULL)
	    , myAIFBlob(NULL)
	{
	    *this = src;
	}
	~BlobBuffer()
	{
	    clear();
	}

	BlobBuffer	&operator=(const BlobBuffer &src)
			 {
			     if (&src != this)
			     {
				 clear();
				 myAttribute = src.myAttribute;
				 myAIFBlob = src.myAIFBlob;
				 for (exint i = 0; i < src.entries(); ++i)
				     append(src.getBlob(i));
			     }
			     return *this;
			 }

	/// Return number of strings referenced in the string buffer
	GA_Size		entries() const	{ return myIndexList.entries(); }

	/// Add a string to the attribute.  Returns the handle of the string in
	/// the attribute (not the string's index in the buffer).
	GA_BlobIndex	append(const GA_BlobRef &blob);

	/// Return the string index from the buffer.  The index refers to the
	/// buffer index (not the blob handle).
	GA_BlobIndex	 getBlobIndex(exint i) const
				    { return myIndexList(i); }
	/// Return a string handle from the buffer.  The index refers to the
	/// buffer index (not the blob handle).
	GA_BlobRef		 getBlob(exint i) const;

	/// Clear references to all strings contained in the buffer.
	void			clear();
    private:
	GA_BlobIndexList	 myIndexList;
	const GA_AIFBlob	*myAIFBlob;
	GA_Attribute		*myAttribute;
    };

public:
    /// The Array API.  This API allows the attribute to store an "array" on
    /// each data element.  This is done by changing the tuple size to match
    /// the array length.  Each tuple, therefore, represents an array.  The
    /// length is determined by the occupied entries in the tuple.
    ///
    /// These methods can all be implemented using the other API functions.
    /// However, they provide a simpler interface.

    /// Reserve space for an array of the given size
    virtual bool	arrayReserve(GA_Attribute *attrib, int length) const
			{
			    if (length <= getTupleSize(attrib))
				return true;
			    return setTupleSize(attrib, length);
			}

    /// Truncate the array to the given entries
    virtual bool	arrayTruncate(GA_Attribute *attrib, int length) const
			{
			    if (length >= getTupleSize(attrib))
				return true;
			    return setTupleSize(attrib, length);
			}

    /// Trim the array to the minimum required size.  This may be an expensive
    /// operation as it traverses all elements to compute the minimum size,
    /// then calls truncate.  The method returns the new size.
    virtual int		arrayTrim(GA_Attribute *attrib) const;

    /// Append an element to the end of the array for the given element
    virtual int		arrayAppendBlob(GA_Attribute *attrib,
					const GA_BlobRef &blob,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Append an element to the array, but only if the element is unique in
    /// the array.
    virtual int		arrayAppendUniqueBlob(GA_Attribute *attrib,
					const GA_BlobRef &blob,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Append an element to the end of the array for the given element
    virtual int		arrayAppendBlobIndex(GA_Attribute *attrib,
					GA_BlobIndex handle,
					GA_Offset element_index=GA_Offset(0)) const
			{
			    return arrayAppendBlob(attrib,
				    lookupBlob(attrib, handle),
				    element_index);
			}

    /// Append an element to the array, but only if the element is unique in
    /// the array.
    virtual int		arrayAppendUniqueBlobIndex(GA_Attribute *attrib,
					GA_BlobIndex handle,
					GA_Offset element_index=GA_Offset(0)) const
			{
			    return arrayAppendUniqueBlob(attrib,
				    lookupBlob(attrib, handle),
				    element_index);
			}


    /// Return the length of occupied blobs in the array.  This is primarily
    /// useful for global attributes.
    virtual int		arrayGetLength(const GA_Attribute *attrib,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Return the maximum length of all elements in the range
    virtual int		arrayGetMaxLength(const GA_Attribute *attrib,
					const GA_Range &range) const;

    /// Return the blob handle for the item at the given array index.
    virtual GA_BlobIndex	arrayGetBlobIndex(const GA_Attribute *attrib,
					int array_index,
					GA_Offset element_index=GA_Offset(0)) const
			{
			    return getBlobIndex(attrib, element_index,
						array_index);
			}
    /// Return the blob for the item at the given array index.
    virtual GA_BlobRef		arrayGetBlob(const GA_Attribute *attrib,
					int array_index,
					GA_Offset element_index=GA_Offset(0)) const
			{
			    return getBlob(attrib, element_index, array_index);
			}
    virtual int		arrayGetBlobs(GA_BlobRefList &list,
					const GA_Attribute *attrib,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Find a blob in the array, returning its index.  Returns -1 if the blob
    /// isn't found.
    virtual int		arrayFindBlob(const GA_Attribute *attrib,
					const GA_BlobRef &blob,
					GA_Offset element_index=GA_Offset(0)) const;
    /// Delete a blob from the index.  This will shift occupied blobs after the
    /// deleted blob so that the tuple contains a contiguous array of occupied
    /// tuple indices.
    virtual bool	arrayDestroyBlob(GA_Attribute *attrib,
					int array_index,
					GA_Offset element_index=GA_Offset(0)) const;

    /// Remove duplicate blobs from the array
    /// The @c element_index specifies the point, primitive, vertex etc.
    virtual int		arrayRemoveDuplicates(GA_Attribute *attrib,
					GA_Offset element_index=GA_Offset(0)) const;
protected:
    /// Methods used by the BlobBuffer methods to add an unreferenced blob to
    /// the attribute.
    virtual GA_BlobIndex	addBlobReference(GA_Attribute *attrib,
					const GA_BlobRef &blob) const = 0;
    virtual void		delBlobReference(GA_Attribute *attrib,
					GA_BlobIndex handle) const=0;
    friend class	BlobBuffer;
};

#endif
