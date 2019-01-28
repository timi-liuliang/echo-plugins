/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_String.h (SYS Library, C++)
 *
 * COMMENTS:
 * 	System-independent string manipulation functions.
 */

#ifndef __SYS_String__
#define __SYS_String__

#include "SYS_API.h"

#include "SYS_Inline.h"
#include "SYS_Types.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/// Append to a hash for a char string
static inline uint32
SYSstring_hashseed(const char *str, exint length = SYS_EXINT_MAX, uint32 hash = 0)
{
    if (str != nullptr && length >= 0 && *str)
    {
        // A note on the magic number 37.
        // We want to scale by SOMETHING so that order is preserved.
        // That something should be prime and not a power of two to
        // avoid wrapping issues.
        // That something should be larger than our range of expected
        // values to avoid interference between consecutive letters.
        // 0-9a-z is 36 letters long.
        //
        // The real reason is that this is what Perl uses.
        do
        {
            hash = (37 * hash) + (*str);
            ++str;
            --length;
        } while (length >= 0 && *str);

	// Make sure we never return zero for non-zero hash, since in many
	// cases we precompute string hashes and zero means "not initialized".
	// This allows us conforming values across different string
	// container implementations.
	if (hash == 0)
	    hash = 1;
    }

    return hash;
}

/// Generate a hash for a char string
static inline uint32
SYSstring_hash(const char *str, exint len = SYS_EXINT_MAX)
{
    return SYSstring_hashseed(str, len, 0);
}


/// A constexpr version of the above. In C++11 we can't use iteration, but
/// we can do recursion. Go figure.
namespace 
{
    static inline constexpr uint32
    SYSstring_hash_inner(const char *str, uint32 h)
    {
	return str[0] ? SYSstring_hash_inner(str+1, str[0] + h * 37) : h;
    }
}

static inline constexpr uint32
SYSstring_hash_literal(const char *str)
{
    return str[0] 
       ? (SYSstring_hash_inner(str, 0) == 0 ? 1 : SYSstring_hash_inner(str, 0))
       : 0;
}


/// A standard name for a strtok that doesn't maintain state between calls.
/// This version is thus both reentrant and threadsafe.
/// SYSstrtok parses a string into a sequence of tokens. On the first call to
/// SYSstrtok, the string to be parsed must be specified as the parameter
/// 'string'. This parameter *will be modified* (destroying your copy).
/// 'delimit' specifies an array of single characters that will be used
/// as delimiters.
/// 'context' is a char * variable used internally by SYSstrtok to maintain
/// context between calls. Subsequent calls must specify the same unchanged
/// context variable as the first call.
/// To use SYSstrtok, on the first call first pass in your string as the 
/// parameter 'string'; on subsequent calls, pass it in as nullptr.
/// SYSstrtok returns non-empty strings pointing to the first non-delimiter
/// character of each token, or nullptr if no further tokens are available.
/// Example:
/// @code
/// char *string = strdup(getString());
/// char *strptr = string;
/// char *context;
/// char *token = SYSstrtok(string, MY_DELIMITERS, &context);
/// while (token)
/// {
///	do_some_stuff();
///	SYSstrtok(nullptr, MY_DELIMITERS, &context);
/// }
/// free(strptr);
/// @endcode
inline char *
SYSstrtok(char *string, const char *delimit, char **context)
{
#ifdef LINUX
    return strtok_r(string, delimit, context);
#else
    // MSVC 2003 doesn't have strtok_r. 2005 has strtok_s, which is the same
    // as strtok_r. Until we upgrade, use this C version of strtok_r.
    if (string == nullptr)
    {
	string = *context;
    }

    // Find and skip any leading delimiters.
    string += strspn(string, delimit);

    // There are only delimiters (or no text at all), so we've reached the end
    // of the string.
    if (*string == '\0')
    {
	*context = string;
	return nullptr;
    }

    // String now points at a token.
    char *token = string;

    // Find the end of the token.
    string = strpbrk(token, delimit);
    if (!string)
    {
	// This token is at the end of the string. Set the context to point at
	// the end of the string so on the next call, we'll return nullptr.
	*context = strchr(token, '\0');
    }
    else
    {
	// This is a token somewhere in the string. Set the found delimiter to
	// zero and initialize the context to the next character.
	*string = '\0';
	*context = string + 1;
    }

    return token;
#endif
}

/// The semantics for strncpy() leave a little to be desired
///  - If the buffer limit is hit, the string isn't guaranteed to be null
///	terminated.
///  - If the buffer limit isn't hit, the entire remainder of the string is
///	filled with nulls (which can be costly with large buffers).
/// The following implements the strlcpy() function from OpenBSD.  The function
/// is very similar to strncpy() but
///	The return code is the length of the src string
///	The resulting string is always null terminated (unless size == 0)
/// 	The remaining buffer is not touched
/// It's possible to check for errors by testing rcode >= size.
///
/// The size is the size of the buffer, not the portion of the sub-string to
/// copy.  If you want to only copy a portion of a string, make sure that the
/// @c size passed in is one @b larger than the length of the string since
/// SYSstrlcpy() will always ensure the string is null terminated.
///
/// It is invalid to pass a size of 0.
///
/// Examples: @code
///	char	buf[8];
///	strncpy(buf, "dog", 8)    // buf == ['d','o','g',0,0,0,0,0]
///	SYSstrlcpy(buf, "dog", 8) // buf == ['d','o','g',0,?,?,?,?]
///	strncpy(buf, "dog", 2)    // buf == ['d','o',0,0,0,0,0,0]
///	SYSstrlcpy(buf, "dog", 2) // buf == ['d',0,?,?,?,?,?,?]
///	SYSstrlcpy(buf, "dog", 3) // buf == ['d','o',0,?,?,?,?]
/// @endcode
inline size_t
SYSstrlcpy(char *dest, const char *src, size_t size)
{
    char *end = (char *)::memccpy(dest, src, 0, size);
    if (end)
    {
	return end - dest - 1;
    }
    // No null terminator found in the first size bytes
    if (size)
	dest[size-1] = 0;
    
    // Return rcode >= size to indicate that we would've busted the buffer.
    return size + 1;
}

/// The following implements the strlcpy() function from OpenBSD.  The
/// differences between strlcpy() and strncpy() are:
///	- The buffer will not be filled with null
///	- The size passed in is the full length of the buffer (not
///		remaining length)
///	- The dest will always be null terminated (unless it is already larger
///		than the size passed in)
/// The function returns strln(src) + SYSmin(size, strlen(dest))
/// If rcode >= size, truncation occurred
inline size_t
SYSstrlcat(char *dest, const char *src, size_t size)
{
    // Find the length of the dest buffer.  Only check for a null within the
    // allocated space of the buffer (i.e. we can't use strlen()).
    size_t dlen;
    for (dlen = 0; dlen < size; dlen++)
	if (!dest[dlen])
	    break;
    if (dlen == size)
	return size + 1;	// Not enough space left
    // Now, copy the source over
    return dlen + SYSstrlcpy(dest+dlen, src, size-dlen);
}

inline int
SYSstrcasecmp(const char *a, const char *b)
{
#if defined(WIN32)
    return ::stricmp(a, b);
#else
    return ::strcasecmp(a, b);
#endif
}

inline int
SYSstrncasecmp(const char *a, const char *b, size_t n)
{
#if defined(WIN32)
    return ::strnicmp(a, b, n);
#else
    return ::strncasecmp(a, b, n);
#endif
}

/// Replacement for strcasestr, since no equivalent exists on Win32.
inline char *
SYSstrcasestr(const char *haystack, const char *needle)
{
#if defined(WIN32)
    // Designed for the normal case (small needle, large haystack).
    // Asymptotic cases will probably perform very poorly. For those, we'll
    // need: https://en.wikipedia.org/wiki/Boyer-Moore-Horspool_algorithm
     if (!haystack || !needle)
	return nullptr;

    // Empty needle gives beginning of string.
    if (!*needle)
	return const_cast<char *>(haystack);
    for(;;)
    {
	// Find the start of the pattern in the string.
	while(*haystack && tolower(*haystack) != tolower(*needle))
	    haystack++;

	if (!*haystack)
	    return nullptr;

	// Found the start of the pattern.
	const char *h = haystack, *n = needle;
	do
	{
	    // End of needle? We found our man.
	    if (!*++n)
		return const_cast<char *>(haystack);
	    // End of haystack? Nothing more to look for.
	    if (!*++h)
		return nullptr;
	} while(tolower(*h) == tolower(*n));

	haystack++;
    }
#else
    return const_cast<char*>(::strcasestr(const_cast<char*>(haystack),needle));
#endif
}

// Implementation of strndup for Windows.
inline char *
SYSstrndup(const char *s, size_t n)
{
#if defined(WIN32)
    size_t	 l = ::strlen(s);
    if (l < n) n = l;
    char	*r = (char *)::malloc(n + 1);
    ::memcpy(r, s, n);
    r[n] = '\0';
    return r;
#else
    return ::strndup(s, n);
#endif
}

// On Windows, is*() methods are badly implemented.
// Running testut -i -t SYS_String shows about at least a 1.3x speed up.
#ifdef _WIN32
SYS_FORCE_INLINE bool
SYSisalpha(unsigned char c)
{
    // This test relies on promoting to unsigned integer
    return (unsigned(c & ~(1<<5)) - 'A') <= ('Z' - 'A');
}
SYS_FORCE_INLINE bool
SYSisdigit(unsigned char c)
{
    // Interestingly, this tends to perform better than one comparison
    return (c >= '0' && c <= '9');
}
#endif // _WIN32

// Windows decided in their infinite wisdom that negative values
// should crash their isfoo() functions, guard by only taking unsigned char
// arguments which get casted again to int's.

#define SYS_IS_WRAPPER(TEST)	\
SYS_FORCE_INLINE bool		\
SYS##TEST(unsigned char c)	\
{				\
    return TEST(c);		\
}				\
/**/

SYS_IS_WRAPPER(isalnum)
#ifndef _WIN32
SYS_IS_WRAPPER(isalpha)
#endif
// isascii is specifically marked deprecated
// SYS_IS_WRAPPER(isascii)
// This does have a POSIX standard, but isn't in Windows.
// SYS_IS_WRAPPER(isblank)
SYS_IS_WRAPPER(iscntrl)
#ifndef _WIN32
SYS_IS_WRAPPER(isdigit)
#endif
SYS_IS_WRAPPER(isgraph)
SYS_IS_WRAPPER(islower)
SYS_IS_WRAPPER(isprint)
SYS_IS_WRAPPER(ispunct)
// isspace is rather important we get very, very, fast.
// SYS_IS_WRAPPER(isspace)
SYS_IS_WRAPPER(isupper)
SYS_IS_WRAPPER(isxdigit)

#undef SYS_IS_WRAPPER


#define CREATE_SYSisspace(TYPE)		\
inline bool				\
SYSisspace(TYPE c)			\
{					\
    /* Fastest exit for non-spaces. */	\
    if (c > ' ')			\
	return false;			\
    /* Either equal to space, or between tab and carriage return */ \
    return (c == ' ' || (c <= '\xd' && c >= '\x9'));	\
}

CREATE_SYSisspace(int)
CREATE_SYSisspace(unsigned char)
CREATE_SYSisspace(signed char)

#undef CREATE_SYSisspace


#endif
