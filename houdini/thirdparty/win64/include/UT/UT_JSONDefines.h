/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_JSONDefines.h ( UT Library, C++)
 *
 * COMMENTS:	Defines for UT_JSONParser/UT_JSONWrite
 */

#ifndef __UT_JDefines__
#define __UT_JDefines__

/// @brief The UT_JID enums are used in byte-stream encoding of binary JSON
///
/// The JSON token stream is encoded using the binary tokens defined below.
/// Each token is stored as a single byte in the stream with possible
/// additional data following in the stream.
///
/// Note:  Our binary encoding of JSON is different than BISON:
///	http://kaijaeger.com/articles/introducing-bison-binary-interchange-standard.html
/// @li This version has optimizations for storing arrays of uniform type data
/// @li This version does *not* support 24, 40, 48 or 56 bit integers
/// @li This version adds support for 16 bit floating point values
/// @li This version adds support for @b unsigned integerers (bytes)
/// @li This version adds supports for optimized encoding of arrays which
///	contain uniform values (including packed boolean arrays).
/// @li This version allows arbitrary endianness (determined by magic number)
/// @li Strings are not null byte encoded.  Instead the length of the string is
///     stored prior to the data.  This allows strings which contain a null
///     byte to be encoded properly.
/// @li Arrays are not limited to 64K entries
/// @li Maps (objects) are not limited to 64K entries
///
/// Parsing of the binary stream does not require seeking forward or backward
/// through the stream.
///
/// @note
/// Each binary token expects 0 or more bytes of data to follow.  The data for
/// multi-byte types is stored in the native format for the machine.  The
/// endianness of the file is determined by the magic number.@n
/// Several entities store an encoded integer value.  Encoded integers are only
/// used to store unsigned integers for length or id in these entities.
/// Encoded integers are @b not used for storing integer data.  Encoding is
/// performed by by using the first byte to determine the size of the integer:
/// @li <tt>byte[0] < 0xf1</tt>: @n
///	This value represents the length (0-240)
/// @li <tt>byte[0] == 0xf2</tt>: @n
///	The next 2 bytes store an unsigned 16-bit length
/// @li <tt>byte[0] == 0xf4</tt>: @n
///	The next 4 bytes store an unsigned 32-bit length
/// @li <tt>byte[0] == 0xf8</tt>: @n
///	The next 8 bytes store an unsigned 64-bit length
///
/// The values: <tt>0xf1, 0xf3, 0xf5, x0f7, 0xf9-0xff</tt> are reserved for
/// future use.
/// @par
///
/// Strings are encoded in two ways
/// @li UT_JID_STRING is followed by an encoded length and the raw string data.
/// The terminating null character is not stored.  For example, the bytestream:
/// @code
///   [ 0x0c 0x48 0x65 0x6c 0x6c 0x20 0x77 0x77 0x6f 0x72 0x6c 0x64 0x21 ]
/// @endcode
/// Would encode the string "Hello world!"
///
/// @li UT_JID_TOKENDEF provides a mechanism to store common strings in a
/// compact fashion.  This token is followed by an encoded integer representing
/// an @b id, then the encoded length and string data (just as UT_JID_STRING).
/// UT_JID_TOKENREF can then be used to reference the defined string.
/// @n
/// Token definitions do @b not return a string entity to the read stream, but
/// simply define a string.  Token definitions can appear anywhere in the
/// stream.
///
/// The binary format also provides for compact storage of arrays composed of a
/// uniform type of data.  These arrays are encoded by specifying the
/// UT_JID_UNIFORM_ARRAY which is followed by a byte indicating the data
/// storage of the array, followed by an encoded length and the raw data for
/// the array.
///
/// Uniform arrays may only store:  UT_JID_BOOL, UT_JID_INT8, UT_JID_INT16,
/// UT_JID_INT32, UT_JID_INT64, UT_JID_REAL16, UT_JID_REAL32, UT_JID_REAL64,
/// UT_JID_STRING, UT_JID_TOKENREF.  All other storage types are considered
/// an error.
///
/// Uniform arrays of booleans are encoded in words of 32-bits using the
/// simple packing method:@code
///	value = (word & (1 << index)) != 0;
/// @endcode
///
/// Uniform arrays of strings are represented using UT_JID_STRING process for
/// each string (i.e. the length of the string, followed by the string data).@n
///
/// Uniform arrays of tokens are represented using @b N encoded id's.
///
/// @see UT_JSONParser, UT_JSONWrite
///
typedef enum {
    /// No data follows the NULL token
    UT_JID_NULL		= 0x00,

    /// Marks the beginning of a map object.  String/Value pairs will follow
    /// until the UT_JID_MAP_END token is reached.
    UT_JID_MAP_BEGIN	= 0x7b,		// '{'

    /// Marks the end of a map object
    UT_JID_MAP_END	= 0x7d,		// '}'

    /// Marks the beginning of an array object.  Values will follow until the
    /// UT_JID_ARRAY_END token is reached.
    UT_JID_ARRAY_BEGIN	= 0x5b,		// '['

    /// Marks the end of an array object
    UT_JID_ARRAY_END	= 0x5d,		// ']'
    
    /// The following byte should be 0 or 1.  However, the use of this token to
    /// store booleans is less efficient than using UT_JID_FALSE/UT_JID_TRUE.@n
    /// This is primarily for completeness (and encoding of fixed boolean
    /// arrays).
    UT_JID_BOOL		= 0x10,		// 0x10 | 0

    /// The following byte represents an 8 bit integer
    UT_JID_INT8		= 0x11,
    /// The following 2 bytes represent an 16 bit integer
    UT_JID_INT16	= 0x12,
    /// The following 4 bytes represent an 32 bit integer
    UT_JID_INT32	= 0x13,
    /// The following 8 bytes represent an 64 bit integer
    UT_JID_INT64	= 0x14,

    /// The following byte represents an unsigned 8 bit integer
    UT_JID_UINT8	= 0x21,
    /// The following 2 bytes represents an unsigned 16 bit integer
    UT_JID_UINT16	= 0x22,
    /// 0x23 and 0x24 are reserved for future use (32/64 bit unsigned)

    /// The following 2 bytes represent an 16 bit real (float)
    UT_JID_REAL16	= 0x18,
    /// The following 4 bytes represent an 32 bit real (float)
    UT_JID_REAL32	= 0x19,
    /// The following 8 bytes represent an 64 bit real (float)
    UT_JID_REAL64	= 0x1a,
    
    /// The data following is a binary encoded string.  The byte is followed by
    /// an encoded length followed by the bytes representing the string (the
    /// terminating NULL should not be included).
    UT_JID_STRING	= 0x27,		// '\''
    /// A compact way of encoding [ UT_JID_BOOL 0x00 ]
    UT_JID_FALSE	= 0x30,		// '0'
    /// A compact way of encoding [ UT_JID_BOOL 0x01 ]
    UT_JID_TRUE		= 0x31,		// '1'

    /// Though gzip compression on a stream can reduce redundancy, storage of
    /// command strings can be simplified by using "token-strings".  On the
    /// first use of a binary string, a token-define can be used.  This assigns
    /// an integer value to the string.  The string can then be referenced by
    /// the token-reference identifier.
    ///
    /// A token definition is followed by: <tt>[id] [length] [string-data]</tt>
    ///
    /// If a string already exists with this id, its definition will be
    /// replaced with the new definition.
    UT_JID_TOKENDEF	= 0x2b,		// '+'
    /// A token reference is followed by: @c [id]
    /// The id refers to a string defined by the "tokendef"
    UT_JID_TOKENREF	= 0x26,		// '*'
    /// The token undefine is followed by: @c [id]
    /// Where the @c id represents the token string to undefine.
    UT_JID_TOKENUNDEF	= 0x2d,		// '-'

    /// This allows for an compact method of storing an array of uniform
    /// values.  The following byte should be one of the elementary types
    /// (bool, int8-int64, real16-real32, or string).  This is followed by the
    /// number of elements of the array (encoded as a length).  The raw data
    /// follows.
    UT_JID_UNIFORM_ARRAY	= 0x40,		// '@'

    /// @private Used in parsing only
    UT_JID_KEY_SEPARATOR	= 0x3a,		// ':'
    /// @private Used in parsing only
    UT_JID_VALUE_SEPARATOR	= 0x2c,		// ','

    /// This should be the first byte of a binary file.  It should be followed
    /// by the 4 bytes representing the ID_MAGIC_NUMBER.  If, when reading the
    /// file, the magic number is swapped (i.e. ID_MAGIC_NUMBER_SWAP), then the
    /// file was written on a machine with different endianness and all data
    /// should be byte-swapped on loading.
    UT_JID_MAGIC		= 0x7f,
} UT_JID;

/// The magic number to identify binary files (prefixed by UT_JID_MAGIC)
#define UT_JID_BINARY_MAGIC		0x624a534e
/// A swapped version of the magic number.  When reading, if the swapped magic
/// number is read, then the binary file was created on a machine with
/// different endianness and data should be byte-swapped where appropriate.
#define UT_JID_BINARY_MAGIC_SWAP	0x4e534a62	// Swapped endian

#endif
