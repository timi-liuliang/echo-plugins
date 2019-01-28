/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATIIndexPair.h ( GA Library, C++)
 *
 * COMMENTS:	Index Pair ATI (Attribute Type Implementation)
 *
 * 		Currently implemented with two data array tuples,
 * 		one for the indices, and one for the values.
 *
 * 		We should support a single index mapping to a value tuple,
 * 		instead of just a single value.  The basic GA_Attribute
 * 		interface simply accesses the first entry in any value
 * 		tuple. 
 *
 * 		The GA_Attribute level tuple size refers to the number of
 * 		index pair entries.  However, the defaults set at the base
 * 		class are the defaults for a single entry, and so should match
 * 		the tuple size of the value tuple.
 *
 * 		// TODO: Turn the base GA_Attribute interface into a sparse
 * 		//       array interface.  getTupleSize() would return an
 * 		//       upper bound on the index and the get/set methods
 * 		//       would function implement an abstract 
 * 		//       maximum index 
 */

#pragma once

#ifndef __GA_ATIIndexPair
#define __GA_ATIIndexPair

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AttributeType.h"
#include "GA_DataArray.h"
#include "GA_DataArrayTuple.h"
#include "GA_Defaults.h"
#include "GA_Range.h"
#include "GA_Types.h"

#include <UT/UT_ConcurrentVector.h>
#include <UT/UT_Lock.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>


class GA_AIFCompare;
class GA_AIFCopyData;
class GA_AIFEdit;
class GA_AIFIndexPair;
class GA_AIFIndexPairObjects;
class GA_AIFInterp;
class GA_AIFJSON;
class GA_AIFMath;
class GA_AIFMerge;
class GA_AIFTuple;
class GA_Defragment;
class GA_IndexMap;
class GA_LoadMap;
class GA_MergeMap;
class GA_SaveMap;
class ga_IndexPairObjectSets;

class UT_JSONParser;
class UT_JSONWriter;


class GA_API GA_ATIIndexPair : public GA_Attribute
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
    static GA_ATIIndexPair *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATIIndexPair *>(attrib);
        return NULL;
    }
    SYS_FORCE_INLINE
    static const GA_ATIIndexPair *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATIIndexPair *>(attrib);
        return NULL;
    }

    GA_ATIIndexPair(const GA_AttributeType &type,
			  const GA_IndexMap &index_map, GA_AttributeScope scope,
			  const UT_StringHolder &name,
			  GA_Storage value_store, int entries, int tuple_size,
			  const GA_Defaults &index_default,
			  const GA_Defaults &value_defaults);
    virtual ~GA_ATIIndexPair();

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
						{ return myAIFPair; }
    virtual const GA_AIFMerge		*getAIFMerge() const
						{ return myAIFMerge; }
    virtual const GA_AIFEdit		*getAIFEdit() const
						{ return myAIFEdit; }
    virtual const GA_AIFTuple		*getAIFTuple() const
						{ return myAIFTuple; }
    virtual const GA_AIFMath		*getAIFMath() const
						{ return myAIFMath; }
    virtual const GA_AIFInterp		*getAIFInterp() const
						{ return myAIFInterp; }
    virtual const GA_AIFCompare		*getAIFCompare() const
						{ return myAIFCompare; }

    /// @section JSON-GA_ATIIndexPair JSON Schema: GA_ATIIndexPair
    /// @code
    /// {
    ///     "name"          : "GA_ATIIndexPair",
    ///     "description"   : "An tuple array of indexed strings",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "idefault": {
    ///             "type"        : {"$ref":"GA_Defaults"},
    ///             "description" : "Defaults for index values",
    ///             "optional"    : true,
    ///         },
    ///         "vdefault": {
    ///             "type"        : {"$ref":"GA_Defaults"},
    ///             "description" : "Defaults for values",
    ///             "optional"    : true,
    ///         },
    ///         "entries": {
    ///             "type"        : "integer",
    ///             "description" : "Number of entries for index/values tuples",
    ///         },
    ///         "istorage": {
    ///             "type"        : "string",
    ///             "description" : "Storage type for index values",
    ///             "enum"        : [ "int8", "int16", "int32", "int64" ],
    ///         },
    ///         "vstorage": {
    ///             "type"        : "string",
    ///             "description" : "Storage type for values",
    ///             "enum"        : [ "fpreal16", "fpreal32", "fpreal64" ],
    ///         },
    ///         "index": {
    ///             "type"        : {"$ref":"GA_DataArrayTuple"},
    ///             "description" : "Index values.",
    ///         },
    ///         "value": {
    ///             "type"        : {"$ref":"GA_DataArrayTuple"},
    ///             "description" : "Weight values.",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    virtual const GA_AIFJSON	*getAIFJSON() const	{ return myAIFJSON; }

    int		 getEntries() const { return myEntryCapacity.relaxedLoad(); }
    /// setEntries() is NOT thread-safe.  To concurrently grow the entry
    /// capacity, use growEntriesTo(size, true).
    bool	 setEntries(int size);
    /// Use growEntriesTo() instead of setEntries() to ensure that the entry
    /// capacity never shrinks.  This is necessary for concurrent growing of
    /// the entry capacity to properly handle the situation where one thread
    /// tries to grow the entries less than another.  Set the concurrent arg
    /// if more than one thread can call this method concurrently.
    bool	 growEntriesTo(int size, bool concurrent = false);

    bool	 setDataStorage(GA_Storage storage);
    bool	 setIndexStorage(GA_Storage storage);

    const GA_Defaults	&getValueDefaults() const { return myValueDefaults; }
    const GA_Defaults	&getValueOperationalDefaults() const
					{ return myValueOperationalDefaults; }

    /// The size of the data tuple
    int		 getDataTupleSize() const { return myTupleSize; }

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
        const GA_ATIIndexPair *thatn = UTverify_cast<const GA_ATIIndexPair *>(that);
        if (getDataStorage() != thatn->getDataStorage())
            return false;
        if (myIndexStorage != thatn->myIndexStorage)
            return false;
        if (getDataTupleSize() != thatn->getDataTupleSize())
            return false;
        if (getEntries() != thatn->getEntries())
            return false;
        return true;
    }
    /// In the case that we're copying from an attribute whose storage
    /// type matches this exactly, this function copies the metadata
    /// not associated with the storage, e.g. myOptions,
    /// *excluding* the name and the data ID.
    virtual void copyNonStorageMetadata(const GA_Attribute *that);

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
    int		getObjectSetCount() const;
    void	setObjectSetCount(int n);

    const GA_AIFIndexPairObjects *getObjectsConst(int s = 0) const;
    GA_AIFIndexPairObjects *getObjects(int s = 0);

    bool        hasSameProperties(const GA_ATIIndexPair *sattrib) const;
    bool        copyProperties(const GA_ATIIndexPair *sattrib);
    bool        mergeObjectValues(const GA_ATIIndexPair *sattrib, UT_IntArray &new_indices);

    bool	setTupleSize(int size);
    GA_Storage	getDataStorage() const { return myDataStorage; }
    bool	getIndex(GA_Offset ai, int entry, int32 &index) const { myIndex[entry]->get(ai, index); return true; }
    bool	getData(GA_Offset ai, int entry, fpreal32 &data, int data_component = 0) const { myData[entry]->getData(data_component)->get(ai, data); return true; }
    bool	getData(GA_Offset ai, int entry, fpreal64 &data, int data_component = 0) const { myData[entry]->getData(data_component)->get(ai, data); return true; }
    bool	getData(GA_Offset ai, int entry, int32 &data, int data_component = 0) const { myData[entry]->getData(data_component)->get(ai, data); return true; }
    bool	getData(GA_Offset ai, int entry, int64 &data, int data_component = 0) const { myData[entry]->getData(data_component)->get(ai, data); return true; }
    bool	setIndex(GA_Offset ai, int entry, int32 index) { myIndex[entry]->set(ai, index); return true; }
    bool	setIndex(const GA_Range &ai, int entry, int32 index) { myIndex[entry]->set(ai, index); return true; }
    bool	setData(GA_Offset ai, int entry, fpreal32 data, int data_component = 0) { myData[entry]->getData(data_component)->set(ai, data); return true; }
    bool	setData(GA_Offset ai, int entry, fpreal64 data, int data_component = 0) { myData[entry]->getData(data_component)->set(ai, data); return true; }
    bool	setData(GA_Offset ai, int entry, int32 data, int data_component = 0) { myData[entry]->getData(data_component)->set(ai, data); return true; }
    bool	setData(GA_Offset ai, int entry, int64 data, int data_component = 0) { myData[entry]->getData(data_component)->set(ai, data); return true; }

    bool	removeIndex(int index);
    void	invalidateTrailingEntries(GA_Offset ai, int start_entry);
    void	invalidateTrailingEntries(const GA_Range &ai, int start_entry);

    /// @{
    /// Copy Data AIF
    bool	copyData(GA_Offset di, const GA_ATIIndexPair &src,
			GA_Offset si);
    bool	copyData(const GA_Range &di, const GA_ATIIndexPair &src,
			const GA_Range &si);
    /// @}

    /// @{
    /// Compare AIF
    bool	isAlmostEqual(GA_Offset di, const GA_ATIIndexPair &src,
			GA_Offset si, int ulps) const;
    bool	isAlmostEqual(const GA_Range &di, const GA_ATIIndexPair &src,
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
    /// @}

    /// @{
    /// GA_AIFTuple
    /// The AIFTuple has a tuple with 2*getEntries() entries.  The entries are
    /// interleaved with ( index0, value0, index1, value1, ...)
    int		tupleGetSize() const
		    { return (myTupleSize + 1) * getEntries(); }
    bool	tupleSetSize(int size);
    GA_Storage	tupleGetStorage() const;
    bool	tupleSetStorage(GA_Storage storage);
    bool	isNan(GA_Offset ai) const;
    bool	isNan(const GA_Range &ai) const;
    template <typename T> inline bool
		tupleGet(GA_Offset ai, T &v, int index) const;
    template <typename T> inline bool
		tupleGet(GA_Offset ai, T *v, int count, int start) const;
    template <typename T> inline bool
		tupleSet(GA_Offset ai, T v, int vi);
    template <typename T> inline bool
		tupleSet(GA_Offset ai, const T *v, int count, int start);
    template <typename T> inline bool
		tupleSet(const GA_Range &di, T v, int vi);
    template <typename T> inline bool
		tupleSet(const GA_Range &di, const T *v, int count, int start);
    inline bool	tupleSet(GA_Offset di, const GA_ATIIndexPair &s,
			    GA_Offset si, int component);
    inline bool	tupleSet(const GA_Range &di, const GA_ATIIndexPair &s,
			    const GA_Range &si, int component);
    /// @}

protected:
    // Protected Data
    ga_IndexPairObjectSets	 *myObjects;
    UT_ConcurrentVector<GA_DataArray *>		 myIndex;
    UT_ConcurrentVector<GA_DataArrayTuple *>	 myData;
    int				  myTupleSize;
    SYS_AtomicInt32		  myEntryCapacity;
    GA_Storage			  myIndexStorage;
    GA_Storage			  myDataStorage;
    GA_Defaults			  myIndexDefault;
    GA_Defaults			  myValueDefaults;
    GA_Defaults			  myValueOperationalDefaults;
    UT_Lock			  myEntryGrowthLock;

    /// Scale all weights by the given amount (used in weighted sums)
    void	interpScale(GA_Offset di, fpreal scale);

private:
    virtual GA_Attribute	*doClone(const GA_IndexMap &index_map,
				       const UT_StringHolder &name) const;

    /// @{ GA_AIFJSON
    bool		 jsonSave(UT_JSONWriter &w, const GA_SaveMap &s) const;
    bool		 jsonLoad(UT_JSONParser &p, const GA_LoadMap &l);
    /// @}

    static GA_AIFIndexPair	*myAIFPair;
    static GA_AIFMerge		*myAIFMerge;
    static GA_AIFCopyData	*myAIFCopyData;
    static GA_AIFEdit		*myAIFEdit;
    static GA_AIFJSON		*myAIFJSON;
    static GA_AIFTuple		*myAIFTuple;
    static GA_AIFMath		*myAIFMath;
    static GA_AIFInterp		*myAIFInterp;
    static GA_AIFCompare	*myAIFCompare;

    static const GA_AttributeType *theAttributeType;

    /// @cond INTERNAL_DOX
    friend class ga_IndexPairAccess;
    friend class ga_IndexPairMerge;
    friend class ga_IndexPairEdit;
    friend class ga_IndexPairEditDelta;
    friend class ga_IndexPairEditSnapshot;
    friend class ga_IndexPairJSON;
    friend class ga_IndexPairTuple;
    friend class ga_IndexPairMath;
    friend class ga_IndexPairInterp;
    friend class ga_IndexPairCopyData;
    friend class ga_IndexPairCompare;
    /// @endcond
};

#endif

