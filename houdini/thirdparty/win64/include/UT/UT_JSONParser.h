/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONParser.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_JSONParser__
#define __UT_JSONParser__

#include "UT_API.h"
#include "UT_IntArray.h"
#include "UT_JSONDefines.h"
#include "UT_Map.h"
#include "UT_StackBuffer.h"
#include "UT_StringHolder.h"
#include "UT_StringArray.h"
#include "UT_ValArray.h"
#include "UT_WorkBuffer.h"

#include <SYS/fpreal16Limits.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

#include <limits>
#include <streambuf>


class UT_BitArray;
class UT_IStream;
class UT_JSONHandle;
class UT_JSONValue;

class ut_JValue;
class ut_SimpleJSONObjectHandle;


/// @brief JSON reader class which handles parsing of JSON or bJSON files
///
/// The JSON format (http://www.json.org) is a generic way to store data.
/// Houdini extends JSON to a byte-stream binary encoding.  This class is used
/// to parse JSON (binary or ASCII) and provides callbacks to handle the data.
///
/// For binary JSON files, Houdini has extended JSON slightly to support
/// compact versions for specific data representations.  The @c ua methods
/// callback methods on the JSONHandle are used to process a uniform array of
/// simple types.  Data for the uniform array can be read directly from the
/// stream.  Care must be taken for uaBool() and uaString() which encode the
/// array data.  uaReadString() can be used to read a string from the data
/// stream.  The uniform array of uaBool encodes the bits into 32-bit words.
///
/// The standard implementation of the uaInt16() method is:
/// @code
/// bool
/// UT_JSONParser::uaInt16(int64 length) {
///     int16     value;
///     if (!jsonBeginArray())
///          return false;
///     for (int64 i=0; i < length; i++) {
///          if (!read(&value, sizeof(int16)))
///               return false;
///          if (getSwapFlag())
///               UTswapBytes(&value, 1);
///          if (!jsonInt(value))
///               return false;
///     }
///     return jsonEndArray();
/// }
/// @endcode
///
/// @see UT_JID, UT_JSONHandle
class UT_API UT_JSONParser
{
public:
    class UT_API iterator;

     UT_JSONParser();
    ~UT_JSONParser();

    /// Seek the parser to a specific location in stream (see streams
    /// seekg)
    /// If the reset flag is true then the parser will be reset so it
    /// expects to see the beginning of a JSON stream.
    bool seekg(exint pos, int dir, bool reset = true);

    /// Retrieves the stream that is being used internally to read
    /// This is used for seekable geometry files to read the footer
    /// without parsing it as JSON.
    /// Normally using this should be avoided.
    UT_IStream* getInternalStream();

    /// Set's binary to given value. This is used to read BJSON without
    /// having the magic infront of it (for embedded geometry)
    void setBinary(bool isBinary) { myBinary = isBinary; }

    /// Parse a single object.
    /// @param handle @n
    ///	  The UT_JSONHandle to perform interpretation of the JSON tokens
    /// @param is @n
    ///   If specified, data will be read from this stream.  The initial
    ///   parseObject() call must have a stream.
    bool		parseObject(UT_JSONHandle &handle, UT_IStream*is=0);

    /// @{
    /// Simple convenience method to parse a single string (keys not valid)
    /// This is done by reading the next object using UT_JSONValue.
    /// The method fails if the next object in the stream is not a string.
    bool		parseString(UT_WorkBuffer &v);
    bool		parseString(UT_StringHolder &v);
    /// @}

    /// @{
    /// Simple convenience method to parse a single map key (strings not valid)
    /// This is done by reading the next object using UT_JSONValue
    /// The method fails if the next object in the stream is not a key.
    bool		parseKey(UT_WorkBuffer &v);
    bool		parseKey(UT_StringHolder &v);
    /// @}
    /// Simple convenience method to parse a single string (keys not valid)
    /// This is done by reading the next object using UT_JSONValue.
    /// The method fails if the next object in the stream is not a string.
    bool		parseBool(bool &v);
    /// Simple convenience method to parse a single integer
    /// This is done by reading the next object using UT_JSONValue
    /// The method fails if the next object in the stream is not a number or
    /// bool.  Real values are cast to integers.
    bool		parseInteger(int64 &v);
    /// Alternate short-form
    bool		parseInt(int64 &v)	{ return parseInteger(v); }
    /// Simple convenience method to parse a single real
    /// This is done by reading the next object using UT_JSONValue
    /// The method fails if the next object in the stream is not a number
    bool		parseReal(fpreal64 &v);
    /// Generic parsing of a number (int)
    bool		parseNumber(int8 &v);
    bool		parseNumber(int16 &v);
    bool		parseNumber(int32 &v);
    bool		parseNumber(int64 &v)	 { return parseInteger(v); }
    bool		parseNumber(uint8 &v);
    bool		parseNumber(uint16 &v);
    /// Generic parsing of a number (real)
    bool		parseNumber(fpreal16 &v);
    bool		parseNumber(fpreal32 &v);
    bool		parseNumber(fpreal64 &v) { return parseReal(v); }

    bool		parseValue(UT_WorkBuffer &v){ return parseString(v); }
    bool		parseValue(UT_StringHolder &v){ return parseString(v); }
    bool		parseValue(bool &v)	    { return parseBool(v); }
    bool		parseValue(int8 &v)	    { return parseNumber(v); }
    bool		parseValue(int16 &v)	    { return parseNumber(v); }
    bool		parseValue(int32 &v)	    { return parseNumber(v); }
    bool		parseValue(int64 &v)	    { return parseNumber(v); }
    bool		parseValue(uint8 &v)	    { return parseNumber(v); }
    bool		parseValue(uint16 &v)	    { return parseNumber(v); }
    bool		parseValue(fpreal16 &v)	    { return parseNumber(v); }
    bool		parseValue(fpreal32 &v)	    { return parseNumber(v); }
    bool		parseValue(fpreal64 &v)	    { return parseNumber(v); }

    /// A utility method to load a uniform array of data (ie. those written
    /// out by UT_JSONWriter::uniformArray()). Returns the number of elements
    /// successfully extracted from the parser. A maximum of len elements will
    /// be saved in data.
    template <typename T>
    int64		parseUniformArray(T *data, int64 len)
    {
	iterator	it = beginArray();
	int64		array_size = getUniformArraySize();
	int64		load_size = SYSmin(len, array_size);
	int64		i;

	switch (getUniformArrayType())
	{
	    case UT_JID_INT8:
		i = UniformArrayHelper<int8, T>::load(it, data, load_size);
		break;
	    case UT_JID_INT16:
		i = UniformArrayHelper<int16, T>::load(it, data, load_size);
		break;
	    case UT_JID_INT32:
		i = UniformArrayHelper<int32, T>::load(it, data, load_size);
		break;
	    case UT_JID_INT64:
		i = UniformArrayHelper<int64, T>::load(it, data, load_size);
		break;
	    case UT_JID_REAL16:
		i = UniformArrayHelper<fpreal16, T>::load(it, data, load_size);
		break;
	    case UT_JID_REAL32:
		i = UniformArrayHelper<fpreal32, T>::load(it, data, load_size);
		break;
	    case UT_JID_REAL64:
		i = UniformArrayHelper<fpreal64, T>::load(it, data, load_size);
		break;
	    default:
		i = 0;
		load_size = 0;
		break;
	}

	if (i < load_size)	// error occurred during load
	    return i;

	// TODO: We eat any trailing array entries for the cases handled above
	//       here as well, and this could be done more efficiently.
	for ( ; !it.atEnd(); ++it, ++i)
	{
	    T val;
	    if (!parseValue(val))
		break;
	    if (i < len)
		data[i] = val;
	}
	return i;
    }

    /// A utility method to load blocks of values from an array which can
    /// load data from uniform arrays very efficiently.  Returns the number
    /// of elements successfully extracted from the parser. A maximum of len
    /// elements will be saved in data.
    template <typename T>
    int64		parseArrayValues(iterator &it, T *data, int64 len)
    {
	int64		unread_size = getUniformArrayUnreadSize();
	int64		load_size = SYSmin(len, unread_size);
	int64		i;

	switch (getUniformArrayType())
	{
	    case UT_JID_INT8:
		i = UniformArrayHelper<int8, T>::load(it, data, load_size);
		break;
	    case UT_JID_INT16:
		i = UniformArrayHelper<int16, T>::load(it, data, load_size);
		break;
	    case UT_JID_INT32:
		i = UniformArrayHelper<int32, T>::load(it, data, load_size);
		break;
	    case UT_JID_INT64:
		i = UniformArrayHelper<int64, T>::load(it, data, load_size);
		break;
	    case UT_JID_REAL16:
		i = UniformArrayHelper<fpreal16, T>::load(it, data, load_size);
		break;
	    case UT_JID_REAL32:
		i = UniformArrayHelper<fpreal32, T>::load(it, data, load_size);
		break;
	    case UT_JID_REAL64:
		i = UniformArrayHelper<fpreal64, T>::load(it, data, load_size);
		break;
	    default:
		for (i = 0; !it.atEnd() && i < len; ++it, ++i)
		{
		    T val;
		    if (!parseValue(val))
			break;
		    data[i] = val;
		}
		break;
	}

	return i;
    }

    /// A utility method to load a uniform array of data (ie. those written
    /// out by UT_JSONWriter::uniformArray()). Returns the number of elements
    /// successfully extracted from the parser. A maximum of len elements will
    /// be saved in data.
    int64		parseUniformBoolArray(UT_BitArray &data, int64 len);

private:
    /// @private - Used by loadPODArray
    template <typename OP_TYPE, typename T> bool
    loadPODUniformArray(OP_TYPE &op, iterator &it, int64 size)
    {
	UT_StackBuffer<T>	b(size);
        if (!it.readUniformArray(b.array(), size))
	    return false;
        bool success = op.setArray((const T*)b, size);
        return success;
    }
    /// @private - Used by parseUniformArray
    template <typename INTERNAL_T, typename T>
	class UniformArrayHelper
	{
	public:
	    /// size MUST be <= it.getUniformArraySize(), so a return value
	    /// less than size indicates an error occurred.
	    static int64	load(iterator &it, T *data, int64 size)
	    {
		UT_StackBuffer<INTERNAL_T>	b(size);
		if (!it.readUniformArray(b.array(), size))
		    return 0;
		if (std::numeric_limits<INTERNAL_T>::is_integer &&
		    std::numeric_limits<T>::is_integer &&
		    std::numeric_limits<INTERNAL_T>::digits >
						std::numeric_limits<T>::digits)
		{
		    for (int64 i = 0; i < size; ++i)
		    {
			INTERNAL_T	v = b[i];
			// Ondrej says INTERNAL_T should always be the larger
			// precision type right here.
			if (v < INTERNAL_T(std::numeric_limits<T>::min()) ||
			    v > INTERNAL_T(std::numeric_limits<T>::max()))
			    return i;
			data[i] = static_cast<T>(v);
		    }
		    return size;
		}
		else
		{
		    for (int64 i = 0; i < size; ++i)
			data[i] = static_cast<T>(b[i]);
		    return size;
		}
	    }
	};
    /// @private - Used by parseUniformArray
    template <typename T>
    class UniformArrayHelper<T,T>
    {
    public:
	/// size MUST be <= it.getUniformArraySize(), so a return value
	/// less than size indicates an error occurred.
	static int64	load(iterator &it, T *data, int64 size)
	{
	    if (!it.readUniformArray(data, size))
		return 0;
	    return size;
	}
    };
public:
    /// @private Used by iterator to determine uniform array information
    UT_JID	getUniformArrayType() const;
    /// @private Used by iterator to determine uniform array information
    int64	getUniformArraySize() const;
    /// @private Used by iterator to determine uniform array information
    int64	getUniformArrayUnreadSize() const;
    /// @{
    /// @private Used by iterator
    bool	readUniformArray(int8 *buffer, int64 buffer_size);
    bool	readUniformArray(int16 *buffer, int64 buffer_size);
    bool	readUniformArray(int32 *buffer, int64 buffer_size);
    bool	readUniformArray(int64 *buffer, int64 buffer_size);
    bool	readUniformArray(uint8 *buffer, int64 buffer_size);
    bool	readUniformArray(uint16 *buffer, int64 buffer_size);
    bool	readUniformArray(fpreal16 *buffer, int64 buffer_size);
    bool	readUniformArray(fpreal32 *buffer, int64 buffer_size);
    bool	readUniformArray(fpreal64 *buffer, int64 buffer_size);
    /// @}

    /// Load an entire array of POD data using the operator passed in.
    /// This method may allocate a temporary buffer to load a uniform
    /// array.
    template <typename OP_TYPE, typename POD_TYPE>
    bool	loadPODArray(OP_TYPE &op)
		{
		    iterator	it = beginArray();
		    int64	n = getUniformArraySize();
		    switch (getUniformArrayType())
		    {
			case UT_JID_INT8:
			    return loadPODUniformArray<OP_TYPE, int8>
					    (op, it, n);
			case UT_JID_INT16:
			    return loadPODUniformArray<OP_TYPE, int16>
					    (op, it, n);
			case UT_JID_INT32:
			    return loadPODUniformArray<OP_TYPE, int32>
					    (op, it, n);
			case UT_JID_INT64:
			    return loadPODUniformArray<OP_TYPE, int64>
					    (op, it, n);
			case UT_JID_REAL16:
			    return loadPODUniformArray<OP_TYPE, fpreal16>
					    (op, it, n);
			case UT_JID_REAL32:
			    return loadPODUniformArray<OP_TYPE, fpreal32>
					    (op, it, n);
			case UT_JID_REAL64:
			    return loadPODUniformArray<OP_TYPE, fpreal64>
					    (op, it, n);
			default:
			    break;
		    }
		    POD_TYPE	val;
		    for (int64 i = 0; !it.atEnd(); ++it, ++i)
		    {
			if (!parseNumber(val))
			    return false;
			if (!op.set(i, val))
			    return false;
		    }
		    return true;
		}

    /// Returns true if the next token was the start of a map.  If there was an
    /// error parsing the stream, then error will be set, otherwise the error
    /// parameter will remain unchanged.
    bool		parseBeginMap(bool &error);
    /// Returns true if the next token was the start of an array.  If there was
    /// an error parsing the stream, then error will be set, otherwise the
    /// error parameter will remain unchanged.
    bool		parseBeginArray(bool &error);
    /// Returns true if the next token was an end map.  If there was an error
    /// parsing the stream, then error will be set, otherwise the error
    /// parameter will remain unchanged.
    inline bool		parseEndMap(bool &error)
    {
        UT_JSONHandle *push = myHandle;
        myHandle = nullptr;
        bool ok = parseBeginEnd(UT_JID_MAP_END, error);
        myHandle = push;
        return ok;
    }
    /// Returns true if the next token was an end of an array.  If there was an
    /// error parsing the stream, then error will be set, otherwise the error
    /// parameter will remain unchanged.
    inline bool		parseEndArray(bool &error)
    {
        UT_JSONHandle *push = myHandle;
        myHandle = nullptr;
        bool ok = parseBeginEnd(UT_JID_ARRAY_END, error);
        myHandle = push;
        return ok;
    }
    /// Simple convenience method to skip the next object in the stream
    bool		skipNextObject();

    /// Get resulting warnings/errors
    const UT_StringArray	&getErrors() const	{ return myErrors; }

    /// The read method should be called whenever data needs to be read from
    /// the stream.  It keeps line counts & character offsets.
    bool		readBytes(void *data, exint bytes);

    /// Returns whether we're reading a binary or ASCII file
    bool		getBinary() const	{ return myBinary; }

    /// Returns the current position in the input stream
    int64		getStreamLineCount() const;
    int64		getStreamLineStartPosition() const;
    int64		getStreamPosition() const;

    /// Returns the true and sets the filename if the stream supports random
    /// seeks.
    bool		isRandomAccessFile(UT_WorkBuffer &filename) const;

    /// When reading raw binary data, this returns whether the endianness of
    /// the file is different than the endianness of the processor (requiring
    /// byte swapping).  There are convenience methods to read integer/real
    /// data which automatically swap.  However, when reading uniform arrays in
    /// batch mode, it is more efficient to swap the entire array rather than
    /// piecemeal.
    bool		getSwapFlag() const	{ return mySwap; }

    /// Convenience method to a int8 values
    bool		readValue(int8 *v, exint n) { return readBytes(v, n); }
    /// Convenience method to read int16 values (possibly byte-swapped)
    bool		readValue(int16 *v, exint n);
    /// Convenience method to read int32 values (possibly byte-swapped)
    bool		readValue(int32 *v, exint n);
    /// Convenience method to read int64 values (possibly byte-swapped)
    bool		readValue(int64 *v, exint n);

    /// Convenience method to read uint8 values
    bool		readValue(uint8 *v, exint n) { return readBytes(v, n); }
    /// Convenience method to read uint16 values (possibly byte-swapped)
    bool		readValue(uint16 *v, exint n);
    /// Convenience method to read uint32 values (possibly byte-swapped)
    bool		readValue(uint32 *v, exint n);
    /// Convenience method to read uint64 values (possibly byte-swapped)
    bool		readValue(uint64 *v, exint n);

    /// Convenience method to read fpreal16 values (possibly byte-swapped)
    bool		readValue(fpreal16 *v, exint n);
    /// Convenience method to read fpreal32 values (possibly byte-swapped)
    bool		readValue(fpreal32 *v, exint n);
    /// Convenience method to read fpreal64 values (possibly byte-swapped)
    bool		readValue(fpreal64 *v, exint n);

    /// @{
    /// Convenience method to read a binary encoded string.@n
    /// @note This should be used when reading uniform arrays of UT_JID_STRING
    /// type.
    bool		readString(UT_WorkBuffer &storage);
    bool		readString(UT_StringHolder &result);
    /// @}

    /// @{
    /// Convenience method to read a binary encoded string token.  Instead of
    /// reading an encoded string (readString()), an integer token will be
    /// read.  This indexes the shared string table (see UT_JID_TOKENDEF,
    /// UT_JID_TOKENREF).
    /// @note This should be used when reading uniform arrays of
    /// UT_JID_TOKENREF type.
    bool		readStringToken(UT_WorkBuffer &storage);
    bool		readStringToken(UT_StringHolder &str);
    /// @}

    /// Add an error message.  If the terminate flag is set, parsing will be
    /// terminated.
    void		addFatal(const char *fmt, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);
    void		addWarning(const char *fmt, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    /// Steal errors from another parser (leaves the other parser intact)
    void		stealErrors(const UT_JSONParser &parser);

    /// @brief Traverse an array object in the parser
    ///
    /// This class will iterate over an array.  There is no object associated
    /// with the array, it's up to the caller to load the objects
    ///
    /// There is no rewind() operation available on this iterator
    /// @code
    /// UT_JSONParser::iterator	it;
    /// for (it = UT_JSONParser::beginMap(); !it.atEnd(); ++it)
    /// or
    /// for (it = UT_JSONParser::beginArray(); !it.atEnd(); ++it)
    /// @endcode
    class UT_API iterator
    {
    public:
	iterator()
	    : myParser(nullptr)
	    , myMap(false)
	    , myValid(false)
	    , myError(false)
	{
	}
	iterator(const iterator &src)
	{
	    *this = src;
	}
	~iterator() {}

	const iterator	&operator=(const iterator &src)
		    {
			myParser = src.myParser;
			myMap = src.myMap;
			myValid = src.myValid;
			myError = src.myError;
			return *this;
		    }

	/// ++iterator
	iterator	&operator++()    { advance(); return *this; }
	/// Caution: The post-increment operator has side effects and is
	/// equivalent to the iterator @b after the increment is performed.
	iterator	&operator++(int) { advance(); return *this; }

	bool	atEnd() const	{ return !myValid || myError; }
	void	advance()
		    {
			if (myParser && !myError)
			{
			    bool	hit;
			    hit = (myMap) ? myParser->parseEndMap(myError)
					: myParser->parseEndArray(myError);
			    if (hit || myError)
				myValid = false;
			}
		    }
	/// Get the key from the map.
	/// If we're iterating over an array, this assumes that the next
	/// array element will be a string token.  This allows for
	/// "ordered" maps to be stored in an array of tuples
	/// (string,value)
	///
	/// The T template can be either UT_WorkBuffer or UT_StringHolder
	template <typename T>
	bool	getKey(T &key)
		    {
			if (myParser && myValid)
			{
			    return myMap ? myParser->parseKey(key)
					 : myParser->parseString(key);
			}
			if (myParser)
			{
			    myParser->addWarning(
				    "Missing key while parsing map");
			}
			myValid = false;
			myError = true;
			return false;
		    }

	/// Get a lower case map key (for case insensitive maps)
	template <typename T>
	bool	getLowerKey(T &key)
		    {
			if (getKey(key))
			{
			    toLower(key);
			    return true;
			}
			return false;
		    }

	/// The error state records whether any errors were encountered
	/// during the iterator's parse operations.
	bool	getErrorState() const { return myError; }

	/// Return the uniform array type (UT_JID_NULL if not uniform array)
	UT_JID	getUniformArrayType() const
		    {
			return myParser ? myParser->getUniformArrayType()
				    : UT_JID_NULL;
		    }
	/// Return the number of elements in the uniform array (0 if empty)
	int64	getUniformArraySize() const
		    {
			return myParser ? myParser->getUniformArraySize()
				    : -1;
		    }
	/// Return the remaining number of elements in the uniform array
	/// (0 if empty)
	int64	getUniformArrayUnreadSize() const
		    {
			return myParser ?
				myParser->getUniformArrayUnreadSize() : -1;
		    }

	/// Read uniform array straight into a buffer.  This template only
	/// works for int8, int16, int32, int64, fpreal16, fpreal32,
	/// fpreal64.
	template <typename T>
	bool	readUniformArray(T *buffer, int64 size)
		    {
			if (!myParser)
			    return (myError = false);
			if (!myParser->readUniformArray(buffer, size))
			    return (myError = false);
			// Now, advance to check end condition of array
			advance();
			return !myError;
		    }

    private:
	static inline void	toLower(UT_WorkBuffer &w) { w.lower(); }
	static void		toLower(UT_StringHolder &w) { w = w.toLower(); }
	 iterator(UT_JSONParser *p, bool map)
	     : myParser(p)
	     , myMap(map)
	     , myValid(false)
	     , myError(false)
	 {
	     if (myParser)
	     {
		 if (myMap)
		 {
		     if ((myValid = myParser->parseBeginMap(myError)))
		     {
			 // Check for an empty map
			 if (!myError)
			     myValid = !myParser->parseEndMap(myError);
		     }
		 }
		 else
		 {
		     if ((myValid = myParser->parseBeginArray(myError)))
		     {
			 if (!myError)
			     myValid = !myParser->parseEndArray(myError);
		     }
		 }
		 if (myError)
		     myValid = false;
	     }
	 }

	UT_JSONParser	*myParser;
	bool		 myMap;
	bool		 myValid;
	bool		 myError;

	friend class UT_JSONParser;
    };
    typedef iterator	traverser;	// For backward compatibility

    iterator	beginMap()	{ return iterator(this, true); }
    iterator	beginArray()	{ return iterator(this, false); }

    /// The TiledStream class reads data that was generated by a
    /// UT_JSONWriter::TiledStream.  The reader expects an array, followed by
    /// multiple arrays of uint8.  The multiple arrays appear as a single
    /// stream to the user.  To read a tiled stream from a JSON file, you would
    /// do something like: @code
    /// bool copyStream(UT_JSONParser &p, ostream &os)
    /// {
    ///    UT_JSONParser::TiledStream      is(p);
    ///    while (true)
    ///    {
    ///        char        buffer[128];
    ///        exint       nread = is.read(buffer, 128);
    ///        if (nread < 0)
    ///            return false;   // See p.getErrors() for error message
    ///        if (nread == 0)     // Read to end of stream
    ///            break;
    ///        os.write(buffer, nread);
    ///    }
    ///    return true;
    /// }
    /// @endcode
    class UT_API TiledStreamBuf : public std::streambuf
    {
    public:
	TiledStreamBuf(UT_JSONParser &p);
	virtual ~TiledStreamBuf();

	virtual int_type	underflow();

    private:
	/// Returns the number of bytes read or less than 0 on error.
	exint	read(void *buffer, exint bufsize);
	exint			 loadBuffer();
	UT_JSONParser		&myParser;
	UT_JSONParser::iterator	 myIt;
	char			*myBuffer;
	exint			 mySize;
	exint			 myCapacity;
    };

    class UT_API TiledStream : public std::istream
    {
    public:
	TiledStream(UT_JSONParser &p);
	virtual ~TiledStream();
    private:
	TiledStreamBuf		myBuf;
    };

private:
    // Different parsing states
    enum ut_JParseState
    {
	JSON_START,		// Prior to parsing JSON file
	JSON_COMPLETE,		// Successful parsing
	JSON_ERROR,		// Error thrown

	JSON_MAP_START,	// Starting a map (almost same as JSON_MAP_NEED_KEY)
	JSON_MAP_SEP,	// Expecting a map separator
	JSON_MAP_NEED_VAL,	// Expecting a map value
	JSON_MAP_GOT_VAL,	// Expecting end of map or value separator
	JSON_MAP_NEED_KEY,	// Expecting a map key (string)

	JSON_ARRAY_START,	// Starting an array
	JSON_ARRAY_NEED_VAL,	// Expecting a value for an array
	JSON_ARRAY_GOT_VAL,	// Expecting end of array or value separator
    };

    /// @private Set a stream for the parser.
    UT_IStream	*setIStream(UT_IStream *is);

    /// @private Parse the stream
    bool	 parseStream(exint nobjects);

    /// @private Parse a single token
    bool	 parseToken(UT_JID &token);

    /// @private Parse a simple object
    bool	 parseSimpleObject(ut_SimpleJSONObjectHandle &h);

    /// @private Test if next token is an end-object
    bool	 parseBeginEnd(UT_JID type, bool &error);

    /// @private: Read an encoded length
    /// @see UT_JID
    bool		readLength(int64 &l);
    /// @private: Peek at the next token.  The result will be in myPeek
    bool		peekToken();
    /// @private: Process a token
    bool		readToken(ut_JValue &value);
    /// @private: Push parse state
    bool		pushState(ut_JParseState state);	// Push state
    /// @private: Set parse state
    void		setState(ut_JParseState state);	// Set top of stack
    /// @private: Pop parse state
    bool		popState();		// Pop state
    /// @private: Query parse state
    ut_JParseState	getState() const	{ return myState; }
    /// @private: Read UTF-8 quoted string
    bool		readQuotedString(UT_StringHolder &buf);
    /// @private: Read number/null/true/false
    bool		readNumber(ut_JValue &value);
    /// @private: defineToken()
    bool		defineToken();
    /// @private: undefineToken()
    bool		undefineToken();
    /// @private: undefineToken()
    void		undefineToken(int64 id);
    /// @private: lookupToken()
    bool		lookupToken(UT_StringHolder &value);
    /// @private: clearTokens()
    void		clearTokens();

    typedef UT_Map<int64, UT_StringHolder>	TokenMap;

    UT_JSONHandle	*myHandle;
    UT_IStream		*myStream;
    ut_JValue		*myPeek;
    ut_JValue		*myPeekCache;
    UT_StringArray	 myErrors;
    UT_IntArray		 myStack; // State stack
    TokenMap		 myTokens;
    ut_JParseState	 myState;
    int64		 myLineCount;
    int64		 myLineStartPosition;
    bool		 mySwap;
    bool		 myBinary;

    friend class	UT_AutoJSONParser;
};

/// Convenience class to create a JSON parser from a simple input
///
/// For example: @code
///	// Function to parse the JSON stream
///	bool load(UT_JSONParser &p) { ... }
///
///	// Function to parse from a std::string
///	bool load(const std::string &str)
///	{
///	    UT_JSONAutoParser	p(str.c_str(), str.size());
///	    return load(*p);
///	}
///
///	// Function to parse from a UT_JSONValue
///	bool load(const UT_JSONValue &v)
///	{
///	    UT_JSONAutoParser	p(v);
///	    return load(*p);
///	}
/// @endcode
///
class UT_API UT_AutoJSONParser
{
public:
    /// Load JSON from the given UT_IStream
    UT_AutoJSONParser(UT_IStream &is);

    /// Load JSON from the given string
    UT_AutoJSONParser(const char *buffer, int64 size);

    /// Load JSON from the UT_JSONValue
    UT_AutoJSONParser(const UT_JSONValue &v);

    ~UT_AutoJSONParser()
    {
	close();
    }

    /// @{
    /// Access to the parser
    UT_JSONParser	&parser()		{ return myParser; }
    UT_JSONParser	&operator*()		{ return myParser; }
    UT_JSONParser	*operator->()		{ return &myParser; }
    /// @}

    // Implicit casting to a UT_JSONParser
    operator		UT_JSONParser &()	{ return myParser; }

    /// Close the parser
    void	close();

private:

    UT_JSONParser	 myParser;
    UT_WorkBuffer 	 myBuffer;
    UT_IStream*		 myIStream;
    bool		 myAllocatedIStream;
};

#endif
