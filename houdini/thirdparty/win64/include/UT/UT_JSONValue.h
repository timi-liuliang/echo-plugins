/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONValue.h ( UT Library, C++)
 *
 * COMMENTS:	This class represents a JSON object.
 */

#ifndef __UT_JSONValue__
#define __UT_JSONValue__

#include "UT_API.h"
#include "UT_ValArray.h"
#include "UT_SymbolTable.h"
#include "UT_VectorTypes.h"
#include "UT_JSONValueMap.h"
#include "UT_JSONValueArray.h"
#include "UT_StringArray.h"
#include "UT_StringHolder.h"
#include "UT_Compression.h"
#include <SYS/SYS_Inline.h>

class UT_JSONValueArray;
class UT_JSONValueMap;
class UT_JSONParser;
class UT_JSONWriter;
class UT_IStream;
class UT_IStream;
class UT_WorkBuffer;

/// @brief Class to store JSON objects as C++ objects
///
/// This class is able to represent a JSON object in its entirety.  Arrays and
/// Maps store arrays and maps of JSON values.
///
/// There are methods to load a JSONValue from a UT_JSONParser and to save to a
/// UT_JSONWriter object.
///
/// To load an entire JSON file into memory, you can do something like
/// @code
///     UT_JSONIStream *is(utistream);
///     UT_JSONParser parser;
///     UT_JSONValue value;
///     if (!value.load(parser, *is))
///             reportFailure(parser.getErrors());
/// @endcode
///
/// To save a value:
/// @code
///     UT_JSONWriterSubclass    &os;
///     value.save(os);
/// @endcode
///
/// Alternatively, it's possible to use the UT_JSONValue class from within
/// other UT_JSONHandle classes.  For example, when loading key/value pairs for
/// an arbitrary map:
/// @code
///     bool
///     my_handle::jsonKey(UT_JSONParser &parser, const char *token, int64)
///     {
///          UT_String          keyword;
///          UT_JSONValue       value;
///          keyword.harden(token);
///          if (!value.load(parser))
///             return false;
///          process(keyword, value);
///          return true;
///     }
/// @endcode
/// @see UT_JSONParser, UT_JSONWriter, UT_JSONHandle, UT_JSONValueArray,
/// UT_JSONValueMap

class UT_API UT_JSONValue {
public:
    /// Types held in the UT_JSONValue.  The number type has been broken out
    /// into an integer and a real.
    enum Type {
	JSON_NULL,	// Null
	JSON_BOOL,	// Bool
	JSON_INT,	// Integer
	JSON_REAL,	// Real
	JSON_STRING,	// String value
	JSON_KEY,	// Key value
	JSON_ARRAY,	// An array of UT_JSON_VALUE
	JSON_MAP,	// A map of keyword, UT_JSONValue object
    };

     UT_JSONValue();
     explicit UT_JSONValue(bool value);
     explicit UT_JSONValue(int64 value);
     explicit UT_JSONValue(fpreal64 value);
     UT_JSONValue(const char *string, int64 length=-1);
     UT_JSONValue(const UT_StringHolder &s);
     UT_JSONValue(const UT_JSONValue &v);
    ~UT_JSONValue();

    UT_JSONValue	&operator=(const UT_JSONValue &v)
			 {
			     copyFrom(v);
			     return *this;
			 }

    /// Read the next value from the parser and store it in this object
    /// @param parser	The parser
    ///	@param is	If specified, data will be read from this stream.
    ///	Otherwise, the stream associated with the parser will be used.
    bool	 parseValue(UT_JSONParser &parser,
				UT_IStream *is = 0,
				bool record_source_offsets = false);

    /// Loads an entire JSON file into this value.
    bool	 loadFromFile(const char* file_name,
				bool record_source_offsets = false);

    /// Saves an entire JSON value to the specified file.
    bool	 saveToFile(const char* file_name,
				bool binary = false,
				UT_CompressionType compressionType =
				    UT_COMPRESSION_TYPE_NONE);

    /// Save the object the output stream
    bool		 save(UT_JSONWriter &os) const;

    /// Dump the value to stdout (same as save to cout)
    void		 dump() const;

    /// Get the type of data stored in the object
    Type		 getType() const	{ return myType; }

    /// Get the bool value.  Interprets integer/float as bool
    bool		 getB() const;
    /// Get the integer value.  Intereprets bool/real as integer
    /// @note Real values are cast to integer.
    int64		 getI() const;
    /// Get the real value.  Interprets bool/int as reals.
    fpreal64		 getF() const;

    /// Get the string value (may return a NULL pointer)
    const char		*getS() const;
    /// Return the string length (returns -1 if not a string)
    int64		 getSLength() const;
    /// Return the string value.
    const UT_StringHolder *getStringHolder() const;

    /// Get a key value
    const char		*getKey() const;
    /// Get the length of the key
    int64		 getKeyLength() const;
    /// Return a string holder for the key.
    const UT_StringHolder *getKeyHolder() const;

    /// Get the array value (may return a NULL pointer)
    UT_JSONValueArray	*getArray() const;
    /// Get the map value (may return a NULL pointer)
    UT_JSONValueMap	*getMap() const;

    /// Get the "uniform" type of an array.  This will return:
    ///  - JSON_NULL:  The array is heterogeneous (or not an array)
    ///  - JSON_BOOL:  The array is only bool values
    ///  - JSON_INT:   The array is all bool or int values
    ///  - JSON_REAL:  The array is bool, int or real values
    ///  - JSON_STRING: The array is all strings
    Type		 getUniformArrayType() const;

    /// Extract a bool (returns false if type is invalid)
    SYS_FORCE_INLINE bool import(bool &result) const
    {
	switch (myType)
	{
	    case JSON_BOOL:
		result = myData.myBool;
		return true;
	    case JSON_INT:
		result = myData.myInt != 0;
		return true;
	    case JSON_REAL:
		result = myData.myReal != 0;
		return true;
	    default:
		break;
	}
	return false;
    }
    /// Extract an integer (returns false if type is invalid)
    SYS_FORCE_INLINE bool import(int64 &result) const
    {
	switch (myType)
	{
	    case JSON_BOOL:
		result = myData.myBool ? 1 : 0;
		return true;
	    case JSON_INT:
		result = myData.myInt;
		return true;
	    case JSON_REAL:
		result = (int64)myData.myReal;
		return true;
	    default:
		break;
	}
	return false;
    }
    /// Extract an float (returns false if type is invalid)
    SYS_FORCE_INLINE bool import(fpreal64 &result) const
    {
	switch (myType)
	{
	    case JSON_BOOL:
		result = myData.myBool ? 1. : 0.;
		return true;
	    case JSON_INT:
		result = myData.myInt;
		return true;
	    case JSON_REAL:
		result = myData.myReal;
		return true;
	    default:
		break;
	}
	return false;
    }
    /// Extract a string @b or key (returns false if type is invalid)
    bool		 import(UT_WorkBuffer &result) const;
    bool		 import(UT_StringHolder &result) const;
    /// Extract a tuple of integers from an JSON_Array.  If there aren't enough
    /// elements in the array, this method fails
    bool		 import(int64 *result, int size) const;
    /// Extract a tuple of floats from an JSON_Array.  If there aren't enough
    /// elements in the array, this method fails
    bool		 import(fpreal64 *result, int size) const;
    /// Extract a vector of integers from a JSON_Array
    bool		 import(UT_Array<int64> &array) const;
    /// Extract a vector of reals from a JSON_Array
    bool		 import(UT_Array<fpreal64> &array) const;
    /// Extract a vector of strings from a JSON_Array
    bool		 import(UT_StringArray &array) const;

    /// Returns whether the value can be interpreted as a number
    bool	 isNumber() const
		 {
		    return  myType == JSON_REAL || myType == JSON_INT ||
			    myType == JSON_BOOL;
		 }

    /// Set value to a null
    void	 setNull()	{ clearValue(); }
    /// Set value to an bool
    void	 setBool(bool v);
    /// Set value to an int
    void	 setInt(int64 v);
    /// Set value to an int
    void	 setReal(fpreal64 v);
    /// Set string.  If the length is not specified, the length of the string
    /// will be used.
    void	 setString(const char *s, int64 length=-1)
		    { setStringType(s, length, JSON_STRING); }
    void	 setString(const UT_StringHolder &s)
		    { setStringType(s, JSON_STRING); }
    /// Set string.  If the length is not specified, the length of the string
    /// will be used.
    void	 setKey(const char *s, int64 length=-1)
		    { setStringType(s, length, JSON_KEY); }
    void	 setKey(const UT_StringHolder &s)
		    { setStringType(s, JSON_KEY); }
    /// Set value to the array
    void	 setArray(UT_JSONValueArray *array);
    /// Set value to the map
    void	 setMap(UT_JSONValueMap *map);

    /// Start building an array from scratch. @see appendArray
    void	 startArray();
    /// Add an element to an array (returns false if operation fails)
    bool	 appendArray(const UT_JSONValue &v);

    /// Start building a map from scratch . @see appendMap
    void	 startMap();
    /// Add an element to a map (returns false if operation fails)
    bool	 appendMap(const UT_StringHolder &key, const UT_JSONValue &v);

    /// Adds a new map child to this value. The current value must
    /// be either an array or a map. If this value is an array,
    /// the map_key parameter is ignored. This is due to overly 
    /// convoluted JSON syntax design.
    UT_JSONValueMap* addMapChild(const UT_StringHolder &map_key);

    /// Adds a new array child to this value. The current value must
    /// be either an array or a map. If this value is an array,
    /// the map_key parameter is ignored. This is due to overly 
    /// convoluted JSON syntax design.
    UT_JSONValueArray* addArrayChild(const UT_StringHolder &map_key);

    /// @{
    /// Build a uniform array of values
    bool	 setUniformArray(int nvalues, const int32 *data);
    bool	 setUniformArray(int nvalues, const int64 *data);
    bool	 setUniformArray(int nvalues, const fpreal16 *data);
    bool	 setUniformArray(int nvalues, const fpreal32 *data);
    bool	 setUniformArray(int nvalues, const fpreal64 *data);
    bool	 setUniformArray(int nvalues, const UT_StringHolder *data);
    /// @}

    /// Used internally by UT_JSONValueMap
    int			 getMapIndex() const
			 { return myMapIndex; }
    /// Used internally by UT_JSONValueMap
    void		 setMapIndex(int i)
			 { myMapIndex = i; }

    /// Used by UT_JSONValue parsing from a stream
    int			 getSourceLine() const
			 { return mySourceLine; }
    int			 getSourceOffset() const
			 { return mySourceOffset; }
    void		 setSourceOffset(int line, int offset)
			 { mySourceLine = line; mySourceOffset = offset; }

    /// @brief Traverse the children of the current value.
    ///
    /// This class will iterate over all items in this value, whether they
    /// are in the form of an array or a map. For values with neither,
    /// it will return the current value itself.
    /// Note that the getKey() function is only available when this
    /// value is a map.
    ///
    /// Example:
    /// @code
    /// UT_JSONValue::traverser	it;
    /// for (it = value.beginTraversal(); !it.atEnd(); ++it)
    /// @endcode
    class UT_API traverser
    {
	public:
	    traverser()
		: myValue(NULL),
		  myIsValid(false)
	    {
		myCurrArrayPos = myNumArrayEntries = 0;
	    }
	    traverser(const traverser &src)
	    {
		*this = src;
	    }
	    ~traverser() {}

	    const traverser	&operator=(const traverser &src)
			{
			    myValue = src.myValue;
			    myIsValid = src.myIsValid;			    
			    myCurrArrayPos = src.myCurrArrayPos; 
			    myNumArrayEntries = src.myNumArrayEntries;
			    myValueType =  src.myValueType;
			    myMapKeys = src.myMapKeys;
			    return *this;
			}

	    /// ++iterator
	    traverser	&operator++()    { advance(); return *this; }
	    /// No post increment as it is harmful.

	    bool	atEnd() const	{ return !myIsValid; }
	    void	advance()
			{
			    if(myValue && myIsValid)
			    {
				if(myValueType == JSON_MAP)
				{
				    myCurrArrayPos++;
				    if(myCurrArrayPos >= myMapKeys.entries())
					myIsValid = false;
				}
				else if(myValueType == JSON_ARRAY)
				{
				    myCurrArrayPos++;
				    if(myCurrArrayPos >= myNumArrayEntries)
					myIsValid = false;
				}
				else
				    // Any non-array, non-map values only
				    // have one item.
				    myIsValid = false;

			    }
			}

	    /// Returns the child value which corresponds to the current 
	    /// traversal position.
	    UT_JSONValue	*getValue()
	    {
		if(myValue && myIsValid)
		{
		    if(myValueType == JSON_MAP)
			return myValue->getMap()->get(myMapKeys(myCurrArrayPos).buffer());
		    else if(myValueType == JSON_ARRAY)
			return myValue->getArray()->get(myCurrArrayPos);
		    else
			return NULL;
		}
		return NULL;
	    }

	    /// Deprecated in favour of UT_StringHolder method
	    SYS_DEPRECATED(14.0) bool getKey(UT_String &key)
	    {
		UT_StringHolder	tmp;
		if (!getKey(tmp))
		    return false;
		key = tmp;
		return true;
	    }
	    /// Deprecated in favour of UT_StringHolder method
	    SYS_DEPRECATED(14.0) bool getLowerCaseKey(UT_String &key)
	    {
		UT_StringHolder	tmp;
		if (!getLowerCaseKey(tmp))
		    return false;
		key = tmp;
		return true;
	    }

	    bool getKey(UT_StringHolder &key)
	    {
		if(myValue && myIsValid)
		{
		    if(myValueType == JSON_MAP)
		    {
			key = myMapKeys(myCurrArrayPos);
			return true;
		    }
		    else
		    {
			key.clear();
		    }
		}
		else
		{
		    key.clear();
		}
		return false;
	    }

	    /// Get a lower case map key (for case insensitive maps)
	    bool	getLowerCaseKey(UT_StringHolder &key)
			{
			    if (getKey(key))
			    {
				key = key.toLower();
				return true;
			    }
			    return false;
			}

	private:
	     traverser(UT_JSONValue *value_in)
		 : myValue(value_in),
		   myIsValid(false)
	     {
		 myCurrArrayPos = 0;
		 myNumArrayEntries = 0;
		 if (value_in)
		 {
		    // See whether this map is a map or an array:
		    myValueType = myValue->getType();  
		    
		    // Check for an empty array or map.
		    myIsValid = true;
		    if(myValueType == JSON_MAP)
		    {	
			if(myValue->getMap()->entries() == 0)			
			    myIsValid = false;
			else
			    myValue->getMap()->getKeys(myMapKeys);
		    }
		    else if(myValueType == JSON_ARRAY)
		    {
			if(myValue->getArray()->entries() == 0)
			    myIsValid = false;
			else
			    myNumArrayEntries = myValue->getArray()->entries();
		    }
		    else
			myIsValid = false;
		 }
	     }

	    UT_JSONValue	*myValue;
	    UT_JSONValue::Type	 myValueType;
	    UT_StringArray	 myMapKeys;
	    int64		 myCurrArrayPos, myNumArrayEntries;
	    bool		 myIsValid;

	    friend class UT_JSONValue;
    };

    traverser	beginTraversal()	{ return traverser(this); }

    /// Searches all children of this value recursively for another value
    /// with a child that contains the given map key set to the given value.
    UT_JSONValue* findValueRecursive(const UT_StringHolder &map_key,
			const char *key_value);
    UT_JSONValue* findParentValueRecursive(const UT_StringHolder &map_key,
			const char *key_value);

    /// Removes NULL values from all arrays recursively.
    /// Note that maps are currently unsupported, and null values will
    /// not be removed from them.
    void removeNullValuesRecursive();

private:
    void		 clearValue();
    void		 copyFrom(const UT_JSONValue &s);
    void		 setStringType(const char *s, int64 l, Type t);
    void		 setStringType(const UT_StringHolder &s, Type t);
    union {
	bool		 myBool;
	int64		 myInt;
	fpreal64	 myReal;
    } myData;
    union {
	// NOTE:  Keep the pointers and scalars separate.  The string is stored
	// as a pointer and a length so that null characters may be contained.
	UT_JSONValueArray	*myArray;
	UT_JSONValueMap		*myMap;
    } myPointers;
    UT_StringHolder	 myString;
    int			 myMapIndex;		// Location in a map
    int			 mySourceLine;		// Line in source stream
    int			 mySourceOffset;	// Offset in source line
    Type		 myType;
};

#endif

