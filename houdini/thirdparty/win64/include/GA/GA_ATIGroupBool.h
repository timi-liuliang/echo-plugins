/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATIGroupBool.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_ATIGroupBool__
#define __GA_ATIGroupBool__

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AttributeType.h"           // for GA_AttributeType
#include "GA_DataBitArray.h"
#include "GA_ElementGroupOrder.h"
#include "GA_Group.h"
#include "GA_IndexMap.h"
#include "GA_Iterator.h"
#include "GA_Range.h"                   // for GA_Range
#include "GA_Types.h"                   // for GA_Offset, GA_Size, etc

#include <UT/UT_Assert.h>
#include <UT/UT_Lock.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>              // for int64

#include <iosfwd>                       // for ostream


class GA_AIFCopyData;
class GA_AIFInterp;
class GA_AIFMerge;
class GA_AIFTuple;
class GA_Defragment;
class GA_Detail;
class GA_LoadMap;
class GA_MergeMap;
class GA_SaveMap;

class UT_BitArray;
class UT_IStream;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
class UT_WorkBuffer;


class GA_API GA_ElementGroup : public GA_Attribute , public GA_Group
{
    // typedef for use inside this class
    typedef GA_ElementGroup GA_ATIGroupBool;
public:
    /// GA_Group has some of the same function names as GA_Attribute,
    /// so we need to pick one.
    using GA_Attribute::getName;
    using GA_Attribute::isDetached;

    virtual const GA_Detail &getDetail() const final
    {
        return GA_Attribute::getDetail();
    }
    GA_Detail &getDetail()
    {
        return GA_Attribute::getDetail();
    }

    static void registerType();// Register group attribute type

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
    static GA_ATIGroupBool *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATIGroupBool *>(attrib);
        return nullptr;
    }
    SYS_FORCE_INLINE
    static const GA_ATIGroupBool *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATIGroupBool *>(attrib);
        return nullptr;
    }
    SYS_FORCE_INLINE
    static bool isType(const GA_Group *group)
    {
        if (!group)
            return false;
        GA_GroupMaskType type = group->classMaskType();
        return (type & (GA_GMASK_POINT | GA_GMASK_VERTEX | GA_GMASK_PRIMITIVE)) != 0;
    }
    SYS_FORCE_INLINE
    static const GA_ElementGroup *cast(const GA_Group *group)
    {
        if (!isType(group))
            return nullptr;
        return UTverify_cast<const GA_ElementGroup *>(group);
    }
    SYS_FORCE_INLINE
    static GA_ElementGroup *cast(GA_Group *group)
    {
        if (!isType(group))
            return nullptr;
        return UTverify_cast<GA_ElementGroup *>(group);
    }

protected:
    GA_ElementGroup(GA_Detail &detail, GA_AttributeOwner owner,
                    const UT_StringHolder &name, bool internal, bool detached);
public:
    GA_ElementGroup(const GA_Detail &detail, GA_AttributeOwner owner);
    virtual ~GA_ElementGroup();

    virtual int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// @{
    /// Interface for defragmentation
    virtual void	defragment(const GA_Defragment &defrag);
    /// @}

    /// Returns \c true if the group is currently ordered.
    virtual bool isOrdered() const final
    {
        return myOrder != nullptr;
    }

    /// When \c ordered is set to \c true, convert this group to an ordered
    /// group, otherwise convert to an unordered group. If the group contains 
    /// mixed entries, a conversion to unordered will fail, and the function
    /// return \c false. Conversion to ordered group will always succeed.
    bool setOrdered(bool ordered)
    {
        if (ordered)
        {
            makeOrdered();
            return true;
        }
        if (!getOrdered())
            return true;
        if (getOrdered()->mixedEntries())
            return false;

        clearOrdered();
        return true;
    }

    void makeOrdered();

    void makeUnordered()
    {
        if (getOrdered() && !getOrdered()->mixedEntries())
            clearOrdered();
    }

    /// Clear all order information, including any mixed entries.
    void clearOrdered();

    /// Copy membership information from the source group.  This will fail if
    /// the groups have different index maps.  This will also copy over the
    /// ordered information if \c copy_ordering is set to \c true.
    bool		copyMembership(const GA_ATIGroupBool &src,
        		               bool copy_ordering = true);

    /// Return GA offset of the Nth element of the group.
    /// NOTE: This may be quite expensive to compute.
    /// Returns GA_INVALID_OFFSET if there aren't enough elements in the group.
    GA_Offset		findOffsetAtGroupIndex(GA_Size i) const;

    /// NOTE: When using setElement(GA_Offset,bool) in parallel,
    ///       you *must* call invalidateGroupEntries() afterward, else
    ///       getGroupEntries() may return an incorrect value.
    /// NOTE: This cannot be used in parallel on an ordered group.
    SYS_FORCE_INLINE
    void		setElement(GA_Offset ai, bool v)
			{
			    if (v != contains(ai))
			    {
				if (v)
				    setMembership<true>(ai);
				else
				    setMembership<false>(ai);
			    }
			}
    /// NOTE: This cannot be used in parallel on an ordered group.
    SYS_FORCE_INLINE
    void		setElement(const GA_Range &it, bool v)
			{
			    if (v)
				setMembership<true>(it);
			    else
				setMembership<false>(it);
			}

    /// NOTE: When using toggleElement(GA_Offset) in parallel,
    ///       you *must* call invalidateGroupEntries() afterward, else
    ///       getGroupEntries() may return an incorrect value.
    /// NOTE: This cannot be used in parallel on an ordered group.
    void		toggleElement(GA_Offset ai);
    /// NOTE: This cannot be used in parallel on an ordered group.
    void		toggleElement(const GA_Range &it);

    /// These methods only work on unordered groups.
    /// @{
    void orEqual(const GA_ATIGroupBool *src);
    void andEqual(const GA_ATIGroupBool *src);
    void xorEqual(const GA_ATIGroupBool *src);
    void subEqual(const GA_ATIGroupBool *src);
    void toggleAll(GA_Size numelements);
    /// @}

    /// Add all elements corresponding with src to this group.
    /// src can be a point, vertex, primitive, or edge group.
    /// Breakpoint groups won't crash, but may add more
    /// elements than required.
    virtual bool combine(const GA_Group *src) final;

    /// operator|= supports point, vertex, and primitive groups,
    /// regardless of this group type.
    GA_ElementGroup &operator|=(const GA_ElementGroup &g);

    /// These operators only support g having the
    /// same owner as this.
    /// @{
    GA_ElementGroup &operator&=(const GA_ElementGroup &g);
    GA_ElementGroup &operator-=(const GA_ElementGroup &g);
    GA_ElementGroup &operator^=(const GA_ElementGroup &g);
    GA_ElementGroup &operator =(const GA_ElementGroup &g)
    {
        copyMembership(g, /* copy_ordering */ true);
        return *this;
    }
    /// @}

    SYS_FORCE_INLINE
    bool contains(GA_Offset offset) const
    {
        return myData.get(offset);
    }
    bool containsIndex(GA_Index ai) const
    {
        GA_Offset ao = getIndexMap().offsetFromIndex(ai);
        return contains(ao);
    }

    SYS_FORCE_INLINE
    bool containsOffset(GA_Offset offset) const
    {
        return contains(offset);
    }
    SYS_FORCE_INLINE
    bool containsAny(const GA_Range &range) const
    {
        return myData.isAnySet(range);
    }
    SYS_FORCE_INLINE
    bool isGroupEmpty() const
    {
        return (getGroupEntries() == 0);
    }
    /// Query whether the group is empty of primary elements.
    SYS_FORCE_INLINE
    bool isEmpty() const
    {
        return (getGroupEntries() == 0);
    }
    /// Query whether the group is empty of elements, primary or mixed.
    bool isEmptyMix() const
    {
        return (entriesMix() == 0);
    }
    /// Will return the number of primary elements.
    virtual GA_Size entries() const final
    { return getGroupEntries(); }
    /// Will return the total number of elements, primary and mixed.
    virtual GA_Size entriesMix() const final
    {
        if (getOrdered())
            return getOrdered()->entries();
        else
            return getGroupEntries();
    }
    GA_Size getGroupEntries() const
    {
        if (myGroupEntries >= 0)
            return myGroupEntries;

        return computeGroupEntries();
    }
    GA_Size computeGroupEntries() const;

    /// Query if the group contains any mixed elements.
    virtual bool isMixed() const final
    {
        return myOrder && (myOrder->mixedEntries() > 0);
    }

    SYS_FORCE_INLINE
    void setOffset(GA_Offset ai, bool v)
    {
        setElement(ai, v);
    }

    /// @{
    /// Access elements of the group by ordered index rather than offset.
    /// Internally, this method just find the data offset and calls appropriate
    /// method.
    SYS_FORCE_INLINE
    void addIndex(GA_Index ai)
    {
        GA_Offset ao = getIndexMap().offsetFromIndex(ai);
        addOffset(ao);
    }
    SYS_FORCE_INLINE
    void toggleIndex(GA_Index ai)
    {
        GA_Offset ao = getIndexMap().offsetFromIndex(ai);
        toggleOffset(ao);
    }
    SYS_FORCE_INLINE
    void removeIndex(GA_Index ai)
    {
        GA_Offset ao = getIndexMap().offsetFromIndex(ai);
        removeOffset(ao);
    }
    /// @}

    /// @{
    /// Access elements by their offset in the index map
    /// NOTE: When using addOffset, toggleOffset, removeOffset in parallel,
    ///       you *must* call invalidateGroupEntries() afterward, else
    ///       entries() may return an incorrect value.
    /// NOTE: These cannot be used in parallel on an ordered group.
    SYS_FORCE_INLINE
    void addOffset(GA_Offset ai)
    {
        if (GAisValid(ai))
            setElement(ai, true);
    }
    SYS_FORCE_INLINE
    void toggleOffset(GA_Offset ai)
    {
        if (GAisValid(ai))
            toggleElement(ai);
    }
    SYS_FORCE_INLINE
    void removeOffset(GA_Offset ai)
    {
        if (GAisValid(ai))
            setElement(ai, false);
    }
    /// @}

    /// @{
    /// Access elements by a GA_Range
    /// These have the advantage of being able to operate a page at a time
    /// when applicable.
    /// NOTE: These cannot be used in parallel on an ordered group.
    SYS_FORCE_INLINE
    void addRange(const GA_Range &it)
    {
        setMembership<true>(it);
    }
    SYS_FORCE_INLINE
    void toggleRange(const GA_Range &it)
    {
        toggleElement(it);
    }
    SYS_FORCE_INLINE
    void removeRange(const GA_Range &it)
    {
        setMembership<false>(it);
    }
    /// @}


    /// Query the length of repeated bit values starting from 'start' until
    /// 'end' or a change in bit values.
    void		getConstantSpan(GA_Offset start, GA_Offset end,
				GA_Size &size, bool &value) const
			{
			    return myData.getConstantSpan(start, end,
						size, value);
			}

    void		clear();
    void		makeConstant(bool value);
    void		invalidateGroupEntries()
			    { myGroupEntries = -1; }

    virtual bool	needDestruction() const;
    virtual void	destructElement(GA_Offset offset);
    virtual void	reconstructElementBlock(GA_Offset offset, GA_Offset nelements);

    /// Add all elements to the group
    virtual void	addAll() final
    {
        makeConstant(true);
    }
    /// Add all elements to the group, only writing to real elements' offsets.
    void setEntries()
    {
        setElement(GA_Range(getIndexMap()), true);
    }
    /// Toggle membership of all elements
    void toggleEntries()
    {
        // We are no longer ordered.
        makeUnordered();
        toggleAll(getIndexMap().offsetSize());
        invalidateGroupEntries();
    }

    /// Data is paged, so concurrent writes to separate pages supported,
    /// unless the group is ordered.  Rather, that would be the case if
    /// the updates to myGroupEntries were thread-safe.
    virtual WriteConcurrence	 getSupportedWriteConcurrence() const
					{ return WRITE_CONCURRENCE_NONE; }

    virtual const GA_AIFCopyData *getAIFCopyData() const {return myAIFCopyData;}
    virtual const GA_AIFTuple	*getAIFTuple() const	{ return myAIFTuple; }
    virtual const GA_AIFInterp	*getAIFInterp() const	{ return myAIFInterp; }
    virtual const GA_AIFMerge	*getAIFMerge() const	{ return myAIFMerge; }

    // TODO: Need to override arithmetic operations too! (for ordered groups)
    GA_ElementGroupOrder        *getOrdered()           { return myOrder; }
    const GA_ElementGroupOrder  *getOrdered() const     { return myOrder; }
    GA_ElementGroup             *getGroup()             { return this; }
    const GA_ElementGroup       *getGroup() const       { return this; }
    GA_ElementGroup             *getAttribute()         { return this; }
    const GA_ElementGroup       *getAttribute() const   { return this; }

    bool loadGroupH9(UT_IStream &is);
    bool loadGroupByIndexOrderH9(const UT_BitArray &array);
    bool saveGroupH9(std::ostream &os, bool binary) const;

    /// @private Method called in merging
    void		 mergeGrowArray(const GA_MergeMap &map,
					const GA_ATIGroupBool &sattrib);

    // Debug code
    // Note:  Groups don't have to have the same capacity?
    virtual bool	 debugValidateArrayCapacity(GA_Offset sz) const;

    /// Save data to a JSON file.  We do not provide an GA_AIFJSON interface
    /// since the data is saved by the group (not the attribute).  This method
    /// is called directly by GA_ElementGroup instead.
    /// @section JSON-GA_ATIGroupBool JSON Schema: GA_ATIGroupBool
    /// @code
    /// {
    ///     "name"          : "GA_ATIGroupBool",
    ///     "description"   :
    ///            "Selection information for a GA_ElementGroup.  There should
    ///             only be one of 'ordered' or 'unordered' depending on
    ///             whether the group is ordered or unordered.",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "defaults": {
    ///             "type"  : { "$ref" : "GA_Defaults" }
    ///             "description" : "Default values",
    ///         },
    ///         "ordered": {
    ///             "type"        : {"$ref":"GA_ElementGroupOrder"},
    ///             "optional"    : true,
    ///             "description" : "Ordered group selection.",
    ///         },
    ///         "unordered": {
    ///             "type"        : {"$ref":"GA_DataBitArray"},
    ///             "optional"    : true,
    ///             "description" : "Unordered selection of group selection.",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool		 jsonSave(UT_JSONWriter &w, const GA_SaveMap&map) const;

    /// Load from a JSON stream
    bool		 jsonLoad(UT_JSONParser &p, const GA_LoadMap &map);

    /// Save data to a JSON stream.
    /// @section JSON-GA_ElementGroup JSON Schema: GA_ElementGroup
    /// Private data for an element group
    /// @code
    /// {
    ///     "name"          : "GA_ElementGroup-Data",
    ///     "description"   : "Data for an element group.",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "selection": {
    ///             "type"   : {"$ref" : "GA_ATIGroupBool" },
    ///             "description" : "Group memebership data",
    ///             "optional" : true
    ///         }
    ///     },
    /// }
    /// @endcode
    ///
    /// @see @ref JSON_FileFormat, GA_GroupTable
    virtual bool	jsonSaveData(UT_JSONWriter &w,
				    const GA_SaveMap &map) const;

    /// Load from a JSON stream
    virtual bool	jsonLoadData(UT_JSONParser &p,
				    const GA_LoadMap &map);

    /// Compute detailed information
    virtual bool	 stat(UT_WorkBuffer &info, uint level) const;

    /// Grow or shrink the array size
    virtual bool	 setArraySize(GA_Offset new_size);

    /// Get the array size of the attribute.  This is primarily here
    /// for checking detached groups, when the detail may have
    /// more elements added since the group was created.
    GA_Offset            getArraySize() const
    { return myData.getArraySize(); }

    /// Returns the writable raw bit array.
    /// WARNING: Only use this if this is an unordered group,
    ///          and invalidateGroupEntries() has been called,
    ///          so that writing doesn't cause problems.
    SYS_FORCE_INLINE
    GA_DataBitArray &getData()
    {
        UT_ASSERT_P(!isOrdered() && myGroupEntries < 0);
        return myData;
    }

    /// Returns the read-only raw bit array.
    SYS_FORCE_INLINE
    const GA_DataBitArray &getData() const
    {
        return myData;
    }

    /// Returns true iff the specified page is constant-compressed,
    /// where a single value is stored to represent GA_PAGE_SIZE
    /// values in the page, (or possibly fewer if only one page).
    SYS_FORCE_INLINE bool isPageConstant(GA_PageNum pagenum) const
    {
        return myData.isPageConstant(pagenum);
    }
    /// If pagenum is a constant page, this returns the constant value
    /// of the page.  Otherwise, this function shouldn't be called.
    SYS_FORCE_INLINE bool getPageValue(GA_PageNum pagenum) const
    {
        return myData.getPageCVal(pagenum);
    }
    /// Sets all elements of the specified page to the given value.
    SYS_FORCE_INLINE void setPageConstant(GA_PageNum pagenum, const bool value)
    {
        myData.makePageConstant(pagenum, value);
    }

    /// Try to compress data pages
    virtual void	tryCompressAllPages(
				GA_Offset start_offset = GA_Offset(0),
				GA_Offset end_offset = GA_INVALID_OFFSET);
    /// Harden data pages
    virtual void	hardenAllPages(
				GA_Offset start_offset = GA_Offset(0),
				GA_Offset end_offset = GA_INVALID_OFFSET);

    /// In the case that we're copying from an attribute whose storage
    /// type matches this exactly, this function copies the metadata
    /// not associated with the storage, e.g. myOptions,
    /// *excluding* the name and the data ID.
    virtual void copyNonStorageMetadata(const GA_Attribute *that) final;

    /// This replaces the entirety of this attribute's content and non-
    /// storage metadata (except the name) with that of the src attribute.
    /// This copies the src group order if present and destroys any if not.
    /// matchesStorage(src) should already return true.
    /// This is primarily for use by GA_AttributeSet::replace().
    /// NOTE: The internal content sizes may not match exactly if the
    ///       attribute type may overallocate, but the sizes should be such
    ///       that any real data will fit in the destination, so be careful
    ///       and deal with the myTailInitialize flag appropriately if
    ///       any extra elements aren't equal to the default.
    virtual void replace(const GA_Attribute &src) final;

    /// Copy attribute values for a single element.
    /// @{
    virtual bool copy(GA_Offset desti, GA_Offset srci) final
    {
        setElement(desti, contains(srci));
        return true;
    }
    virtual bool copy(GA_Offset desti, const GA_Attribute &src, GA_Offset srci) final
    {
        if (!GA_ATIGroupBool::isType(&src))
            return false;
        setElement(desti, UTverify_cast<const GA_ATIGroupBool *>(&src)->contains(srci));
        return true;
    }
    bool copy(GA_Offset desti, const GA_ATIGroupBool &src, GA_Offset srci)
    {
        setElement(desti, src.contains(srci));
        return true;
    }
    /// @}

    /// Copy attribute values for a range of elements.
    /// @{
    virtual bool copy(const GA_Range &destrange, const GA_Range &srcrange) final
    {
        return tupleSet(destrange, *this, srcrange);
    }
    virtual bool copy(const GA_Range &destrange, const GA_Attribute &src, const GA_Range &srcrange) final
    {
        if (!GA_ATIGroupBool::isType(&src))
            return false;
        return tupleSet(destrange, *UTverify_cast<const GA_ATIGroupBool*>(&src), srcrange);
    }
    bool copy(const GA_Range &destrange, const GA_ATIGroupBool &src, const GA_Range &srcrange)
    {
        return tupleSet(destrange, src, srcrange);
    }
    /// @}

    /// Assign all elements of a range from a single attribute value.
    /// @{
    virtual bool fill(const GA_Range &destrange, GA_Offset srci) final
    {
        setElement(destrange, contains(srci));
        return true;
    }
    virtual bool fill(const GA_Range &destrange, const GA_Attribute &src, GA_Offset srci) final
    {
        if (!GA_ATIGroupBool::isType(&src))
            return false;
        setElement(destrange, UTverify_cast<const GA_ATIGroupBool *>(&src)->contains(srci));
        return true;
    }
    bool fill(const GA_Range &destrange, const GA_ATIGroupBool &src, GA_Offset srci)
    {
        setElement(destrange, src.contains(srci));
        return true;
    }
    /// @}

private:

    template <bool ADD>
    void		setMembership(GA_Offset ai)
			{
                            UT_ASSERT_MSG_P(isDetached() || (GAisValid(ai) && ai < myData.getArraySize() && getIndexMap().isOffsetActive(ai)), "Writing to attached group at an inactive offset!");
                            UT_ASSERT_MSG_P(!isDetached() || (GAisValid(ai) && ai < myData.getArraySize() && getIndexMap().isOffsetActive(ai)), "Writing to detached group out of range!  Note that they don't update when you add or remove elements from the detail!");

			    if (myOrder)
			    {
				if (ADD)
				    myOrder->append(ai);
				else
				    myOrder->remove(ai);
			    }

			    // maintain count
			    if (myGroupEntries >= 0)
				myGroupEntries += ADD ? +1 : -1;

			    myData.set(ai, ADD);
			}

    template <bool ADD>
    void		setMembership(const GA_Range &range)
			{
			    if (myOrder && range.canContainDuplicates())
			    {
				for (GA_Iterator it(range); !it.atEnd(); ++it)
				{
				    if (ADD != contains(*it))
					setMembership<ADD>(*it);
				}
				return;
			    }
			    // else unordered group, or ordered group with
			    // range that cannot contain duplicates.

			    if (myOrder)
			    {
				for (GA_Iterator it(range); !it.atEnd(); ++it)
				{
				    if (ADD != contains(*it))
				    {
					if (ADD)
					    myOrder->append(*it);
					else
					    myOrder->remove(*it);
				    }
				}
			    }
                            UT_IF_ASSERT_P(
                            if (isDetached())
                            {
                                for (GA_Iterator it(range); !it.atEnd(); ++it)
                                {
                                    GA_Offset ai = *it;
                                    if (!GAisValid(ai) || ai >= myData.getArraySize() || !getIndexMap().isOffsetActive(ai))
                                        UT_ASSERT_MSG_P(0, "Writing to detached group out of range!  Note that they don't update when you add or remove elements from the detail!");
                                }
                            })
			    myData.set(range, ADD);
			    invalidateGroupEntries();
			}

    /// Only called by GA_GroupTable::rename(const char *, const char *).
    virtual bool	 setName(const UT_StringHolder &n);

    virtual GA_Attribute	*doClone(const GA_IndexMap &index_map,
					 const UT_StringHolder &name) const;

    /// @{ GA_AIFTuple
    bool	tupleSet(GA_Offset di, const GA_ATIGroupBool &s, GA_Offset si);
    bool	tupleSet(const GA_Range &di, const GA_ATIGroupBool &s,
			 const GA_Range &si);
    /// @}

    /// @{ GA_AIFMerge
    bool		 mergeAppendData(const GA_MergeMap &map,
					const GA_Attribute *sattrib);
    /// @}

    GA_DataBitArray		 myData;

    GA_ElementGroupOrder	*myOrder;
    mutable GA_Size		 myGroupEntries;
    mutable UT_Lock		 myGroupEntriesConstLock;

    static GA_AIFCopyData	*myAIFCopyData;
    static GA_AIFTuple		*myAIFTuple;
    static GA_AIFInterp		*myAIFInterp;	// GA_AIFCondInterp
    static GA_AIFMerge		*myAIFMerge;

    static const GA_AttributeType *theAttributeType;

    friend class GU_Group;          // For operators
    friend class GU_ElementGroup;   // For operators
    friend class GOP_GroupParse;    // For |= operator

    /// @cond INTERNAL_DOX
    class ga_GroupBoolCopyData;
    class ga_GroupBoolTuple;
    class ga_GroupBoolMerge;
    class ga_GroupType;
    /// @endcond
};

typedef GA_ElementGroup GA_ATIGroupBool;

#endif
