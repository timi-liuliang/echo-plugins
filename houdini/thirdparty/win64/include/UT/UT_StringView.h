/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StringView.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_StringView__
#define __UT_StringView__

#include "UT_API.h"

#include "UT_Array.h"

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_String.h>
#include <string.h>
#include <iosfwd>

class UT_StringView;
typedef UT_Array<UT_StringView> UT_StringViewArray;

/// @brief A utility class to do read-only operations on a subset of an 
/// existing string.
/// @note This object does not take ownership over the data being looked at,
/// so care must be taken that the owning object doesn't not go out of scope
/// before the view object. 
class UT_API UT_StringView
{
public:
    /// Default constructor. Constructs an empty non-string.
    SYS_FORCE_INLINE
    UT_StringView() 
	: myStart(nullptr), myEnd(nullptr) 
    { /**/ }
    
    /// Construct a string view from the entirety of a null-terminated string.
    SYS_FORCE_INLINE
    UT_StringView(const char *str) 
	: myStart(str), myEnd(nullptr)
    {
	if (myStart)
	    myEnd = myStart + ::strlen(str);
    }
    
    /// Construct a string view on a string of a given length.
    SYS_FORCE_INLINE
    explicit UT_StringView(const char *str, exint len)
	: myStart(str), myEnd(nullptr)
    {
	UT_ASSERT(len >= 0 && "String View length should not be negative");
	if (myStart)
	    myEnd = myStart + len;
    }

    /// Construct a string view on a given string range. The @c end pointer
    /// should point to one past the end of the string (i.e. in the case of
    /// null terminated strings, it should point at the null character).
    SYS_FORCE_INLINE
    explicit UT_StringView(const char *start, const char *end)
	: myStart(start), myEnd(end)
    {
	UT_ASSERT((!myStart && !myEnd) || myStart <= myEnd); 
    }

    /// Copy constructor.
    SYS_FORCE_INLINE
    UT_StringView(const UT_StringView &o) = default;
    
    /// @name Query functions
    /// @{
    
    /// Returns the length of the string in bytes.
    SYS_FORCE_INLINE
    exint 	length() const { return exint(myEnd - myStart); }
    
    /// Returns @c true if the string is empty.
    SYS_FORCE_INLINE
    bool 	isEmpty() const { return myStart == myEnd; }
    
    /// Returns @c true if the view points to a valid string, even an empty one
    SYS_FORCE_INLINE
    bool 	isstring() const { return myStart; } 
    
    /// Test whether the string is not an empty string (or nullptr)
    SYS_FORCE_INLINE
    SYS_SAFE_BOOL	operator bool() const	{ return !isEmpty(); }

    /// Returns the memory, in bytes, used by this object. 
    int64	getMemoryUsage(bool inclusive) const
		{ return inclusive ? sizeof(*this) : 0; }
    
    /// Returns the character at index @c i. No bounds checking is performed.
    SYS_FORCE_INLINE
    char	operator[](exint i) const { return myStart[i]; }
    
    /// @}
    
    /// @name Iterators
    /// @{
    
    /// The iterator type. @ref UT_StringView only provides read-only iterators.
    typedef const char *	const_iterator;
    
    /// Returns a constant iterator pointing to the beginning of the string.
    SYS_FORCE_INLINE
    const_iterator begin() const { return myStart; }
    
    /// Returns a constant iterator pointing to the end of the string.
    SYS_FORCE_INLINE
    const_iterator end() const { return myEnd; }
    /// @}
    
    /// @name Manipulators
    /// @{
    
    /// Clears the string. After this operation @ref isstring will return false,
    /// and @ref isEmpty will return true.
    void 	clear() { myStart = myEnd = 0; }
    
    /// Trim characters from the left- and right-hand side of the string.
    /// By default this will trim the ASCII space characters.
    SYS_FORCE_INLINE
    UT_StringView	trim(const char *c =" \t\n\r") const
			    { return trimInternal(c, true, true); }
    
    /// Trim characters from the left-hand side of the string.
    /// By default this will trim the ASCII space characters.
    SYS_FORCE_INLINE
    UT_StringView 	trimLeft(const char *c =" \t\n\r") const
			    { return trimInternal(c, true, false); }
    
    /// Trim characters from the right-hand side of the string.
    /// By default this will trim the ASCII space characters.
    SYS_FORCE_INLINE
    UT_StringView	trimRight(const char *c =" \t\n\r") const
			    { return trimInternal(c, false, true); }

    /// Return the first token
    UT_StringView	firstToken(const char *sep_chars = " \t\n\r") const;
    
    /// Splits the string into individual tokens, separated by one or more of 
    /// the @c sep characters given.
    UT_StringViewArray tokenize(const char *sep_chars =" \t\n\r") const;
    
    /// Splits the string into a list of words, using sep_str as the separator
    /// string. Unlike tokenize, consecutive delimiters are not grouped 
    /// together and are instead taken to delimit empty strings.
    /// If @c max_split is set, the string is split into at most @c max_sep
    /// pieces.
    UT_StringViewArray split(const char *sep_str = " ", 
                             int max_split = INT_MAX) const;
    
    /// Returns a sub-string of the current string. If positive, the 
    /// @c index parameter is relative to the start. If negative, it's
    /// relative to the end (e.g. substr(-1,1) will return the last character
    /// of the string).
    /// The empty string will be returned for out-of-range values.
    UT_StringView substr(exint index, exint length=SYS_EXINT_MAX ) const;
    
    /// @}

    /// @name Operators
    /// @{
    
    /// The @ref compare function compares this string with another, and returns
    /// and integer less than, equal to, or greater than zero if this string
    /// is found to be less than, equal to, or greater than the given string,
    /// respectively. If a length is given, then the strings are compared as
    /// if they were both of that length, or smaller.
    int 	compare(const char *str, exint str_len, 
        	        bool case_sensitive=true) const;
    int 	compare(const UT_StringView &sv,
			bool case_sensitive=true) const;
    
    /// Returns true if the two strings compare as being equal. 
    /// If @c case_sensitive is set to @c false then the strings are compared 
    /// in a case-insensitive fashion.
    SYS_FORCE_INLINE
    bool	equal(const char *str, bool case_sensitive=true) const
		{ return compare(UT_StringView(str), 
				 case_sensitive) == 0; }
    SYS_FORCE_INLINE
    bool	equal(const UT_StringView &other, 
        	      bool case_sensitive=true) const
		{ return compare(other, case_sensitive) == 0; }
    
    /// Returns true if the two strings compare as being equal. 
    SYS_FORCE_INLINE
    bool 	operator==(const char *str) const
		{ return compare(UT_StringView(str)) == 0; }
    SYS_FORCE_INLINE
    bool 	operator==(const UT_StringView &other) const
		{ return compare(other) == 0; }
    
    /// Returns true if the two strings compare as being not equal. 
    SYS_FORCE_INLINE
    bool 	operator!=(const char *str) const
		{ return compare(UT_StringView(str)) != 0; }
    SYS_FORCE_INLINE
    bool 	operator!=(const UT_StringView &other) const
		{ return compare(other) != 0; }
    
    /// Returns true if this string is lexicographically less than the given
    /// string.
    SYS_FORCE_INLINE
    bool 	operator<(const char *str) const
		{ return compare(UT_StringView(str)) < 0; }
    SYS_FORCE_INLINE
    bool 	operator<(const UT_StringView &other) const
		{ return compare(other) < 0; }

    /// Find the first instance of the given character in this string.
    const_iterator find(char c, const_iterator start) const;
    SYS_FORCE_INLINE
    const_iterator find(char c) const
		   { return find(c, begin()); }
    
    /// Find the first instance of the given substring in this string.
    const_iterator find(const char *str, const_iterator start) const;
    SYS_FORCE_INLINE
    const_iterator find(const char *str) const
		   { return find(str, begin()); }

    /// Find last instance of the given character in this string, searching
    /// backwards from 'pos'.
    const_iterator rfind(char c, const_iterator pos) const;
    SYS_FORCE_INLINE
    const_iterator rfind(char c) const
		   { return rfind(c, end() - 1); }
    
    /// @}
private:
    UT_StringView trimInternal(const char *c, bool left, bool right) const;
     
    const char 	*myStart, *myEnd;
};

UT_API std::ostream &
operator<<(std::ostream &os, const UT_StringView &sv);


#endif // __UT_StringView__
