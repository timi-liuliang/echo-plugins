/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFBlobArray.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_AIFBlobArray__
#define __GA_AIFBlobArray__

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
class GA_API GA_AIFBlobArray 
{
public:
	     GA_AIFBlobArray();
    virtual ~GA_AIFBlobArray();

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
    virtual void	getBlobIndex(const GA_Attribute *atr,
				    UT_Array<GA_BlobIndex> &indices,
				    GA_Offset offset) const = 0;
    /// Get the blob data associated with the tuple index of the given element.
    virtual void	getBlob(const GA_Attribute *attrib,
					UT_Array<GA_BlobRef> &blobs,
					GA_Offset offset) const = 0;
    /// Store a blob on the given element
    virtual bool	setBlob(GA_Attribute *attrib,
					const UT_Array<GA_BlobRef> &blobs,
					GA_Offset offset) const = 0;

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

protected:
    /// Methods used by the BlobBuffer methods to add an unreferenced blob to
    /// the attribute.
    virtual GA_BlobIndex	addBlobReference(GA_Attribute *attrib,
					const GA_BlobRef &blob) const = 0;
    virtual void		delBlobReference(GA_Attribute *attrib,
					GA_BlobIndex handle) const=0;
};

#endif
