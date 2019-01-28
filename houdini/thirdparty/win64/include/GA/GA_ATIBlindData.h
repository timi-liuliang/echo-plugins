/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATIBlindData.h ( GA Library, C++)
 *
 * COMMENTS:	Blind Data ATI (Attribute Type Implementation)
 */

#pragma once

#ifndef __GA_ATIBlindData__
#define __GA_ATIBlindData__

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AttributeType.h"           // for GA_AttributeType
#include "GA_Types.h"                   // for GA_Size, GA_Offset, etc
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Types.h>              // for uint8, int64

#include <stddef.h>                     // for NULL


class GA_AIFBlindData;
class GA_AIFCompare;
class GA_AIFCopyData;
class GA_AIFJSON;
class GA_AIFMerge;
class GA_AIFStat;
class GA_Defragment;
class GA_IndexMap;
class GA_LoadMap;
class GA_MergeMap;
class GA_Range;
class GA_SaveMap;

class UT_JSONParser;
class UT_JSONWriter;
class UT_Options;


/// @brief Attribute type implementation to store blind data per element
///
/// This attribute type allocates a chunk of data per element (point, vertex or
/// detail).  The data is typeless.  Users can write any data to this 
///
/// Creation options may contain:
///   "blinddata:size" - The initial size of the attribute (default 0)
///   "blinddata:align" - The initial alignment (default sizeof(void *))
///   "blinddata:copy" - Copy data from element to element (default false)
class GA_API GA_ATIBlindData : public GA_Attribute
{
public:
    static void registerType();
    SYS_FORCE_INLINE
    static const UT_StringHolder &getTypeName()
    { return theAttributeType->getTypeName(); }
    SYS_FORCE_INLINE
    static const GA_AttributeType &getType() { return *theAttributeType; }

    SYS_FORCE_INLINE
    static bool isType(const GA_Attribute *attrib)
    {
        return attrib && &attrib->getType() == theAttributeType;
    }
    SYS_FORCE_INLINE
    static GA_ATIBlindData *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATIBlindData *>(attrib);
        return NULL;
    }
    SYS_FORCE_INLINE
    static const GA_ATIBlindData *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATIBlindData *>(attrib);
        return NULL;
    }
    static GA_Attribute *create(const GA_IndexMap &index_map,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				GA_Size bytes=-1,
				GA_Size align=-1,
				const GA_AttributeOptions *attribute_options=NULL);

    /// Create blind data attribute
    GA_ATIBlindData(const GA_AttributeType &type,
			const GA_IndexMap &index_map,
			GA_AttributeScope scope,
			const UT_StringHolder &name,
			GA_Size size,
			GA_Size align,
			bool copy_flag,
			bool little_endian);
    virtual ~GA_ATIBlindData();

    /// Report memory usage
    virtual int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// @{
    /// Interface for defragmentation
    virtual void	defragment(const GA_Defragment &defrag);
    /// @}

    /// Supports concurrent writes to separate elements.
    virtual WriteConcurrence	 getSupportedWriteConcurrence() const
					{ return WRITE_CONCURRENCE_ELEMENT; }

    virtual const GA_AIFStat		*getAIFStat() const
						{ return myAIFStat; }
    virtual const GA_AIFBlindData	*getAIFBlindData() const
						{ return myAIFBlindData; }
    virtual const GA_AIFCopyData	*getAIFCopyData() const
					{
					    return myCopyFlag
							? myAIFCopyData
							: NULL;
					}
    const GA_AIFCopyData *getAIFCopyDataAnyway() const
    {
        return myAIFCopyData;
    }
    virtual const GA_AIFCompare		*getAIFCompare() const
					{
					    return myAIFCompare;
					}
    virtual const GA_AIFMerge		*getAIFMerge() const;

    /// @section JSON-GA_ATIBlindData JSON Schema: GA_ATIBlindData
    /// @code
    /// {
    ///     "name"          : "GA_ATIBlindData",
    ///     "description"   : "An array of blind data",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "size": {
    ///             "type"        : "integer",
    ///             "minimum"     : 1,
    ///             "description" : "Bytes per element",
    ///         },
    ///         "alignment": {
    ///             "type"        : "integer",
    ///             "description" : "Data alignment: 0 indicates sizeof(void*)",
    ///         },
    ///		"defaultsize": {
    ///             "type"        : "integer",
    ///             "minimum"     : 1,
    ///             "description" : "Bytes stored in the defualt",
    ///         },
    ///		"endian": {
    ///             "type"        : "string",
    ///             "description" : "Endian storage of binary data",
    ///             "enum"        : [ "little", "big" ],
    ///         },
    ///         "defaults": {
    ///             "type" 	  : "array",
    ///             "description" : "binary data (bytes)",
    ///     	    "items"       : "number",
    ///         },
    ///         "values": {
    ///             "type" 	  : "array",
    ///             "description" : "binary data (bytes)",
    ///     	    "items"       : "number",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    virtual const GA_AIFJSON	*getAIFJSON() const	{ return myAIFJSON; }

    /// Grow data
    virtual bool	 setArraySize(GA_Offset new_size);

    /// Compressing data pages is not applicable to blind data,
    /// so this does nothing.
    virtual void	 tryCompressAllPages(
				GA_Offset start_offset = GA_Offset(0),
                                GA_Offset end_offset = GA_INVALID_OFFSET) {}
    /// Hardening data pages is not applicable to blind data,
    /// so this does nothing.
    virtual void	 hardenAllPages(
				GA_Offset start_offset = GA_Offset(0),
                                GA_Offset end_offset = GA_INVALID_OFFSET) {}

    /// Returns true iff that is an attribute whose content can be copied
    /// from this without any type conversions.  This is important to
    /// avoid reallocation of an attribute if its storage type,
    /// including tuple size, matches the source attribute exactly.
    virtual bool matchesStorage(const GA_Attribute *that) const
    {
        if (!GA_Attribute::matchesStorage(that))
            return false;
        const GA_ATIBlindData *thatn = UTverify_cast<const GA_ATIBlindData *>(that);
        if (getDataSize() != thatn->getDataSize())
            return false;
        if (getAlignedSize() != thatn->getAlignedSize())
            return false;
        if (getAlignment() != thatn->getAlignment())
            return false;
        return true;
    }
    /// In the case that we're copying from an attribute whose storage
    /// type matches this exactly, this function copies the metadata
    /// not associated with the storage, e.g. myOptions,
    /// *excluding* the name and the data ID.
    virtual void copyNonStorageMetadata(const GA_Attribute *that)
    {
        GA_Attribute::copyNonStorageMetadata(that);
        const GA_ATIBlindData *thatn = UTverify_cast<const GA_ATIBlindData *>(that);
        myCopyFlag = thatn->myCopyFlag;
        myLittleEndian = thatn->myLittleEndian;
        setDefault(thatn->myDefault, thatn->myDefaultSize);
    }

    /// This replaces the entirety of this attribute's content and non-
    /// storage metadata (except the name) with that of the src attribute.
    /// The matchesStorage(src) should already return true.
    /// This is primarily for use by GA_AttributeSet::replace().
    /// NOTE: The internal content sizes may not match exactly if the
    ///       attribute type may overallocate, but the sizes should be such
    ///       that any real data will fit in the destination, so be careful
    ///       and deal with the myTailInitialize flag appropriately if
    ///       any extra elements aren't equal to the default.
    virtual void replace(const GA_Attribute &src);

    /// Get read-only access to the data for the specified offset
    const void *getRead(GA_Offset offset) const;

    /// Get read-write access to the data for the specified offset
    void *getWrite(GA_Offset offset);

protected:
    // Create a new ATI_BlindData
    virtual GA_Attribute *doClone(const GA_IndexMap &index_map,
				  const UT_StringHolder &name) const;

    /// Re-initialize data for a deleted element
    virtual void	  reconstructElementBlock(GA_Offset offset, GA_Offset nelements);
private:
    void		 setDefault(const void *data, GA_Size size);
    bool		 growArraySize(GA_Offset new_size);
    void		 growDataSize(GA_Size size=-1, GA_Size align=-1);
    GA_Size		 getDataSize() const	{ return mySize; }
    GA_Size		 getAlignedSize() const	{ return myAlignSize; }
    GA_Size		 getAlignment() const	{ return myAlignment; }
    bool		 getCopyFlag() const	{ return myCopyFlag; }
    void		 setCopyFlag(bool f)	{ myCopyFlag = f; }
    bool		 getLittleEndian() const { return myLittleEndian; }
    void		 swapEndian()	{ myLittleEndian = !myLittleEndian; }
    bool		 copyData(GA_Offset di,
				const GA_ATIBlindData &src, GA_Offset si);
    bool		 copyData(const GA_Range &di,
				const GA_ATIBlindData &src, const GA_Range &si);
    bool		 isEqual(GA_Offset di,
				const GA_ATIBlindData &src,
				GA_Offset si) const;
    bool		 isEqual(const GA_Range &di,
				const GA_ATIBlindData &src,
				const GA_Range &si) const;

    void		 mergeDestroyDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    GA_Attribute	*mergeAddDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    bool		 mergeAppendData(const GA_MergeMap &map,
					const GA_ATIBlindData *sattrib);

    bool		 jsonSave(UT_JSONWriter &w, const GA_SaveMap &s) const;
    bool		 jsonLoad(UT_JSONParser &p, const GA_LoadMap &map,
				GA_Size size,
				GA_Size alignment,
				bool endian);

    static GA_AIFBlindData	*myAIFBlindData;
    static GA_AIFCopyData	*myAIFCopyData;
    static GA_AIFStat		*myAIFStat;
    static GA_AIFJSON		*myAIFJSON;
    static GA_AIFCompare	*myAIFCompare;
    static GA_AIFMerge		*myAIFMerge;

    static const GA_AttributeType *theAttributeType;

    GA_Size			 mySize;
    GA_Size			 myAlignSize;
    GA_Size			 myDefaultSize;
    GA_Size			 myAlignment;
    GA_Offset			 myElementCapacity;
    uint8			*myData;
    uint8			*myDefault;
    bool			 myCopyFlag;
    bool			 myLittleEndian;

    /// @cond INTERNAL_DOX
    friend class ga_BlindDataAccess;
    friend class ga_BlindCopyData;
    friend class ga_BlindDataStat;
    friend class ga_BlindJSON;
    friend class ga_BlindCompare;
    friend class ga_BlindMerge;
    /// @endcond
};

#endif
