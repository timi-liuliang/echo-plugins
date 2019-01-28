/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONWriter.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_JSONWriter__
#define __UT_JSONWriter__

#include "UT_Compression.h"
#include "UT_IntArray.h"
#include "UT_JSONDefines.h"
#include "UT_StringMap.h"
#include "UT_WorkBuffer.h"
#include <iosfwd>

class UT_Options;
class UT_BitArray;
class UT_JSONValue;

/// @brief Class which writes ASCII or binary JSON streams
///
/// The JSON format (http://www.json.org) is a generic way to store data.
/// Houdini extends JSON to a byte-stream binary encoding (see UT_JID for
/// details).  The UT_JSONWriter class is used to write JSON (binary or
/// ASCII) to a stream.
/// @see UT_JID
class UT_API UT_JSONWriter
{
public:
    /// @param options @n
    /// A UT_Options class which sets specific format options.
    /// @see setOptions()
	     UT_JSONWriter(const UT_Options *options=0);
    virtual ~UT_JSONWriter();

    /// Allocate a JSON Writer which will fill a UT_WorkBuffer.
    /// Please delete when finished with the writer.
    static UT_JSONWriter	*allocWriter(UT_WorkBuffer &buffer);

    /// Allocate a JSON Writer which writes to an output stream.
    /// Please delete when finished with the writer.
    static UT_JSONWriter *allocWriter(
				std::ostream &os,
				bool binary,
				UT_CompressionType compressionType = UT_COMPRESSION_TYPE_NONE);

    /// Allocate a JSON Writer which writes to a named file.
    /// Please delete when finished with the writer.
    static UT_JSONWriter *allocWriter(
				const char *filename,
				bool binary,
				UT_CompressionType compressionType = UT_COMPRESSION_TYPE_NONE);

    /// Allocate a JSON Writer which writes into a given UT_JSONValue.  The
    /// value will only be set properly at the conclusion of writing.
    /// Please delete when finished with the writer.
    static UT_JSONWriter	*allocWriter(UT_JSONValue &value);

    /// Set formatting/control options base on the UT_Options.  The following
    /// options are scanned:
    /// - @b int @b "json:precision" -- ASCII float precision [9].
    ///   Note half-precision floats will use half this precision, while double
    ///   precision will use twice the digits.
    /// - @b int @b "json:indentstep" -- ASCII indent step for maps/arrays [8]
    /// - @b int @b "json:textwidth" -- ASCII wrap lines after byte count [256]
    /// - @b bool @b "json:comments" -- Allow comments in ASCII stream [false]
    /// - @b bool @b "json:prettyprint" -- Pretty print ASCII stream [true]
    /// - @b bool @b "json:usetokens" -- Binary string token compression [true]
    void	setOptions(const UT_Options &options);

    /// Get the current options.  This will set the token/values in
    /// the UT_Options based on options from setOptions()
    void	getOptions(UT_Options &options) const;

    /// Turn the JSON writer into a binary JSON (bJSON) writer.
    /// @warning This method must be called before any other data is written to
    /// the JSON file.  It's a @b good idea to call in the sub-class
    /// constructor.
    bool	setBinary(bool bin);

    /// Return whether writing binary or ASCII JSON
    bool	getBinary() const	{ return myBinary; }

    /// Get the number of bytes written to output so far.
    int64	getNumBytesWritten() const { return myCurrentFilePosition; }

    /// Start a new embedded file
    void	startNewFile();

    /// End a started embedded file
    void	endNewFile();

    /// Output a comment in the JSON stream.  Not all JSON parsers (actually
    /// very few) support comments.  The json:comments must be enabled for this
    /// statement to have any effect.
    bool	jsonComment(const char *format, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    // Standard JSON types
    /// Write a @b null to the stream
    bool	jsonNull();
    /// Write true/false
    bool	jsonBool(bool value);
    /// Write an integer value
    bool	jsonInt(int32 value);
    /// Write a 64 bit integer value
    bool	jsonInt(int64 value);
    /// Write a 64 bit integer as a 64 bit integer
    bool	jsonInt64(int64 value);
    /// Write a 16 bit real
    bool	jsonReal(fpreal16 value);
    /// Write a 32 bit real
    bool	jsonReal(fpreal32 value);
    /// Write a 64 bit real
    bool	jsonReal(fpreal64 value);
    /// Write a quoted string.
    /// @warning Please use jsonKey() to write key strings.
    /// @note Use jsonStringToken() to take advantage of token compression
    /// (this can be turned off with the @c usetokens option in the
    /// constructor).
    /// @see UT_JID
    bool	jsonString(const char *value, int64 length=0);

    /// For binary streams, common strings can be output more efficiently using
    /// the token interface.  Token strings will appear as strings in ASCII
    /// streams.
    bool	jsonStringToken(const UT_StringRef &value);
    bool        jsonStringToken(const char *value, int64 length)
    {
	UT_StringRef		result;
	if (length <= 0)
	    result.reference(value);
	else
	    result.fastReferenceWithStrlen(value, length);
        return jsonStringToken(result);
    }

    /// @{
    /// Write a standard JSON value to the stream.  Overloaded method to allow
    /// convenient use in templated code.
    bool	jsonValue(bool value) { return jsonBool(value); }
    bool	jsonValue(int8 value) { return jsonInt((int32)value); }
    bool	jsonValue(int16 value) { return jsonInt((int32)value); }
    bool	jsonValue(int32 value) { return jsonInt(value); }
    bool	jsonValue(int64 value) { return jsonInt(value); }
    bool	jsonValue(fpreal16 value) { return jsonReal(value); }
    bool	jsonValue(fpreal32 value) { return jsonReal(value); }
    bool	jsonValue(fpreal64 value) { return jsonReal(value); }
    bool	jsonValue(const char *v) { return jsonStringToken(v); }
    bool	jsonValue(const UT_String &v)
		    { return jsonStringToken(v.c_str(), v.length()); }
    bool	jsonValue(const std::string &v)
		    { return jsonStringToken(v.c_str(), v.length()); }
    bool	jsonValue(const UT_StringRef &v) { return jsonStringToken(v); }
    bool	jsonValue(const UT_Int16Array &v)
		    { return jsonUniformArray(v); }
    bool	jsonValue(const UT_Int32Array &v)
		    { return jsonUniformArray(v); }
    bool	jsonValue(const UT_Int64Array &v)
		    { return jsonUniformArray(v); }
    bool	jsonValue(const UT_Fpreal32Array &v)
		    { return jsonUniformArray(v); }
    bool	jsonValue(const UT_Fpreal64Array &v)
		    { return jsonUniformArray(v); }
    bool	jsonValue(const UT_StringArray &v)
		    { return jsonStringTokenArray(v); }
    bool	jsonValue(const UT_JSONValue &v);
    /// @}

    /// Begin a map/object dictionary
    bool	jsonBeginMap();
    /// Write a quoted key for the object
    /// @note Use jsonKeyToken() to take advantage of token compression
    /// (this can be turned off with the @c usetokens option in the
    /// constructor).
    /// @see UT_JID
    bool	jsonKey(const char *value, int64 length=0);
    /// For binary streams, common key strings can be output more efficiently
    /// using the token interface.  Token key strings will appear as strings in
    /// ASCII streams.
    bool	jsonKeyToken(const UT_StringRef &value);
    bool	jsonKeyToken(const char *value, int64 length)
    {
	UT_StringRef		result;
	if (length <= 0)
	    result.reference(value);
	else
	    result.fastReferenceWithStrlen(value, length);
        return jsonKeyToken(result);
    }

    /// @{
    /// Convenience method to write the key/value pair to a map
    template <typename T> SYS_FORCE_INLINE bool
    jsonKeyValue(const UT_StringRef &key, const T &value)
    {
	bool	ok = jsonKeyToken(key);
	return ok && jsonValue(value);
    }

    template <typename T> SYS_FORCE_INLINE bool
    jsonKeyTokenValue(const UT_StringRef &key, const T &value)
    {
	bool	ok = jsonKeyToken(key);
	return ok && jsonValue(value);
    }
    /// @}

    /// End the object
    bool	jsonEndMap();

    /// Begin a generic array object
    bool	jsonBeginArray();
    /// End a generic array object
    /// By default, a new line will be output for pretty printing.  The newline
    /// can give a hint to omit the new line before the end of the array
    /// marker.
    bool	jsonEndArray(bool newline=true);

    /// When writing binary JSON files, uniform arrays can be encoded without
    /// repetition of the type information which results in a more compressed
    /// data format.
    ///
    /// @c beingUniformArray() starts a uniform array.
    /// The @c id should be one of:
    ///	- UT_JID_BOOL
    /// - UT_JID_INT8
    /// - UT_JID_INT16
    /// - UT_JID_INT32
    /// - UT_JID_INT64
    /// - UT_JID_UINT8
    /// - UT_JID_UINT16
    /// - UT_JID_REAL16
    /// - UT_JID_REAL32
    /// - UT_JID_REAL64
    /// - UT_JID_STRING
    bool	beginUniformArray(int64 length, UT_JID id);
    /// Write a bool value to a uniform array.  The bool values are packed into
    /// a bit array (where each 32-bit word stores 32 bits).
    bool	  uniformWrite(bool value);
    /// Write an 8 bit integer value to the uniform array
    bool	  uniformWrite(int8 value);
    /// Write an 16 bit integer value to the uniform array
    bool	  uniformWrite(int16 value);
    /// Write an 32 bit integer value to the uniform array
    bool	  uniformWrite(int32 value);
    /// Write an 64 bit integer value to the uniform array
    bool	  uniformWrite(int64 value);
    /// Write an 16 bit float/real value to the uniform array
    bool	  uniformWrite(fpreal16 value);
    /// Write an 32 bit float/real value to the uniform array
    bool	  uniformWrite(fpreal32 value);
    /// Write an 64 bit float/real value to the uniform array
    bool	  uniformWrite(fpreal64 value);
    /// Write an 8 bit unsigned integer value to the uniform array
    bool	  uniformWrite(uint8 value);
    /// Write an 16 bit unsigned integer value to the uniform array
    bool	  uniformWrite(uint16 value);
    /// Write a string to the uniform array
    bool	  uniformWrite(const char *value, int64 length=0);
    /// Write a block of 8 bit integer values to the uniform array
    bool	  uniformBlockWrite(const int8 *value, int64 count);
    /// Write a block of 16 bit integer values to the uniform array
    bool	  uniformBlockWrite(const int16 *value, int64 count);
    /// Write a block of 32 bit integer values to the uniform array
    bool	  uniformBlockWrite(const int32 *value, int64 count);
    /// Write a block of 64 bit integer values to the uniform array
    bool	  uniformBlockWrite(const int64 *value, int64 count);
    /// Write a block of 16 bit float/real values to the uniform array
    bool	  uniformBlockWrite(const fpreal16 *value, int64 count);
    /// Write a block of 32 bit float/real values to the uniform array
    bool	  uniformBlockWrite(const fpreal32 *value, int64 count);
    /// Write a block of 64 bit float/real values to the uniform array
    bool	  uniformBlockWrite(const fpreal64 *value, int64 count);
    /// Write a block of 8 bit unsigned integer values to the uniform array
    bool	  uniformBlockWrite(const uint8 *value, int64 count);
    /// Write a block of 16 bit unsigned integer values to the uniform array
    bool	  uniformBlockWrite(const uint16 *value, int64 count);
    /// End the uniform array.
    /// @param nwritten @n
    /// If provided, the number of items written is returned.
    /// @note The JSONWriter class will automatically clamp or fill the uniform
    /// array with 0, but nwritten will return the actual number of calls to
    /// uniformWrite().
    bool	endUniformArray(int64 *nwritten=0);

    /// Internally called to write raw data to output.
    virtual bool	writeData(const void *data, int64 bytes);

    /// Returns the JID that matches the given type.
    static UT_JID	jidFromValue(const bool *) { return UT_JID_BOOL; }
    static UT_JID	jidFromValue(const int8 *) { return UT_JID_INT8; }
    static UT_JID	jidFromValue(const int16 *) { return UT_JID_INT16; }
    static UT_JID	jidFromValue(const int32 *) { return UT_JID_INT32; }
    static UT_JID	jidFromValue(const int64 *) { return UT_JID_INT64; }
    static UT_JID	jidFromValue(const fpreal16 *) { return UT_JID_REAL16; }
    static UT_JID	jidFromValue(const fpreal32 *) { return UT_JID_REAL32; }
    static UT_JID	jidFromValue(const fpreal64 *) { return UT_JID_REAL64; }
    static UT_JID	jidFromValue(const uint8 *) { return UT_JID_UINT8; }
    static UT_JID	jidFromValue(const uint16 *) { return UT_JID_UINT16; }

    /// Efficent method of writing a uniform array of int8 values
    bool	jsonUniformArray(int64 length, const int8 *value);
    /// Efficent method of writing a uniform array of int16 values
    bool	jsonUniformArray(int64 length, const int16 *value);
    /// Efficent method of writing a uniform array of int32 values
    bool	jsonUniformArray(int64 length, const int32 *value);
    /// Efficent method of writing a uniform array of int64 values
    bool	jsonUniformArray(int64 length, const int64 *value);
    /// Efficent method of writing a uniform array of fpreal16 values
    bool	jsonUniformArray(int64 length, const fpreal16 *value);
    /// Efficent method of writing a uniform array of fpreal32 values
    bool	jsonUniformArray(int64 length, const fpreal32 *value);
    /// Efficent method of writing a uniform array of fpreal64 values
    bool	jsonUniformArray(int64 length, const fpreal64 *value);

    /// Efficent method of writing a uniform array of uint8 values
    bool	jsonUniformArray(int64 length, const uint8 *value);
    /// Efficent method of writing a uniform array of uint16 values
    bool	jsonUniformArray(int64 length, const uint16 *value);

    /// Efficient method of writing a uniform array of bool values
    bool	jsonUniformArray(int64 length, const UT_BitArray &value,
				 bool verbose_ascii = false);

    /// @{
    /// Convenience method for saving common array types
    bool	jsonUniformArray(const UT_Int16Array &a)
		    { return jsonUniformArray(a.size(), a.data()); }
    bool	jsonUniformArray(const UT_Int32Array &a)
		    { return jsonUniformArray(a.size(), a.data()); }
    bool	jsonUniformArray(const UT_Int64Array &a)
		    { return jsonUniformArray(a.size(), a.data()); }
    bool	jsonUniformArray(const UT_Fpreal32Array &a)
		    { return jsonUniformArray(a.size(), a.data()); }
    bool	jsonUniformArray(const UT_Fpreal64Array &a)
		    { return jsonUniformArray(a.size(), a.data()); }
    /// @}

    /// Convenience method to save an array of strings
    bool	jsonStringArray(const UT_StringArray &a);
    /// Convenience method to save an array of strings (as tokens)
    bool	jsonStringTokenArray(const UT_StringArray &a);

    /// A TiledStream allows you to write a blob of binary data to a JSON
    /// stream in an efficient manner.  The data is buffered into uniform
    /// arrays which are output into a standard JSON array.  The resulting data
    /// looks something like: @code
    ///   [ {options} tile1-data tile2-data ]
    /// @endcode
    /// The tile-data is written as JSON string objects.  You can read these
    /// yourself, or use the TiledStream in UT_JSONParser.
    ///
    /// You should never write to the JSONWriter while the TiledStream is open.
    ///
    /// An example of using this might be something like: @code
    /// bool storeStream(UT_IStream &is)
    /// {
    ///	 UT_JSONWriter::TiledStream	os(writer);
    ///  char				buffer[128];
    ///	 int				nread;
    ///	 while ((nread = is.bread(buffer, 128)) > 0)
    ///	    if (!os.write(buffer, nread)) return false;
    ///  return true;
    /// }
    /// @endcode
    class UT_API TiledStreamBuf : public std::streambuf
    {
    public:
	 TiledStreamBuf(UT_JSONWriter &w);
	virtual ~TiledStreamBuf();

    protected:
	// write one character
	virtual int_type	overflow(int_type c);

	// write multiple characters
	virtual std::streamsize	xsputn (const char* s, std::streamsize num);

    private:
	bool	write(const void *data, exint bytes);
	void	close();

	bool	writeTile(const char *data, exint bytes);
	bool	flush();
	UT_JSONWriter	&myWriter;
	char		*myBuffer;
	exint		 myLength;
    };

    class UT_API TiledStream : public std::ostream
    {
    public:
	TiledStream(UT_JSONWriter &w);
	virtual ~TiledStream();

    private:
	TiledStreamBuf		myBuf;
    };

protected:
    /// The sub-class must implement this method to write data
    virtual bool	subclassWriteData(const void* data, int64 bytes) = 0;

protected:
    /// Get the nesting level (i.e. how deeply nested in maps/arrays)
    int		getNesting() const	{ return myStack.entries(); }
private:
    int		precision16() const	{ return (myPrecision+1)/2; }
    int		precision32() const	{ return myPrecision; }
    int		precision64() const	{ return myPrecision*2; }
    /// @private: ASCII indent
    bool	indent();
    /// @private: ASCII prefix
    bool	prefix();
    /// @private: Convenience method to write a UT_JID
    bool	writeId(UT_JID id);
    /// @private: Convenience method to write an encoded length @see UT_JID
    bool	writeLength(int64 length);
    /// @private: Write a token string
    bool	writeTokenString(const UT_StringRef &str);
    /// @private: Write a string of data (not quoted)
    bool	writeString(const char *str, int64 bytes=0);
    /// @private: Write a quoted string
    bool	quotedString(const char *str, int64 bytes=0);
    /// @private: Write an ASCII "bool" value
    bool	writeAB(bool i);
    /// @private: Write an ASCII "integer" value
    bool	writeAI(int64 i);
    /// @private: Write an ASCII "float" value
    bool	writeAF(fpreal64 i, int precision);
    /// @private: Start an array (possibly uniform encoding)
    bool	startArray(UT_JID id, UT_JID type=UT_JID_NULL);
    /// @private: Finish array
    bool	finishArray(UT_JID id, bool newline=true);
    /// @private: Flush bits for a uniform array of bools
    bool	flushBits();
    /// @private: Push nesting of map/array
    void	pushState(UT_JID id);
    /// @private: Pop state
    void	popState(UT_JID id);
    /// @private: Set the prefix for ASCII encoding
    void	setPrefix();
    /// @private: Build indent string
    void	bumpIndent(int dir);
    /// @private: New line & indent
    bool	newLine(bool force=false);

    /// Method to detail with long lines in ASCII files
    bool	countAndWrite(const void *data, int64 bytes);

    template <typename T> bool
	uniformArrayI(UT_JID id, int64 length, const T *data);
    template <typename T> bool
	uniformArrayF(UT_JID id, int64 length, const T *data);
    template <typename T> bool
	uniformBlockWriteImplI(const T *data, int64 count);
    template <typename T> bool
	uniformBlockWriteImplF(const T *data, int64 count, int prec);

    enum
    {
	PREFIX_FIRST,	// First element (no prefix)
	PREFIX_ARRAY,	// Use array prefix
	PREFIX_MAP,	// Use object prefix
    };

    UT_IntArray		 myStack;
    UT_WorkBuffer	 myIndentString;
    UT_StringMap<int64>	 myTokens;
    UT_Int64Array	 myTokenCountStack;
    char		*myASeparator;	// Array separator string
    char		*myKSeparator;	// Keyword separator string
    int64		 myCurrentFilePosition;
    int64		 myUWriteSize;	// Uniform write size
    int64		 myUWriteCount;	// Uniform entities written
    int64		 myTokenCount;
    UT_JID		 myUniform;	// Type of data for fixed array
    uint32		 myBitValue;	// For packing bit arrays
    int			 myBitCount;
    int			 myPrefix;
    int			 myPrecision;	// ASCII float precision
    int			 myIndent;	// ASCII indent level
    int			 myIndentStep;	// ASCII indent step
    int			 myTextWidth;	// ASCII max line length
    int			 myLineLength;	// ASCII line length
    bool		 myBinary;	// ASCII or binary
    bool		 myTokenStrings;	// Use token strings
    bool		 myComments;	// Emit comments
    bool		 myPrettyPrint;	// Pretty print
    bool		 myNewLine;	// For line wrapping
    bool		 myJoinNext;
};

/// Convenience class to allocate a writer which is automatically deleted
/// For example: @code
/// bool save(UT_JSONWriter &w) { ... }   // Method to save to standard writer
///
/// bool save(UT_WorkBuffer &buffer)	// Save to a UT_WorkBuffer
/// {
///      UT_AutoJSONWriter  w(buffer);
///      return save(w);
/// }
/// bool save(UT_JSONValue &value)	// Save to a UT_JSONValue
/// {
///      UT_AutoJSONWriter  w(value);
///      return save(w);
/// }
/// @endcode
class UT_API UT_AutoJSONWriter
{
public:
    /// Write directly to the UT_WorkBuffer
    UT_AutoJSONWriter(UT_WorkBuffer &b)
	: myWriter(UT_JSONWriter::allocWriter(b))
    {
    }
    /// Write to the given ostream (binary and optionally gzipped)
    UT_AutoJSONWriter(std::ostream &os,
	    bool binary,
	    UT_CompressionType compressionType = UT_COMPRESSION_TYPE_NONE )
	: myWriter(UT_JSONWriter::allocWriter(os, binary, compressionType))
    {
    }
    /// Write to the given filename (binary and optionally gzipped)
    UT_AutoJSONWriter(const char *filename,
	    bool binary,
	    UT_CompressionType compressionType = UT_COMPRESSION_TYPE_NONE)
	: myWriter(UT_JSONWriter::allocWriter(filename, binary, compressionType))
    {
    }
    /// Write directly to the UT_JSONValue
    UT_AutoJSONWriter(UT_JSONValue &value)
	: myWriter(UT_JSONWriter::allocWriter(value))
    {
    }
    ~UT_AutoJSONWriter()
    {
	delete myWriter;
    }

    /// @{
    /// Access to the writer
    UT_JSONWriter	&writer() const		{ return *myWriter; }
    UT_JSONWriter	&operator*() const	{ return *myWriter; }
    UT_JSONWriter	*operator->() const	{ return myWriter; }
    /// @}

    /// Allow implicit casting between an auto-writer and the underlying writer
    operator		UT_JSONWriter &()	{ return *myWriter; }

    /// Close the stream (causing all data to be flushed)
    void		close()
			{
			    delete myWriter;
			    myWriter = NULL;
			}
private:
    UT_JSONWriter	*myWriter;
};

#endif
