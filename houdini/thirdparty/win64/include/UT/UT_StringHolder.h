/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_StringHolder.h
 *
 * COMMENTS:	A simple holder for strings which acts like copy on
 *		write, but has special methods for minimizing memory
 *		copies when you know what you are doing.
 *
 *		Has a trivial move constructor.
 *
 *		c_str does not change when it moves.
 */

// #pragma once

#ifndef __UT_StringHolder_h__
#define __UT_StringHolder_h__

#include "UT_API.h"
#include "UT_Format.h"
#include "UT_String.h"
#include "UT_StringView.h"
#include "UT_Swap.h"

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Pragma.h>
#include <SYS/SYS_StaticAssert.h>
#include <SYS/SYS_Types.h>

#include <functional>
#include <iosfwd>

#include <stdlib.h>

class UT_IStream;
class UT_OStream;
class UT_StringHolder;
class UT_StringRef;
class UT_WorkBuffer;

// Forward declare the UDSL so that we can friend it inside of UT_StringHolder.
namespace UT { inline namespace Literal {
UT_StringHolder operator"" _sh(const char *s, std::size_t const length);
} }

/// A string literal with length and compile-time computed hash value.
/// @note This will only be compile-time when used in compile time expressions
/// like template parameters and constexpr variables.
/// @note String literals whose length requires more than 31 bits not supported
/// @see UT_StringRef
class UT_StringLit
{
public:
    constexpr SYS_FORCE_INLINE
    UT_StringLit()
	: myData(nullptr)
	, myLength(0)
	, myHash(0)
    {
    }

    template <size_t N>
    constexpr SYS_FORCE_INLINE
    UT_StringLit(const char (&str)[N])
	: myData(str)
	, myLength(N-1)
	, myHash(SYSstring_hash_inner(str, 0))
    {
	SYS_STATIC_ASSERT(N-1 < (((exint)1) << 31));
    }

    SYS_FORCE_INLINE constexpr const char*  buffer() const { return myData; }
    SYS_FORCE_INLINE constexpr const char*  data() const { return myData; }
    SYS_FORCE_INLINE constexpr const char*  c_str() const { return myData; }

    SYS_FORCE_INLINE constexpr exint	    length() const { return myLength; }

    SYS_FORCE_INLINE constexpr uint32	    hash() const { return myHash; }

    SYS_FORCE_INLINE const UT_StringHolder& asHolder() const;
    SYS_FORCE_INLINE const UT_StringRef&    asRef() const;

private:
    const char*	myData;
    int32	myLength;
    uint32	myHash;

    friend class UT_StringRef;
};

/// A holder for a string, which stores the length and caches the hash value.
/// A UT_StringRef does not necessarily own the string, and it is therefore not
/// safe to e.g. store a UT_StringRef in a container or member variable.
/// @see UT_StringHolder
/// @see UTmakeUnsafeRef
class UT_API UT_StringRef
{
public:
    typedef char value_type;

    enum	StorageMode
    {
	STORE_EXTERNAL,
	STORE_NEW,
	STORE_MALLOC,
	STORE_INLINE
    };

    class UT_API Holder
    {
    public:
	Holder()
	    : myRefCount(0), myStorageMode(STORE_INLINE),
	      myHash(0), myLength(0), myData(0)
	{
	}

	static Holder *build(const char *str,
			    StorageMode storage);
	static Holder *buildFromData(const char *str,
			    exint len,
			    StorageMode storage,
                            int hash = 0);
	static Holder *buildBuffer(
			    exint length,
			    StorageMode storage);

	const char *c_str() const
	{
	    if (myStorageMode == STORE_INLINE)
		return myDataIfInline;

	    return myData;
	}

	void	incref()
	{
	    myRefCount.add(1);
	}
	void	decref()
	{
	    if (myRefCount.add(-1) == 0)
	    {
		// We can't invoke delete as we were malloced.
		// Further, we have custom behaviour on ownership!
		switch (myStorageMode)
		{
		    case STORE_EXTERNAL:
		    case STORE_INLINE:
			// Do nothing.
			break;
		    case STORE_NEW:
			delete [] myData;
			break;
		    case STORE_MALLOC:
			if (myData)
			    free( (void*)SYSconst_cast(myData));
			break;
		}
SYS_PRAGMA_PUSH_WARN()
SYS_PRAGMA_DISABLE_FREE_NONHEAP_OBJECT()
		free(this);
SYS_PRAGMA_POP_WARN()
	    }
	}

	exint length() const
	{
	    return myLength;
	}

	unsigned hash() const
	{
	    if (!myHash)
	    {
		myHash = hash_string(c_str(), length());
	    }
	    return myHash;
	}

	int64		getMemoryUsage(bool inclusive) const
	{
	    int64 mem = inclusive ? sizeof(*this) : 0;

	    switch (myStorageMode)
	    {
		case STORE_EXTERNAL:
		    // Nothing extra.
		    break;
		case STORE_INLINE:
                    UT_ASSERT_MSG_P(inclusive,
                        "If a Holder is STORE_INLINE, it *really* can't be a "
                        "member variable, because the size isn't determined "
                        "at runtime.");
                    mem += myLength + 1 - sizeof(const char *);
                    break;
		case STORE_NEW:
		case STORE_MALLOC:
		    // External reference we own.
		    mem += myLength + 1;
		    break;
	    }
	    return mem;
	}

    private:
	char *bufferNC() const
	{
	    if (myStorageMode == STORE_INLINE)
		return SYSconst_cast(myDataIfInline);

	    return SYSconst_cast(myData);
	}

	friend class UT_StringHolder;
	friend class UT_StringRef;

    private:
	SYS_AtomicInt32	myRefCount;
	StorageMode	myStorageMode;
	mutable int	myHash;
	exint		myLength;

        // This union makes viewing in a debugger much easier.
        union {
            const char *myData;
            char myDataIfInline[sizeof(const char *)];
        };
    };

    SYS_FORCE_INLINE
    UT_StringRef()
    {
	init();
    }

    /// Will make a shallow reference.
    SYS_FORCE_INLINE
    UT_StringRef(const char *str)
    {
	init();
	if (UTisstring(str))
	{
	    reference(str);
	}
    }

    /// Construct from string literal
    SYS_FORCE_INLINE explicit
    UT_StringRef(const UT_StringLit &lit)
	: UT_StringRef(lit.asRef())
    {
    }

    /// This will make a shallow reference to the contents of the string.
    SYS_FORCE_INLINE
    UT_StringRef(const std::string &str)
    {
	init();
	const char *s = str.c_str();
	if (UTisstring(s))
	{
	    fastReferenceWithStrlen(s, str.length());
	}
    }

    /// This will make a shallow reference to the contents of the string.
    UT_StringRef(const UT_WorkBuffer &str);

    /// This will make a shallow reference to the contents of the string.
    SYS_FORCE_INLINE
    UT_StringRef(const UT_String &str)
    {
	init();
	if (str.isstring())
	    reference(str);
    }

    /// Shares a reference with the source.
    SYS_FORCE_INLINE
    UT_StringRef(const UT_StringRef &s)
    {
	Holder *sh = s.getHolder();
	if (!isSentinelOrNullPtr(sh))
	    sh->incref();
	// Can now blindly copy.
	copy(s);
    }

    /// Move constructor. Steals the working data from the original.
    SYS_FORCE_INLINE
    UT_StringRef(UT_StringRef &&s) SYS_NOEXCEPT
    {
	copy(s);
	s.init();
    }

    SYS_FORCE_INLINE
    ~UT_StringRef()
    {
	Holder *h = getHolder();
	if (!isSentinelOrNullPtr(h))
	    h->decref();
    }

    /// Special sentinel value support
    /// @{
    enum UT_StringSentinelType { SENTINEL };

    SYS_FORCE_INLINE explicit
    UT_StringRef(UT_StringSentinelType)
        : myData((const void*)SENTINEL_DATA)
	, myLength(0)
	, myHash(SENTINEL_HASH)
    {
    }

    SYS_FORCE_INLINE
    bool isSentinel() const
    {
        return uintptr_t(myData) == SENTINEL_DATA && myHash == SENTINEL_HASH && !myLength;
    }

    SYS_FORCE_INLINE
    void makeSentinel()
    {
        Holder *h = getHolder();
        if (!isSentinelOrNullPtr(h))
            h->decref();
        myData = (const void*)SENTINEL_DATA;
        myLength = 0;
        myHash = SENTINEL_HASH;
    }
    /// @}

    /// Shares a reference with the source.
    UT_StringRef &operator=(const UT_StringRef &s)
    {
	Holder *sh = s.getHolder();
	Holder *th = getHolder();

	if (!isSentinelOrNullPtr(sh))
	    sh->incref();
	if (!isSentinelOrNullPtr(th))
	    th->decref();
	// Can now blindly copy.
	copy(s);
	return *this;
    }

    /// Move the contents of about-to-be-destructed string
    /// s to this string.
    SYS_FORCE_INLINE
    UT_StringRef &operator=(UT_StringRef &&s)
    {
        // Can just swap, since s is about to be destructed.
        swap(s);
        return *this;
    }

    bool		operator==(const UT_StringRef &s) const
    { 
	// It is sensible to fast-path this based on myData!
	// If our two pointers are equal, we either are pointing to
	// the same Holder or to the same const char *, so are good
	// to call it equal.
	if (myData == s.myData)
	    return true;
	exint		tl = length();
	exint		sl = s.length();
	if (tl != sl)
	    return false;
        if (tl == 0)
            return (uintptr_t(myData) == SENTINEL_DATA) == (uintptr_t(s.myData) == SENTINEL_DATA);
	return ::memcmp(c_str(), s.c_str(), tl) == 0; 
    }
    bool		operator==(const char *s) const
    { 
	// It is sensible to fast-path this based on myData!
	// If our two pointers are equal, we either are pointing to
	// the same Holder or to the same const char *, so are good
	// to call it equal.
	// We don't test for myData being a Holder because it should
	// never alias a const char *.
	if (myDataIfChars == s)
	    return true;
	// Avoid comparison with null.
	if (!s)
	    return !isstring();
	return ::strcmp(c_str(), s) == 0; 
    }
    bool                operator==(const UT_String &s) const
                            { return operator==(s.buffer()); }

    bool		operator!=(const UT_StringRef &s) const
                            { return !operator==(s); }
    bool		operator!=(const char *s) const
                            { return !operator==(s); }
    bool                operator!=(const UT_String &s) const
                            { return operator!=(s.buffer()); }

    bool		operator<(const UT_StringRef &k) const
			    { return ::strcmp(c_str(), k.c_str()) < 0; }
    bool		operator<=(const UT_StringRef &k) const
			    { return ::strcmp(c_str(), k.c_str()) <= 0; }
    bool		operator>(const UT_StringRef &k) const
			    { return ::strcmp(c_str(), k.c_str()) > 0; }
    bool		operator>=(const UT_StringRef &k) const
			    { return ::strcmp(c_str(), k.c_str()) >= 0; }
    int		 	compare(const UT_StringRef &str, 
       		 	        bool ignore_case=false) const
			    { return ignore_case ?
				 SYSstrcasecmp(c_str(), str.c_str()) : 
				 ::strcmp(c_str(), str.c_str()); }
    bool		equal(const UT_StringRef &str, 
       		 	        bool ignore_case=false) const
			    { return compare(str, ignore_case) == 0; }

    /// Test whether the string is defined or not
    SYS_FORCE_INLINE
    SYS_SAFE_BOOL	operator bool() const	{ return isstring(); }

    /// Imported from UT_String.
    bool	startsWith(const char *prefix, bool case_sense = true,
			    exint len=-1) const
    {
	return UT_StringWrap(c_str()).startsWith(prefix, case_sense, len);
    }
    bool	endsWith(const char *prefix, bool case_sense = true,
			    exint len=-1) const
    {
	return UT_StringWrap(c_str()).endsWith(prefix, case_sense, len);
    }
    unsigned	match(const char *pattern, int caseSensitive=1) const
    { return UT_StringWrap(c_str()).match(pattern, caseSensitive); }
    bool	contains(const char *pattern, bool case_sensitive=true) const
    { return UT_StringWrap(c_str()).contains(pattern, case_sensitive); }
    unsigned	multiMatch(const char *pattern, 
			   int caseSensitive, char separator) const
    { return UT_StringWrap(c_str()).multiMatch(pattern, caseSensitive, separator); }
    unsigned	multiMatch(const char *pattern, int caseSensitive = 1,
			   const char *separators = ", ",
			   bool *explicitlyExcluded = 0,
			   int *matchIndex = 0,
			   ut_PatternRecord *pattern_record=NULL) const
    { return UT_StringWrap(c_str()).multiMatch(pattern, caseSensitive, separators, explicitlyExcluded, matchIndex, pattern_record); }
    unsigned	multiMatch(const UT_StringMMPattern &pattern,
			   bool *explicitlyExcluded = 0,
			   int *matchIndex = 0,
			   ut_PatternRecord *pattern_record=NULL) const
    { return UT_StringWrap(c_str()).multiMatch(pattern, explicitlyExcluded, matchIndex, pattern_record); }

    int		 toInt() const
    { return UT_StringWrap(c_str()).toInt(); }
    fpreal	 toFloat() const
    { return UT_StringWrap(c_str()).toFloat(); }

    /// Determine if string can be seen as a single floating point number
    unsigned	 isFloat(int skip_spaces = 0, int loose = 0) const
    { return UT_StringWrap(c_str()).isFloat(skip_spaces, loose); }
    /// Determine if string can be seen as a single integer number
    unsigned	 isInteger(int skip_spaces = 0) const
    { return UT_StringWrap(c_str()).isInteger(skip_spaces); }

    SYS_FORCE_INLINE
    operator	 const char *() const
		 { return c_str(); }

    SYS_FORCE_INLINE
    const char  *buffer() const
		 { return c_str(); }
    // We are always non-null by definition!
    SYS_FORCE_INLINE
    const char  *nonNullBuffer() const
		 { return c_str(); }

    /// Iterators
    typedef const char *	const_iterator;

    SYS_FORCE_INLINE
    const_iterator begin() const
		 { return c_str(); }
    SYS_FORCE_INLINE
    const_iterator end() const
		 { return begin() + length(); }


    /// Converts the contents of this UT_String to a std::string.  Since
    /// we are never null this is easy
    std::string toStdString() const
    { return std::string(c_str(), length()); }

    SYS_FORCE_INLINE
    void swap( UT_StringRef &other )
    {
	UTswap(myData, other.myData);
	UTswap(myLength, other.myLength);
	UTswap(myHash, other.myHash);
    }

    /// Friend specialization of std::swap() to use UT_StringRef::swap()
    friend void	swap(UT_StringRef& a, UT_StringRef& b) { a.swap(b); }

    SYS_FORCE_INLINE
    bool isstring() const
    {
	// This test works for both references and inline.
	return !isSentinelOrNullPtr(myData);
    }

    SYS_FORCE_INLINE
    bool
    hasNonSpace() const
    {
	const char *ptr = c_str();
	while (*ptr && SYSisspace(*ptr))
	    ptr++;
	return (*ptr) ? true : false;
    }

    SYS_FORCE_INLINE
    int
    findChar(char c) const
    {
	const char *ptr = c_str();
	while (*ptr && *ptr != c)
	    ptr++;
	return (*ptr) ? (ptr - c_str()) : -1;
    }

    SYS_FORCE_INLINE
    int
    findChar(const char *str) const
    {
	const char *ptr = c_str();
	const char *found_char = nullptr;

	if (UTisstring(ptr) && UTisstring(str))
	{
	    found_char = strpbrk(ptr, str);

	    if (found_char)
		return (intptr_t)(found_char - ptr);
	}

	return -1;
    }

    SYS_FORCE_INLINE
    int
    findChar(char c, exint start_offset) const
    {
	const char *ptr = c_str() + start_offset;
	while (*ptr && *ptr != c)
	    ptr++;
	return (*ptr) ? (ptr - c_str()) : -1;
    }

    SYS_FORCE_INLINE
    int
    findChar(const char *str, exint start_offset) const
    {
	const char *ptr = c_str() + start_offset;
	const char *found_char = nullptr;

	if (UTisstring(ptr) && UTisstring(str))
	{
	    found_char = strpbrk(ptr, str);

	    if (found_char)
		return (intptr_t)(found_char - c_str());
	}

	return -1;
    }

    SYS_FORCE_INLINE
    int
    lastChar(char c, int occurance_number = 1) const
    {
	const char *ptr = c_str();
	for (int i = length(); i --> 0;)
	{
	    if(c == ptr[i])
	    {
		occurance_number--;
		if(occurance_number <= 0)
		    return i;
	    }
	}

	return -1;
    }

    SYS_FORCE_INLINE
    int
    countChar(char c) const
    {
	const char *ptr = c_str();
	int count = 0;
	while (*ptr)
	{
	    if (*ptr == c)
		count++;
	    ptr++;
	}
	return count;
    }

    SYS_FORCE_INLINE
    void clear()
    {
	if (myData)
	{
	    Holder *h = getHolder();
	    if (!isSentinelOrNullPtr(h))
		h->decref();
	    init();
	}
    }

    SYS_FORCE_INLINE
    const char *c_str() const
    { 
	UT_ASSERT_P(!isSentinel());
	Holder *holder = getHolder();
	if (!isSentinelOrNullPtr(holder))
	    return holder->c_str();
	// Harden empty strings.
	if (isSentinelOrNullPtr(myDataIfChars))
	{
#if SYS_IS_GCC_GE(6, 0) && !SYS_IS_GCC_GE(7, 0)
	    // We need to do this to fix bad GCC 6 warning:
	    // offset outside bounds of constant string
	    const char *volatile empty = "";
	    return empty;
#else
	    return "";
#endif
	}
	return myDataIfChars;
    }

    exint		length() const
    {
	Holder *holder = getHolder();

	if (!isSentinelOrNullPtr(holder))
	    return holder->length();

	if (myLength >= 0)
	    return myLength;

	exint	result = -myLength;
	result <<= 31;
	result += myHash;

	return result;
    }

    bool                isEmpty() const
    {
        return length() == 0;
    }

    unsigned		hash() const
    {
	Holder *holder = getHolder();
	if (!isSentinelOrNullPtr(holder))
	    return holder->hash();
	if (myLength >= 0)
	{
	    if (!myLength)
		return 0;
	    if (!myHash)
	    {
		myHash = hash_string(c_str(), length());
	    }
	    return myHash;
	}
	return hash_string(c_str(), length());
    }

    /// Make a light weight reference to the source.
    /// Caller must make sure src lives for the duration of this object,
    /// and any objects value copied from this!
    void		reference(const char *src)
    {
	Holder *h = getHolder();
	if (!isSentinelOrNullPtr(h))
	    h->decref();

	if (!src || !*src)
	{
	    init();
	}
	else
	{
	    myDataIfChars = src;

	    exint len = ::strlen(src);
	    if (len >= (((exint)1) << 31))
	    {
		myLength = -(len >> 31);
		myHash = len & (0x7fffffff);
	    }
	    else
	    {
		myLength = len;
		myHash = 0;
	    }
	}
    }
    /// Fast reference that takes the length of the string.
    /// This is NOT a string view, the string must actually null terminate
    /// at the given length or later functions will be confused.
    void		fastReferenceWithStrlen(const char *src, exint length)
    {
	Holder *h = getHolder();
	if (!isSentinelOrNullPtr(h))
	    h->decref();

	if (!src || length <= 0)
	{
	    UT_ASSERT(length == 0);
	    init();
	}
	else
	{
	    UT_ASSERT_P(!src[length]);
	    myDataIfChars = src;

	    if (length >= (((exint)1) << 31))
	    {
		myLength = -(length >> 31);
		myHash = length & (0x7fffffff);
	    }
	    else
	    {
		myLength = length;
		myHash = 0;
	    }
	}
    }

    int64		getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;

	const Holder *holder = getHolder();
	if (!isSentinelOrNullPtr(holder))
            mem += holder->getMemoryUsage(true);
        return mem;
    }

    // This hash function does not look for null termination, but
    // instead goes directly for the length.
    SYS_FORCE_INLINE
    static unsigned hash_string(const char *str, exint len)
    {
	return SYSstring_hash(str, len);
    }

    UT_StringRef &operator+=(const UT_StringRef &src);

    /// A version of trimSpace() that only removes leading and following spaces
    /// from a string, leaving any between words intact.
    bool	 trimBoundingSpace();

    /// trimSpace() will remove all space characters (leading and following)
    /// from a string.  If the string consists of multiple words, the words
    /// will be collapsed. To keep a single space between words, pass in true.
    /// The function returns true if space was trimmed.
    bool	 trimSpace(bool leave_single_space_between_words = false);

    /// Convert to lower case.  If the string is already lower case, the string
    /// itself is returned.
    UT_StringRef toLower() const;

    /// Often people reflexively use this from UT_String days to
    /// this increases code compataibility.
    void	 harden(const char *src);

    // Steals the given string, gaining ownership of it.
    // Will be freed by this when the reference count hits zero.
    void		adoptFromMalloc(const char *str, exint length=-1)
    {
	clear();

	if (length == 0 || !UTisstring(str))
	    return;
	else
	{
	    if (length < 0)
		length = ::strlen(str);
	    Holder *h = Holder::buildFromData(str, length, STORE_MALLOC);
	    h->incref();
	    myLength = 0;
	    myData = h;
	}
    }
    void		adoptFromNew(const char *str, exint length=-1)
    {
	clear();

	if (length == 0 || !UTisstring(str))
	    return;
	else
	{
	    if (length < 0)
		length = ::strlen(str);
	    Holder *h = Holder::buildFromData(str, length, STORE_NEW);
	    h->incref();
	    myLength = 0;
	    myData = h;
	}
    }
    void		adoptFromString(UT_String &str)
    {
	if (!str.isstring())
	{
	    clear();
	    return;
	}
	// We want to steal from always deep strings as well.
	// We will erase the data in the source string!
	str.harden();

	// Clear after harden in case str refers to us!
	clear();

	Holder *h = Holder::build(str.myData, STORE_MALLOC);
	h->incref();
	myLength = 0;
	myData = h;

	str.myData = 0;
	str.myIsReference = true;
	// Leave always deep as it was.
    }

    // Extracts a string from ourself of the given allocation mode.
    // The result can be freed with that allocation mode.
    // Will always clear myself afterwards.  This will return our
    // own string without copying if reference count is 1.
    // Will return 0 for empty strings.
    char 		*stealAsMalloc();

    // Tests to see if UT_StringLit's memory layout is the same
    static bool		 verifyStringLit();

    /// Does a "smart" string compare which will sort based on numbered names.
    /// That is "text20" is bigger than "text3".  In a strictly alphanumeric
    /// comparison, this would not be the case.
    struct NumberedCompare
    {
        bool operator()(const UT_StringRef &s1, const UT_StringRef &s2) const
        {
            return UT_String::compareNumberedString(s1.c_str(), s2.c_str()) < 0;
        }
    };

    ///	Save string to binary stream.
    void	 saveBinary(std::ostream &os) const
		    { UT_StringWrap(c_str()).saveBinary(os); }

    /// Save string to ASCII stream. This will add double quotes and escape to
    /// the stream if necessary (empty string or contains spaces).
    void	 saveAscii(std::ostream &os) const
		    { UT_StringWrap(c_str()).saveAscii(os); }

private:
    void init()
    {
	myData = nullptr;
	myLength = 0;
	myHash = 0;
    }

    void copy(const UT_StringRef &s)
    {
        myData = s.myData;
	myLength = s.myLength;
	myHash = s.myHash;
    }

    SYS_FORCE_INLINE
    Holder *getHolder() const
    {
        if (!myLength)
            return myDataIfHolder;
        return NULL;
    }

    // This union makes viewing in a debugger much easier.
    union {
        const void      *myData;
        const char      *myDataIfChars;
        Holder          *myDataIfHolder;
    };
    int			 myLength;
    mutable int		 myHash;

    /// This operator saves the string to the stream via the string's
    /// saveAscii() method, protecting any whitespace (by adding quotes),
    /// backslashes or quotes in the string.
    friend UT_API std::ostream	&operator<<(std::ostream &os, const UT_StringRef &d);
    friend UT_API UT_OStream	&operator<<(UT_OStream &os, const UT_StringRef &d);

    // UT_StringHolder needs to be a friend class so that the
    // UT_StringHolder(const UT_StringRef &) constructor can access myHash and
    // getHolder() for the UT_StringRef that is passed in.
    friend class UT_StringHolder;

    static constexpr uintptr_t SENTINEL_DATA = 1;
    static constexpr uint32 SENTINEL_HASH = 0xdeadbeef;

    template<typename T>
    static SYS_FORCE_INLINE bool isSentinelOrNullPtr(const T *p)
    {
        return uintptr_t(p) <= SENTINEL_DATA;
    }
};

SYS_FORCE_INLINE size_t hash_value(const UT_StringRef &str)
{
    return str.hash();
}

/// A holder for a string, which stores the length and caches the hash value.
/// The lifetime of the string is >= the lifetime of the UT_StringHolder.
class UT_API UT_StringHolder : public UT_StringRef
{
public:
    /// UT_StringHolder can be constructed with UT_StringHolder::REFERENCE to
    /// create a shallow reference to the const char *.
    enum UT_StringReferenceType { REFERENCE };

    SYS_FORCE_INLINE
    UT_StringHolder()
        : UT_StringRef()
    {
    }

    /// Will make a copy of the provided string.
    SYS_FORCE_INLINE
    UT_StringHolder(const char *str)
    {
	if (UTisstring(str))
	{
	    Holder *h = Holder::build(str, STORE_INLINE);
	    h->incref();
	    myLength = 0;
	    myDataIfHolder = h;
	}
    }

    /// Will make a shallow reference.
    SYS_FORCE_INLINE
    UT_StringHolder(UT_StringReferenceType, const char *str)
        : UT_StringRef(str)
    {
    }

    /// Will make a copy of the provided string.
    SYS_FORCE_INLINE
    UT_StringHolder(const char *data, exint length)
    {
	if (data && length > 0)
	{
	    Holder *h = Holder::buildFromData(data, length, STORE_INLINE);
	    h->incref();
	    myLength = 0;
	    myDataIfHolder = h;
	}
    }

    // Prohibit accidents when converting from UT_String like:
    //    myStringHolder(buffer, /*deep*/true)
    UT_StringHolder(const char *data, bool bad) = delete;

    // Add back explicit conversions for the length parameter
    SYS_FORCE_INLINE
    UT_StringHolder(const char *data, int32 length)
	: UT_StringHolder(data, exint(length)) { }
    SYS_FORCE_INLINE
    UT_StringHolder(const char *data, uint32 length)
	: UT_StringHolder(data, exint(length)) { }
    SYS_FORCE_INLINE
    UT_StringHolder(const char *data, uint64 length)
	: UT_StringHolder(data, exint(length)) { }
#if defined(MBSD)
    SYS_FORCE_INLINE
    UT_StringHolder(const char *data, size_t length)
	: UT_StringHolder(data, exint(length)) { }
#endif

    /// Will make a copy of the provided string.
    SYS_FORCE_INLINE
    UT_StringHolder(const std::string &str)
    {
	const char	*s = str.c_str();
	if (UTisstring(s))
	{
	    Holder *h = Holder::buildFromData(s, str.length(), STORE_INLINE);
	    h->incref();
	    myLength = 0;
	    myDataIfHolder = h;
	}
    }

    /// This will make a shallow reference to the contents of the string.
    SYS_FORCE_INLINE
    UT_StringHolder(UT_StringReferenceType, const std::string &str)
        : UT_StringRef(str)
    {
    }

    /// Will make a copy of the provided string.
    UT_StringHolder(const UT_WorkBuffer &str);

    /// This will make a shallow reference to the contents of the string.
    SYS_FORCE_INLINE
    UT_StringHolder(UT_StringReferenceType, const UT_WorkBuffer &str)
        : UT_StringRef(str)
    {
    }

    /// Will make a copy of the provided string.
    SYS_FORCE_INLINE
    UT_StringHolder(const UT_String &str)
    {
        *this = UT_StringHolder(str.buffer());
    }

    /// This will make a shallow reference to the contents of the string.
    SYS_FORCE_INLINE
    UT_StringHolder(UT_StringReferenceType, const UT_String &str)
        : UT_StringRef(str)
    {
    }

    /// Will make a copy of the provided string.
    SYS_FORCE_INLINE
    UT_StringHolder(const UT_StringView &sv)
    {
	if (!sv.isEmpty())
	{
	    Holder *h = Holder::buildFromData(sv.begin(), sv.length(),
	                                      STORE_INLINE);
	    h->incref();
	    myLength = 0;
	    myDataIfHolder = h;
	}
    }

    /// Makes a shallow reference to the contents of the UT_StringRef.
    SYS_FORCE_INLINE
    UT_StringHolder(UT_StringReferenceType, const UT_StringRef &ref)
        : UT_StringRef(ref)
    {
    }

    /// Makes a deep copy of the provided UT_StringRef.
    /// This constructor is not marked explicit since we often want this
    /// conversion (e.g. when inserting a UT_StringRef into a UT_StringMap, as
    /// with the const char* constructor).
    UT_StringHolder(const UT_StringRef &ref);

    /// Construct as a sentinel value
    SYS_FORCE_INLINE explicit
    UT_StringHolder(UT_StringSentinelType sentinel)
        : UT_StringRef(sentinel)
    {
    }

    /// Makes a copy of the provided string.
    SYS_FORCE_INLINE
    UT_StringHolder(const UT_StringHolder &str)
        : UT_StringRef(str)
    {
    }

    /// Move constructor. Steals the working data from the original.
    SYS_FORCE_INLINE
    UT_StringHolder(UT_StringHolder &&a) SYS_NOEXCEPT
        : UT_StringRef(std::move(a))
    {
    }

    /// Makes a bit-wise copy of the string and adjust the reference count.
    SYS_FORCE_INLINE
    UT_StringHolder &operator=(const UT_StringHolder &s)
    {
        UT_StringRef::operator=(s);
        return *this;
    }

    /// Move the contents of about-to-be-destructed string
    /// s to this string.
    SYS_FORCE_INLINE
    UT_StringHolder &operator=(UT_StringHolder &&s)
    {
        UT_StringRef::operator=(std::move(s));
        return *this;
    }

    SYS_FORCE_INLINE
    void swap(UT_StringHolder &other)
    {
	UT_StringRef::swap(other);
    }

    SYS_FORCE_INLINE
    void swap(UT_StringRef &other)
    {
	UT_StringRef::swap(other);
	// harden ourselves like UT_StringHolder::operator=(UT_StringRef&)
	Holder *h = getHolder();
	if (!isSentinelOrNullPtr(h))
	   return; // already a holder
       	if (!isSentinelOrNullPtr(myDataIfChars))
	{
	    exint len = (myLength >= 0) ? myLength
				        : (exint(-myLength) << 31) + myHash;
	    h = Holder::buildFromData(myDataIfChars, len, STORE_INLINE, myHash);
	    h->incref();
	    myLength = 0;
	    myData = h;
	}
    }

    /// Friend specialization of std::swap() to use UT_StringHolder::swap()
    /// @{
    friend void	swap(UT_StringHolder& a, UT_StringRef& b) { a.swap(b); }
    friend void	swap(UT_StringHolder& a, UT_StringHolder& b) { a.swap(b); }
    /// @}

    /// In some functions it's nice to be able to return a const-reference to a
    /// UT_StringHolder.  However, in error cases, you likely want to return an
    /// empty string.  This would mean that you'd have to return a real
    /// UT_StringHolder (not a const reference).  This static lets you return a
    /// reference to an empty string.
    static const UT_StringHolder theEmptyString;

    static const UT_StringHolder theSentinel;
    
    /// Format a string using the same formatting codes as @c UTformat.
    template<typename... Args>
    size_t format(const char *fmt, const Args &...args)
    {
	clear();
	
	using namespace UT::Format;
	Writer w;
	Formatter<Writer> f;
	size_t nb_needed = f.format(w, fmt, {args...});
	
	auto h = Holder::buildBuffer(nb_needed, STORE_MALLOC);
	h->incref();
	myLength = 0;
	myData = h;
	    
	w.setBuffer(h->bufferNC(), nb_needed);
	f.format(w, fmt, {args...});
	h->bufferNC()[nb_needed] = '\0';
	
	return nb_needed;
    }

    /// Format a string using the same formatting codes as @c UTprintf.
    template<typename... Args>
    size_t sprintf(const char *fmt, const Args &...args)
    {
	clear();
	
	using namespace UT::Format;
	Writer w;
	Formatter<Writer> f;
	size_t nb_needed = f.printf(w, fmt, {args...});
	
	auto h = Holder::buildBuffer(nb_needed, STORE_MALLOC);
	h->incref();
	myLength = 0;
	myData = h;
	    
	w.setBuffer(h->bufferNC(), nb_needed);
	f.printf(w, fmt, {args...});
	h->bufferNC()[nb_needed] = '\0';
	
	return nb_needed;
    }

    /// Replace all occurances of 'find' with 'replacement'.
    /// Returns number of replacements.
    int substitute(const char *find, const char *replacement, bool all = true)
    {
	UT_String s(buffer());
	int n = s.substitute(find, replacement, all);
	if (n > 0)
	    adoptFromString(s);
	return n;
    }

    /// Load string from stream. Use is.eof() to check eof status
    bool load(UT_IStream &is)
    {
	UT_String s;
	if (s.load(is))
	{
	    adoptFromString(s);
	    return true;
	}
	return false;
    }

protected:
    friend UT_StringHolder UT::Literal::operator"" _sh(
	    const char *s, std::size_t const length);
    friend UT_StringHolder operator"" _UTsh(
	    const char *s, std::size_t const length);
    
    /// A marker enum to use this constructor.
    enum UT_StringLiteralType { LITERAL };
    
    /// Only accepts string literals. Since there's no way to guarantee that
    /// a const char * argument is a string literal, we do this through the
    /// use of user-defined literal and *only* provide this constructor to our
    /// user-defined literal operator.
    SYS_FORCE_INLINE UT_StringHolder(UT_StringLiteralType, const char *str, size_t length)
    {
	if (str && length)
	{
	    myDataIfChars = str;
	    myLength = length;
	    // As of C++14, user defined literals don't support constexpr so
	    // this hash computation here is actually done at run-time except
	    // for in some cases where the compiler (clang?) is able to
	    // optimize this. Therefore, disable this to avoid extraneous hash
	    // computation in cases where we just want to use "foo"_sh to avoid
	    // heap allocation but never use its hash.
	    // Use UT_StringLit if you want to get compile time hashes
#if 0
	    myHash = SYSstring_hash_literal(str);
#else
	    myHash = 0;
#endif
	}
    }
};

template <typename T> class UT_Array;
UT_EXTERN_TEMPLATE(UT_Array<UT_StringHolder>);

/// Convert a UT_StringRef into a UT_StringHolder that is a shallow reference.
SYS_FORCE_INLINE const UT_StringHolder &UTmakeUnsafeRef(const UT_StringRef &ref)
{
    SYS_STATIC_ASSERT(sizeof(UT_StringRef) == sizeof(UT_StringHolder));
    return reinterpret_cast<const UT_StringHolder &>(ref);
}

/// Convert a UT_StringRef into a UT_StringHolder that is a shallow reference,
/// and also precompute the hash.  Use this for string literals
/// that will be used repeatedly in hash tables.
SYS_FORCE_INLINE const UT_StringHolder &UTmakeUnsafeRefHash(const UT_StringRef &ref)
{
    SYS_STATIC_ASSERT(sizeof(UT_StringRef) == sizeof(UT_StringHolder));
    ref.hash();
    return reinterpret_cast<const UT_StringHolder &>(ref);
}

SYS_FORCE_INLINE const UT_StringHolder &
UT_StringLit::asHolder() const
{
    SYS_STATIC_ASSERT(sizeof(UT_StringHolder) == sizeof(*this));
    return reinterpret_cast<const UT_StringHolder &>(*this);
}

SYS_FORCE_INLINE const UT_StringRef &
UT_StringLit::asRef() const
{
    SYS_STATIC_ASSERT(sizeof(UT_StringRef) == sizeof(*this));
    return reinterpret_cast<const UT_StringRef &>(*this);
}

SYS_FORCE_INLINE size_t tbb_hasher(const UT_StringRef &s) { return s.hash(); }
SYS_FORCE_INLINE size_t tbb_hasher(const UT_StringHolder &s) { return s.hash(); }

/// A user-defined string literal to construct UT_StringHolder objects.
/// E.g:
/// @code
/// auto lit = "This is my UT_StringHolder literal"_sh;
/// @endcode
namespace UT { inline namespace Literal {
SYS_FORCE_INLINE UT_StringHolder operator"" _sh(const char *s, std::size_t const length)
{
    return UT_StringHolder(UT_StringHolder::LITERAL, s, length);
}
} }

/// A user-defined literal in the global namespace. Uglier, but allows the use
/// of UT_StringHolder UDLs in headers. 
SYS_FORCE_INLINE UT_StringHolder operator"" _UTsh(const char *s, std::size_t const length)
{
    return UT_StringHolder(UT_StringHolder::LITERAL, s, length);
}


namespace std
{
    template<>
    struct hash<UT_StringRef>
    {
	size_t operator()(const UT_StringRef &s) const
	{
	    return s.hash();
	}
    };
    template<>
    struct hash<UT_StringHolder>
    {
	size_t operator()(const UT_StringHolder &s) const
	{
	    return s.hash();
	}
    };
}

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <>
struct DefaultClearer<UT_StringHolder>
{
    static void clear(UT_StringHolder &v) { v.makeSentinel(); }
    static bool isClear(const UT_StringHolder &v) { return v.isSentinel(); }
    static void clearConstruct(UT_StringHolder *p)
    {
        new ((void *)p) UT_StringHolder(UT_StringRef::SENTINEL);
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif // __UT_StringHolder_h__
