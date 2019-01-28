/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Unicode.h (RE Library, C++)
 *
 */

#ifndef __UT_Unicode__
#define __UT_Unicode__

#include <SYS/SYS_Types.h>
#include "UT_StringView.h"

/// Specifies the maximum possible size of a UTF8 encoding for any given
/// code point.
#define UT_UTF8_MAX_ENCODING_LEN	4

// Helper function to cast unsigned char literals to UTF-8
static inline const utf8 *UTF8(const uchar *lit)
{
    return reinterpret_cast<const utf8 *>(lit);
}

/// Helper functions for Unicode and the UTF-8 variable length encoding.
class UT_Unicode
{
public:
    /// Parses a code point from a UTF-8 encoding and returns it as a single
    /// code point value. Returns a pointer to the next encoding if the
    /// current one got successfully decoded. If the decoding fails, it
    /// return @c NULL and cp is set to zero.
    static inline const utf8 *convert(const utf8 *str, utf32 &cp);

    /// Converts a code point to its UTF-8 encoding. If no buffer is given,
    /// returns the number of characters needed to store the resulting
    /// encoded sequence. Does not write out a terminating zero but moves the 
    /// pointer to where the next character after the sequence should be
    /// written.
    static inline int convert(utf32 cp, utf8 *str, exint buflen);

    /// Given a current location in a buffer, moves to the next character.
    /// If the location is inside a UTF-8 multi-character encoding (i.e not at
    /// the beginning of one), it moves to the next encoded character start
    /// after.
    /// If the current location is already at the terminating NUL character
    /// the function does nothing and just returns the current pointer.
    /// If it is unable to move successfully to the next encoded character (e.g.
    /// it's already at the end of the string, or the encoding is garbage and
    /// no recovery is possible) the function returns NULL.
    static inline const utf8 *next(const utf8 *current);
    static inline utf8 *next(utf8 *current);

    /// Given a location in a buffer, moves to the to the previous character,
    /// unless already at the beginning of the string, as defined by 'start'.
    /// If the location is inside a UTF-8 multi-character encoding, it moves to
    /// the beginning of that encoding.
    /// If going back lands on an invalid character, it encounters bad
    /// encoding (e.g. too many continuation bytes), or it's already at the
    /// start, the function returns NULL.
    static inline const utf8 *prev(const utf8 *start, const utf8 *current);
    static inline utf8 *prev(const utf8 *start, utf8 *current);

    /// Given a pointer inside of a string representing variable length
    /// encoding, moves the pointer so that it points to the beginning of the
    /// encoding, if not there already.
    /// Returns @c false if it was unable to fix the position and @c true if
    /// successful or the position was already valid.
    static inline bool fixpos(const utf8 *start, const utf8 *&current);
    static inline bool fixpos(const utf8 *start, utf8 *&current);

    /// Returns the number of code points this variable encoding represents.
    static inline exint count(const utf8 *start, const utf8 *end = 0);

    /// Returns the number of octets for this variable encoding. One octet
    /// is the same as a byte for UTF-8 encodings.
    static inline exint length(const utf8 *start, const utf8 *end = 0);

    /// Duplicates the string using malloc. Use free() to free the resulting
    /// string. If a NULL pointer is passed, a NULL pointer is returned.
    static inline utf8 *duplicate(const utf8 *start, const utf8 *end = 0);

    /// Find a code point in a variable length string and return a pointer to
    /// it. An optional end point can be supplied, which delineates a search
    /// range. Otherwise the string is searched up to the terminating NUL.
    static inline const utf8 *find(utf32 cp,
                                   const utf8 *start, const utf8 *end = 0);
    
    /// Find a UTF8 encoded string in another UTF8 encoded string and return
    /// a pointer to the start of the match. Returns NULL if the string
    /// was not found.
    static inline const utf8 *find(const utf8 *str,
                                   const utf8 *start, const utf8 *end = 0);

    
    /// Parses a code point from a UTF-16 encoding and returns it as a single
    /// code point value. Returns a pointer to the next encoding if the
    /// current one got successfully decoded. If the decoding fails, it
    /// return @c NULL and cp is set to zero.
    /// Set @c big_endian to true if the incoming UTF-16 string is encoded as
    /// big endian (UTF-16BE).
    static inline const utf16 *convert(const utf16 *str, utf32 &cp, 
                                       bool big_endian = false);

    /// Converts a code point to its UTF-16LE encoding into the buffer given. 
    /// If no buffer is given, or if the buffer size is too small, returns the 
    /// number of bytes needed to store the resulting encoded sequence.
    /// @c buflen should be given in bytes, and not number of utf16 entries.
    /// Does not write out a terminating zero but moves the pointer to where 
    /// the next character after the sequence should be written. 
    static inline int convert(utf32 cp, utf16 *str, exint buflen);
    
    /// Returns the replacement character, which is returned by the convert
    /// functions, when they encounter an invalid, but recoverable, encoding.
    static inline utf32 replacementCodePoint()
    {
	return 0xFFFD;
    }
    
    /// Returns @c true if the code point given is a surrogate pair. This is
    /// valid UTF-16 character, since it is used to encode greater-than 0xFFFF
    /// code points. It is not a valid UTF-32 code point, however.
    static inline bool isSurrogatePair(utf32 cp)
    {
	return cp >= 0xD800 && cp < 0xE000;
    }
    
    static inline bool isFromSupplementaryPlane(utf32 cp)
    {
	return cp >= 0x10000 && cp <= 0x10FFFF;
    }
    
    // Returns true if the given value is a valid Unicode code point.
    static inline bool isValidCodePoint(utf32 cp)
    {
	// Unicode is specified up to 0x10FFFF. Surrogate pairs are only valid
	// for UTF-16, not for UTF-32.
	return !isSurrogatePair(cp) && cp <= 0x10FFFF;
    }

    // Returns true if the code point represents a control character (newline,
    // tab, etc).
    static inline bool isControlChar(utf32 cp)
    {
	return cp <= 0x1F || cp == 0x7F;
    }

    // Code point validation for different character sets.
    static inline bool isASCII(utf32 cp)
    {
	return cp <= 0x7F;
    }

    static inline bool isLatin1(utf32 cp)
    {
	return cp <= 0xFF;
    }

    // Character type queries.
    static inline bool isSpace(utf32 cp, bool break_only = true);
    static inline bool isDigit(utf32 cp);
    static inline bool isAlpha(utf32 cp);
    static inline bool isAlnum(utf32 cp);
    static inline bool isPunct(utf32 cp);
    static inline bool isUpper(utf32 cp);
    static inline bool isLower(utf32 cp);
    
    /// Returns true if the character is from any of the Unicode CJK Unified 
    /// Ideographs blocks.
    static inline bool isCJK(utf32 cp);
    static inline utf32 toLower(utf32 cp);
    static inline utf32 toUpper(utf32 cp);
    
    // Returns true if the byte given is a valid UTF-8 octet (any position).
    static inline bool isUTF8(utf8 octet);

    class transform
    {
    public:
        /// Transform a code point from one to another.
        virtual utf32 transformCodepoint(utf32 cp) const = 0;

        virtual ~transform() {};
    };


    /// Simple string iterator to iterate over an UTF-8 string and peel off
    /// the code points in sequence. This iterator can always get the 'next'
    /// code point, which is useful for function which require sequential
    /// code point pairs, such as kerning.
    /// The iterator terminates when the current character is nil, invalid,
    /// or at the 'end' pointer.
    class iterator
    {
    public:
	inline iterator();

	// Create a new iterator starting at 'start' and, optionally, ending
	// at 'end'. If end is not given, the string will stop iterating
	// at the terminating NUL or if it hits an invalid encoding. If 'end'
	// is given it should be a valid pointer and follow 'start' in memory.
	inline iterator(const utf8 *start, const utf8 *end = 0,
	                const UT_Unicode::transform *transform = 0);
	
	inline iterator(const UT_StringView &str, 
	                const UT_Unicode::transform *transform = 0);

	inline void reset(const utf8 *to = 0);
	inline bool advance();
	inline bool retreat();

	/// Returns the pointer to the current UTF-8 sequence. Valid if atEnd
	/// is not false.
	const utf8 *at() const		{ return myCurrent; }

	// Returns a pointer to the next UTF-8 sequence, or the terminating NUL.
	// Valid if atEnd is not false.
	const utf8 *next() const	{ return myNext; }

	// Returns the code point of the encoding at the current location.
	utf32 getCP() const		{ return myCP; }

	// Returns the next code point of the encoding following the current
	// one, unless it happens to be invalid, in which case it returns zero.
	utf32 getNextCP() const		{ return myNextCP; }

	bool atEnd() const 		{ return myCP == 0; }

	iterator &operator++()		{ advance(); return *this; }
	iterator &operator--()		{ retreat(); return *this; }

    private:
	inline void init(const utf8 *start, const utf8 *end,
	                 const UT_Unicode::transform *transform);
	
	iterator &operator++(int);	// Post-increment is verboten

	const utf8 		*myStart, *myEnd;
	const utf8		*myCurrent, *myNext, *myNext2;
	utf32		 	 myCP, myNextCP;
	const UT_Unicode::transform *myTransform;
    };
};

#include "UT_UnicodeImpl.h"

#endif // __UT_Unicode__
