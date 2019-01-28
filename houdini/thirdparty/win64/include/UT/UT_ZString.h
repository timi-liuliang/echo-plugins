/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ZString.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_ZString__
#define __UT_ZString__

#include "UT_StringHolder.h"
#include <SYS/SYS_Hash.h>

/// Storage to hold compressed data.
///
/// This class will take a buffer of data and compress it.  You can access the
/// raw compressed data using @c compressedString(), or easily get a copy of
/// the uncompressed data by calling @c uncompress().
///
/// Since @c UT_StringRef can hold binary data, this is a convenient way of
/// compressing data that has infrequent access (or possibly never accessed).
/// For example: @code
///    void *binary_data;
///    UT_ZString  z(UT_StringRef((const char *)binary_data, binary_size));
/// @endcode
///
/// @note BLOSC compression typically works best on larger buffers with numeric
/// data.  It's faster than GZIP compression, but has worse compression.
class UT_API UT_ZString
{
public:
    enum Style
    {
	NONE,
	GZIP,
	BLOSC
    };

    static const char	*style(Style s);
    static Style	 style(const char *s);

    UT_ZString() = default;
    UT_ZString(const UT_StringRef &src, Style style=GZIP, int level=9)
    {
	compress(src, style, level);
    }

    /// Store a string as compressed data, returning the compressed style.  If
    /// no compression is done, the returned style will be NONE.
    ///
    /// For GZIP compression, level can be between 1 and 9.  1 is faster, 9 is
    /// better compression (but can be significantly slower).
    Style		compress(const UT_StringRef &s,
				Style style=GZIP,
				int level=1);

    /// Return the uncompressed contents
    UT_StringHolder	uncompress() const;

    /// @{
    /// Member data access
    const UT_StringHolder	&compressedString() const { return myString; }
    Style			 compressedStyle() const { return myStyle; }
    exint			 originalSize() const { return mySize; }
    /// @}

    /// @{
    /// Standard "string" type operations.
    int64		getMemoryUsage(bool inclusive) const
    {
	int64	mem = inclusive ? sizeof(*this) : 0;
	return mem += myString.getMemoryUsage(false);
    }
    SYS_HashType	hash() const
    {
	SYS_HashType	h = myString.hash();
	SYShashCombine(h, mySize);
	SYShashCombine(h, myStyle);
	return h;
    }
    friend std::size_t	hash_value(const UT_ZString &s) { return s.hash(); }
    bool	operator==(const UT_ZString &s) const
		{
		    return mySize == s.mySize
			&& myStyle == s.myStyle
			&& myString == s.myString;
		}
    /// @}

private:
    UT_StringHolder	myString;
    exint		mySize = 0;
    Style		myStyle = NONE;
};

#endif
