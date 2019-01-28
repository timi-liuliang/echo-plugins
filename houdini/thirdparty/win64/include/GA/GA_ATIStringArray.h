/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ATIStringArray.h ( GA Library, C++)
 *
 * COMMENTS:	String Array ATI (Attribute Type Implementation)
 */

#pragma once

#ifndef __GA_ATIStringArray__
#define __GA_ATIStringArray__

#include "GA_API.h"
#include "GA_ATIBlobArray.h"
#include "GA_AIFSharedStringArray.h"	// For GA_StringIndexType type
#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_VectorTypes.h>

#include <iosfwd>
#include <stddef.h>


class GA_AIFJSON;
class GA_AIFStringArray;
class GA_Attribute;
class GA_AttributeType;
class GA_IndexMap;

class UT_StringArray;
class UT_IStream;
class UT_Options;

class GA_API GA_ATIStringArray : public GA_ATIBlobArray
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
    static GA_ATIStringArray *cast(GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<GA_ATIStringArray *>(attrib);
        return NULL;
    }
    SYS_FORCE_INLINE
    static const GA_ATIStringArray *cast(const GA_Attribute *attrib)
    {
        if (attrib && &attrib->getType() == theAttributeType)
            return static_cast<const GA_ATIStringArray *>(attrib);
        return NULL;
    }

    static GA_Attribute *create(const GA_IndexMap &index_map,
				GA_AttributeScope scope,
				const UT_StringHolder &name,
				const GA_AttributeOptions *attribute_options=NULL);
    static GA_Attribute *create(const GA_IndexMap &index_map,
				const UT_StringHolder &name)
		{ return create(index_map, GA_SCOPE_PUBLIC, name); }

    GA_ATIStringArray(const GA_AttributeType &type,
			const GA_IndexMap &index_map,
			GA_AttributeScope scope,
			const UT_StringHolder &name,
                        int tuple_size);
    virtual ~GA_ATIStringArray();

    // AIFCopyData is implemented in GA_ATIBlob.
    virtual const GA_AIFSharedStringArray *getAIFSharedStringArray() const
					{ return myAIFSharedStringArray; }

    /// @section JSON-GA_ATIStringArray JSON Schema: GA_ATIStringArray
    /// @code
    /// {
    ///     "name"          : "GA_ATIStringArray",
    ///     "description"   : "An tuple array of indexed strings",
    ///     "type"          : "orderedmap",
    ///     "properties": {
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

    /// @{
    /// Get a string
    void		 getString(UT_StringArray &strings, GA_Offset offset) const;
    void		 getString(UT_Array<UT_StringHolder> &strings, GA_Offset offset) const;
    void		 getStringIndex(UT_Array<GA_StringIndexType> &indices, GA_Offset offset) const;
    /// @}
    /// @{
    /// Set a string
    void		 setString(GA_Offset offset, const UT_StringArray &strings);
    void		 setString(GA_Offset offset, const UT_Array<UT_StringHolder> &strings);
    void		 setStringIndex(GA_Offset offset, const UT_Array<GA_StringIndexType> &indices);
    /// @}

protected:
    // Create a new ATIBlob
    virtual GA_Attribute *doClone(const GA_IndexMap &index_map,
				  const UT_StringHolder &name) const;
private:
    const char		*lookupString(GA_StringIndexType handle) const;
    const char		*lookupOrderedString(exint handle) const;
    GA_StringIndexType	 lookupHandle(const char *s) const;
    GA_StringIndexType	 validateHandle(GA_StringIndexType h) const;
    bool		 replaceString(GA_StringIndexType handle,
				    const char *string);

    /// @{ GA_AIFStringTuple
    /// @warning If you call addStringReference(), at some point, you must call
    /// delHandleReference() on the string you added.
    GA_StringIndexType	addStringReference(const char *str);
    void	delHandleReference(GA_StringIndexType handle);
    bool	tupleGet(GA_Offset di, GA_StringIndexType &v, int vi) const;
    bool	tupleSet(GA_Offset di, GA_StringIndexType v, int vi);
    /// @}

    static GA_AIFSharedStringArray	*myAIFSharedStringArray;
    static GA_AIFJSON			*myAIFJSON;

    static const GA_AttributeType *theAttributeType;

    /// @cond INTERNAL_DOX
    friend class ga_StringArray;
    friend class ga_StringJSON;
    friend class GA_ATIStringArrayDelayedWriter;
    /// @endcond
};

//
// Designed to delay all the reference count adjustments
// until this object is flushed.  Also maintains a local
// table for looking up the string indices with, thus avoiding
// concurrency issues.
//
class GA_API GA_ATIStringArrayDelayedWriter
{
public:
    GA_ATIStringArrayDelayedWriter();
    GA_ATIStringArrayDelayedWriter(GA_ATIStringArray *attrib);
    ~GA_ATIStringArrayDelayedWriter();

    void		bind(GA_ATIStringArray *attrib);


    // The index cache is provided to avoid having to potentially resize
    // this inside this function.
    void		setString(GA_Offset offset, const UT_StringArray &strings, UT_Array<GA_StringIndexType> &indexcache);
    void		setString(GA_Offset offset, const UT_Array<UT_StringHolder> &strings, UT_Array<GA_StringIndexType> &indexcache);
    // Directly set the index.
    void		setStringIndex(GA_Offset offset, const UT_Array<GA_StringIndexType> &idx);

    void		flush();

    SYS_FORCE_INLINE bool hasChanges() const
    {
        return myHasChanges;
    }

private:
    void		setString(GA_Offset offset, const UT_StringHolder *strings, exint n, UT_Array<GA_StringIndexType> &indexcache);

    GA_ATIStringArray *myAttribute;
    volatile bool myHasChanges;

    UT_Map<GA_StringIndexType, exint>	myRefCountChanges;
    UT_StringMap<GA_StringIndexType>	myStringToIndex;
};

// NOTE: It may seem strange to define GA_ATIBlobArray::isType() and cast()
//       in GA_ATIStringArray.h, but GA_ATIBlobArray::isType() and cast() need
//       GA_ATIStringArray::isType(), and GA_ATIStringArray inherits from GA_ATIBlobArray,
//       so these are defined here, and GA_ATIStringArray.h is included
//       in GA_ATIBlobArray.h, below the class.
bool
GA_ATIBlobArray::isType(const GA_Attribute *attrib)
{
    return attrib && (&attrib->getType() == theAttributeType || GA_ATIStringArray::isType(attrib));
}
GA_ATIBlobArray *
GA_ATIBlobArray::cast(GA_Attribute *attrib)
{
    if (attrib && (&attrib->getType() == theAttributeType || GA_ATIStringArray::isType(attrib)))
        return static_cast<GA_ATIBlobArray *>(attrib);
    return NULL;
}
const GA_ATIBlobArray *
GA_ATIBlobArray::cast(const GA_Attribute *attrib)
{
    if (attrib && (&attrib->getType() == theAttributeType || GA_ATIStringArray::isType(attrib)))
        return static_cast<const GA_ATIBlobArray *>(attrib);
    return NULL;
}

#endif
