/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_ATINumeric.h (GA Library, C++)
 *
 * COMMENTS:    Numeric ATI (Attribute Type Implementation)
 */

#pragma once

#ifndef __GA_ATINumeric
#define __GA_ATINumeric

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_PageArray.h"
#include "GA_Defaults.h"
#include "GA_AttributeType.h"           // for GA_AttributeType
#include "GA_Range.h"                   // for GA_Range
#include "GA_Types.h"                   // for GA_Offset, etc

#include <UT/UT_Assert.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Inline.h>		// for SYS_FORCE_INLINE
#include <SYS/SYS_Types.h>              // for fpreal32, fpreal64, fpreal, etc


class GA_AIFCompare;
class GA_AIFCopyData;
class GA_AIFDelta;
class GA_AIFEdit;
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
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;


class GA_API GA_ATINumeric : public GA_Attribute
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
    static GA_ATINumeric *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATINumeric *>(attrib);
        return nullptr;
    }
    SYS_FORCE_INLINE
    static const GA_ATINumeric *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATINumeric *>(attrib);
        return nullptr;
    }

    GA_ATINumeric(const GA_AttributeType &type,
			const GA_IndexMap &index_map,
			GA_AttributeScope scope,
			const UT_StringHolder &name,
			GA_Storage store,
			int tuple_size,
			const GA_Defaults &defaults);
    virtual ~GA_ATINumeric();

    virtual int64	getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// @{
    /// Interface for defragmentation
    virtual void	defragment(const GA_Defragment &defrag);
    /// @}

    virtual void	reconstructElementBlock(GA_Offset offset, GA_Offset nelements);

    /// Data is paged, so concurrent writes to separate pages supported.
    virtual WriteConcurrence	 getSupportedWriteConcurrence() const
					{ return WRITE_CONCURRENCE_PAGE; }

    virtual const GA_AIFCopyData *getAIFCopyData() const {return myAIFCopyData;}
    virtual const GA_AIFDelta	*getAIFDelta() const	{ return myAIFDelta; }
    virtual const GA_AIFEdit	*getAIFEdit() const	{ return myAIFEdit; }
    virtual const GA_AIFTuple	*getAIFTuple() const	{ return myAIFTuple; }
    virtual const GA_AIFMath	*getAIFMath() const
				    { return myDoMath ? myAIFMath:myAIFNoMath; }
    virtual const GA_AIFMerge	*getAIFMerge() const	{ return myAIFMerge; }
    virtual const GA_AIFInterp	*getAIFInterp() const	{ return myAIFInterp; }
    virtual const GA_AIFCompare	*getAIFCompare() const  { return myAIFCompare; }

    /// @section JSON-GA_ATINumeric JSON Schema: GA_ATINumeric
    /// @code
    /// {
    ///     "name"          : "GA_ATINumeric",
    ///     "description"   : "An tuple array of numeric data",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "size": {
    ///             "type"          : "integer",
    ///             "minimum"       : 1,
    ///             "description"   : "Tuple size",
    ///         },
    ///         "storage": {
    ///             "type"        : "string",
    ///             "description" : "Tuple storage",
    ///             "enum"        : [ "uint8","int8","int16","int32","int64",
    ///                               "fpreal16", "fpreal32", "fpreal64" ],
    ///         },
    ///         "defaults": {
    ///             "type"  : { "$ref" : "GA_Defaults" }
    ///             "description" : "Default values",
    ///         },
    ///         "values": {
    ///             "type"  : {"$ref":"GA_DataArrayTuple"},
    ///             "description" : "Numeric values.",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    virtual const GA_AIFJSON	*getAIFJSON() const	{ return myAIFJSON; }

    /// Called during JSON load
    bool		jsonLoad(UT_JSONParser &p,
				const GA_LoadMap &map,
				const GA_Defaults &defs,
				int tuple_size,
				GA_Storage storage);

    const GA_Defaults	&getDefaults() const { return myDefaults; }

    int			 getTupleSize() const { return myData.getTupleSize(); }
    GA_Storage		 getStorage() const { return myData.getStorage(); }

    bool		 setTupleSize(int size);
    bool		 setStorage(GA_Storage storage);

    /// Checks if an attribute should be treated as a texture coordinate attribute.
    /// Ideally, we could rely on the type info being GA_TYPE_TEXTURE_COORD,
    /// but old files don't have this, and various importers don't set this.
    /// If a float[3] (optionnally float[2]) attribute's name starts with "uv"
    /// and is followed by zero or more digits (and nothing else), it's probably
    /// safe to assume that it should be treated as a texture attribute.
    bool shouldInterpretAsTexCoord(bool allow_float2=false) const
    {
        if (getTupleSize() != 3 && (!allow_float2 || getTupleSize() != 2))
            return false;

        // If it's not a float type, it's probably not a texture coordinate,
        // even if its typeinfo says otherwise.
        GA_Storage storage = getStorage();
        if (storage != GA_STORE_REAL16 && storage != GA_STORE_REAL32 && storage != GA_STORE_REAL64)
            return false;

        // If it's marked as texture coord, go with that.
        // If it's not that and not void, it's not texture coord.
        GA_TypeInfo typeinfo = getTypeInfo();
        if (typeinfo != GA_TYPE_VOID)
            return (typeinfo == GA_TYPE_TEXTURE_COORD);

        // Void type info; check name next.
        const char *name = getName().c_str();
        if (name[0] != 'u' || name[1] != 'v')
            return false;

        // Name starts with "uv".  Check if everything else is digits.
        for (name += 2; *name; ++name)
        {
            if (*name < '0' || *name > '9')
                return false;
        }
        return true;
    }

    /// Some types, especially integers, may not want to be lerped
    bool		 isNonArithmetic() const { return !myDoMath; }

    /// Debug code
    virtual bool	 debugValidateArrayCapacity(GA_Offset sz) const;

    /// Grow or shrink the array size
    virtual bool	 setArraySize(GA_Offset new_size);

    /// Try to compress data pages
    virtual void	 tryCompressAllPages(
				GA_Offset start_offset = GA_Offset(0),
				GA_Offset end_offset = GA_INVALID_OFFSET);
    /// Harden data pages
    virtual void	 hardenAllPages(
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
        const GA_ATINumeric *thatn = UTverify_cast<const GA_ATINumeric *>(that);
        if (getStorage() != thatn->getStorage())
            return false;
        if (getTupleSize() != thatn->getTupleSize())
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
        const GA_ATINumeric *thatn = UTverify_cast<const GA_ATINumeric *>(that);
        myDoMath = thatn->myDoMath;
        myDefaults = thatn->myDefaults;
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
        return tupleSet(desti, *this, srci, -1);
    }
    virtual bool copy(GA_Offset desti, const GA_Attribute &src, GA_Offset srci) final
    {
        if (!GA_ATINumeric::isType(&src))
            return false;
        return tupleSet(desti, *UTverify_cast<const GA_ATINumeric*>(&src), srci, -1);
    }
    bool copy(GA_Offset desti, const GA_ATINumeric &src, GA_Offset srci)
    {
        return tupleSet(desti, src, srci, -1);
    }
    /// @}

    /// Copy attribute values for a range of elements.
    /// WARNING: These do not support overlapping ranges within a single attribute!
    /// @{
    virtual bool copy(const GA_Range &destrange, const GA_Range &srcrange) final
    {
        return tupleSet(destrange, *this, srcrange, -1);
    }
    virtual bool copy(const GA_Range &destrange, const GA_Attribute &src, const GA_Range &srcrange) final
    {
        if (!GA_ATINumeric::isType(&src))
            return false;
        return tupleSet(destrange, *UTverify_cast<const GA_ATINumeric*>(&src), srcrange, -1);
    }
    bool copy(const GA_Range &destrange, const GA_ATINumeric &src, const GA_Range &srcrange)
    {
        return tupleSet(destrange, src, srcrange, -1);
    }
    /// @}

    /// Assign all elements of a range from a single attribute value.
    /// @{
    virtual bool fill(const GA_Range &destrange, GA_Offset srci) final;
    virtual bool fill(const GA_Range &destrange, const GA_Attribute &src, GA_Offset srci) final;
    bool fill(const GA_Range &destrange, const GA_ATINumeric &src, GA_Offset srci);
    /// @}

    typedef GA_PageArray<void, -1, true, false> DataType;

    /// @{
    /// Provide access to data
    const DataType  &getData() const    { return myData; }
    DataType        &getData()          { return myData; }
    /// @}

    /// Returns true iff the specified page is constant-compressed,
    /// where a single value is stored to represent GA_PAGE_SIZE
    /// values in the page, (or possibly fewer if only one page).
    SYS_FORCE_INLINE bool isPageConstant(GA_PageNum pagenum) const
    {
        return myData.isPageConstant(pagenum);
    }
    /// Sets all components of all elements of the specified page to
    /// the given values.
    /// NOTE: The length of values must be equal to the tuple size.
    template<typename SRC_DATA_T>
    SYS_FORCE_INLINE void setPageConstant(GA_PageNum pagenum, const SRC_DATA_T *values)
    {
        myData.setPageConstant(pagenum, values);
    }

protected:
    DataType    	 myData;
    GA_Defaults		 myDefaults;
    bool		 myDoMath;	// Off for non-arithmetic integer

    static GA_AIFCopyData	*myAIFCopyData;
    static GA_AIFDelta		*myAIFDelta;
    static GA_AIFEdit		*myAIFEdit;
    static GA_AIFTuple		*myAIFTuple;
    static GA_AIFMath		*myAIFMath;
    static const GA_AIFMath	*myAIFNoMath;
    static GA_AIFMerge		*myAIFMerge;
    static GA_AIFInterp		*myAIFInterp;
    static GA_AIFJSON		*myAIFJSON;
    static GA_AIFCompare	*myAIFCompare;

    static const GA_AttributeType *theAttributeType;

private:
    void			updateIntegerTypeInfo();

    virtual GA_Attribute	*doClone(const GA_IndexMap &index_map,
					 const UT_StringHolder &name) const;

    /// @{ GA_AIFTuple
    template <typename T> inline bool
	tupleGet(GA_Offset di, T &v, int vi) const;
    template <typename T> inline bool
	tupleGet(GA_Offset di, T *v, int count, int start) const;

    template <typename T> inline bool
	tupleSet(GA_Offset di, T v, int vi);
    template <typename T> inline bool
	tupleSet(GA_Offset di, const T *v, int count, int start);
    template <typename T> inline bool
	tupleSet(const GA_Range &di, T v, int vi);
    template <typename T> inline bool
	tupleSet(const GA_Range &di, const T *v, int count, int start);
    bool tupleSet(GA_Offset di, const GA_ATINumeric &s,
		GA_Offset si, int component);
    /// WARNING: These do not support overlapping ranges within a single attribute!
    bool tupleSet(const GA_Range &di, const GA_ATINumeric &s,
		const GA_Range &si, int component);

    template <typename T> inline bool
	makeConstant(T v, int vi);
    template <typename T> inline bool
	makeConstant(const T *v, int count, int start);

    bool	isNan(GA_Offset ai) const;
    bool	isNan(const GA_Range &ai) const;
    /// @}

    /// @{ GA_AIFInterp
    bool	interpLerp(GA_Offset di,
			const GA_ATINumeric &a, GA_Offset ai,
			const GA_ATINumeric &b, GA_Offset bi,
			fpreal t);
    bool	interpLerp(const GA_Range &di,
			const GA_ATINumeric &a, const GA_Range &ai,
			const GA_ATINumeric &b, const GA_Range &bi,
			fpreal t);
    bool	interpSmooth(GA_Offset di, fpreal min, fpreal max,
			const GA_ATINumeric &a, GA_Offset ai);
    bool	interpSmooth(const GA_Range &di, fpreal min, fpreal max,
			const GA_ATINumeric &a, const GA_Range &ai);
    bool	interpFit(GA_Offset di, const GA_ATINumeric &a, GA_Offset ai,
			fpreal omin, fpreal omax,
			fpreal nmin, fpreal nmax);
    bool	interpFit(const GA_Range &di,
			const GA_ATINumeric &a, const GA_Range &ai,
			fpreal omin, fpreal omax,
			fpreal nmin, fpreal nmax);
    /// @}

    /// @{ GA_AIFMerge
    void		 mergeDestroyDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    GA_Attribute	*mergeAddDestination(const GA_MergeMap &map,
					GA_Attribute *dattrib) const;
    void		 mergeGrowArray(const GA_MergeMap &map,
					const GA_ATINumeric &s);
    bool		 mergeAppendData(const GA_MergeMap &map,
					const GA_Attribute *sattrib);
    /// @}

    /// @{ GA_AIFJSON
    /// Save data to a JSON stream.
    bool		 jsonSave(UT_JSONWriter &w, const GA_SaveMap &s) const;
    /// @}

    /// @cond INTERNAL_DOX
    friend class ga_NumericCopyData;
    friend class ga_NumericDelta;
    friend class ga_NumericEdit;
    template <typename T> friend class ga_NumericEditDelta;
    template <typename T> friend class ga_NumericEditSnapshot;
    friend class ga_NumericInterp;
    friend class ga_NumericJSON;
    friend class ga_NumericMarshal;
    friend class ga_NumericMath;
    friend class ga_NumericMerge;
    friend class ga_NumericTuple;
    friend class ga_NumericCompare;

    template <typename T> friend class binderScalar;
    template <typename T> friend class binderVector;
    template <typename T, typename TB> friend class binderPage;
    /// @endcond
};

#endif

