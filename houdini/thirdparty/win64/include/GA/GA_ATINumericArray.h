/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATINumericArray.h (GA Library, C++)
 *
 * COMMENTS:	Array DAta ATI (Attribute Type Implementation)
 *
 * 		Each element can have a different sized array
 *		of data.  This is *not* the same as a tuple as the
 *		size is variable!
 */

#pragma once

#ifndef __GA_ATINumericArray
#define __GA_ATINumericArray

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AttributeType.h"
#include "GA_ArrayDataArray.h"
#include "GA_Range.h"
#include "GA_Types.h"

#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Types.h>

class GA_AIFCompare;
class GA_AIFCopyData;
class GA_AIFEdit;
class GA_AIFJSON;
class GA_Defragment;
class GA_IndexMap;
class GA_LoadMap;
class GA_MergeMap;
class GA_SaveMap;

class UT_JSONParser;
class UT_JSONWriter;


class GA_API GA_ATINumericArray : public GA_Attribute
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
    static GA_ATINumericArray *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATINumericArray *>(attrib);
        return NULL;
    }
    SYS_FORCE_INLINE
    static const GA_ATINumericArray *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATINumericArray *>(attrib);
        return NULL;
    }

    GA_ATINumericArray(const GA_AttributeType &type,
			  const GA_IndexMap &index_map, GA_AttributeScope scope,
			  const UT_StringHolder &name,
			  GA_Storage value_store, int tuple_size);
    virtual ~GA_ATINumericArray();

    /// Report memory used
    virtual int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    virtual void	reconstructElementBlock(GA_Offset offset, GA_Offset nelements);

    /// @{
    /// Interface for defragmentation
    virtual void	defragment(const GA_Defragment &defrag);
    /// @}

    /// Adding entries is thread-safe, so we're only subject to GA_DataArray
    /// limitations.
    virtual WriteConcurrence	 getSupportedWriteConcurrence() const
					{ return WRITE_CONCURRENCE_PAGE; }

    virtual const GA_AIFCopyData	*getAIFCopyData() const
						{ return myAIFCopyData; }
    virtual const GA_AIFIndexPair	*getAIFIndexPair() const
						{ return 0; }
    virtual const GA_AIFMerge		*getAIFMerge() const
						{ return myAIFMerge; }
    virtual const GA_AIFEdit		*getAIFEdit() const
						{ return 0; }
    virtual const GA_AIFTuple		*getAIFTuple() const
						{ return 0; }
    virtual const GA_AIFMath		*getAIFMath() const
						{ return 0; }
    virtual const GA_AIFInterp		*getAIFInterp() const
						{ return 0; }
    virtual const GA_AIFCompare		*getAIFCompare() const
						{ return myAIFCompare; }
    virtual const GA_AIFNumericArray	*getAIFNumericArray() const
						{ return myAIFNumericArray; }

    /// @section JSON-GA_ATINumericArray JSON Schema: GA_ATINumericArray
    /// @code
    /// {
    ///     "name"          : "GA_ATINumericArray",
    ///     "description"   : "An array of arrays of tuples",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "size": {
    ///             "type"          : "integer",
    ///             "minimum"       : 1,
    ///             "description"   : "Tuple size",
    ///         },
    ///         "storage": {
    ///             "type"        : "string",
    ///             "description" : "Storage type for values",
    ///             "enum"        : [ "uint8","int8","int16","int32","int64",
    ///                               "fpreal16", "fpreal32", "fpreal64" ],
    ///         },
    ///         "values": {
    ///             "type"        : {"$ref":"GA_DataArrayTuple"},
    ///             "description" : "Array values.",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    virtual const GA_AIFJSON	*getAIFJSON() const	{ return myAIFJSON; }

    GA_Storage   getDataStorage() const;

    bool	 setDataStorage(GA_Storage storage);

    /// The size of the data tuple
    int		 getDataTupleSize() const;

    /// Grow or shrink the array size
    virtual bool	 setArraySize(GA_Offset new_size);

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
        const GA_ATINumericArray *thatn = UTverify_cast<const GA_ATINumericArray *>(that);
        if (myData.getStorage() != thatn->myData.getStorage())
            return false;
        if (myData.getTupleSize() != thatn->myData.getTupleSize())
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

protected:
    bool	setDataTupleSize(int size);

    /// @{
    /// Copy Data AIF
    bool	copyData(GA_Offset di, const GA_ATINumericArray &src,
			GA_Offset si);
    bool	copyData(const GA_Range &di, const GA_ATINumericArray &src,
			const GA_Range &si);
    /// @}

    /// @{
    /// Compare AIF
    bool	isAlmostEqual(GA_Offset di, const GA_ATINumericArray &src,
			GA_Offset si, int ulps) const;
    bool	isAlmostEqual(const GA_Range &di, const GA_ATINumericArray &src,
			const GA_Range &si, int ulps) const;
    /// @}

    /// @{ GA_AIFMerge
    /// Base class implementation of GA_AIFMerge::destroyDestination()
    void		 mergeDestroyDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    /// Base class implementation of GA_AIFMerge::addDestination()
    GA_Attribute	*mergeAddDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    /// Base class implementation of GA_AIFMerge::copyArray()
    bool		 mergeAppendData(const GA_MergeMap &map,
					const GA_Attribute *sattrib);
    void		 mergeGrowArray(const GA_MergeMap &map,
					const GA_ATINumericArray &s);
    /// @}

protected:
    GA_ArrayDataArray		  myData;

private:
    virtual GA_Attribute	*doClone(const GA_IndexMap &index_map,
				       const UT_StringHolder &name) const;

    /// @{ GA_AIFJSON
    bool		 jsonSave(UT_JSONWriter &w, const GA_SaveMap &s) const;
    bool		 jsonLoad(UT_JSONParser &p, const GA_LoadMap &l);
    /// @}

    static GA_AIFMerge		*myAIFMerge;
    static GA_AIFCopyData	*myAIFCopyData;
    static GA_AIFJSON		*myAIFJSON;
    static GA_AIFCompare	*myAIFCompare;
    static GA_AIFNumericArray	*myAIFNumericArray;

    static const GA_AttributeType *theAttributeType;

    /// @cond INTERNAL_DOX
    friend class ga_ArrayDataMerge;
    friend class ga_ArrayDataJSON;
    friend class ga_ArrayDataCopyData;
    friend class ga_ArrayDataCompare;
    friend class ga_ArrayNumericArray;
    /// @endcond
};

#endif

