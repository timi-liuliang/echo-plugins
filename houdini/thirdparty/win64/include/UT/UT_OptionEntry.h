/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OptionEntry.h (C++, Utility Library)
 *
 * COMMENTS:	Individual options held within a UT_Options class.
 *
 */

#ifndef _UT_OPTIONENTRY_H_
#define _UT_OPTIONENTRY_H_

#include "UT_API.h"
#include "UT_SmallObject.h"
#include "UT_Assert.h"
#include "UT_IntArray.h"
#include "UT_StringArray.h"
#include "UT_StringHolder.h"
#include "UT_VectorTypes.h"

class UT_JSONValue;
class UT_JSONValueMap;
class UT_JSONWriter;

// NOTE: The values of UT_OptionType are saved to disk, thus we should NOT 
//       reorder these options if we want to maintain compatibility with older
//	 saved options.
enum UT_OptionType
{
    UT_OPTION_INVALID = -1,
    UT_OPTION_INT = 0,
    UT_OPTION_BOOL = 1,
    UT_OPTION_FPREAL = 2,
    UT_OPTION_STRING = 3,
    UT_OPTION_VECTOR2 = 4,
    UT_OPTION_VECTOR3 = 5,
    UT_OPTION_VECTOR4 = 6,
    UT_OPTION_QUATERNION = 7,
    UT_OPTION_MATRIX3 = 8,
    UT_OPTION_MATRIX4 = 9,
    UT_OPTION_UV = 10,
    UT_OPTION_UVW = 11,
    UT_OPTION_STRINGRAW = 12,	// used by getPyOptionString
    UT_OPTION_INTARRAY = 13,	// Array of integers
    UT_OPTION_FPREALARRAY = 14,	// Array of float/double
    UT_OPTION_MATRIX2 = 15,
    UT_OPTION_STRINGARRAY = 16,

    UT_OPTION_NUM_TYPES	// Sentinal
};

/// String formatting options for getOptionString
enum UT_OptionFormat
{
    UT_OPTFMT_DISPLAY,		// Format for display
    UT_OPTFMT_PYTHON,		// Format for python
    UT_OPTFMT_VEX,		// Format for VEX arguments
    UT_OPTFMT_NUM_TYPES
};

// The base class for data held by the UT_Options.
class UT_API UT_OptionEntry : public UT_SmallObject<UT_OptionEntry,
					UT_SMALLOBJECT_CLEANPAGES_DEFAULT,
                                        UT_SMALLOBJECT_PAGESIZE_DEFAULT,
					UT_SMALLOBJECT_THREADSAFE_ON>
{
public:
				 UT_OptionEntry()
				 { }
    virtual			~UT_OptionEntry()
				 { }

    virtual UT_OptionType	 getType() const
				 { return UT_OPTION_INVALID; }
    virtual UT_OptionEntry	*clone() const
				 { return new UT_OptionEntry(); }
    virtual int64		 getMemoryUsage(bool inclusive) const
				 { return inclusive ? sizeof(this) : 0; }

    /// Hash a single option entry.
    unsigned			 hash() const;

    /// Compute equivalence for a given option entry
    unsigned			 isEqual(const UT_OptionEntry &cmp,
					fpreal64 tolerance = 0.0) const;

    bool			 getOptionString(UT_OptionFormat format,
					 UT_WorkBuffer &result) const;
    bool			 getOptionString(UT_OptionFormat format,
					 UT_String &result) const;

    enum UT_OptionTypeStorage
    {
	UT_OPTION_STORE_INVALID=-1,
	UT_OPTION_STORE_BOOL,
	UT_OPTION_STORE_INT,
	UT_OPTION_STORE_REAL,
	UT_OPTION_STORE_STRING,
    };

    /// Return the underlying storage type for the option
    static UT_OptionTypeStorage	 optionStorage(UT_OptionType t)
    {
	switch (t)
	{
	    case UT_OPTION_INT:
	    case UT_OPTION_INTARRAY:
		return UT_OPTION_STORE_INT;

	    case UT_OPTION_FPREAL:
	    case UT_OPTION_VECTOR2:
	    case UT_OPTION_VECTOR3:
	    case UT_OPTION_VECTOR4:
	    case UT_OPTION_QUATERNION:
	    case UT_OPTION_MATRIX3:
	    case UT_OPTION_MATRIX4:
	    case UT_OPTION_UV:
	    case UT_OPTION_UVW:
	    case UT_OPTION_FPREALARRAY:
	    case UT_OPTION_MATRIX2:
		return UT_OPTION_STORE_REAL;

	    case UT_OPTION_STRING:
	    case UT_OPTION_STRINGRAW:
	    case UT_OPTION_STRINGARRAY:
		return UT_OPTION_STORE_STRING;

	    case UT_OPTION_BOOL:
		return UT_OPTION_STORE_BOOL;

	    case UT_OPTION_INVALID:
	    case UT_OPTION_NUM_TYPES:
		return UT_OPTION_STORE_INVALID;
	}
	UT_ASSERT(0);
	return UT_OPTION_STORE_INVALID;
    }
    /// Returns the tuple size, or 0 if the option is variadic (i.e. INTARRAY)
    static exint	optionSize(UT_OptionType t)
    {
	switch (t)
	{
	    case UT_OPTION_INT:
	    case UT_OPTION_FPREAL:
	    case UT_OPTION_STRING:
	    case UT_OPTION_STRINGRAW:
	    case UT_OPTION_BOOL:
		return 1;

	    case UT_OPTION_VECTOR2:
	    case UT_OPTION_UV:
		return 2;

	    case UT_OPTION_VECTOR3:
	    case UT_OPTION_UVW:
		return 3;

	    case UT_OPTION_VECTOR4:
	    case UT_OPTION_MATRIX2:
	    case UT_OPTION_QUATERNION:
		return 4;
	    case UT_OPTION_MATRIX3:
		return 9;
	    case UT_OPTION_MATRIX4:
		return 16;

	    case UT_OPTION_INTARRAY:
	    case UT_OPTION_FPREALARRAY:
	    case UT_OPTION_STRINGARRAY:
		return 0;

	    case UT_OPTION_INVALID:
	    case UT_OPTION_NUM_TYPES:
		return -1;
	}
	UT_ASSERT(0);
	return -1;
    }
    UT_OptionTypeStorage	 optionStorage() const
				    { return optionStorage(getType()); }
    exint			 optionSize() const
				    { return optionSize(getType()); }

    bool			 importOption(int64 &value) const;
    bool			 importOption(bool &value) const;
    bool			 importOption(fpreal64 &value) const;
    bool			 importOption(UT_StringHolder &value) const;
    bool			 importOption(UT_Vector2D &value) const;
    bool			 importOption(UT_Vector3D &value) const;
    bool			 importOption(UT_Vector4D &value) const;
    bool			 importOption(UT_QuaternionD &value) const;
    bool			 importOption(UT_Matrix2D &value) const;
    bool			 importOption(UT_Matrix3D &value) const;
    bool			 importOption(UT_Matrix4D &value) const;

    // Generic import which handles arbitrary length int/float/string data
    bool			 importOption(UT_Array<int32> &value) const;
    bool			 importOption(UT_Array<int64> &value) const;
    bool			 importOption(UT_Array<fpreal32> &value) const;
    bool			 importOption(UT_Array<fpreal64> &value) const;
    bool			 importOption(UT_StringArray &value) const;

    // Returns the values directly, or a default value if the data is an
    // inappropriate type.
    int64			 getOptionI() const;
    bool			 getOptionB() const;
    fpreal64			 getOptionF() const;
    const UT_Vector2D		&getOptionV2() const;
    const UT_Vector3D		&getOptionV3() const;
    const UT_Vector4D		&getOptionV4() const;
    const UT_QuaternionD	&getOptionQ() const;
    const UT_Matrix2D		&getOptionM2() const;
    const UT_Matrix3D		&getOptionM3() const;
    const UT_Matrix4D		&getOptionM4() const;
    const UT_Vector2D		&getOptionUV() const;
    const UT_Vector3D		&getOptionUVW() const;

    // Note that, if the string value contains binary data with null
    // characters, the UT_String version of getOptionS will not be able to
    // tell you the length of the data, since UT_String doesn't store the
    // length.
    UT_StringHolder		 getOptionS() const;
    const UT_Int64Array		&getOptionIArray() const;
    const UT_Fpreal64Array	&getOptionFArray() const;
    const UT_StringArray	&getOptionSArray() const;

    /// Save a single option to a JSON stream.  This is saved with the schema
    /// @code
    /// {
    ///     "name":"UT_OptionEntry",
    ///     "type":"object",
    ///     "properties": {
    ///        "type":{
    ///           "type":"string",
    ///           "enum":{ UT_Options::getStringFromOptionType() },
    ///        },
    ///        "value":{
    ///            "type":["bool", "integer", "float", "string", "array"],
    ///            "items":{"type":"float"},
    ///            "description":"Basic type or an array.
    ///                           Arrays must be 2,3,4,9 or 16 floats.
    ///                           Data should match given type",
    ///        },
    ///     },
    /// }
    /// @endcode
    bool			 saveJSON(UT_JSONWriter &w, bool compact) const;
    /// As above, except save to a UT_JSONValue instead of requiring the value
    /// go directly to a stream.
    void			 saveJSON(UT_JSONValue &v, bool compact) const;

    /// Create a UT_OptionEntry from a simple JSON value. This is not a
    /// dictionary with type information. It is a single value which may be
    /// an int, float, string, etc. The UT_OptionEntry type is inferred from
    /// the JSON type.
    static UT_OptionEntry	*createEntry(const UT_JSONValue &v);
    static UT_OptionEntry	*createEntry(const UT_JSONValueMap &map,
					UT_String &error_str);

    /// Convert string to and from UT_OptionType values.
    static UT_OptionType getOptionTypeFromString(const char *optiontypestr);
    static const char	*getStringFromOptionType(UT_OptionType optiontype);
};

// A templatized version of the class above for holding specific data types.
template<class T, UT_OptionType ID>
class UT_OptionEntryImpl : public UT_OptionEntry
{
public:
    typedef T			 value_type;

				 UT_OptionEntryImpl(const T &value)
				     : myValue(value)
				 { }
    virtual			~UT_OptionEntryImpl()
				 { }

    const T			&getValue() const
				 { return myValue; }
    virtual UT_OptionType	 getType() const
				 { return ID; }
    virtual UT_OptionEntry	*clone() const
				 { return new UT_OptionEntryImpl(myValue); }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

protected:
    T				 myValue;
};

typedef UT_OptionEntryImpl<int64, UT_OPTION_INT> UT_OptionInt;
typedef UT_OptionEntryImpl<bool, UT_OPTION_BOOL> UT_OptionBool;
typedef UT_OptionEntryImpl<fpreal64, UT_OPTION_FPREAL> UT_OptionFpreal;
typedef UT_OptionEntryImpl<UT_Vector2D, UT_OPTION_VECTOR2> UT_OptionVector2;
typedef UT_OptionEntryImpl<UT_Vector3D, UT_OPTION_VECTOR3> UT_OptionVector3;
typedef UT_OptionEntryImpl<UT_Vector4D, UT_OPTION_VECTOR4> UT_OptionVector4;
typedef UT_OptionEntryImpl<UT_QuaternionD, UT_OPTION_QUATERNION> UT_OptionQuaternion;
typedef UT_OptionEntryImpl<UT_Matrix2D, UT_OPTION_MATRIX2> UT_OptionMatrix2;
typedef UT_OptionEntryImpl<UT_Matrix3D, UT_OPTION_MATRIX3> UT_OptionMatrix3;
typedef UT_OptionEntryImpl<UT_Matrix4D, UT_OPTION_MATRIX4> UT_OptionMatrix4;
typedef UT_OptionEntryImpl<UT_Vector2D, UT_OPTION_UV> UT_OptionUV;
typedef UT_OptionEntryImpl<UT_Vector3D, UT_OPTION_UVW> UT_OptionUVW;

class UT_OptionString :
    public UT_OptionEntryImpl<UT_StringHolder, UT_OPTION_STRING>
{
public:
    typedef UT_OptionEntryImpl<UT_StringHolder, UT_OPTION_STRING> Base;

    UT_OptionString(const UT_StringHolder &value) : Base(value) {}

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myValue.getMemoryUsage(inclusive);
        return mem;
    }
};

class UT_OptionInt64Array :
    public UT_OptionEntryImpl<UT_Int64Array, UT_OPTION_INTARRAY>
{
public:
    typedef UT_OptionEntryImpl<UT_Int64Array, UT_OPTION_INTARRAY> Base;

    UT_OptionInt64Array(const UT_Int64Array &value)
	: Base(value) {}
    UT_OptionInt64Array(const UT_Array<int64> &value)
	: Base(UT_Int64Array())
    {
	myValue = value;
    }
    UT_OptionInt64Array(const UT_Array<int32> &value)
	: Base(UT_Int64Array())
    {
	myValue.setSizeNoInit(value.entries());
	for (exint i = 0; i < value.entries(); ++i)
	    myValue(i) = value(i);
    }

    void fetchValue(UT_Int64Array &value) const { value = myValue; }
    void fetchValue(UT_IntArray &value) const
    {
	value.setSizeNoInit(myValue.entries());
	for (exint i = 0; i < myValue.entries(); ++i)
	    value(i) = myValue(i);
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myValue.getMemoryUsage(false);
        return mem;
    }
};

class UT_OptionFpreal64Array :
    public UT_OptionEntryImpl<UT_Fpreal64Array, UT_OPTION_FPREALARRAY>
{
public:
    typedef UT_OptionEntryImpl<UT_Fpreal64Array, UT_OPTION_FPREALARRAY> Base;

    UT_OptionFpreal64Array(const UT_Fpreal64Array &value)
	: Base(value) {}
    UT_OptionFpreal64Array(const UT_Array<fpreal64> &value)
	: Base(UT_Fpreal64Array())
    {
	myValue = value;
    }
    UT_OptionFpreal64Array(const UT_Array<fpreal32> &value)
	: Base(UT_Fpreal64Array())
    {
	myValue.setSizeNoInit(value.entries());
	for (exint i = 0; i < value.entries(); ++i)
	    myValue(i) = value(i);
    }

    void fetchValue(UT_Fpreal64Array &value) const { value = myValue; }
    void fetchValue(UT_Fpreal32Array &value) const
    {
	value.setSizeNoInit(myValue.entries());
	for (exint i = 0; i < myValue.entries(); ++i)
	    value(i) = myValue(i);
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myValue.getMemoryUsage(false);
        return mem;
    }
};

class UT_OptionStringArray :
    public UT_OptionEntryImpl<UT_StringArray, UT_OPTION_STRINGARRAY>
{
public:
    typedef UT_OptionEntryImpl<UT_StringArray, UT_OPTION_STRINGARRAY> Base;

    UT_OptionStringArray(const UT_StringArray &value) : Base(value) {}
    UT_OptionStringArray(const UT_Array<UT_StringHolder> &value)
	: Base(UT_StringArray())
    {
	myValue.setSize(value.entries());
	for (exint i = 0; i < value.entries(); ++i)
	    myValue(i) = value(i);
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myValue.getMemoryUsage(false);
        return mem;
    }
};

// UT_OptionStringRaw is the same as UT_OptionString, but it returns a getType
// value tagging it as a "raw" string so that getPyOptionString() and
// appendPyDictionary() won't add quotes around the value.
class UT_OptionStringRaw : public UT_OptionString
{
public:
    UT_OptionStringRaw(const UT_StringHolder &value) : UT_OptionString(value) {}

    virtual UT_OptionType   getType() const { return UT_OPTION_STRINGRAW; }
};

#endif
