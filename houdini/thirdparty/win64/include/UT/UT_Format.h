/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Format.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_Format__
#define __UT_Format__

#include "UT_API.h"

#include "UT_Assert.h"
#include "UT_NonCopyable.h"

#include <SYS/SYS_Types.h>

#include <algorithm>
#include <initializer_list>
#include <iosfwd>
#include <limits>
#include <string>
#include <type_traits>

#include <stdio.h>

class UT_String;
class UT_StringHolder;
class UT_StringRef;
class UT_StringView;

/// @file UT_Format.h
/// @brief Type-safe formatting, modeled on the Python @c str.format function.
///
/// This formatter supports a sub-set of the Python format syntax. Notably,
/// only indexed arguments are supported for now, not named ones.
///
/// Like with the @c printf() formatting, the format string contains
/// replacement fields that will be substituted by the textual representation
/// of the arguments to the various format functions. The replacement fields
/// for this formatter are surrounded by curly braces `{}`, rather than
/// prefixed with a `%`.
///
/// Note the string `{{` or `}}` will result in a single `{` or `}` being
/// output.
///
/// @section format_grammar Format Grammar
/// The grammar for the replacement field is as follows:
///
///     {[<index>][:<spec>]}
///
/// The `<index>` is an optional integer value, which refers to the position of
/// the argument after the format string. If the `<index>` is omitted for all
/// replacement fields, then they are automatically substituted with
/// `0, 1, 2, ...`
/// It is not permitted to mix indexed and non-indexed.
///
/// If a colon separator is found, the string after that is interpreted as a
/// formatting specification. The grammar for the `<spec>` field is as follows:
///
///     [[<fill>]<align>][<sign>][#][,][0][<width>][.<precision>][<type>]
///
/// Where:
/// - `<fill>` is the fill character to use when the width of the field is
///   greater than the length of the argument value being printed.
/// - `<align>` is a single character code which sets the alignment of the
///   value within the field:
///   - `<`: The field is left-aligned. This is the default for non-numeric
///          values.
///   - `>`: The field is right-aligned. This is the default for numeric values.
///   - `^`: The field is center-aligned.
///   - `=`: The field is number aligned. This is the same as right-aligned,
///          except the fill value is inserted between the sign indicator and
///          the numeric value. This alignment is only available for numeric
///          values.
/// - <sign> is a single character that specifies how the sign is printed for 
///   numeric values:
///   - `+`: Always print a sign, whether positive or negative.
///   - `-`: Only print a sign when the number is negative.
///   - `<space>`: Use a leading space on positive numbers, minus sign for
///     negative.
/// - The '#' option is only available for binary, octal or hexadecimal output.
///   It indicates that they should be prefixed with '0b', '0' and '0x',
///   respectively.
/// - The ',' option will add digit grouping to numerical outputs. For float 
////  numbers, this only applies to the integer portion of the float.
/// - `<width>` specifies the minimum width of the field. Arguments wider than
///   this value will still be printed out in full. Preceding the width field
///   with `0` will automatically set the alignment to number alignment, and
///   the fill value to `0`.
/// - `<precision>` sets the precision for floating point numbers. For other
///   types it specifies the maximum width of the field.
/// - `<type>` is a single character code that specifies how the type should be 
///   interpreted. If an argument type is not valid for the formatting type, 
///   the formatting type is ignored. The supported types are:
///   - `b`: Output integers as binary numbers.
///   - `B`: Output integers as binary numbers with an upper-case prefix (if
///          using the `#` option.
///   - `c`: Output integers interpreted as a Unicode code point.
///   - `d`: Output integers as decimal numbers. This is the default for
///          integers.
///   - `e`: Use the exponent notation for floating point numbers.
///   - `E`: Same as `e`, except the exponent separator is upper-cased.
///   - `f`: Use the fixed point notation for floating point numbers.
///   - `F`: Same as `f`.
///   - `g`: Format a floating point number as either fixed point or scientific,
///          depending on its magnitude, which mostly results in the shortest
///          form being used. This is the default for floats.
///   - `G`: Same as `g`, except the exponent separator is upper-cased.
///   - `o`: Output integers as octals.
///   - `x`: Output integers and floats as hexadecimal. For floats this uses
///          the special hexadecimal float output.
///   - `X`: The same as `x`, except all alphabetic digits and prefix are
///          output in upper-case.
///   - `%`: Takes a floating point number, multiplies it by 100 and outputs it
///          with a percentage sign appended, using fixed point notation. The
///          default precision in this mode is 2.
/// 
/// Example:
/// @code{.cpp}
/// UTformat("Number: {}\n", 1);
/// UTformat("String: {}\n", "foobar");
/// UTformat("Field : {:=^20}\n", 1);
/// UTformat("Index : {3} {0} {2} {1}\n", 'A', 'B', 'C', 'D');
/// UTformat("Perc  : {:.1%}\n", 0.1234);
/// @endcode
/// Result:
/// @code{.txt}
/// Number: 1
/// String: foobar
/// Field : =========1==========
/// Index : D A C B
/// Perc  : 12.3% 
/// @endcode
/// @section printf_grammar Printf Grammar
/// The UTprintf and UT_Format::printf functions use formatting code very 
/// similar to the POSIX specification for printf.
/// The code begins with a single @c % character, followed by zero or more
/// modifiers, ending with the type specifier. Where this implementation of the
/// @c printf formatting code differs, is that there is no need to add length
/// modifiers. Any length modifers will simply be ignored.

/// @section custom_format Custom Formatting
/// Custom types are supported through custom formatting functions. To define
/// a custom formatting function for a specific type, the following function
/// specialization needs to be implemented:
/// @code{.cpp}
/// size_t format(char *buffer, size_t buffer_size, const CustomType &val);
/// @endcode
/// Where:
/// - @c buffer is a byte buffer to write the result into. If @c buffer is a 
///   @c nullptr, then the function should return the number of bytes required 
///   to store the final result.
/// - @c buffer_size is the size of the byte buffer passed in via @c buffer. 
///   This should be ignored if @c buffer is @c nullptr.
/// - @val is value to custom format.
/// The function should return the number of bytes written out, or, if @c buffer
/// is @c nullptr, the amount of bytes needed to write out the entire custom 
/// type representation.


/// @name Standalone formatting functions
/// @{

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string to the FILE pointer. 
/// @param file The @c FILE pointer to write to. The FILE must be opened in
///     write or append mode.
/// @param format The formatting codes, as defined in @ref format_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out. 
template<typename... Args>
size_t UTformat(FILE *file, const char *format, const Args& ...args);

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string to @c stdout.
/// @param format The formatting codes, as defined in @ref format_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out. 
template<typename... Args>
size_t UTformat(const char *format, const Args& ...args);

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string to the output stream object. 
/// @param os The output stream to write to.
/// @param format The formatting codes, as defined in @ref format_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out. 
template<typename... Args>
size_t UTformat(std::ostream &os, const char *format, const Args& ...args);

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string into the buffer provided. If @c buffer is @c nullptr,
/// then nothing is written out, but the number of bytes needed to store
/// the entire formatted string is returned. A termination byte is not written
/// out to the buffer. If termination is required, the caller must take care
/// of it.
/// @param buffer The character buffer to write the formatted string to.
/// @param buffer_size The size of the character buffer.
/// @param format The formatting codes, as defined in @ref format_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out. 
template<typename... Args>
size_t UTformat(char *buffer, size_t buffer_size, const char *format, 
		const Args& ...args);

/// @}

/// @name Standalone printf-like functions
/// @{

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string to the FILE pointer.
/// @param file The @c FILE pointer to write to. The FILE must be opened in
///     write or append mode.
/// @param format The formatting codes, as defined in @ref printf_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out.
template<typename... Args>
size_t UTprintf(FILE *file, const char *format, const Args& ...args);

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string to @c stdout.
/// @param format The formatting codes, as defined in @ref printf_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out.
template<typename... Args>
size_t UTprintf(const char *format, const Args& ...args);

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string to the output stream object.
/// @param os The output stream to write to.
/// @param format The formatting codes, as defined in @ref printf_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out.
template<typename... Args>
size_t UTprintf(std::ostream &os, const char *format, const Args& ...args);

/// Takes a formatting string and a set of arguments and writes out the
/// formatted string into the buffer provided. If @c buffer is @c nullptr,
/// then nothing is written out, but the number of bytes needed to store
/// the entire formatted string is returned. A termination byte is not written
/// out to the buffer. If termination is required, the caller must take care
/// of it.
/// @param buffer The character buffer to write the formatted string to.
/// @param buffer_size The size of the character buffer.
/// @param format The formatting codes, as defined in @ref printf_grammar.
/// @param args The arguments to the formatting.
/// @return The number of bytes successfully written out.
template<typename... Args>
size_t UTprintf(char *buffer, size_t buffer_size, const char *format,
		const Args& ...args);

/// @}


namespace UT { namespace Format {

/// @name Writer functors for formatted strings
/// @{

/// A writer class that writes to a raw character buffer. If initializing with
/// a default constructor, the writer won't store anything. This can be useful
/// to count the number of bytes required for storage before allocating a buffer
/// for the final result.
struct Writer : public UT_NonCopyable
{
    Writer() :
	myBuffer(nullptr),
	myBufferSize(std::numeric_limits<size_t>::max()) { }

    Writer(char *buffer, size_t buffer_size) :
	myBuffer(buffer),
	myBufferSize(buffer_size) { }

    void setBuffer(char *buffer, size_t buffer_size)
    {
	myBuffer = buffer;
	myBufferSize = buffer_size;
    }

    size_t operator()(const char *begin, const char *end)
    {
	// Exhausted?
	if (!myBufferSize)
	    return false;

	size_t size = size_t(end - begin);
	size = std::min(size, myBufferSize);
	if (myBuffer)
	{
	    std::copy(begin, begin + size, myBuffer);
	    myBuffer += size;
	}
	myBufferSize -= size;

	return size;
    }

private:
    char *myBuffer;
    size_t myBufferSize;
};

/// A specialized @c Writer class that writes to a stdio @c FILE buffer.
struct UT_API FileWriter : public UT_NonCopyable
{
    FileWriter(FILE *file) : myFile(file) {}

    size_t operator()(const char *begin, const char *end);

private:
    FILE *myFile;
};

/// A specialized @c Writer class that writes to a @c std::ostream object.
struct UT_API StreamWriter : public UT_NonCopyable
{
    StreamWriter(std::ostream &os) : myOS(os) {}

    size_t operator()(const char *begin, const char *end);

private:
    std::ostream &myOS;
};

/// Class outline of a writer class for the formatter. This serves as a
/// template when writing a new @ref Formatter writer for cases that the
/// @ref FileWriter, @ref StreamWriter, and @ref Writer don't cover.
struct CustomWriter : public UT_NonCopyable
{
    /// Write the range of bytes from @c begin to @c end, non-inclusive (i.e.
    /// don't write out the byte pointed to by @c end). Return the number of
    /// bytes written.
    size_t operator()(const char *begin, const char *end);
};

/// @}

// A template to check whether a variation of format(...) exists for the
// given type.
namespace Impl
{
    template<typename> struct TrueType : std::true_type {};
    // SFINAE will pick one of the two HasFormat() variants below, depending on
    // if it can resolve the declaration type of the format() declaration given
    // below. We wrap the result through the TrueType class above (which 
    // accepts any type) in order to get a true value for it in the actual
    // HasFormat implementation below.
    template<typename T>
    static auto TestHasFormat(int) -> 
	TrueType<decltype(format(std::declval<char *>(), 
				 size_t(), std::declval<T>()))>;
    template<typename T>
    static auto TestHasFormat(long) -> std::false_type;
}

template<typename T>
struct HasFormat 
{ 
    static constexpr bool value = decltype(Impl::TestHasFormat<T>(0))::value; 
};

/// A container class that holds the individual argument values to be formatted
/// with the format string.
class UT_API ArgValue
{    
   
public:
    enum class Type 
    { 
	None, 
	Int8, UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64,
	Float16, Float32, Float64,
	ZeroString, SizedString, CharString,
	Pointer, Custom
    };

    ArgValue(char v) : myType(Type::CharString), myChar(v) {}
    ArgValue(int8 v) : myType(Type::Int8), myI8(v) {}
    ArgValue(uint8 v) : myType(Type::UInt8), myU8(v) {}
    ArgValue(int16 v) : myType(Type::Int16), myI16(v) {}
    ArgValue(uint16 v) : myType(Type::UInt16), myU16(v) {}
    ArgValue(int32 v) : myType(Type::Int32), myI32(v) {}
    ArgValue(uint32 v) : myType(Type::UInt32), myU32(v) {}
    ArgValue(int64 v) : myType(Type::Int64), myI64(v) {}
    ArgValue(uint64 v) : myType(Type::UInt64), myU64(v) {}
    ArgValue(fpreal16 v) : myType(Type::Float16), myF16(v) {}
    ArgValue(fpreal32 v) : myType(Type::Float32), myF32(v) {}
    ArgValue(fpreal64 v) : myType(Type::Float64), myF64(v) {}
    
#if defined(LINUX) && defined(AMD64)
    // Special overload to deal with long longs, which both gcc and clang
    // treat as a distinct type from long, on 64-bit Linux, even if they're the
    // same size.
    static_assert(sizeof(long long) == 8, "long long should be 8 bytes.");
    ArgValue(long long v) : myType(Type::Int64), myI64(int64(v)) {}
    ArgValue(unsigned long long v) : myType(Type::UInt64), myU64(uint64(v)) {}
#endif
#if defined(MBSD) && defined(__clang__)
    // Special overload for size_t with clang on OSX.
    ArgValue(size_t myValuePtr) : myType(Type::UInt64), myU64(uint64(myValuePtr)) {}
#endif

    ArgValue(bool v) : myType(Type::SizedString)
    {
	if (v)
	{
	    mySizedStr.myStr = "true";
	    mySizedStr.mySize = 4;
	}
	else
	{
	    mySizedStr.myStr = "false";
	    mySizedStr.mySize = 5;
	}
    }

    ArgValue(char *v) : myType(Type::ZeroString), myStr(v) {}
    ArgValue(const char *v) : myType(Type::ZeroString), myStr(v) {}
    ArgValue(const UT_String &v);
    ArgValue(const UT_StringRef &v);
    ArgValue(const UT_StringHolder &v);
    ArgValue(const UT_StringView &v);
    ArgValue(const std::string &v) :
        myType(Type::SizedString), mySizedStr(v.c_str(), v.length()) {}

    // void pointer overrides because std::is_pointer<void*>::value is false!
    ArgValue(void *v) :
	myType(Type::Pointer), myPtr(v)
    {
    }
    ArgValue(const void *v) :
	myType(Type::Pointer), myPtr(v)
    {
    }

#define UT_ENABLE_IF(T)	    typename std::enable_if<T>::type * = nullptr
#define UT_HAS_FMT(T)	    HasFormat<T>::value
#define UT_REMOVE_PTR(T)    typename std::remove_pointer<T>::type
#define UT_IS_PTR(T)	    std::is_pointer<T>::value

    // Accept any pointer type that doesn't have a custom formatter.
    // We use std::enable_if magic to allow SFINAE to elide any constructor
    // that doesn't properly evaluate (a template argument value of false to
    // std::enable_if results in std::enable_if::type not being defined).
    template<typename T>
    ArgValue(T v, 
	     UT_ENABLE_IF( UT_IS_PTR(T) && !UT_HAS_FMT(UT_REMOVE_PTR(T)) )) :
	myType(Type::Pointer), myPtr(v)
    {
    }

    // For pointer types that have custom formatters. In this case we print 
    // out the pointer value and the de-referenced formatted value pointed to,
    // or (null), if the value is \c nullptr.
    template<typename T>
    ArgValue(T v, 
	     UT_ENABLE_IF( UT_IS_PTR(T) && UT_HAS_FMT(UT_REMOVE_PTR(T)) )) :
	myType(Type::Custom)
    {
	myCustom.myFormatFunc = customPtrWrapper<UT_REMOVE_PTR(T)>;
	myCustom.myValuePtr = static_cast<const void *>(v);
    }

    // For fixed length arrays 
    template<typename T, unsigned N>
    ArgValue(T (&v)[N]) :
	myType(Type::Custom)
    {
	myCustom.myFormatFunc = customArrayWrapper<UT_REMOVE_PTR(T),N>;
	myCustom.myValuePtr = static_cast<const void *>(&v[0]);
    }

    // For reference types that have custom formatters.
    template<typename T>
    ArgValue(T &t, UT_ENABLE_IF( !UT_IS_PTR(T) )) :
	myType(Type::Custom)
    {
	static_assert(UT_HAS_FMT(T), "No formatting function for custom type.");
	myCustom.myFormatFunc = customRefWrapper<T>;
	myCustom.myValuePtr = &t;
    }

    size_t formatCustom(char *buffer, size_t buffer_size) const
    {
	UT_ASSERT(isCustom());
	if (!isCustom())
	    return 0;

	return myCustom.myFormatFunc(buffer, buffer_size, myCustom.myValuePtr);
    }

#undef UT_ENABLE_IF
#undef UT_HAS_FMT
#undef UT_REMOVE_PTR
#undef UT_IS_PTR

    // In the presence of a union with non-POD types, we have to provide our
    // own destructor, even if there are no destructors to call.
    ~ArgValue() {}
    
    Type type() const { return myType; }

    bool isEmpty() const { return myType == Type::None; }

    bool isInteger() const
    { 
	return myType == Type::Int8  || myType == Type::UInt8  || 
	       myType == Type::Int16 || myType == Type::UInt16 ||
	       myType == Type::Int32 || myType == Type::UInt32 || 
	       myType == Type::Int64 || myType == Type::UInt64; 
    }
    
    bool isFloat() const
    {
	return myType == Type::Float16 || 
	       myType == Type::Float32 || 
	       myType == Type::Float64;
    }

    bool isSigned() const
    {
	return myType == Type::Int8  ||
	       myType == Type::Int16 ||
	       myType == Type::Int32 ||
	       myType == Type::Int64 ||
	       isFloat();
    }
    
    bool isNumeric() const 
    { 
	return isInteger() || isFloat(); 
    }
    
    bool isString() const 
    { 
	return myType == Type::CharString ||
	       myType == Type::ZeroString ||
	       myType == Type::SizedString;
    }
    
    bool isPointer() const
    {
	return myType == Type::Pointer;
    }

    bool isCustom() const
    {
	return myType == Type::Custom;
    }

protected:
    template<typename W> friend class Formatter;

    ArgValue() : myType(Type::None), myU64(0) {}
    ArgValue(Type type) : myType(type), myI64(0) {}

    struct SizedString
    {
	SizedString(const char *str, size_t size) : myStr(str), mySize(size) {}
	const char *myStr;
	size_t	mySize;
    };
    
    using CustomFormat = size_t(*)(char *buffer, size_t buffer_size, const void *value);
    
    struct Custom
    {
	const void *myValuePtr;
	CustomFormat myFormatFunc;
    };
    
    union
    {
	int8		myI8;
	uint8		myU8;
	int16		myI16;
	uint16		myU16;
	int32		myI32;
	uint32		myU32;
	int64		myI64;
	uint64		myU64;
	fpreal16	myF16;
	fpreal32	myF32;
	fpreal64	myF64;
	char		myChar;
	const char	*myStr;
	SizedString 	mySizedStr;
	const void	*myPtr;
	Custom		myCustom;
    };
    
private:
    Type myType;

    // Wraper function to invoke the writer for the custom type.
    template <typename T>
    static inline size_t
    customRefWrapper(char *buffer, size_t buffer_size, const void *value) 
    {
	return format(buffer, buffer_size, *static_cast<const T*>(value));
    }

    template <typename T>
    static inline size_t
    customPtrWrapper(char *buffer, size_t buffer_size, const void *value) 
    {
	if (value)
	    return UTformat(buffer, buffer_size, "<{} {}>", 
			    value, *static_cast<const T*>(value));
	else
	    return UTformat(buffer, buffer_size, "<{}>", value);
    }

    template <typename T, unsigned N>
    static inline size_t
    customArrayWrapper(char *buffer, size_t buffer_size, const void *value);
};

/// The main formatting class. This accepts a set of arguments and can
/// subsequently format them in various ways.
template<typename W = Writer>
class Formatter : public UT_NonCopyable
{
    // A utility class to keep track of number of bytes written.
    struct WriteTracker
    {
        WriteTracker(W &writer) : myWriter(writer), myBytesWritten(0) {}
        
        bool write(const char *begin, const char *end)
        {
	    size_t bytes_to_write = size_t(end - begin);
	    size_t n_written = myWriter(begin, end);
	    myBytesWritten += n_written;
	    return n_written == bytes_to_write;
        }
        size_t written() const { return myBytesWritten; }
        
        void increment(size_t n_bytes) { myBytesWritten += n_bytes; }
        
    private:
        W &myWriter;
        size_t myBytesWritten;
    };
public:
    using WriterType = W;
    
    /// Constructs a new @ref Formatter object. 
    Formatter() :
	myNextArgIndex(0),
	myReportErrors(true),
	myDigitGroupings(false) {}

    /// Sets whether to report format string errors or not.
    void setReportErrors(bool flag) { myReportErrors = flag; }
    
    /// Sets whether to force numbers to print with digit grouping. Otherwise
    /// only the ',' (format), or '\'' (printf) flags dictate whether digit 
    /// grouping is done.
    void setForceDigitGroupings(bool flag) { myDigitGroupings = flag; }  
    
    /// Format the arguments into the writer, using the format string given.
    /// Returns the number of bytes written.
    /// @param writer A functor that does the writing of the formatted output.
    ///    See @ref CustomWriter for an example of how to do a custom writer.
    /// @param format The formatting codes, as defined in @ref format_grammar.
    /// @param args An initializer list that forms the list of arguments to
    ///   bake into the formatted result using the format string.
    /// @return The number of bytes written out to the @p writer functor.
    size_t format(W &writer, const char * format,
		  std::initializer_list<ArgValue> args);

    /// Format the arguments into the writer, using the format string given.
    /// Returns the number of bytes written.
    /// @param writer A functor that does the writing of the formatted output.
    ///    See @ref CustomWriter for an example of how to do a custom writer.
    /// @param format The formatting codes, as defined in @ref format_grammar.
    /// @param args An array of arguments to bake into the formatted result 
    ///   using the format string.
    /// @param nb_args The number of items in the argument array passed in.
    /// @return The number of bytes written out to the @p writer functor.
    size_t format(W &writer, const char * format,
		  const ArgValue args[], size_t nb_args);

    /// Format the arguments into the writer, using the format string given.
    /// Returns the number of bytes written.
    /// @param writer A functor that does the writing of the formatted output.
    ///    See @ref CustomWriter for an example of how to do a custom writer.
    /// @param format The formatting codes, as defined by
    ///    [std::printf](http://www.cplusplus.com/reference/cstdio/printf/)
    /// @param args An initializer list that forms the list of arguments to
    ///   bake into the formatted result using the format string.
    /// @return The number of bytes written out to the @p writer functor.
    size_t printf(W &writer, const char * format,
		  std::initializer_list<ArgValue> args);

    /// Format the arguments into the writer, using the format string given.
    /// Returns the number of bytes written.
    /// @param writer A functor that does the writing of the formatted output.
    ///    See @ref CustomWriter for an example of how to do a custom writer.
    /// @param format The formatting codes, as defined by
    ///    [std::printf](http://www.cplusplus.com/reference/cstdio/printf/)
    /// @param args An array of arguments to bake into the formatted result
    ///   using the format string.
    /// @param nb_args The number of items in the argument array passed in.
    /// @return The number of bytes written out to the @p writer functor.
    size_t printf(W &writer, const char * format,
		  const ArgValue args[], size_t nb_args);

private:
    struct FormatSpec
    {
        FormatSpec() :
	    field_width(0),
	    str_width(std::numeric_limits<size_t>::max()),
	    precision(-1),
	    align(Left),
	    sign(Minus),
	    alt_form(false),
	    digit_grouping(false),
            fill(' '),
	    type(0)
        {
        }

        size_t field_width;
        size_t str_width;
        int64  precision;
        enum { Left, Center, Right, Number } align;
        enum { Minus, Plus, Space } sign;
        bool alt_form;
        bool digit_grouping;
        char fill;
        char type;
    };

    bool parseFormatSpec(const char *&ptr, const ArgValue &arg,
			 const ArgValue args[], size_t nb_args,
			 FormatSpec &spec);
    bool parsePrintfSpec(const char *&ptr, const ArgValue &arg,
			 const ArgValue args[], size_t nb_args,
			 FormatSpec &spec);

    size_t formatArg(W &writer, const FormatSpec &spec, const ArgValue &arg);
    size_t formatInteger(W &writer, const FormatSpec &spec, const ArgValue &arg);
    size_t formatFloat(W &writer, const FormatSpec &spec, const ArgValue &arg);
    size_t formatString(W &writer, const FormatSpec &spec, const ArgValue &arg);
    size_t formatPointer(W &writer, const FormatSpec &spec, const ArgValue &arg);
    size_t formatCustom(W &writer, const FormatSpec &spec, const ArgValue &arg);
    size_t formatCodePoint(W &writer, const FormatSpec &spec, const ArgValue &arg);
    size_t formatPercentage(W &writer, const FormatSpec &spec, const ArgValue &arg);
    
    size_t formatAdjustAndFill(W &writer, const char *str, size_t str_size,
			       const FormatSpec &spec);

    // Get the argument pointed at by the current location of the format string.
    // If the first character is a digit, assume the argument is being referred
    // to by index, otherwise assume we're using the next argument.
    const ArgValue &getFormatArg(const char *&ptr, const ArgValue args[],
				 size_t nb_args);
    const ArgValue &getPrintfArg(const char *&ptr, const ArgValue args[],
				 size_t nb_args);
    static int64 getIntValueFromArg(const ArgValue &arg);
    static fpreal64 getFloatValueFromArg(const ArgValue &arg);

    int64 myNextArgIndex;
    bool myReportErrors:1,
	 myDigitGroupings:1;
};

    } // namespace Format 
} // namespace UT


// The inline implementation
#include "UT_FormatImpl.h"

#endif // __UT_Format__
