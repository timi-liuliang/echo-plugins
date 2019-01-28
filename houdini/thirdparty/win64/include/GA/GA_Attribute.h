/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Attribute.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_Attribute__
#define __GA_Attribute__

#include "GA_API.h"
#include "GA_AttributeProxy.h"
#include "GA_AttributeType.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_StringHolder.h>

#include <SYS/SYS_StaticAssert.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <string>
#include <string.h>

class GA_AIFBlindData;
class GA_AIFBlob;
class GA_AIFBlobArray;
class GA_AIFCompare;
class GA_AIFCopyData;
class GA_AIFDelta;
class GA_AIFEdit;
class GA_AIFIndexPair;
class GA_AIFInterp;
class GA_AIFJSON;
class GA_AIFMath;
class GA_AIFMerge;
class GA_AIFNumericArray;
class GA_AIFSharedStringArray;
class GA_AIFSharedStringTuple;
class GA_AIFStat;
class GA_AIFStringTuple;
class GA_AIFTuple;
class GA_Attribute;
class GA_CEAttribute;
class GA_Defragment;
class GA_Detail;
class GA_IndexMap;
class GA_Range;

class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
class UT_OptionEntry;
class UT_Options;
class UT_String;
class UT_WorkBuffer;

class GA_API GA_AttributeOptions
{
public:
    GA_AttributeOptions();
    ~GA_AttributeOptions();

    GA_AttributeOptions(const GA_AttributeOptions &src);

    GA_AttributeOptions &operator=(const GA_AttributeOptions &src);

    /// Reset to default.
    void		 clear();

    /// Constructs a full UT_Options that includes all symbols that
    /// are otherwise handled with member variables.
    void		 buildFullOptions(UT_Options &fulloptions) const;
    
    /// Removes from our myOptions list any options that aren't custom
    /// and assigns them to local variables.  May then delete myOptions
    /// if it is empty.
    void		 extractBuiltinOptions();

    /// If no custom options are present, create it.
    UT_Options		&createOptions();

    /// Merges option list on top of our own
    void		 mergeOptions(const GA_AttributeOptions &src);

    /// @{
    /// Accessors
    GA_TypeInfo	typeInfo() const		{ return myTypeInfo; }
    void	setTypeInfo(GA_TypeInfo t)	{ myTypeInfo = t; }
    bool	isNonTransforming() const	{ return myIsNonTransforming; }
    void	setIsNonTransforming(bool v)	{ myIsNonTransforming = v; }
    bool	exportToInfoBlock() const	{ return myExportToInfoBlock; }
    void	setExportToInfoBlock(bool v)	{ myExportToInfoBlock = v; }
    /// @}

    /// Check whether the exportOnMerge() flag has been set, or whether it's at
    /// its default.
    bool	exportOnMergeSet() const	{ return myExportOnMergeValid; }
    /// Check to see whether the export on merge is enabled.  If the flag
    /// hasn't been set, the default value will be used.
    bool	exportOnMerge(bool defval=false) const
		    { return myExportOnMergeValid ? myExportOnMerge : defval; }
    /// Set the export on merge flag
    void	setExportOnMerge(bool v)
		{
		    myExportOnMergeValid = true;
		    myExportOnMerge = v;
		}

    /// Check whether the exportOnSave() flag has been set, or whether it's at
    /// its default.
    bool	exportOnSaveSet() const	{ return myExportOnSaveValid; }
    /// Check to see whether the export on save is enabled.  If the flag
    /// hasn't been set, the default value will be used.
    bool	exportOnSave(bool defval=false) const
		    { return myExportOnSaveValid ? myExportOnSave : defval; }
    /// Set the export on save flag
    void	setExportOnSave(bool v)
		{
		    myExportOnSaveValid = true;
		    myExportOnSave = v;
		}

    /// Check to see whether the attribute is a "string set".  If the flag
    /// hasn't been set, the default value will be used.
    bool	isStringSet(bool defval=false) const
		    { return myIsStringSetValid ? myIsStringSet : defval; }
    /// Set the "string set" flag
    void	setIsStringSet(bool v)
		{
		    myIsStringSetValid = true;
		    myIsStringSet = v;
		}

    /// @{
    /// Access to options
    const UT_Options *options() const	{ return myOptions; }
    void setOptions(const UT_Options &o);
    void mergeOptions(const UT_Options &o);
    void mergeOptions(const UT_Options &o,
            bool (*match)(const UT_StringHolder &name, 
        		  const UT_OptionEntry *, void *),
            void *data);
    void clearOptions();
    bool removeOption(const char *name);
    /// @}

private:
    // Our custom options.
    UT_Options	*myOptions;

    GA_TypeInfo	 myTypeInfo;

    bool	 myIsNonTransforming : 1;
    bool	 myExportOnMergeValid : 1, myExportOnMerge : 1;
    bool	 myExportOnSaveValid : 1, myExportOnSave : 1;

    bool	 myIsStringSetValid : 1, myIsStringSet : 1;

    bool	 myExportToInfoBlock : 1;

    bool         myDetached : 1;

    friend class	GA_Attribute;
};

template<typename T>
static inline const T *GAgetAIF(const GA_Attribute &attrib)
{
    SYS_UNIMPLEMENTED_TEMPLATE(T);
    return NULL;
}


/// @brief Definition of a geometry attribute
///
/// An attribute can be attached to points, vertices, primitives or the detail
/// itself.  The attribute is responsible for maintaining the data array.
///
/// @see @ref Iterators for implications of paged allocation strategies.
///
/// The attribute is responsible for providing interfaces (@ref AIF).
///
class GA_API GA_Attribute
{
public:
    GA_Attribute(
        const GA_AttributeType &type, const GA_IndexMap &index_map,
        GA_AttributeScope scope, const UT_StringHolder &name);
    virtual ~GA_Attribute();

    const GA_AttributeType	&getType() const	{ return myType; }
    const GA_IndexMap		&getIndexMap() const	{ return myIndexMap; }
    const GA_Detail		&getDetail() const;
    GA_Detail			&getDetail();
    GA_AttributeOwner		 getOwner() const;

    GA_AttributeScope		 getScope() const	{ return myScope; }

    /// @{
    /// Obtains the proxy representative used as a reference holder to this
    /// attribute. This proxy gets invalidated when the attribute gets deleted.
    /// Invalidation clears the proxy's pointer to NULL, but the proxy's memory
    /// stays valid, unlike the memory of the attribute object.
    GA_AttributeProxyHandle	getProxy()
				    { return GA_AttributeProxyHandle(myProxy); }
    GA_ConstAttributeProxyHandle getProxy() const
				    { return GA_ConstAttributeProxyHandle(myProxy); }
    /// @}

    /// Uses the "type" option to determine type information
    /// @see getOptions()
    SYS_FORCE_INLINE
    GA_TypeInfo getTypeInfo() const
    {
        return myOptions.myTypeInfo;
    }

    /// Set the option.  This sets the "type" option on the attribute
    /// @see getOptions()
    SYS_FORCE_INLINE
    void setTypeInfo(GA_TypeInfo type)
    {
        myOptions.myTypeInfo = type;
    }

    /// Checks if an attribute should be treated as a texture coordinate attribute.
    /// Ideally, we could rely on the type info being GA_TYPE_TEXTURE_COORD,
    /// but old files don't have this, and various importers don't set this.
    /// If a float[3] (optionnally float[2]) attribute's name starts with "uv"
    /// and is followed by zero or more digits (and nothing else), it's probably
    /// safe to assume that it should be treated as a texture attribute.
    bool shouldInterpretAsTexCoord(bool allow_float2=false) const;

    /// Returns the approximate type of the attribute
    GA_StorageClass		getStorageClass() const;

    /// Size of the AIFTuple, if it exists.  If it doesn't, 1.
    int				getTupleSize() const;

    // Methods to get intrinsic data
    // getName() returns the unprefixed name (as opposed to getFullName())
    // We have separate const char * versions as this is often passed
    // to sprintf().
    const UT_StringHolder &getName() const	{ return myName; }
    const UT_StringHolder &getFullName() const	{ return myName; }

    /// The data ID is shared by attributes that represent the same underlying
    /// data and is used by cache mechanisms to avoid rebuilding when data has
    /// not really changed.  An ID of -1 indicates that this attribute is not
    /// going to get a new ID when its data is changed, and therefore is not
    /// participating in this scheme.
    ///
    /// Two attributes can share the same data ID even when they don't share
    /// the same internal data pages or even fragmentation patterns.  Note
    /// however that the data ID is not necessarily changed for topology
    /// changes, so it is necessary to also compare topology data IDs in order
    /// to be absolutely sure that two attributes sharing the same data ID
    /// represent the same data (taking into account defragmentation).
    SYS_FORCE_INLINE
    GA_DataId getDataId() const { return myDataId; }
    SYS_FORCE_INLINE
    void assignNewDataId()
    {
        myDataId = nextDataId();
    }
    SYS_FORCE_INLINE
    void bumpDataId()
    {
        if (myDataId != GA_INVALID_DATAID)
            assignNewDataId();
    }
    SYS_FORCE_INLINE
    void clearDataId() { myDataId = GA_INVALID_DATAID; }

    /// This clones the data ID from src.  If src has an invalid data ID,
    /// which can be the case for details not created by SOP_Node or
    /// SIM_GeometryCopy, we default to bumping the destination data ID.
    /// This should be used by functions like GA_Attribute::replace()
    /// when an attribute's data is replaced with that of src.
    ///
    /// NOTE: We must always either clone or bump the data ID if we've
    ///       copied from another attribute, even if that src attribute
    ///       has an invalid data ID.  See Bug 79446 and 79438.
    SYS_FORCE_INLINE
    void cloneDataId(const GA_Attribute &src,
                     bool allow_clear = false)
    {
        if (allow_clear || src.myDataId != -1)
            myDataId = src.myDataId;
        else
            bumpDataId();
    }
    /// NOTE: Just call cloneDataId(src).
    SYS_FORCE_INLINE
    void cloneOrBumpDataId(const GA_Attribute &src,
                           bool allow_clear = false)
    {
        cloneDataId(src, allow_clear);
    }

    /// The method to set array size
    /// *Must* be implemented in subclasses
    /// Return true if the attribute was resized.
    virtual bool setArraySize(GA_Offset size) = 0;

    /// Try to compress data pages
    /// Will try to compress *all* pages overlapping the specified offset range.
    /// *Must* be implemented in subclasses
    virtual void tryCompressAllPages(
            GA_Offset start_offset = GA_Offset(0),
            GA_Offset end_offset = GA_INVALID_OFFSET) = 0;

    /// Harden data pages
    /// Will harden *all* pages overlapping the specified offset range.
    /// Once this is done, multiple threads can write to an attribute in
    /// parallel at a finer granularity than pages.
    /// *Must* be implemented in subclasses
    virtual void hardenAllPages(
            GA_Offset start_offset = GA_Offset(0),
            GA_Offset end_offset = GA_INVALID_OFFSET) = 0;

    /// The various concurrency levels supported by attributes for writing
    /// per-element data.
    enum WriteConcurrence {
	/// No concurrent writes supported.
	WRITE_CONCURRENCE_NONE,
	/// Concurrent writes to separate pages supported.
	WRITE_CONCURRENCE_PAGE,
	/// Concurrent writes to separate elements supported.
	WRITE_CONCURRENCE_ELEMENT
    };

    virtual WriteConcurrence	 getSupportedWriteConcurrence() const = 0;

    /// Return the attribute's copy interface or NULL
    virtual const GA_AIFCopyData	*getAIFCopyData() const;
    /// Return the attribute's delta interface or NULL
    virtual const GA_AIFDelta		*getAIFDelta() const;
    /// Return the attribute's tuple interface or NULL
    virtual const GA_AIFTuple		*getAIFTuple() const;
    /// Return the attribute's math interface or NULL
    virtual const GA_AIFMath		*getAIFMath() const;
    /// Return the attribute's merge interface or NULL
    virtual const GA_AIFMerge		*getAIFMerge() const;
    /// Return the attribute's interpolation interface or NULL
    virtual const GA_AIFInterp		*getAIFInterp() const;
    /// Return the attribute's blob interface or NULL
    virtual const GA_AIFBlob		*getAIFBlob() const;
    /// Return the attribute's blob array interface or NULL
    virtual const GA_AIFBlobArray	*getAIFBlobArray() const;
    /// Return the attribute's string tuple interface or NULL
    virtual const GA_AIFStringTuple	*getAIFStringTuple() const;
    /// Return the attribute's shared string tuple interface or NULL
    virtual const GA_AIFSharedStringTuple *getAIFSharedStringTuple() const;
    /// Return the attribute's shared string array interface or NULL
    virtual const GA_AIFSharedStringArray *getAIFSharedStringArray() const;
    /// Return the attribute's index pair interface or NULL
    virtual const GA_AIFIndexPair	*getAIFIndexPair() const;
    /// Return the attribute's blind data interface or NULL
    virtual const GA_AIFBlindData	*getAIFBlindData() const;
    /// Return the attribute's edit interface or NULL
    virtual const GA_AIFEdit		*getAIFEdit() const;
    /// Return the attribute's comparison interface or NULL
    virtual const GA_AIFCompare		*getAIFCompare() const;
    /// Return the attribute's arraydata interface or NULL
    virtual const GA_AIFNumericArray	*getAIFNumericArray() const;

    /// Return the attribute's stat interface.  Unlike other interfaces, this
    /// method has default behaviour.
    virtual const GA_AIFStat		*getAIFStat() const;

    /// Generic method for getting an AIF by type
    template <typename T> const T	*getAIF() const { return GAgetAIF<T>(*this); }

    // Options can be used to associated arbitrary data with an attribute.
    // Common options are defined in GA_Options.h
    const GA_AttributeOptions	&getOptions() const	{ return myOptions; }
    GA_AttributeOptions		&getOptions()		{ return myOptions; }

    /// Although an attribute may be a "point" or "normal", we may not want to
    /// transform the point.  This is the case with "rest" for example.
    /// At the current time, this sets the flag corresponding with
    /// "attribute:non_transforming" in the options.
    /// @see needsTransform()
    /// @see getTypeInfo()
    void setNonTransforming(bool val)
    {
        myOptions.setIsNonTransforming(val);
    }

    /// needTransform() checks whether the attribute is a
    /// floating-point attribute whose type info is a transforming
    /// type and whose non-transforming flag is false.
    bool needsTransform(bool include_P = true) const
    {
        return getStorageClass() == GA_STORECLASS_FLOAT
            && !myOptions.isNonTransforming()
            && GAisTransformingType(getTypeInfo())
            && (include_P || strcmp(getName(), "P") != 0);
    }

    bool isDetached() const
    {
        return myOptions.myDetached;
    }

    /// @{
    /// Houdini forces certain restrictions on attribute names.  However,
    /// external software doesn't always have these restrictions.  When
    /// importing attributes, you can store the "real" attribute name, or use
    /// the "export" name when exporting.
    ///
    /// @c getExportName() will return the attribute name if there's no export
    /// name set.
    void	setExportName(const char *external_name);
    bool	hasExportName() const;
    std::string getExportName() const;
    /// @}

    /// Merge the options passed in with the options in the attribute
    void		 mergeOptions(const GA_AttributeOptions &src)
				{ myOptions.mergeOptions(src); }

    // Check to see the value of a specific option
    bool		 importOption(const char *name, int &v) const;
    bool		 importOption(const char *name, bool &v) const;
    bool		 importOption(const char *name, fpreal32 &v) const;
    bool		 importOption(const char *name, fpreal64 &v) const;
    bool		 importOption(const char *name, UT_String &v) const;

    /// Methods which can be overridden from GA_Attribute

    /// Element destruction event.  When an element is destroyed, the attribute
    /// may be notified of this event.
    virtual bool	needDestruction() const;
    /// Callback invoked if needsDestruction() returns true
    virtual void	destructElement(GA_Offset offset);

    /// The GA library expects any elements allocated at the end of the array
    /// to be set to their default values.  If this is not the case, then the
    /// attribute can register itself for special initialization.
    void		setTailInitialization(bool onoff);

    /// Check whether the attribute is set to tail initialization
    bool		isTailInitialization() const
				{ return myTailInitialize; }

    /// When the array size is grown, attributes should initialize the value
    /// appropriately.  When an element is reused, however, the
    /// reconstructElement is called.  This happens if an element is deleted
    /// then a new element is allocated (using the existing slot)
    void		reconstructElement(GA_Offset offset)
                                { reconstructElementBlock(offset, GA_Offset(1)); }

    /// When the array size is grown, attributes should initialize the value
    /// appropriately.  When elements are reused, however,
    /// reconstructElementBlock is called.  This happens if an element is
    /// deleted and then a new element is allocated (using the existing slot)
    virtual void	reconstructElementBlock(GA_Offset offset, GA_Offset nelements);

    /// A method to "compact" any storage.  For example, if there's a string
    /// table associated with the attribute, this gives the attribute the option
    /// to adjust the table to be more compact.
    virtual void	compactStorage();

    /// This method is called before a batch deletion operation.  It allows the
    /// attribute to mark objects as dirty so that there can be lazy computation
    /// of data.  For example, re-computing the number of elements in a group.
    /// This method may be called multiple times, and so should typically just
    /// set a flag (not perform a lot of computation).
    virtual void	batchDeletion();

    /// This method returns a new attribute of the same type with identical
    /// settings.  Attribute data is not copied.  Derived classes implement
    /// this method by overriding the virtual doClone().  Those classes not
    /// supporting cloning are permitted to return NULL.
    GA_Attribute	*clone(const GA_IndexMap &index_map,
                               const UT_StringHolder &name,
			       bool clone_options) const;

    /// @section JSON-GA_Attribute JSON Schema: GA_Attribute
    ///
    /// The schema for an attribute consists of an array of two items.  The
    /// first entry contains of the definition of the attribute.  The second
    /// entry is the "private" data for the attribute.  If there is no data for
    /// the attribute (i.e. the attribute doesn't provide an AIFJSON), then a
    /// null will be saved as the second element.
    ///
    /// The private data is saved/loaded by the GA_AIFJSON class for the
    /// attribute.
    ///
    /// @code
    /// [
    ///    "name"   : "GA_Attribute",
    ///    "description" :
    ///          "An attribute is defined by an array with two items.  The
    ///           first item is the definition of the attribute, the second
    ///           is private data associated with the attribute (may be null).",
    ///    "type" : "array",
    ///    "items" : [
    ///        { "$ref" : "GA_Attribute-Definition" }, // Attribute Definition
    ///        { "$ref" : "GA_Attribute-Data" },       // Specific type data
    ///    ],
    /// ]
    /// @endcode
    ///
    /// @section JSON-GA_Attribute-Definition JSON Schema: GA_Attribute-Definition
    /// The attribute definition is a map containing the fields required to
    /// recreate the attribute.
    /// @code
    /// {
    ///     "name"          : "GA_Attribute-Definition",
    ///     "description"   : "An map defining the attribute",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "scope": {
    ///             "type"        : "string",
    ///		    "optional"	  : true,
    ///		    "default"	  : "public",
    ///             "description" : "Scope of the attribute",
    ///         },
    ///         "type": {
    ///             "type"          : "string",
    ///             "description"   : "Attribute type (ATI name)",
    ///         },
    ///         "name": {
    ///             "type"        : "string",
    ///             "description" : "Name of the attribute",
    ///         },
    ///         "version": {
    ///             "type"        : "integer",
    ///		    "optional"	  : true,
    ///             "description" : "Version level for save",
    ///         },
    ///         "options": {
    ///             "type"  : { "$ref" : "UT_Options" }
    ///             "description" : "Arbitrary options on the attribute",
    ///         },
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat, GA_AttributeSet, GA_AttributeType

    /// Save the GA_Attribute-Definition section
    bool	jsonSaveDefinition(UT_JSONWriter &w) const;

    /// Load the GA_Attribute-Definition section
    static bool	jsonLoadDefinition(UT_JSONParser &p,
				    GA_AttributeScope &scope,
				    UT_WorkBuffer &type,
				    UT_WorkBuffer &name,
				    int64 &version,
				    GA_AttributeOptions &options);

    /// Return the AIF to handle saving/loading of the private data section
    virtual const GA_AIFJSON		*getAIFJSON() const;

    /// Return the optional "save" version number.  This is passed to the JSON
    /// attribute loader.  If the version is <= 0, the version is not saved to
    /// the file and 0 will be passed to the loader.
    virtual int				 getSaveVersion() const;

    /// Debug validation of allocated array size
    virtual bool	debugValidateArrayCapacity(GA_Offset sz) const;

    /// Report the memory usage
    ///
    /// NOTE: If you're implementing this in a direct subclass of GA_Attribute,
    ///       call getBaseMemoryUsage() to get the amount of memory in the base
    ///       class.  If in a more distant descendant class, call
    ///       Base::getMemoryUsage(false) to count the memory in Base and
    ///       above, excluding sizeof(Base).
    virtual int64	getMemoryUsage(bool inclusive) const = 0;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    ///
    /// NOTE: If you're implementing this in a direct subclass of GA_Attribute,
    ///       call getBaseMemoryUsage() to get the amount of memory in the base
    ///       class.  If in a more distant descendant class, call
    ///       Base::countMemory(counter, false) to count the memory in Base and
    ///       above, excluding sizeof(Base).
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const = 0;

    /// @{
    /// @c defragment() defragments the memory used to store data elements
    virtual void	defragment(const GA_Defragment &defrag) = 0;
    /// @}

    /// @private
    /// Used only for optimizing merging of attributes
    const GA_Attribute	*getMergeSource() const	{ return myMergeSource; }

    /// This is only here so that select data outside of GA_Attribute
    /// can also have data IDs, like GA_PrimitiveList and GA_EdgeGroup.
    static GA_DataId nextDataId();

    /// @{
    /// The attribute data may be moved to the GPU, and hence backed
    /// by a compute buffer (GA_CEAttribute).  Flushing will delete
    /// our buffer & copy it back from the GPU if it has been marked
    /// as modified.

    /// Caches a CE Attribute.  If a cache already exists, the cached
    /// data is returned and the write flag updated.  If the cache
    /// does not exist, an attempt is made to build a buffer.  If
    /// read is true, the buffer is initialized with the geometry data.
    /// This can return NULL if a failure to build an attribute occurs
    /// (for example, unsupported type or no GPU)
    GA_CEAttribute	*getCEAttribute(GA_StorageClass storage, int &tuplesize, bool isarray, bool read, bool write);

    /// Any CE cache which was marked as written to will be copied
    /// back to the CPU.  It will be left on the GPU, however.
    void		 flushCEWriteCaches(bool clearwriteback=true);
    bool		 hasPendingCEWriteBack() const;

    /// Remove all CE Caches, copying back any marked as written to.
    void		 flushCECaches();

    /// @}
protected:
    /// For use by getMemoryUsage and countMemory in subclasses to count the
    /// amount of memory used by this base class (NOTE: *always* exclusive)
    int64 getBaseMemoryUsage() const;

    /// Only called by GA_AttributeSet and GA_ElementGroup
    void setDetached(bool detached)
    {
        myOptions.myDetached = detached;
    }
private:
    /// Method called by the GA_AttributeType::create() factory method to
    /// initialize the object by calling virtual overrides not accessible
    /// in our constructor.
    void		initialize();

    /// Create a new attribute of this type.
    virtual GA_Attribute *doClone(const GA_IndexMap &index_map,
				  const UT_StringHolder &name) const;

    friend GA_Attribute *GA_AttributeType::create(const GA_IndexMap &,
						  GA_AttributeScope,
						  const UT_StringHolder &,
						  const UT_Options *,
						  const GA_AttributeOptions *) const;

    /// @{
    /// @private
    /// Only called by GA_AttributeSet and GA_ElementGroup.
    bool	 setName(const UT_StringHolder &newname);
    void	 setProxy(GA_AttributeProxy *proxy);
    friend class GA_AttributeSet;
    friend class GA_ElementGroup;
    friend class GA_MergeMap;
    /// @}

public:

    /// Returns true iff that is an attribute whose content can be copied
    /// from this without any type conversions.  This is important to
    /// avoid reallocation of an attribute if its storage type,
    /// including tuple size, matches the source attribute exactly.
    /// Subclasses should call this first and return false if
    /// this returns false.
    virtual bool matchesStorage(const GA_Attribute *that) const
    {
        if (that == NULL)
            return false;
        if (getType().getTypeId() != that->getType().getTypeId())
            return false;
        return true;
    }
    /// In the case that we're copying from an attribute whose storage
    /// type matches this exactly, this function copies the metadata
    /// not associated with the storage, e.g. myOptions,
    /// *excluding* the name and the data ID.
    /// The attribute class of that must match the attribute class of that.
    /// Subclasses must call this.
    virtual void copyNonStorageMetadata(const GA_Attribute *that)
    {
        myOptions = that->myOptions;
        // NOTE: We can't just copy myScope, since private scope attributes
        //       are stored with a prefix and public scope attributes
        //       aren't.
        UT_ASSERT_P(myScope == that->myScope);

        // NOTE: DO NOT set tail initialization here!  This is just an
        //       indication of whether there may be data at non-default values
        //       past myIndexMap.myMaxTouchedOffset.
        //       This should be copied in the implementations of replace(),
        //       instead, since that's associated with referencing the data.
        //setTailInitialization(that->myTailInitialize);
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
    virtual void replace(const GA_Attribute &src) = 0;

    /// Copy attribute values for a single element.
    /// NOTE: The default implementation just tries to use GA_AIFCopyData,
    /// incurring 2 more virtual calls!
    /// @{
    virtual bool copy(GA_Offset desti, GA_Offset srci)
    { return copy(desti, *this, srci); }
    virtual bool copy(GA_Offset desti, const GA_Attribute &src, GA_Offset srci);
    /// @}

    /// Copy attribute values for a range of elements.
    /// NOTE: The default implementation just tries to use GA_AIFCopyData,
    /// incurring 2 more virtual calls!
    /// WARNING: These do not support overlapping ranges within a single attribute!
    /// @{
    virtual bool copy(const GA_Range &destrange, const GA_Range &srcrange)
    { return copy(destrange, *this, srcrange); }
    virtual bool copy(const GA_Range &destrange, const GA_Attribute &src, const GA_Range &srcrange);
    /// @}

    /// Assign all elements of a range from a single attribute value.
    /// NOTE: The default implementation just tries to use GA_AIFCopyData,
    /// incurring 2 more virtual calls, falling back on calling copy() for
    /// each element in the destination range, which incurs the cost of the
    /// virtual call for each!
    /// @{
    virtual bool fill(const GA_Range &destrange, GA_Offset srci)
    { return fill(destrange, *this, srci); }
    virtual bool fill(const GA_Range &destrange, const GA_Attribute &src, GA_Offset srci);
    /// @}
private:
    const GA_AttributeType	&myType;
    const GA_IndexMap		&myIndexMap;	// For mapping
    UT_StringHolder		 myName;	// Unprefixed name
    GA_AttributeScope		 myScope;	// Public or private
    GA_AttributeOptions		 myOptions;	// Attribute options
    GA_AttributeProxy		*myProxy;	// representative referencer
    int64			 myDataId;
    const GA_Attribute		*myMergeSource;	// Needed to optimize merging
    bool			 myTailInitialize;
    GA_CEAttribute		*myCEAttribute;	// OpenCL Backing.
};

#define GA_GET_AIF_SPECIALIZATION(AIF_NAME) \
    template <> SYS_STATIC_INLINE const GA_##AIF_NAME * \
    GAgetAIF(const GA_Attribute &attrib) { return attrib.get##AIF_NAME(); } \
    /**/
GA_GET_AIF_SPECIALIZATION(AIFCopyData)
GA_GET_AIF_SPECIALIZATION(AIFDelta)
GA_GET_AIF_SPECIALIZATION(AIFTuple)
GA_GET_AIF_SPECIALIZATION(AIFMath)
GA_GET_AIF_SPECIALIZATION(AIFMerge)
GA_GET_AIF_SPECIALIZATION(AIFInterp)
GA_GET_AIF_SPECIALIZATION(AIFBlob)
GA_GET_AIF_SPECIALIZATION(AIFBlobArray)
GA_GET_AIF_SPECIALIZATION(AIFStringTuple)
GA_GET_AIF_SPECIALIZATION(AIFSharedStringTuple)
GA_GET_AIF_SPECIALIZATION(AIFSharedStringArray)
GA_GET_AIF_SPECIALIZATION(AIFIndexPair)
GA_GET_AIF_SPECIALIZATION(AIFBlindData)
GA_GET_AIF_SPECIALIZATION(AIFEdit)
GA_GET_AIF_SPECIALIZATION(AIFCompare)
GA_GET_AIF_SPECIALIZATION(AIFNumericArray)
#undef GA_GET_AIF_SPECIALIZATION

/// @brief Automatically expand attribute data pages for threading
///
/// Normally, threading is allowed only if each thread is guaranteed to write
/// to an individual page of data.  Not all algorithms are amenable to this
/// constraint.  Using the GA_AutoThreadExpand class will force the attribute
/// to harden all its data pages so that multiple threads can write across page
/// boundaries.  Multiple threads are still prohibited from writing to the same
/// offset.  In its destructor, the class will automatically call the compress
/// method to regain memory efficiency.  For example: @code
/// void process(GA_RWAttributeRef &write, GA_ROAttributeRef &read)
/// {
///	GA_AutoHardenForThreading	thread_write(*write.getAttribute());
///	// There's no need to harden the read-only attribute, but now that
///	// we've hardened the write attribute, our threaded algorithm is able
///	// to write data across page boundaries.
///	UTparallelFor(range, functor(write, read));
/// }
/// @endcode
class GA_API GA_AutoHardenForThreading
{
public:
    GA_AutoHardenForThreading(GA_Attribute &attribute,
	    GA_Offset start_offset = GA_Offset(0),
	    GA_Offset end_offset = GA_INVALID_OFFSET)
	: myAttribute(attribute)
	, myStart(start_offset)
	, myEnd(end_offset)
    {
	myAttribute.hardenAllPages(myStart, myEnd);
    }
    ~GA_AutoHardenForThreading()
    {
	myAttribute.tryCompressAllPages(myStart, myEnd);
    }
private:
    GA_Attribute	&myAttribute;
    GA_Offset		 myStart;
    GA_Offset		 myEnd;
};

/// Utility for updating a data id by combining it with another.
/// If 'other' is negative then false is returned without any updates.
/// To use this for combining multiple data id's, initialize id with
/// -1. For example: @code
/// GA_DataId getDataId()
/// {
///     GA_DataId id = -1;
///     if (!mixDataId(id, myDataId0))
///         return -1;
///     if (!mixDataId(id, myDataId1))
///         return -1;
///	    return id;
/// }
/// @endcode
static inline bool
GAcombineDataId(GA_DataId &id, GA_DataId other)
{
    if (other < 0)
	return false;
    if (other > id)
	id = other;
    return true;
}

#endif
