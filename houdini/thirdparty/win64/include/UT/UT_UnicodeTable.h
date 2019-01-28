/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_UnicodeTable.h ( UI Library, C++)
 *
 * COMMENTS:	A table of all Unicode characters to implement Unicode-aware
 * 		ctype emulations (isspace, isalnum, etc.)
 */

#ifndef __UT_UnicodeTable__
#define __UT_UnicodeTable__

#include "UT_API.h"

// Unicode is huge. For our purposes, we only make use of the Basic
// Multilingual Plane (0x0000 -> 0xFFFF). A lot of that is empty for 
// Categorisation purposes, and so we divide into two tables: one which is
// 256 entries and points to the lower byte portion of the BMP. If the lower
// byte portion is defined, it will also contain 256 entries.

// The categorisation of the given character.
// See ftp://ftp.unicode.org/Public/3.0-Update1/UnicodeData-3.0.1.html#General Category
// for details.
enum UT_UnicodeCategory
{
    UT_UNICODE_NONE,		// No classification
    UT_UNICODE_LT_UPPER,	// Letter - Upper case [Lu]
    UT_UNICODE_LT_LOWER,	// Letter - Lower case [Ll]
    UT_UNICODE_LT_TITLE,	// Letter - Title case [Lt]
    UT_UNICODE_NUMBER,		// Number (Decimal, letter, other) [Nd, Nl, No]
    UT_UNICODE_SPACE,		// Break Space (horizontal only) [Zs]
    UT_UNICODE_SPACE_NONBREAK,	// Non-break Space (horizontal only) [Zs]
    UT_UNICODE_PUNCTUATION	// Punctuation [P*]
};

struct UT_UnicodeCharacter
{
    // Upper case complementary Unicode character, if this one is lower,
    // and vice versa. In case of title-case characters, we give the lower
    // case.
    unsigned int	myComplement:16;
    
    // See UT_UnicodeCategory
    unsigned int	myCategory:4;
};

UT_API extern UT_UnicodeCharacter *theUnicodeTable[256];

#endif // __UT_UnicodeTable__
