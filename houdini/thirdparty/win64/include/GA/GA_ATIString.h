/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATIString.h ( GA Library, C++)
 *
 * COMMENTS:	String ATI (Attribute Type Implementation)
 */

#pragma once

#ifndef __GA_ATIString__
#define __GA_ATIString__

#define GA_NEW_STRING_DATA_STRUCTURE 1

#include "GA_API.h"
#include "GA_AIFSharedStringTuple.h"	// For GA_StringIndexType type
#include "GA_ATIBlob.h"
#include "GA_Attribute.h"
#include "GA_AttributeType.h"
#include "GA_Types.h"

#include <UT/UT_IndexedHashSet.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>

class GA_AIFJSON;
class GA_AIFStringTuple;
class GA_IndexMap;
class GA_Range;

class UT_IStream;

#if GA_NEW_STRING_DATA_STRUCTURE
class GA_API GA_ATIString : public GA_Attribute
#else
class GA_API GA_ATIString : public GA_ATIBlob
#endif
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
    static GA_ATIString *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATIString *>(attrib);
        return nullptr;
    }
    SYS_FORCE_INLINE
    static const GA_ATIString *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATIString *>(attrib);
        return nullptr;
    }

    static GA_Attribute	*create(const GA_IndexMap &index_map,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				int tuple_size,
				const GA_AttributeOptions *attribute_options=nullptr);
    static GA_Attribute	*create(const GA_IndexMap &index_map,
				const UT_StringHolder &name,
				int tuple_size)
		{ return create(index_map, GA_SCOPE_PUBLIC, name, tuple_size); }

    GA_ATIString(const GA_AttributeType &type,
			const GA_IndexMap &index_map,
			GA_AttributeScope scope,
			const UT_StringHolder &name,
			int tuple_size);
    virtual ~GA_ATIString();

#if GA_NEW_STRING_DATA_STRUCTURE
    /// Report approximate memory usage
    virtual int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// Interface for defragmentation
    virtual void defragment(const GA_Defragment &defrag);

    virtual void reconstructElementBlock(GA_Offset offset, GA_Offset nelements);

    /// Get the tuple size
    SYS_FORCE_INLINE
    int getTupleSize() const
    { return myHandles.getTupleSize(); }

    /// Return the entries in the string container
    exint entries() const
    { return myStrings.entries(); }

    /// Return the maximum index of any string in the container.  This may be
    /// more than the number of strings in the container.  If the maximum index
    /// is less than 0, there are no strings in the container.
    GA_BlobIndex getMaximumIndex() const
    { return myStrings.getItemIdUpperBound(); }

    /// Return the capacity of the string container
    exint capacity() const
    { return myStrings.getItemIdUpperBound()+1; }

#endif

    virtual bool	loadStringsH9(UT_IStream &is, UT_IntArray &handlemap);
    virtual void	loadStringsFinishH9(const UT_IntArray &handlemap);

    virtual bool	saveStringsH9(std::ostream &os, int binary,
					UT_IntArray &handlemap) const;

#if GA_NEW_STRING_DATA_STRUCTURE
    /// Grow or shrink the tuple size
    bool setTupleSize(int tuplesize);

    /// This method will "compact" the attribute container, possibly changing
    /// all the handles in the attribute data.
    virtual void compactStorage();

    /// Get a measure of the vacancy entropy of the storage container.  This
    fpreal getStorageOccupancy()
    { return myStrings.getOccupancy(); }

    /// Adding strings is thread-safe, so we're only subject to GA_PageArray
    /// limitations.
    virtual WriteConcurrence getSupportedWriteConcurrence() const
    { return WRITE_CONCURRENCE_PAGE; }
#endif

#if GA_NEW_STRING_DATA_STRUCTURE
    virtual const GA_AIFMerge	*getAIFMerge() const	{ return myAIFMerge; }
    virtual const GA_AIFCompare	*getAIFCompare() const	{ return myAIFCompare;}
    virtual const GA_AIFCopyData *getAIFCopyData() const{ return myAIFCopyData;}
    virtual const GA_AIFInterp	*getAIFInterp() const	{ return myAIFInterp; }
#else
    // AIFCopyData is implemented in GA_ATIBlob.
    // AIFInterp is implemented in GA_ATIBlob.
#endif
    virtual const GA_AIFSharedStringTuple *getAIFSharedStringTuple() const
						{ return myAIFStringTuple; }
    virtual const GA_AIFStringTuple	*getAIFStringTuple() const
						{ return myAIFStringTuple; }

    /// @section JSON-GA_ATIString JSON Schema: GA_ATIString
    /// @code
    /// {
    ///     "name"          : "GA_ATIString",
    ///     "description"   : "An tuple array of indexed strings",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "size": {
    ///             "type"          : "integer",
    ///             "minimum"       : 1,
    ///             "description"   : "Tuple size",
    ///             "default"       : 1,
    ///             "optional"      : true,
    ///         },
    ///         "storage": {
    ///             "type"        : "string",
    ///             "description" : "Tuple storage",
    ///             "enum"        : [ "int8", "int16", "int32", "int64" ],
    ///         },
    ///         "strings": {
    ///             "type"  : { "$ref":"GA_BlobData", },
    ///             "description" : "Array of strings used in the attribute",
    ///         },
    ///         "indices": {
    ///             "type"  : {"$ref":"GA_DataArrayTuple"},
    ///             "description" : "Int index for each element of the array",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    virtual const GA_AIFJSON	*getAIFJSON() const	{ return myAIFJSON; }

    /// Debug code
    virtual bool	 debugValidateArrayCapacity(GA_Offset sz) const;

    /// @{
    /// Get a string
#if GA_NEW_STRING_DATA_STRUCTURE
    const UT_StringHolder &getString(GA_Offset offset, int component = 0) const
    {
        using namespace UT::Literal;

        const GA_StringIndexType handle = getStringIndex(offset, component);
        if (handle < 0)
            return UT_StringHolder::theEmptyString;
        const UT_StringHolder *ps = myStrings.get(handle);
        UT_ASSERT_P(ps);
        return *ps;
    }
    SYS_FORCE_INLINE
    GA_StringIndexType getStringIndex(GA_Offset offset, int component = 0) const
    {
        return GA_StringIndexType(myHandles.get(offset, component));
    }
#else
    const char		*getString(GA_Offset offset, int component=0) const;
    GA_StringIndexType	 getStringIndex(GA_Offset offset, int component=0) const;
#endif
    /// @}
    /// @{
    /// Set a string
#if GA_NEW_STRING_DATA_STRUCTURE
    void setString(GA_Offset offset, const UT_StringHolder &str, int component=0);
    void setStringIndex(GA_Offset offset, GA_StringIndexType i, int component=0);
    void setStringIndex(const GA_Range &dst_range, GA_StringIndexType i, int component=0);
#else
    void setString(GA_Offset offset, const char *str, int component=0);
    void setStringIndex(GA_Offset offset, GA_StringIndexType i, int component=0)
    { tupleSet(offset, i, component); }
#endif
    /// @}

    const char		*lookupString(GA_StringIndexType handle) const;
    const char		*lookupOrderedString(exint handle) const;
#if GA_NEW_STRING_DATA_STRUCTURE
    GA_StringIndexType lookupHandle(const UT_StringHolder &s) const;
#else
    GA_StringIndexType	 lookupHandle(const char *s) const;
#endif
    GA_StringIndexType validateHandle(GA_StringIndexType h) const;
#if GA_NEW_STRING_DATA_STRUCTURE
    bool replaceString(GA_StringIndexType handle,
        const UT_StringHolder &new_string);
#else
    bool		 replaceString(GA_StringIndexType handle,
				    const char *string);
#endif

#if GA_NEW_STRING_DATA_STRUCTURE
    bool jsonSave(UT_JSONWriter &w, const GA_SaveMap &s,
            const char *token) const;
    bool jsonLoad(UT_JSONParser &p,
            const GA_LoadMap &load, const char *token);
    bool jsonLoadStrings(UT_JSONParser &p,
        const GA_LoadMap &lmap,
        UT_StringArray &strings);

    /// Convenience function to extract all the strings (and their handles)
    /// The string handles are guaranteed to be in ascending order, but
    /// may or may not be contiguous.
    /// @{
    exint extractStrings(UT_StringArray &strings,
        UT_IntArray &handles) const;
    exint extractStrings(UT_StringArray &strings,
        UT_IntArray &handles, exint maxstrings) const;
    /// @}

    virtual bool setArraySize(GA_Offset new_size);
    virtual void tryCompressAllPages(
        GA_Offset start_offset = GA_Offset(0),
        GA_Offset end_offset = GA_INVALID_OFFSET);
    virtual void hardenAllPages(
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
        const GA_ATIString *thatn = UTverify_cast<const GA_ATIString *>(that);
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
        if (!GA_ATIString::isType(&src))
            return false;
        copyData(desti, UTverify_cast<const GA_ATIString *>(&src), srci);
        return true;
    }
    bool copy(GA_Offset desti, const GA_ATIString &src, GA_Offset srci)
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
        if (!GA_ATIString::isType(&src))
            return false;
        return copyData(destrange, UTverify_cast<const GA_ATIString*>(&src), srcrange);
    }
    bool copy(const GA_Range &destrange, const GA_ATIString &src, const GA_Range &srcrange)
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
        if (!GA_ATIString::isType(&src))
            return false;
        fillData(destrange, UTverify_cast<const GA_ATIString *>(&src), srci);
        return true;
    }
    bool fill(const GA_Range &destrange, const GA_ATIString &src, GA_Offset srci)
    {
        fillData(destrange, &src, srci);
        return true;
    }
    /// @}

    /// @{ GA_AIFMerge
    /// Base class implementation of GA_AIFMerge::destroyDestination()
    void mergeDestroyDestination(const GA_MergeMap &map,
        GA_Attribute *dattrib) const;
    /// Base class implementation of GA_AIFMerge::addDestination()
    GA_Attribute *mergeAddDestination(const GA_MergeMap &map,
        GA_Attribute *dattrib) const;
    /// Base class implementation of GA_AIFMerge::growArray()
    void mergeGrowArray(const GA_MergeMap &map, const GA_ATIString &s);

    /// Base class implementation of GA_AIFMerge::copyArray()
    bool mergeAppendData(const GA_MergeMap &map,
        const GA_Attribute *sattrib);
    /// @}

    /// @{
    /// Interface used by GA_AIFCompare.
    bool isEqual(GA_Offset offset,
        const GA_ATIString &b, GA_Offset b_offset) const;
    bool isEqual(const GA_Range & range,
        const GA_ATIString &b, const GA_Range &b_range) const;
    /// @}

    /// Validates the internal structure for debugging purposes.
    bool validate() const;
#endif

    typedef GA_PageArray<int32> HandleArrayType;

protected:
#if GA_NEW_STRING_DATA_STRUCTURE
    /// String attributes need each element to properly destruct for accurate
    /// reference counting.
    virtual bool needDestruction() const;

    /// Callback method to destruct an offset.
    virtual void destructElement(GA_Offset offset);
#endif

    // Create a new ATIBlob
    virtual GA_Attribute *doClone(const GA_IndexMap &index_map,
				  const UT_StringHolder &name) const;

#if GA_NEW_STRING_DATA_STRUCTURE
    /// Array of handles.  The default value is -1.
    HandleArrayType myHandles;

    /// String references.
    UT_IndexedHashSet<UT_StringHolder> myStrings;
#endif

private:
#if GA_NEW_STRING_DATA_STRUCTURE
    /// @{
    /// Methods to implement copying of data for AIFCopyData
    bool copyData(GA_Offset di,
        const GA_ATIString *s,
        GA_Offset si);
    bool copyData(const GA_Range &di,
        const GA_ATIString *s,
        const GA_Range &si);
    bool fillData(const GA_Range &di,
        const GA_ATIString *s,
        GA_Offset si);
    /// @}
#endif

    /// @{ GA_AIFStringTuple
    /// @warning If you call addStringReference(), at some point, you must call
    /// delHandleReference() on the string you added.
#if GA_NEW_STRING_DATA_STRUCTURE
    GA_StringIndexType addStringReference(const UT_StringHolder &str);
#else
    GA_StringIndexType addStringReference(const char *str);
#endif
    void	delHandleReference(GA_StringIndexType handle);
    bool	tupleGet(GA_Offset di, GA_StringIndexType &v, int vi) const;
    bool	tupleSet(GA_Offset di, GA_StringIndexType v, int vi);
    /// @}

#if GA_NEW_STRING_DATA_STRUCTURE
    static GA_AIFMerge      *myAIFMerge;
    static GA_AIFCopyData   *myAIFCopyData;
    static GA_AIFCompare    *myAIFCompare;
    static GA_AIFInterp     *myAIFInterp;
#endif
    static GA_AIFSharedStringTuple  *myAIFStringTuple;
    static GA_AIFJSON               *myAIFJSON;

    static const GA_AttributeType *theAttributeType;

    /// @cond INTERNAL_DOX
    friend class ga_StringCopyData;
    friend class ga_StringMerge;
    friend class ga_StringCompare;
    friend class ga_StringTuple;
    friend class ga_StringJSON;
    friend class GA_ATIStringDelayedWriter;
    /// @endcond
};

//
// Designed to delay all the reference count adjustments
// until this object is flushed.  Also maintains a local
// table for looking up the string indices with, thus avoiding
// concurrency issues.
//
class GA_API GA_ATIStringDelayedWriter
{
public:
    GA_ATIStringDelayedWriter();
    GA_ATIStringDelayedWriter(GA_ATIString *attrib);
    ~GA_ATIStringDelayedWriter();

    void		bind(GA_ATIString *attrib);

    // The holder version should be preferred as it can store the hash
    // and avoid hardening to the internal map.
    void		setString(GA_Offset offset, const char *str, int tuple=0);
    void		setString(GA_Offset offset, const UT_StringHolder &str, int tuple=0);
    void		setString(const GA_Range &range, const char *str, int tuple=0);
    void		setString(const GA_Range &range, const UT_StringHolder &str, int tuple=0);
    // Directly set the index.
    void		setStringIndex(GA_Offset offset, GA_StringIndexType idx, int tuple=0);
    void		setStringIndex(const GA_Range &range, GA_StringIndexType idx, int tuple=0);

    void		flush();

    SYS_FORCE_INLINE bool hasChanges() const
    {
        return myHasChanges;
    }

private:
    GA_ATIString *myAttribute;
    volatile bool myHasChanges;

    UT::ArrayMap<GA_StringIndexType, exint> myRefCountChanges;
    UT_ArrayStringMap<GA_StringIndexType>   myStringToIndex;
};

#if !GA_NEW_STRING_DATA_STRUCTURE
// NOTE: It may seem strange to define GA_ATIBlob::isType() and cast()
//       in GA_ATIString.h, but GA_ATIBlob::isType() and cast() need
//       GA_ATIString::isType(), and GA_ATIString inherits from GA_ATIBlob,
//       so these are defined here, and GA_ATIString.h is included
//       in GA_ATIBlob.h, below the class.
bool
GA_ATIBlob::isType(const GA_Attribute *attrib)
{
    return attrib && (&attrib->getType() == theAttributeType || GA_ATIString::isType(attrib));
}
GA_ATIBlob *
GA_ATIBlob::cast(GA_Attribute *attrib)
{
    if (attrib && (&attrib->getType() == theAttributeType || GA_ATIString::isType(attrib)))
        return static_cast<GA_ATIBlob *>(attrib);
    return nullptr;
}
const GA_ATIBlob *
GA_ATIBlob::cast(const GA_Attribute *attrib)
{
    if (attrib && (&attrib->getType() == theAttributeType || GA_ATIString::isType(attrib)))
        return static_cast<const GA_ATIBlob *>(attrib);
    return nullptr;
}
#endif

#endif
