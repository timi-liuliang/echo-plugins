/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_ParseNumber.h ( SYS Library, C++)
 *
 * COMMENTS:	A comprehensive set of functions to parse numbers, 
 * 		integers and floating point.
 */

#ifndef __SYS_ParseNumber__
#define __SYS_ParseNumber__

#include "SYS_API.h"

#include "SYS_Types.h"

/// List of possible states the parsing ended in.
enum class SYS_ParseStatus
{
    Success,		///< A number was successfully parsed.
    NoNumberFound,	///< No numeric characters were found. A value of zero
			///  is returned and @c end will be set to the value of
			///  @c begin.
    InvalidArgs,	///< Invalid arguments were passed in (e.g. bad number base, invalid range).
    Overflow		
};

enum class SYS_ParseFlags
{
    None 		= 0x00,	///< Default use.
    DigitSeparator 	= 0x01,	///< Allow underscore ('_') to be used as a digit separator.
};

static inline SYS_ParseFlags operator|(SYS_ParseFlags a, SYS_ParseFlags b)
{ return SYS_ParseFlags(int(a) | int(b)); }

static inline bool operator&(SYS_ParseFlags a, SYS_ParseFlags b)
{ return (int(a) & int(b)) == int(b); }

/// Parse a string, delimited by @c begin and @c end , into an integer of a
/// certain bit size.
/// Unlike @c strtod and family, @c end should always be a valid pointer to the
/// character one past the last character to be parsed, or @c nullptr. If @c
/// end is nullptr, then the string will be parsed until the end of the string,
/// which, in that case, should be zero-terminated.
/// Upon return, @c end will point to the next character after the last one
/// parsed.  By default the number base is automatically determined by the
/// number prefix (if any), although a specific base can be given to override
/// this automatic choice.  The range of supported bases is from 2 to 36,
/// inclusive. A value of zero can be given for the function to automatically
/// determine the base, from the prefix. In the absence of prefix the number is
/// assumed to be decimal. Invalid @c base values will cause the function to
/// return immediately with @c SYS_ParseStatus::InvalidArgs.
/// @{
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, int8 &number, int base=0, 
                SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, uint8 &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, int16 &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, uint16 &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, int32 &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, uint32 &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, int64 &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, uint64 &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None);
// Some systems have size_t as a seperate type from uint.  Some don't.
#if defined(MBSD)
static inline SYS_ParseStatus
SYSparseInteger(const char *begin, const char *&end, size_t &number, int base=0,
                SYS_ParseFlags flags = SYS_ParseFlags::None)
{
    uint64 num64 = number;
    SYS_ParseStatus result = SYSparseInteger(begin, end, num64, base, flags);
    number = num64;
    return result;
}
#endif
/// @}

/// Parse a delimited string into a float of a certain bit size. 
/// If @c end is nullptr, then the string will be parsed until end. Upon return,
/// @c end will point to the character after the last one parsed. 
/// @{
SYS_API SYS_ParseStatus
SYSparseFloat(const char *begin, const char *&end, fpreal16 &number,
              SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseFloat(const char *begin, const char *&end, fpreal32 &number,
              SYS_ParseFlags flags = SYS_ParseFlags::None);
SYS_API SYS_ParseStatus
SYSparseFloat(const char *begin, const char *&end, fpreal64 &number,
              SYS_ParseFlags flags = SYS_ParseFlags::None);
/// @}


// Compatibility shims
static inline int32
SYSatoi32(const char *buf, int base=10)
{
    int32	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    return n;
}

static inline uint32
SYSatou32(const char *buf, int base=10)
{
    uint32	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    return n;
}

static inline int64
SYSatoi64(const char *buf, int base=10)
{
    int64	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    return n;
}

static inline uint64
SYSatou64(const char *buf, int base=10)
{
    uint64	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    return n;
}

static inline int32
SYSstrtoi32(const char *buf, char **endptr=0, int base=0)
{
    int32	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    if (endptr)
	*endptr = const_cast<char *>(end);
    return n;
}
static inline uint32
SYSstrtou32(const char *buf, char **endptr=0, int base=0)
{
    uint32	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    if (endptr)
	*endptr = const_cast<char *>(end);
    return n;
}
static inline int64
SYSstrtoi64(const char *buf, char **endptr=0, int base=0)
{
    int64	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    if (endptr)
	*endptr = const_cast<char *>(end);
    return n;
}
static inline uint64
SYSstrtou64(const char *buf, char **endptr=0, int base=0)
{
    uint64	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base);
    if (endptr)
	*endptr = const_cast<char *>(end);
    return n;
}

static inline fpreal64
SYSstrtod(const char *buf, char **endptr=0)
{
    fpreal64	n = 0;
    const char	*end = nullptr;
    SYSparseFloat(buf, end, n);
    if (endptr)
	*endptr = const_cast<char *>(end);
    return n;
}

static inline fpreal32
SYSstrtof32(const char *buf, char **endptr=0)
{
    fpreal32	n = 0;
    const char	*end = nullptr;
    SYSparseFloat(buf, end, n);
    if (endptr)
	*endptr = const_cast<char *>(end);
    return n;
}

static inline fpreal64
SYSstrtof64(const char *buf, char **endptr=0)
{
    fpreal64	n = 0;
    const char	*end = nullptr;
    SYSparseFloat(buf, end, n);
    if (endptr)
	*endptr = const_cast<char *>(end);
    return n;
}

static inline fpreal32
SYSatof32(const char *buf)
{
    fpreal32	n = 0;
    const char	*end = nullptr;
    SYSparseFloat(buf, end, n);
    return n;
}

static inline fpreal64
SYSatof64(const char *buf)
{
    fpreal64	n = 0;
    const char	*end = nullptr;
    SYSparseFloat(buf, end, n);
    return n;
}

static inline  int SYSstrtoi(const char *buf) { return SYSstrtoi32(buf); }
static inline uint SYSstrtou(const char *buf) { return SYSstrtou32(buf); }

static inline int SYSatoi(const char *buf, int base=10)
	    { return SYSatoi32(buf, base); }
static inline uint SYSatou(const char *buf, int base=10)
	    { return SYSatou32(buf, base); }

static inline fpreal
SYSatof(const char *buf)
{
    fpreal	n = 0;
    const char	*end = nullptr;
    SYSparseFloat(buf, end, n);
    return n;
}


/// Convert string to int, but allow for '_' as digits separator (1_332_344)
static inline int64
SYSstrtoi_underscore(const char *buf, int base=10)
{
    int64	n = 0;
    const char	*end = nullptr;
    SYSparseInteger(buf, end, n, base, SYS_ParseFlags::DigitSeparator);
    return n;
}


/// Convert string to real, but allow for '_' as digits separator (1_024.34)
static inline fpreal64
SYSstrtod_underscore(const char *buf)
{
    fpreal64	n = 0;
    const char	*end = nullptr;
    SYSparseFloat(buf, end, n, SYS_ParseFlags::DigitSeparator);
    return n;
}

#endif // __SYS_ParseNumber__
