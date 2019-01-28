/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATITopology.h ( GA Library, C++)
 *
 * COMMENTS:	Topology ATI (Attribute Type Implementation)
 *
 * 		Topology attributes are used to store information about how
 *		elements are connected together.
 */

#pragma once

#ifndef __GA_ATITopology__
#define __GA_ATITopology__

#define USE_PAGEARRAY_TOPOLOGY 1
#if USE_PAGEARRAY_TOPOLOGY
#include "GA_PageArray.h"
#else
#include "GA_DataArray.h"
#include "GA_DataArrayPageTableImpl.h"
#endif

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AttributeType.h"
#include "GA_IndexMap.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Types.h>


class GA_AIFCopyData;
class GA_AIFMerge;
class GA_Defragment;
class GA_LoadMap;
class GA_MergeMap;
class GA_SaveMap;
class UT_JSONParser;
class UT_JSONWriter;

class GA_API GA_ATITopology : public GA_Attribute
{
public:
    static void registerType();
    static GA_ATITopology *create(
            const GA_IndexMap &index_map,
            GA_AttributeScope scope,
            const UT_StringHolder &name,
            GA_AttributeOwner linkowner);
    SYS_FORCE_INLINE
    static const GA_AttributeType &getType() { return *theAttributeType; }
    SYS_FORCE_INLINE
    static const UT_StringHolder &getTypeName()
    { return theAttributeType->getTypeName(); }

    SYS_FORCE_INLINE
    static bool isType(const GA_Attribute *attrib)
    {
        return attrib && &attrib->getType() == theAttributeType;
    }
    SYS_FORCE_INLINE
    static GA_ATITopology *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATITopology *>(attrib);
        return NULL;
    }
    SYS_FORCE_INLINE
    static const GA_ATITopology *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATITopology *>(attrib);
        return NULL;
    }

    GA_ATITopology(
        const GA_AttributeType &type,
        const GA_IndexMap &index_map,
        GA_AttributeScope scope,
        const UT_StringHolder &name,
        GA_AttributeOwner linkowner);
    virtual ~GA_ATITopology();

    /// Report memory used
    int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// We do not need to implement the reconstructElement() method as the
    /// GA_Topology owner explicitly initializes each value as the element
    /// is added.
    /// TODO: Ideally we'd do so in reconstructElement() to avoid useless
    ///       work when elements are first constructed.

    /// Data is paged, so concurrent writes to separate pages supported.
    virtual WriteConcurrence	 getSupportedWriteConcurrence() const
					{ return WRITE_CONCURRENCE_PAGE; }

    /// Merge interface
    virtual const GA_AIFMerge	*getAIFMerge() const	{ return theAIFMerge; }

    bool	mergeAppendData(const GA_MergeMap &map,
			const GA_ATITopology &sattrib);

    virtual const GA_AIFCopyData *getAIFCopyData() const { return theAIFCopyData; }

    /// @{
    /// Vertex references are managed via multiple coupled attributes, so
    /// AIF_Merge cannot resolve non-trivial vertex merges.  Instead, the
    /// AIF will set this flag and let GA_Topology::mergeRebuild() handle
    /// updating the vertex reference attributes.
    bool	getUnresolvedMergeFlag() const { return myUnresolvedMerge; }
    void	setUnresolvedMergeFlag(bool f) { myUnresolvedMerge = f; }
    /// @}

    SYS_FORCE_INLINE
    GA_Offset	getLink(GA_Offset ai) const
    {
        UT_ASSERT_P(getIndexMap().isOffsetInRange(ai));

#if USE_PAGEARRAY_TOPOLOGY
        return GA_Offset(myData.getGuaranteedInt(ai));
#else
	if (myDataPageTableI16)
	    return GA_Offset(myDataPageTableI16->get(ai));
        if (myDataPageTableI32)
	    return GA_Offset(myDataPageTableI32->get(ai));
	return GA_Offset(myDataPageTableI64->get(ai));
#endif
    }

    SYS_FORCE_INLINE
    void	setLink(GA_Offset ai, GA_Offset v)
    {
	UT_ASSERT_P(getIndexMap().isOffsetInRange(ai));

#if USE_PAGEARRAY_TOPOLOGY
	UT_ASSERT_P(ai < myData.size());
        myData.set(ai, int64(v));
        UT_ASSERT_P(myData.getGuaranteedInt(ai) == int64(v));
#else
	UT_ASSERT_P(ai < myData->getArraySize());
	if (myDataPageTableI16)
	{
	    myDataPageTableI16->set(ai, v);
	}
	else if (myDataPageTableI32)
	{
	    myDataPageTableI32->set(ai, v);
	}
	else
	{
	    myDataPageTableI64->set(ai, v);
	}
#endif
    }

    SYS_FORCE_INLINE
    bool	isPageConstant(GA_PageNum pageno) const
    {
	return myData.isPageConstant(pageno);
    }

    /// Sets all links for offsets >= start and < end to value.
    void setLinksConstant(GA_Offset start, GA_Offset end, GA_Offset value);

    /// Set all values to the default
    void	clear();

    GA_Storage	getStorage() const
    {
#if USE_PAGEARRAY_TOPOLOGY
        return myData.getStorage();
#else
        return myData->getStorage();
#endif
    }
    bool	setStorage(GA_Storage storage);

    /// Returns true iff that is an attribute whose content can be copied
    /// from this without any type conversions.  This is important to
    /// avoid reallocation of an attribute if its storage type,
    /// including tuple size, matches the source attribute exactly.
    virtual bool matchesStorage(const GA_Attribute *that) const
    {
        if (!GA_Attribute::matchesStorage(that))
            return false;
        const GA_ATITopology *thatn = UTverify_cast<const GA_ATITopology *>(that);
        if (getStorage() != thatn->getStorage())
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

    /// Query the owner type of the link stored in this attribute.  For
    /// example, the vertex PointRef attribute would have:
    ///         getOwner() == GA_ATTRIB_VERTEX
    ///         getLinkOwner() == GA_ATTRIB_POINT
    GA_AttributeOwner getLinkOwner() const { return myLinkOwner; }

    /// Save to a JSON stream.
    /// @section JSON-GA_ATITopology JSON Schema: GA_ATITopology
    /// @code
    /// {
    ///     "name"          : "GA_ATITopology",
    ///     "description"   : "An tuple array of indexed strings",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "data": {
    ///             "type"  : "indices",
    ///             "items" : "integer",
    ///             "description" : "Integer reference indices.",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    /// @note There is no GA_AIFJSON interface for topology.
    bool	jsonSave(UT_JSONWriter &w, const GA_SaveMap &map) const;

    /// Load from a JSON stream.
    /// @note There is no GA_AIFJSON interface for topology.
    bool	jsonLoad(UT_JSONParser &p, const GA_LoadMap &map);

#if USE_PAGEARRAY_TOPOLOGY
    typedef GA_PageArray<void, 1, true, false> DataType;

    /// @{
    /// Provide access to data
    const DataType	&getData() const	{ return myData; }
    DataType		&getData()		{ return myData; }
    /// @}
#else
    /// @{
    /// Provide access to data
    const GA_DataArray	&getData() const	{ return *myData; }
    GA_DataArray	&getData()		{ return *myData; }
    /// @}
#endif

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

private:
    // Defragmentation functions should only be called internally or from GA_Topology.
    friend class GA_Topology;

    /// @{
    /// Interface for defragmentation
    /// This implementation does nothing, because defragmentation
    /// of topology attributes must be handled separately.
    virtual void defragment(const GA_Defragment &defrag);
    /// @}

    /// Swap the elements in the array
    void swapElements(GA_Offset a, GA_Offset b, GA_Size n);
    /// Move elements in the array from a to b
    void moveElements(GA_Offset a, GA_Offset b, GA_Size n);

    /// Change the values in the array
    /// This bumps the attribute's data ID iff myData != NULL
    /// and getLinkOwner() != getOwner().
    void swapLinks(const GA_Defragment &defrag);

    /// Set the link data type
    void setLinkOwner(GA_AttributeOwner t) { myLinkOwner = t; }

#if USE_PAGEARRAY_TOPOLOGY
    DataType myData;
#else
    /// Rebuild our direct page pointers.
    /// Must be invoked if the page type changes.
    void updatePagePointers();

    GA_DataArray	*myData;
#endif
    GA_AttributeOwner	 myLinkOwner;
    bool		 myUnresolvedMerge;

#if !USE_PAGEARRAY_TOPOLOGY
    GA_Private::ga_DataArrayPageTableImplI16
				*myDataPageTableI16;
    GA_Private::ga_DataArrayPageTableImplI32
				*myDataPageTableI32;
    GA_Private::ga_DataArrayPageTableImplI64
				*myDataPageTableI64;
#endif

    static const GA_AIFMerge *theAIFMerge;
    static const GA_AIFCopyData *theAIFCopyData;
    static const GA_AttributeType *theAttributeType;

    friend class ga_TopologyCopy;
};

#endif
