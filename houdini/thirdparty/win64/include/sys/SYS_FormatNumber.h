/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_FormatNumber.h ( SYS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SYS_FormatNumber__
#define __SYS_FormatNumber__

#include "SYS_API.h"

#include "SYS_Types.h"

enum class SYS_FormatFlags
{
    None 	= 0x00,	///< Just output the raw number in the given base.
    AddCPrefix	= 0x01,	///< Add the C prefixes for the appropriate base.
			///  E.g. 0x for hexadecimal (base 16), 0 for octal
			///  (base 8) and 0b for binary (base 2, 
			///  a C++11 extension).
    UpperCase	= 0x02,	///< Alphabetic digits should be printed in upper-case
			///  instead of default lower-case.
    AddPlus	= 0x04,	///< Always show a sign to indicate the sign of the
			///  number. Usually only the negative sign is shown
			///  for negative numbers, but this will add a positive
			///  sign for positive numbers too.
    DigitGrouping = 0x08, ///< Add digit groupings for the integer part of the 
			/// resulting number. For decimal numbers, this is
			/// every three digits, for hexadecimal, octal and 
			/// binary numbers, this is every four digits. Other
			/// bases ignore this flag. 
};

static inline SYS_FormatFlags operator|(SYS_FormatFlags a, SYS_FormatFlags b)
{ return SYS_FormatFlags(int(a) | int(b)); }

static inline bool operator&(SYS_FormatFlags a, SYS_FormatFlags b)
{ return (int(a) & int(b)) == int(b); }


/// Write the text version of an integer value into a character buffer specified
/// by @c buffer of size @c buffer_size. The number of characters written is
/// returned.
/// Note that a string terminator value is *not* written out.
/// @param buffer The character buffer to copy the formatted number into. If 
///               this value is @c nullptr, then the value returned will be the
///               number of bytes required to store the result.
/// @param buffer_size The size of the buffer given in @c buffer. If @c buffer
///               is @c nullptr, this value is ignored.
/// @param number The number to convert to a text representation.
/// @param base   The numerical base the number will be converted to.
/// @param min_digits The minimum number of digits to write out. If this value
///               is greater than the actual number of digits needed, the
///               final value will be zero-padded after the sign charadcter.
/// @param flags  The flags specifying the formatting.
/// @return       The number of bytes written out to @c buffer. If @c buffer
///               is @c nullptr, then the size of the buffer required to write
///               out the entire value is returned.
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, int8 number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, uint8 number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, int16 number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, uint16 number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, int32 number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, uint32 number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, int64 number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);
SYS_API size_t
SYSformatInteger(char *buffer, size_t buffer_size, uint64 number,
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None);

#if defined(MBSD)
static inline size_t
SYSformatInteger(char *buffer, size_t buffer_size, size_t number, 
		 int base=10, int min_digits=0,
		 SYS_FormatFlags flags = SYS_FormatFlags::None)
{
    return SYSformatInteger(buffer, buffer_size, (uint64)number, base, min_digits, flags);
}
#endif


/// Specifies the notation to use when formatting floating point numbers.
enum class SYS_FormatNotation
{
    Shortest,		///< Print using the shortest representation, picking
			///  either fixed-point or normalized scientific
			///  notation, as needed.
			///  Corresponds to printf's '%g'/'%G' formatting codes.
    Fixed,		///< Print using fixed-point notation.
			///  Corresponds to printf's '%f'/'%F' formatting code.
    Scientific,		///< Print using normalized scientific notation. 
			///  Corresponds to printf's '%e'/'%E' formatting code.
    Hexadecimal,	///< Print using the C99 hexadecimal float notation.
			///  Corresponds to printf's '%a'/'%A' formatting code.
    Engineering,	///< Print using engineering notation. This is similar
			///  to normalized scientific notation, except the
			///  powers are always divisible by 3.
};

/// Write the text version of float value into a character buffer specified by
/// @c buffer of size @c buffer_size. The number of characters written is
/// returned. No string terminator value is written out. If the buffer given is
/// @c nullptr, then this function immediately returns the amount of space
/// needed to store the string.
SYS_API size_t
SYSformatFloat(char *buffer, size_t buffer_size, fpreal16 number, 
	       uint32 precision = 4,
	       SYS_FormatNotation notation = SYS_FormatNotation::Shortest,
	       SYS_FormatFlags flags = SYS_FormatFlags::None);

SYS_API size_t
SYSformatFloat(char *buffer, size_t buffer_size, fpreal32 number, 
	       uint32 precision = 6,
	       SYS_FormatNotation notation = SYS_FormatNotation::Shortest,
	       SYS_FormatFlags flags = SYS_FormatFlags::None);

SYS_API size_t
SYSformatFloat(char *buffer, size_t buffer_size, fpreal64 number, 
	       uint32 precision = 8,
	       SYS_FormatNotation notation = SYS_FormatNotation::Shortest,
	       SYS_FormatFlags flags = SYS_FormatFlags::None);

#endif // __SYS_FormatNumber__
