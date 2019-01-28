/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_UnicodeImpl.h (RE Library, C++)
 *
 */

#ifndef __UT_UnicodeImpl__
#define __UT_UnicodeImpl__

// TODO: Make the UTF8 portion table-driven.

#include <string.h>

#include "UT_Assert.h"
#include "UT_UnicodeTable.h"

// ============================================================================

namespace /* anonymous */ {

const UT_UnicodeCharacter &
getCharacterInfo(utf32 cp)
{
    static UT_UnicodeCharacter empty = {0,0};

    if (cp < 65536)
    {
	UT_UnicodeCharacter	*block = theUnicodeTable[cp >> 8];
	if (block)
	    return block[cp & 255];
    }
    return empty;
}

static inline bool
isASCII(utf8 c)
{
    return ((uchar(c) & 0x80) == 0);
}
    
static inline bool
isContinuation(utf8 c)
{
    return ((uchar(c) & 0xC0) == 0x80);
}

static inline bool
isValidLeading(utf8 c)
{
    return isASCII(c) ||
	   ((uchar(c) & 0xE0) == 0xC0) ||
	   ((uchar(c) & 0xF0) == 0xE0) ||
	   ((uchar(c) & 0xF8) == 0xF0);
}

static inline int
getContinuationCount(utf8 c)
{
    if ((uchar(c) & 0xE0) == 0xC0)
	return 1;
    else if ((uchar(c) & 0xF0) == 0xE0)
	return 2;
    else if ((uchar(c) & 0xF8) == 0xF0)
	return 3;
    return 0;
}

}

bool 
UT_Unicode::isUTF8(utf8 octet)
{
    return isValidLeading(octet) || isContinuation(octet);
}


const utf8 *
UT_Unicode::convert(const utf8 *str, utf32 &cp)
{
    cp = 0;
    if (!str)
	return NULL;
    
    utf8	c = *str++;

    // Quick ASCII check.
    if (isASCII(c))
    {
	cp = c;
	return str;
    }

    // Get the number of expected continuation bytes.
    int		cont_bytes = getContinuationCount(c);

    // Is the leading byte broken?
    if (cont_bytes == 0)
	return NULL;

    // The minimum value representable by continuation byte count. Any value
    // below the minimum value is illegal. This is to avoid multiple encodings
    // of the same value (e.g. '.' -> 0x
    static const utf32 least_values[4] = { 0x0, 0x80, 0x800, 0x10000 };
    utf32	least_value = least_values[cont_bytes];

    // The amount of shift is determined by the number of continuation
    // bytes. The shift is progressively reduced as more bytes are read.
    int		shift = cont_bytes * 6;
    
    // Set up the initial mask for the data in the leading byte.
    utf8	mask = 0x3F >> cont_bytes;
    
    utf32	result;
    result = 0;
    for(;;)
    {
	result |= (c & mask) << shift;

	if (cont_bytes-- == 0)
	    break;

	c = *str++;

	// Make sure the continuation byte is of the right form.
	if (!isContinuation(c))
	    return NULL;

	// Every continuation byte has the same mask but contributes
	// six bits lower than the byte before.
	mask = 0x3F;
	shift -= 6;
    }

    // If the code point is not valid, return the current string position but
    // a zero code point. I.e. the encoding is correct, but the code point
    // is not, for the given encoding.
    if (result < least_value || !isValidCodePoint(result))
	cp = replacementCodePoint();	// The replacement character
    else
	cp = result;

    return str;
}



int
UT_Unicode::convert(utf32 cp, utf8 *buf, exint buflen)
{
    if (cp < 0x00080)
    {
	if (buf && buflen >= 1)
	    buf[0] = utf8(cp);
	return 1;
    }
    else if (cp < 0x00000800)
    {
	if (buf && buflen >= 2)
	{
	    buf[0] = 0xC0 | utf8(cp >> 6);
	    buf[1] = 0x80 | utf8(cp & 0x3F);
	}
	return 2;
    }
    else if (cp < 0x00010000)
    {
	// We don't encode surrogate pairs.
	if (isSurrogatePair(cp))
	    return 0;
	
	if (buf && buflen >= 3)
	{
	    buf[0] = 0xE0 | utf8(cp >> 12);
	    buf[1] = 0x80 | utf8((cp >> 6) & 0x3F);
	    buf[2] = 0x80 | utf8(cp & 0x3F);
	}
	return 3;
    }
    else if (cp < 0x110000)
    {
	if (buf && buflen >= 4)
	{
	    buf[0] = 0xF0 | utf8(cp >> 18);
	    buf[1] = 0x80 | utf8((cp >> 12) & 0x3F);
	    buf[2] = 0x80 | utf8((cp >> 6) & 0x3F);
	    buf[3] = 0x80 | utf8(cp & 0x3F);
	}
	return 4;
    }
    else
    {
	/// 0x10FFFF is the greatest code point value allowed by Unicode and 
	// hence UTF-8 encodings.
	return 0;
    }
}

// 
namespace
{
    static utf16 norm16(utf16 c, bool big_endian)
    {
	if (big_endian)
	    return (c & 0xFF) << 8 | (c >> 8);
	else
	    return c;
    }
}

const utf16 *
UT_Unicode::convert(const utf16 *str, utf32 &cp, bool big_endian)
{
    cp = 0;
    if (!str)
	return NULL;
    
    // Check non-surrogate characters first
    utf16	c0 = norm16(str[0], big_endian);
    
    if (!isSurrogatePair(c0))
    {
	cp = utf32(c0);
	return str + 1;
    }
    
    utf16	c1 = norm16(str[1], big_endian);
    if ((c0 >= 0xD800 && c0 < 0xDC00) && (c1 >= 0xDC00 && c1 < 0xE000))
    {
	static const utf32 offset = ((0xD800 << 10) + 0xDC00) - 0x10000; 
	cp = utf32((c0 << 10) + c1) - offset;
	return str + 2;
    }
    else
    {
	// The second character wasn't a surrogate pair character, so skip
	// over the first of the invalid pair.
	cp = replacementCodePoint();
	return str + 1;
    }
}

int 
UT_Unicode::convert(utf32 cp, utf16 *buf, exint buflen)
{
    if (!isValidCodePoint(cp))
	return 0;
    
    // Characters outside the 64K range are encoded as surrogate pairs. 
    if (!isFromSupplementaryPlane(cp))
    {
	if (buf && buflen >= sizeof(utf16))
	    buf[0] = utf16(cp);
	return 2;
    }
    else 
    {
	if (buf && buflen >= sizeof(utf16[2]))
	{
	    cp -= 0x10000;
	    buf[0] = utf16(0xD800 | ((cp >> 10) & 0x03FF));   
	    buf[1] = utf16(0xDC00 | (cp & 0x03FF));   
	}
	return 4;
    }
}



utf8 *
UT_Unicode::next(utf8 *current)
{
    return const_cast<utf8 *>(next(const_cast<const utf8 *>(current)));
}

const utf8 *
UT_Unicode::next(const utf8 *current)
{
    if (!current)
	return NULL;
    
    utf8	c = *current;

    // End of string already?
    if (c == 0)
	return current;
    
    // Quick check for plain ASCII. 
    if (isASCII(c))
    {
	current++;
	return current;
    }

    int		nb_cont;
    if (isContinuation(c))
    {
	// Are we inside a continuation byte? Then we'll have to scan forward
	// until we reach a non-continuation byte or end. If we scan forward
	// more than two bytes, then the continuation is invalid.
	nb_cont = 2;
	current++;
	while (isContinuation(*current++) && nb_cont--) { }

	if (nb_cont == 0)
	    return NULL;
	
	if (!isValidLeading(*current))
	    return NULL;

	return current;
    }
    else if ((nb_cont = getContinuationCount(c)) == 0)
    {
	// We didn't encounter a valid byte. We probably got passed a non-UTF8
	// encoded string.
	return NULL;
    }

    current++;
    for (int i = 0; i < nb_cont; i++)
    {
	if (!isContinuation(*current++))
	    return NULL;
    }

    if (*current && !isValidLeading(*current))
	return NULL;

    return current;
}

utf8 *
UT_Unicode::prev(const utf8 *start, utf8 *current)
{
    return const_cast<utf8 *>(prev(start, const_cast<const utf8 *>(current)));
}

const utf8 *
UT_Unicode::prev(const utf8 *start, const utf8 *current)
{
    if (!current || !start)
	return NULL;
    
    // Already at the start (or beyond) ?
    if (start >= current)
	return NULL;

    // If the current byte is either ASCII or UTF8 leading byte, and the
    // previous is ASCII, we can skip right back to it. Otherwise the previous
    // character must be a part of an UTF8 encoding, or garbage.
    if ( isValidLeading(current[0]) && isASCII(current[-1]))
    {
	current--;
	return current;
    }

    // If we're currently on an ASCII character or a leading byte, go one back
    // and try passing over the continuation bytes until we hit a leading byte.
    if (isValidLeading(*current))
    {
	current--;
    }
    else if (!isContinuation(*current))
    {
	// Previous character is garbage. There's no safe way to go back and
	// expect the current character to be valid.
	return NULL;
    }

    int		nb_cont = 0;
    while(current >= start && isContinuation(*current))
    {
	current--;
	nb_cont++;
    }

    // We got no continuation bytes. We should've gotten at least one.
    if (nb_cont == 0)
	return NULL;

    // We passed too many continuation bytes. The encoding is garbage.
    if (nb_cont > getContinuationCount(*current))
	return NULL;
    
    return current;
}


bool
UT_Unicode::fixpos(const utf8 *start, utf8 *&current)
{
    return fixpos(start, const_cast<const utf8 *&>(current));
}

bool
UT_Unicode::fixpos(const utf8 *start, const utf8 *&current)
{
    if (isContinuation(*current))
	return prev(start, current) != NULL;

    return true;
}

exint
UT_Unicode::count(const utf8 *start, const utf8 *end)
{
    if (!start)
	return 0;

    exint	nb_cp = 0;

    UT_ASSERT(isValidLeading(*start));
    UT_ASSERT(!end || isValidLeading(*end));

    for(iterator it(start, end); !it.atEnd(); ++it)
	nb_cp++;
    return nb_cp;
}

exint
UT_Unicode::length(const utf8 *start, const utf8 *end)
{
    if (!start)
	return 0;
    
    UT_ASSERT(isValidLeading(*start));
    UT_ASSERT(!end || isValidLeading(*end));

    if (!end)
	return strlen((const char *)start);
    else
	return exint(end - start);
}

inline utf8 *
UT_Unicode::duplicate(const utf8 *start, const utf8 *end)
{
    if (!start)
	return NULL;

    UT_ASSERT(isValidLeading(*start));
    UT_ASSERT(!end || isValidLeading(*end));

    if (!end)
	return (utf8 *)::strdup((const char *)start);
    else
    {
	size_t	 length = (end - start);
	utf8 	*buf = (utf8 *)malloc(length + 1);


	::memcpy(buf, start, length);
	buf[length] = '\0';
	return buf;
    }
}


const utf8 *
UT_Unicode::find(utf32 cp, const utf8 *start, const utf8 *end)
{
    if (cp == 0 || !start)
	return NULL;

    UT_ASSERT(isValidLeading(*start));
    UT_ASSERT(!end || isValidLeading(*end));

    if (isASCII(cp))
    {
	if (!end)
	    return (const utf8 *)strchr((const char *)start, char(cp));
	else
	{
	    while(start < end && *start != cp)
		start++;
	    return start == end ? NULL : start;
	}
    }
    else
    {
	for(;;)
	{
	    const utf8	*pos = start, *next;
	    utf32	 ccp;

	    next = convert(start, ccp);
	    if (!ccp)
		break;

	    if (cp == ccp)
		return pos;

	    pos = next;
	}
	return NULL;
    }
}

const utf8 *
UT_Unicode::find(const utf8 *str, const utf8 *start, const utf8 *end)
{
    if (!str || !start)
	return NULL;

    UT_ASSERT(isValidLeading(*start));
    UT_ASSERT(!end || isValidLeading(*end));

    if (!end)
    {
	return (const utf8 *)::strstr((const char *)start, (const char *)str);
    }
    else
    {
	size_t		 len = ::strlen((const char *)str);

	// If the section given is shorter than the string to search for,
	// or the search string is empty, bail early.
	if (!len || (end - start) < len)
	    return NULL;

	const utf8 	*find = str;
	while(start < (end - len))
	{
	    if (*find == *start)
	    {

//		if (strncmp())
		return NULL;
	    }

	    start = next(start);
	}
    }
    return NULL;
}





bool
UT_Unicode::isSpace(utf32 cp, bool break_only)
{
    const UT_UnicodeCharacter	&c = getCharacterInfo(cp);
    return (c.myCategory == UT_UNICODE_SPACE ||
	    (!break_only && c.myCategory == UT_UNICODE_SPACE_NONBREAK));
}

bool
UT_Unicode::isDigit(utf32 cp)
{
    return getCharacterInfo(cp).myCategory == UT_UNICODE_NUMBER;
}

bool
UT_Unicode::isAlpha(utf32 cp)
{
    return isUpper(cp) || isLower(cp);
}

bool
UT_Unicode::isAlnum(utf32 cp)
{
    return isAlpha(cp) || isDigit(cp);
}

bool
UT_Unicode::isPunct(utf32 cp)
{
    return getCharacterInfo(cp).myCategory == UT_UNICODE_PUNCTUATION;
}

bool
UT_Unicode::isUpper(utf32 cp)
{
    const UT_UnicodeCharacter	&c = getCharacterInfo(cp);
    return c.myCategory == UT_UNICODE_LT_UPPER ||
	   c.myCategory == UT_UNICODE_LT_TITLE;
}

bool
UT_Unicode::isLower(utf32 cp)
{
    return getCharacterInfo(cp).myCategory == UT_UNICODE_LT_LOWER;
}

/// 
bool UT_Unicode::isCJK(utf32 cp)
{
    return (cp >= 0x04E00 && cp <= 0x09FFF) ||	// CJK Unified Ideographs
	   (cp >= 0x03400 && cp <= 0x04DBF) ||	// - Extension A
	   (cp >= 0x20000 && cp <= 0x2A6D6);	// - Extension B
}


utf32
UT_Unicode::toLower(utf32 cp)
{
    const UT_UnicodeCharacter	&c = getCharacterInfo(cp);
    if (c.myComplement &&
	(c.myCategory == UT_UNICODE_LT_UPPER ||
	 c.myCategory == UT_UNICODE_LT_TITLE))
	return c.myComplement;
    return cp;
}

utf32
UT_Unicode::toUpper(utf32 cp)
{
    const UT_UnicodeCharacter	&c = getCharacterInfo(cp);
    if (c.myComplement &&
	c.myCategory == UT_UNICODE_LT_LOWER)
	return c.myComplement;
    return cp;
}


// UT_Unicode::iterator
UT_Unicode::iterator::iterator()
{
    init(NULL, NULL, NULL);
}

UT_Unicode::iterator::iterator(const utf8 *start, const utf8 *end,
                               const UT_Unicode::transform *transform)
{
    init(start, end, transform);
}

UT_Unicode::iterator::iterator(const UT_StringView &str, 
                               const UT_Unicode::transform *transform)
{
    init(str.begin(), str.end(), transform);
}

void UT_Unicode::iterator::init(const utf8 *start, const utf8 *end,
                                const UT_Unicode::transform *transform)
{
    myCurrent = NULL;
    myCP = myNextCP = 0;
    myTransform = transform;

    if (start && isValidLeading(*start) && (!end || start < end))
    {
	myStart = start;
	myEnd = end;
	reset();
    }
    else
    {
	myStart = myEnd = myNext = myNext2 = NULL;
    }
}


void
UT_Unicode::iterator::reset(const utf8 *to)
{
    if (!myStart)
	return;
    
    if (!to)
	to = myStart;
    else if (to < myStart || (myEnd && to > myEnd))
	return;
    
    // Make sure we start at a decent place.
    UT_Unicode::fixpos(myStart, to);
    
    myCurrent = to;
    myNext = convert(myCurrent, myCP);
    if (!myCP)
	myNext = NULL;
    else
    {
	if (myTransform && myCP)
	    myCP = myTransform->transformCodepoint(myCP);
	if (!myEnd || (myNext < myEnd))
	{
	    myNext2 = convert(myNext, myNextCP);
	    if (myTransform && myNextCP)
		myNextCP = myTransform->transformCodepoint(myNextCP);
	}
	else
	    myNextCP = 0;
    }
}

bool
UT_Unicode::iterator::advance()
{
    // Invalid iterator or at the end already?
    if (!myStart || !myCP || (myEnd && myCurrent >= myEnd))
	return true;

    myCP = myNextCP;
    myCurrent = myNext;
    myNext = myNext2;
    if (myCP)
    {
	if(!myEnd || (myNext < myEnd))
	{
	    myNext2 = convert(myNext, myNextCP);
	    if (myTransform && myNextCP)
		myNextCP = myTransform->transformCodepoint(myNextCP);
	}
	else
	    myNextCP = 0;
	return true;
    }
    else
	return false;
}

bool
UT_Unicode::iterator::retreat()
{
    // Invalid iterator or at the start already?
    if (!myStart || myCurrent == myStart)
	return false;

    const utf8	*prev;
    utf32	 cp;

    prev = UT_Unicode::prev(myStart, myCurrent);
    UT_Unicode::convert(prev, cp);
    if (prev && cp)
    {
	if (myTransform && cp)
	    cp = myTransform->transformCodepoint(cp);
	myNextCP = myCP;
	myCP = cp;
	myNext2 = myNext;
	myNext = myCurrent;
	myCurrent = prev;
	return true;
    }
    else
	return false;
}

#endif // __UT_UnicodeImpl__
