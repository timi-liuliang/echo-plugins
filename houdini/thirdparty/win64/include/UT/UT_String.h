/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:	String class
 *
 */

#ifndef __UT_String_h__
#define __UT_String_h__

#include "UT_API.h"

#include "UT_Assert.h"
#include "UT_VectorTypes.h"

#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_String.h>
#include <SYS/SYS_Types.h>

#include <functional>
#include <iosfwd>
#include <string>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
    #define strcasecmp stricmp
    #define strncasecmp strnicmp
#endif

class UT_OStream;
class UT_String;
class UT_StringCshIO;
class UT_WorkArgs;
class UT_IStream;
class ut_PatternRecord;
class UT_StringMMPattern;
class UT_StringArray;
class UT_StringHolder;
class UT_StringRef;
class UT_StringView;

//  The following lookup functions are used by cshParse.  By default,
//	varLookup simply uses getenv, exprLookup opens the command as
//	a pipe and uses the result.
UT_API extern void	UTvarLookup(const char *name, UT_String &result);
UT_API extern void	UTexprLookup(const char *name, UT_String &result);

SYS_FORCE_INLINE bool UTisstring(const char *s) { return s && *s; }

// Because invoking isdigit with a negative value is undefined,
// some MSVC compilers decide to crash.  Thus we cast explicitly
// to unsigned.
SYS_FORCE_INLINE bool UTisdigit(char c)
			 { return isdigit((unsigned char) c); }

/// @file
/// @class UT_String
///
/// UT_String is a string class that support two different types of assignment
/// semantics:
///	- Shallow (default): Just reference the given string and NOT take
///	  ownership.
///	- Deep: Make a copy of the given string, taking ownership in the
///	  process (aka it making it "hard").
///
/// If UT_String::harden() is called, or any other UT_String method that
/// requires modifying the string, it will make a copy of its reference pointer
/// (and take ownership) first.
/// 
class UT_API UT_String
{
public:

    /// UT_String can be constructed with UT_String::ALWAYS_DEEP to create an
    /// object that will always perform deep copies when assigned to.
    enum UT_AlwaysDeepType { ALWAYS_DEEP };

    /// @brief Construct UT_String from a C string, using shallow semantics
    ///
    /// @param str	The initial string.
    /// @param deepCopy	If true, a copy of @em str will be used.
    /// @param len	Number of characters to use from @em str. Use -1 to
    ///			use the entire string. If len is non-negative, then
    ///			deepCopy will be implicitly set to true. If str is NULL
    ///			and len is non-negative, then it will be initialized
    ///			with "".
    SYS_FORCE_INLINE
    UT_String(const char *str = 0) 
	    : myIsReference(true)
	    , myIsAlwaysDeep(false)
	    , myData(SYSconst_cast(str))
    {}
    UT_String(const char *str, int deepCopy, int len = -1);

    /// @brief Construct UT_String from a std::string, always doing
    /// a deep copy.  The result will only be a UT_AlwaysDeep if the
    /// appropriate version is used, however!
    ///
    /// NOTE: You cannot do:
    ///   UT_String foo;
    ///   std::string bar = "hello world";
    ///   foo = UT_String(bar.substr(2, 5));
    ///
    /// It provides an shortcut for constructing a UT_String from a function
    /// that returns a std::string by value.  For example, it lets you write
    /// @code
    ///     UT_String str(func());
    /// @endcode
    /// instead of
    /// @code
    ///     UT_String str(func().c_str(), /*harden=*/true);
    /// @endcode
    explicit UT_String(const std::string &str)
	: myIsReference(false),
	  myIsAlwaysDeep(false)
    { myData = strdup(str.c_str()); }

    /// @brief Construct UT_String from a UT_StringHolder.
    /// This always duplicates and uses ALWAYS_DEEP semantics.
    explicit UT_String(const UT_StringHolder &str);

private:
    /// This is intentionally not implemented - callers should choose between
    /// the const char * and UT_StringHolder constructors, depending on whether
    /// they want to make a deep copy.
    /// @see UT_StringWrap.
    UT_String(const UT_StringRef &);

public:
    /// @brief Construct UT_String from a UT_StringView.
    /// This always duplicates and uses ALWAYS_DEEP semantics.
    explicit UT_String(const UT_StringView &sv);

    /// @brief Construct UT_String from a C string, using ALWAYS_DEEP semantics
    UT_String(UT_AlwaysDeepType, const char *str = 0)
	: myIsReference(false),
	  myIsAlwaysDeep(true)
    { myData = str ? strdup(str) : 0; }

    /// @brief Construct UT_String from a std::string, using ALWAYS_DEEP
    /// semantics
    UT_String(UT_AlwaysDeepType, const std::string &str)
	: myIsReference(false),
	  myIsAlwaysDeep(true)
    { myData = strdup(str.c_str()); }

    /// Copy constructor
    ///
    /// If the string we're copying from is ALWAYS_DEEP, then this object will
    /// also become ALWAYS_DEEP. This way, you can pass/return a string by
    /// value.
    UT_String(const UT_String &str);
		
    ~UT_String();

    /// Move operators
    /// @{
    UT_String(UT_String &&str) SYS_NOEXCEPT
	: myData(str.myData)
	, myIsReference(str.myIsReference)
	, myIsAlwaysDeep(str.myIsAlwaysDeep)
    {
	str.myData = nullptr;
	str.myIsReference = !str.myIsAlwaysDeep;
    }
    UT_String &operator=(UT_String &&str)
    {
	freeData();
	myData = str.myData;
	myIsReference = str.myIsReference;
	myIsAlwaysDeep = str.myIsAlwaysDeep;
	str.myData = nullptr;
	str.myIsReference = !str.myIsAlwaysDeep;
	return *this;
    }
    /// @}
		
    /// Make a string always deep
    void	 setAlwaysDeep(bool deep)
		 {
		     myIsAlwaysDeep = deep;
		     if (deep && myIsReference)
		     {
			 if (myData != NULL)
			     harden();
			 else
			 {
			     // This takes the same semantic as
			     //     str = NULL;
			     // where str is an always deep string
			     myIsReference = false;
			 }
		     }
		 }
    bool	 isAlwaysDeep() const
		 {
		     return myIsAlwaysDeep;
		 }

    void	 swap( UT_String &other );

    ///	Take shallow copy and make it deep.
    // @{
    void	 harden()
		 {
		    if (!myIsReference && myData)
			return;
		    myData = strdup(myData ? myData : "");
		    myIsReference = false;
		 }

    void	 harden(const char *s, int len = -1);
    void	 hardenIfNeeded()
		 {
		     if (myIsReference)
		     {
			 if (isstring())
			     harden();
			 else
			     *this = "";
		     }
		 }
    void	 hardenIfNeeded(const char *s)
		 {
		     if (s && *s)
			 harden(s);
		     else
			 *this = "";
		 }
    // @}

    /// Returns whether this string is hardened already.
    bool	 isHard() const { return !myIsReference; }

    /// Give up ownership of string
    ///
    /// Take a hard reference and make it shallow. This method makes sure
    /// it gives back something you can delete, because this UT_String is
    /// taking its hands off the data. Use it with care since it may lead
    /// to memory leaks if, for example, you harden it again later.
    ///
    /// In the case of ALWAYS_DEEP strings, this is disallowed so it will
    /// just return a copy of the data.
    char *	 steal(void)
		 {
		     if (!myIsAlwaysDeep)
		     {
			 if (myIsReference) 
			     myData = strdup(myData ? myData : ""); // harden
			 myIsReference = true;		  // but say it's soft
			 return myData;
		     }
		     else
		     {
			 // return a new copy of the data without releasing
			 // ownership for always deep strings
			 return strdup(myData ? myData : "");
		     }
		 }
    
    /// Take ownership of given string
    ///
    /// adopt() is the opposite of steal(). Basically, you're giving
    /// the UT_String ownership of the string.
    // @{
    void	 adopt(char *s)
		 {
		    if (!myIsReference)
		    {
			if (s != myData)
			    free(myData);
		    }
		    myData = s;
		    myIsReference = false;
		 }
    void	 adopt(UT_String &str)
		 {
		     adopt(str.steal());
		 }
    void	 adopt(UT_StringHolder &holder);

    // @}

    ///	Save string to binary stream.
    void	 saveBinary(std::ostream &os) const	{ save(os, true); }

    /// Save string to ASCII stream. This will add double quotes and escape to
    /// the stream if necessary (empty string or contains spaces).
    void	 saveAscii(std::ostream &os) const	{ save(os, false); }
    void	 saveAscii(UT_OStream &os) const    	{ save(os, false); }

    /// Save string to stream. Saves as binary if @em binary is true.
    void	 save(std::ostream &os, bool binary) const;
    void	 save(UT_OStream &os, bool binary) const;

    /// Load string from stream. Use is.eof() to check eof status
    bool	 load(UT_IStream &is);

    /// Reset the string to the default constructor.
    void	 clear()
		    { *this = (const char *)NULL; }

    /// Prepend a string (or character)
    // @{
    void	 prepend(const char *prefix);
    void	 prepend(char ch);
    // @}

    /// Append a character
    void	 append(char ch);

    /// Append a string or a section of a string.
    void	 append(const char *str, exint len = -1);

    /// Remove the last character
    void	 removeLast()	{ truncate(length()-1); }
    /// Truncate the string at the Nth character
    void	 truncate(exint len);

    UT_String	&operator=(const UT_String &str);
    UT_String	&operator=(const char *str);
    UT_String	&operator=(const std::string &str);
    UT_String	&operator=(const UT_StringHolder &str);
    UT_String	&operator=(const UT_StringView &str);
private:
    /// Not implemented - see UT_String(const UT_StringRef &).
    UT_String   &operator=(const UT_StringRef);

public:
    UT_String	&operator+=(const char *str)
		 {
		     if (!isstring())
		     {
			// We are an empty string, so we merely copy
			// the incoming string rather than trying to append
			// to it.
			harden(str);
		     }
		     else
		     {
			bool same = (str == myData);
			harden();
			if (str)
			{
			    int mylen = (int)strlen(myData);
			    myData = (char *)realloc(myData,
						     mylen+strlen(str)+1);
			    if (!same)
			    {
				strcpy(&myData[mylen], str);
			    }
			    else
			    {
				memcpy(myData + mylen, myData, mylen);
				myData[mylen * 2] = '\0';
			    }
			}
		     }
		    return *this;
		 }

    UT_String	&operator+=(const UT_String &str)
		 {
		     *this += (const char *)str.myData;
		     return *this;
		 }
    UT_String	&operator+=(const UT_StringRef &str);

    // Basic equality functions and operators
    int		 compare(const char *str, bool case_sensitive=true) const
		 {
		     // Unlike std::string, UT_String treats NULL and
		     // the empty string as distinct (empty has precedence).
		     if (myData==0 || str==0)
		     {
			 if (myData) return 1;
			 if(str) return -1;
			 return 0;
		     }
		     if (case_sensitive)
			 return strcmp(myData, str);
		     return strcasecmp(myData, str);
		 }
    int		 compare(const UT_String &str, bool case_sensitive=true) const
		 {
		     return compare(str.myData,case_sensitive);
		 }
    int		 compare(const UT_StringRef &str, bool case_sensitive=true) const;
		
    bool	 equal(const char *str, bool case_sensitive=true) const
		 {
		     return compare(str,case_sensitive)==0;
		 }
    bool	 equal(const UT_String &str, bool case_sensitive=true) const
		 {
		     return compare(str.myData,case_sensitive)==0;
		 }
    bool	 equal(const UT_StringRef &str, bool case_sensitive=true) const
		 {
		     return compare(str,case_sensitive)==0;
		 }
    
    bool	 operator==(const char *str) const
		 {
		     return compare(str)==0;
		 }
    bool	 operator==(const UT_String &str) const
		 {
		     return compare(str.myData)==0;
		 }
    bool	 operator==(const UT_StringRef &str) const
		 {
		     return compare(str)==0;
		 }
    bool	 operator!=(const char *str) const
		 {
		     return compare(str)!=0;
		 }
    bool	 operator!=(const UT_String &str) const
		 {
		     return compare(str.myData)!=0;
		 }
    bool	 operator!=(const UT_StringRef &str) const
		 {
		     return compare(str)!=0;
		 }
    bool 	operator<(const char *str) const
		 {
		     return compare(str)<0;
		 }
    bool	 operator<(const UT_String &str) const
		 {
		     return compare(str.myData)<0;
		 }
    bool	 operator<(const UT_StringRef &str) const
		 {
		     return compare(str)<0;
		 }
    bool 	operator<=(const char *str) const
		 {
		     return compare(str)<=0;
		 }
    bool	 operator<=(const UT_String &str) const
		 {
		     return compare(str.myData)<=0;
		 }
    bool	 operator<=(const UT_StringRef &str) const
		 {
		     return compare(str)<=0;
		 }
    bool	 operator>(const char *str) const
		 {
		     return compare(str)>0;
		 }
    bool	 operator>(const UT_String &str) const
		 {
		     return compare(str.myData)>0;
		 }
    bool	 operator>(const UT_StringRef &str) const
		 {
		     return compare(str)>0;
		 }
    bool 	operator>=(const char *str) const
		 {
		     return compare(str)>=0;
		 }
    bool	 operator>=(const UT_String &str) const
		 {
		     return compare(str.myData)>=0;
		 }
    bool	 operator>=(const UT_StringRef &str) const
		 {
		     return compare(str)>=0;
		 }

    /// Test whether the string is defined or not
    SYS_SAFE_BOOL	operator bool() const	{ return isstring(); }

    /// Return the edit distance between two strings.
    /// See http://en.wikipedia.org/wiki/Levenshtein_distance for details.
    /// allow_subst controls whether a substitution of a character with
    /// another is a single operation, rather than two operations of
    /// insert and delete.
    int		 distance(const char *str,
       		          bool case_sensitive = true,
       		          bool allow_subst = true) const;

    operator	 const char *() const
		 { return (const char *)myData; }
    operator	 char *()
		 { return myData; }

    const char	*c_str() const	{ return buffer(); }
    const char	*buffer() const { return myData; }
    const char	*nonNullBuffer() const { return myData ? myData : ""; }

    char	 operator()(unsigned i) const
		 {
		     UT_ASSERT_P( isstring() );
		     UT_ASSERT_SLOW(i <= strlen(myData));
		     return myData[i];
		 }

    char	&operator()(unsigned i)	
		 {
		     harden();
		     return myData[i];
		 }

    // Prefer using write() since ideally the non-const operator() is removed
    inline void  write(unsigned i, char c)
		 {
		    hardenIfNeeded();
		    myData[i] = c;
		 }

    int		 toInt() const;
    fpreal	 toFloat() const;

    /// Converts the contents of this UT_String to a std::string.  Note that
    /// std::string can't be constructed with a null pointer, so you can't
    /// just write std::string s = ut_string.buffer();
    std::string toStdString() const;

    //
    //  Here, we're finished with operators
    //

    /// Return length of string
    unsigned	 length(void) const
		 { return (myData) ? (unsigned)strlen(myData) : 0; }

    /// Return memory usage in bytes
    int64	 getMemoryUsage(bool inclusive=true) const
    {
        return (inclusive ? sizeof(*this) : 0)
             + (!myIsReference ? (length() + 1)*sizeof(char) : 0);
    }

    /// Find first occurrance of character. Returns NULL upon failure.
    char	*findChar(int c) const
		 { return (myData) ? strchr(myData, c) : 0; }

    ///	Find first occurrance of any character in @em str
    // @{
    char	*findChar(const char *str) const 
		 { return (myData) ? strpbrk(myData, str) : NULL; }
    char	*findChar(const UT_String &str) const
		 { return findChar((const char *) str); }
    // @}

    /// Find last occurance of character
    char	*lastChar(int c) const
		 { return myData ? strrchr(myData, c):0; }

    /// Return the number of occurrences of the specified character.
    int		 countChar(int c) const;

    /// Count the occurrences of the string
    int		 count(const char *str, bool case_sensitive = true) const;

    char	*findNonSpace() const;
    const char	*findWord(const char *word) const;
    bool	 findString(const char *str, bool fullword,
			    bool usewildcards) const;
    int		 changeWord(const char *from, const char *to, int all=1);
    int		 changeString(const char *from, const char *to, bool fullword);
    int		 changeQuotedWord(const char *from, const char *to,
				  int quote = '`', int all = 1);

    int		 findLongestCommonSuffix( const char *with );

    /// Perform deep copy of the substring starting from @em index
    /// for @em len characters into the specified UT_String.
    /// If @em len is too long, then a substring starting from @em index to
    /// the end of the string is copied.
    /// Returns the length of the copied substring.
    int		 substr(UT_String &buf, int index, int len=0) const;

    /// Determine if string can be seen as a single floating point number
    unsigned	 isFloat(int skip_spaces = 0, int loose = 0, bool allow_underscore = false) const;
    /// Determine if string can be seen as a single integer number
    unsigned	 isInteger(int skip_spaces = 0) const;

    void	 toUpper()
		 {
		    char *ptr; 
		    harden();
		    for (ptr=myData; *ptr; ptr++)
			*ptr = (char)toupper(*ptr);
		 }
    void	 toLower()
		 {
		    char *ptr;
		    harden();
		    for (ptr=myData; *ptr; ptr++)
			*ptr = (char)tolower(*ptr);
		 }


    /// Return last component of forward slash separated path string
    ///
    /// If there is a slash in the string, fileName() returns the string
    /// starting after the slash.  Otherwise, it returns the contents of
    /// this string.  Note that it returns a pointer into this string.
    const char  *fileName() const
		 {
		     const char		*fname;

		     if (!myData)
			 return 0;

		     fname = lastChar('/');

		     if (!fname)
		     {
			 fname = myData;
		     }
		     else
		     {
			 fname++;	// Get past the /
		     }
		     return fname;
		 }
    /// Return the extension of a file path string
    const char	*fileExtension( ) const
		 {
		     if( !isstring() )
			 return 0;

		     const char *dot = lastChar('.');
		     if (dot)
		     {
			 const char	*slash = lastChar('/');

			 if (slash && slash > dot)
			     dot = NULL;
		     }
		    return dot;
		 }
    /// Return whether the file extension matches.  The extension passed in
    /// should include the '.' separator.  For example: @code
    ///     matchFileExtension(".jpg")
    /// @endcode
    bool	 matchFileExtension(const char *match_extension) const
		 {
		     const char	*ext = fileExtension();
		     return ext && !SYSstrcasecmp(ext, match_extension);
		 }
    /// Return path terminated just before the extension.
    /// If the filename starts with '.' and no path is provided,
    /// returns NULL
    UT_String	pathUpToExtension() const;

    /// Replace the file extension and return the new string
    UT_String   replaceExtension(const UT_String &new_ext) const;

    /// Split a path into @em dir_name and @em file_name, where @em file_name
    /// is everything after the final slash (i.e. the same as fileName()).
    /// Either part may be empty.  Note that if the string starts with / and
    /// only contains that one slash, the @em dir_name will be / and not blank.
    /// @em dir_name and @em file_name will either be set to hardened strings
    /// or an empty string.
    void	 splitPath(UT_String &dir_name, UT_String &file_name) const;

    /// Decompose a filename into various parts
    ///
    /// parseNumberedFileName will breakup a filename into its various
    /// parts: file = prefix$Fsuffix (note: suffix is
    /// not the same as file extension.) 0 is returned if there is
    /// no frame number. 'negative' allows -[frame] to be interpreted as a
    /// negative number. 'fractional' allows [frame].[number] to be interpreted
    /// as a fractional frame.
    int		 parseNumberedFilename(UT_String &prefix,
				       UT_String &frame, 
				       UT_String &suff, 
				       bool negative = true,
				       bool fractional = false) const;

    bool	 isstring() const
		 { return (myData && *myData); }

    /// trimSpace() will remove all space characters (leading and following)
    /// from a string.  If the string consists of multiple words, the words will
    /// be collapsed.  The function returns 1 if space was trimmed.
    int		 trimSpace(bool leaveSingleSpaceBetweenWords = false);

    /// A version of trimSpace() that only removes leading and following spaces
    /// from a string, leaving any between words intact.
    int		 trimBoundingSpace();

    /// strips out all characters found in 'chars'. The string length will be
    /// reduced by the number of characters removed. The number of characters
    /// removed is returned.
    int		 strip(const char *chars);

    /// protectString() will modify the existing string to escape double quotes
    /// and backslashes.  It will only wrap the string in double quotes if
    /// it has spaces in it. If 'protect_empty' is true, the string will
    /// become '""', otherwise it will stay empty.
    void         protectString(bool protect_empty=false);

    /// returns true if the string begins and ends with a (non-escaped) quote
    /// 'delimiter'.
    bool	 isQuotedString(char delimiter='\'') const;
    
    /// makeQuotedString() is similar to protectString() except it returns a
    /// new string instead of changing this string, it does wrap the string
    /// in quotes, and it lets you use either ' or " as the delimiter.
    /// The quoted string can also be optionally be made to escape non-printing
    /// characters. The string that's returned is UT_String::ALWAYS_DEEP.
    UT_String	 makeQuotedString(char delimiter='\'',
             	                  bool escape_nonprinting=false) const;

    /// makeSmartQuotedString() will use either ' or " as the delimiter to
    /// avoid escaped quotes, using the default delimiter if it doesn't
    /// matter.  The quoted string can also be optionally be made to escape
    /// non-printing characters.  The string that's returned is
    /// UT_String::ALWAYS_DEEP.
    UT_String	 makeSmartQuotedString(char default_delimiter='\'',
             	                       bool escape_nonprinting=false) const;

    /// Expands standard control sequences ('\\n', '\\r', '\\t', '\\0') to their
    /// corresponding ASCII values (10, 13, 9, 0, respectively).
    /// If the expand_extended flag is enabled, an extended expansion is enabled
    /// which adds hexadecimal, decimal and Unicode control sequence expansion.
    /// Any values resulting from that expansion, which are outside the standard
    /// ASCII range, will be encoded as UTF8-encoded control points.
    void	 expandControlSequences(bool expand_extended = false);

    bool	 hasWhiteSpace() const;

    void	 removeTrailingSpace();
    void	 removeTrailingChars(char chr);

    void	 removeTrailingDigits();

    //  cshParse() does not need to harden the string.  It does very robust
    //	parsing in the style of csh.  It actually does better parsing than
    //	csh.  Variable expansion & backquote expansion are done in the
    //	correct order for the correct arguments.  One caveat is that the
    //	string cannot have \0377 (0xff) as a character in it.
    //
    //  If there is an error in parsing, the error flag (if passed in) will be
    //  set to:
    //		0 = no error
    //		1 = line too long
    int		cshParse(char *argv[], int maxArgs,
			void (*vlookup)(const char *, UT_String&)=UTvarLookup,
			void (*elookup)(const char *, UT_String&)=UTexprLookup,
			int *error = 0,
			UT_StringCshIO *io=0);

    int		cshParse(UT_WorkArgs &argv,
			void (*vlookup)(const char *, UT_String&)=UTvarLookup,
			void (*elookup)(const char *, UT_String&)=UTexprLookup,
			int *error = 0,
			UT_StringCshIO *io=0);

    //  dosParse() uses the semi-braindead approach of ms-dos to argument
    //  parsing.  That is, arguments are separated by a double quote or space
    //  (being a space or a tab). If 'preserve_backslashes' is set to
    //  false (the default), back-slashes are passed through verbatim, unless
    //  the following character is a double quote. Likewise, any pairs of
    //  back-slashes preceding a double quote are turned into single
    //  back-slashes.
    int		dosParse(UT_WorkArgs &argv, bool preserve_backslashes=false);
    int		dosParse(char *argv[], int maxArgs,
       		         bool preserve_backslashes=false);

    /// Perform dos parsing modifying the buffer passed in.  The args will be
    /// stored as raw pointers into the given buffer
    static int	dosParse(char *buffer, UT_WorkArgs &args,
			bool preserve_backslashes);

    //	parse will insert nulls into the string.
    //  NB: The argv array is null terminated, thus the effective
    //      maximum number of arguments is one less than maxArgs.
    //	NB: The maxArgs variants are all deprecated, use UT_WorkArgs
    //      instead.
    int		 parse(char *argv[], int maxArgs,
			const char *quotes = "\"'", bool keep_quotes = false)
		 {
		     harden();
		     return parseInPlace(argv, maxArgs, quotes, keep_quotes);
		 }
    int		 parse(UT_WorkArgs &argv, int start_arg = 0,
			const char *quotes = "\"'", bool keep_quotes = false)
		 {
		     harden();
		     return parseInPlace(argv, start_arg, quotes, keep_quotes);
		 }
    // Warning: the following methods insert nulls into the string without
    // hardening.
    int		 parseInPlace(char *argv[], int maxArgs,
			const char *quotes = "\"'", bool keep_quotes = false);
    int		 parseInPlace(UT_WorkArgs &argv, int start_arg = 0,
			const char *quotes = "\"'", bool keep_quotes = false);

    // Splits the string at specific separator characters. Unlike the parse
    // methods, the tokenize methods ignore quoting completely.
    int		 tokenize(char *argv[], int maxArgs, char separator)
		 {
		     harden();
		     return tokenizeInPlace(argv, maxArgs, separator);
		 }
    int		 tokenizeInPlace(char *argv[], int maxArgs, char separator);
    int		 tokenize(UT_WorkArgs &argv, char separator)
		 {
		     harden();
		     return tokenizeInPlace(argv, separator);
		 }
    int		 tokenizeInPlace(UT_WorkArgs &argv, char separator);
    int		 tokenize(char *argv[], int maxArgs,
			  const char *separators = " \t\n")
		 {
		     harden();
		     return tokenizeInPlace(argv, maxArgs, separators);
		 }
    int		 tokenizeInPlace(char *argv[], int maxArgs, 
				 const char *separators = " \t\n");
    int		 tokenize(UT_WorkArgs &argv, const char *separators = " \t\n")
		 {
		     harden();
		     return tokenizeInPlace(argv, separators);
		 }
    int		 tokenizeInPlace(UT_WorkArgs &argv,
				 const char *separators = " \t\n");

    template<typename T>
    int		 tokenize(T &list, const char *separators = " \t\n")
		 {
		     harden();
		     return tokenizeInPlace(list, separators);
		 }
		 
    template<typename T>
    int		 tokenizeInPlace(T &list,
				 const char *separators = " \t\n")
		 {
		     char	*token;
		     char	*context;

		     if (!isstring())
			 return 0;
		     if (!(token = SYSstrtok(myData, separators, &context)))
			 return 0;

		     list.append(token);

		     while ((token = SYSstrtok(0, separators, &context)) != NULL)
			 list.append(token);

		     return list.entries();
		 }
    

    // Replaces the contents with variables expanded.
    void	 expandVariables();

    //	Functions to hash a string
    SYS_FORCE_INLINE uint32 hash() const
    {
        return hash(myData);
    }

    static SYS_FORCE_INLINE uint32 hash(const char *str, uint32 code = 0)
    {
        return SYSstring_hashseed(str, SYS_EXINT_MAX, code);
    }

    //	This does pattern matching on a string.  The pattern may include
    //	the following syntax:
    //		? = match a single character
    //		* = match any number of characters
    //		[char_set] = matches any character in the set
    unsigned	 match(const char *pattern, int caseSensitive=1) const;

    // Similar to match() except it assumes that we're dealing with file paths
    // so that it determines whether to do a case-sensitive match depending on
    // the platform.
    unsigned	 matchFile(const char *pattern) const;

    // Similar to match() but uses rsync style matching:
    //		*  = match any number of characters up to a slash
    //		** = match any number of characters, including a slash
    unsigned	 matchPath(const char *pattern, int caseSensitive=1) const;

    //  multiMatch will actually check multiple patterns all separated
    //	by the separator character:  i.e. geo1,geo2,foot*
    //
    //	NOTE: No pattern or may contain the separator 
    unsigned	multiMatch(const char *pattern, 
			   int caseSensitive, char separator) const;
    unsigned	multiMatch(const char *pattern, int caseSensitive = 1,
			   const char *separators = ", ",
			   bool *explicitlyExcluded = 0,
			   int *matchIndex = 0,
			   ut_PatternRecord *pattern_record=NULL) const;
    unsigned	multiMatch(const UT_StringMMPattern &pattern,
			   bool *explicitlyExcluded = 0,
			   int *matchIndex = 0,
			   ut_PatternRecord *pattern_record=NULL) const;

    // this method matches a pattern while recording any wildcard
    // patterns used.
    unsigned	multiMatchRecord(const char *pattern, int maxpatterns,
	    			 char *singles, int &nsingles,
				 char **words,  int &nwords,
				 int case_sensitive = 1, 
				 const char *separators = ", ") const;
    unsigned	multiMatchRecord(const UT_StringMMPattern &pattern,
	    			 int maxpatterns,
	    			 char *singles, int &nsingles,
				 char **words,  int &nwords) const;
    unsigned	multiMatchRecord(const char *pattern, 
				 UT_StringHolder &singles,
				 UT_StringArray &words,
				 int case_sensitive = 1, 
				 const char *separators = ", ") const;

    /// matchPattern(UT_WorkArgs &) assumes that the arguments contain the
    /// components of a pattern to be matched against.  The method returns
    /// true if the pattern matches, false if it doesn't.  This matching
    /// process handles ^ expansion properly (and efficiently).
    /// If the string doesn't match any components of the pattern, then the
    /// assumed value is returned.
    bool	matchPattern(const UT_WorkArgs &pattern_args,
			     bool assume_match=false) const;

    static int	multiMatchCheck(const char *pattern);
    static int  wildcardMatchCheck(const char *pattern);

    // Same as match but equivalent to "*pattern*"
    bool	contains(const char *pattern, bool case_sensitive=true) const;

    // Returns true if our string starts with the specified prefix.
    bool	startsWith(const char *prefix,
        	           bool case_sensitive = true,
			   exint len = -1) const;

    // Returns true if our string ends with the specified suffix.
    bool	endsWith(const char *suffix,
        	         bool case_sensitive = true,
			 exint len = -1) const;

    /// Pluralize an English noun ending (i.e. box->boxes or tube->tubes).  The
    /// ending must be lower case to be processed properly.
    void	pluralize();

    // Will parse strings like 1-10:2,3 and call func for every element
    // implied.  It will stop when the func returns 0 or the parsing
    // is complete, in which case it returns 1.  
    // Parsing also allows secondary elements to be specified eg 3.4 0.12
    // The secfunc is used to find the maximum index of secondary elements
    // for each compound num. The elements are assumed to be 
    // non-negative integers.
    int		 traversePattern(int max, void *data, 
			         int (*func)(int num, int sec, void *data),
				 unsigned int (*secfunc)(int num,void *data)=0,
				 int offset=0) const;

    // Fast containment, assumes no special characters
    const char	*fcontain(const char *pattern, bool case_sensitive=true) const
		 {
		    if (!myData) return NULL;
		    return case_sensitive ? strstr(myData, pattern)
					  : SYSstrcasestr(myData, pattern);
		 }

    // Given the match pattern which fits our contents, any assigned wildcards
    // are subsitituted. The wildcards may also be indexed.
    // Returns true if rename was successful.
    //
    // @note This code was adapted from CHOP_Rename::subPatterns() and
    //	     works the same way.
    //
    // eg. this = apple, match = a*le, replace = b* ---> bpp
    //     this = a_to_b, match = *_to_*, replace = *(1)_to_*(0) ---> b_to_a
    bool patternRename(const char *match_pattern, const char *replace);

    // Given the name rule according to which a name consists of a base name
    // (char sequence ending in a non-digit) and a numerical suffix, the 
    // following two methods return the base and the suffix respectively. 
    // base() needs a string buffer and will return a const char* pointing to it.
    // base() always returns a non-zero pointer, 
    // while suffix() returns 0 if no suffix is found.
    const char	*base(UT_String &buf) const;
    const char	*suffix(void) const;

    // incrementNumberedName will increment a name.  If it has a numerical
    // suffix, that suffix is incremented.  If not, "2" is appended to the
    // name.
    void	 incrementNumberedName();

    // setFormat is used to set how an outstream formats its ascii output.
    // So you can use printf style formatting. eg:
    //		UT_String::setFormat(cout, "%08d") << 100;
    //
    // Note: Don't do:
    //		cout << UT_String::setFormat(cout, "%08d") << 100;
    //		^^^^
    // Also: The formating changes (except for field width) are permanent,
    // so you'll have to reset them manually.
    //
    // TODO: A resetFormat, and a push/pop format pair.
    static std::ostream	&setFormat(std::ostream &os, const char *fmt);
    std::ostream	&setFormat(std::ostream &os);

    int		 replacePrefix(const char *oldpref, 
			       const char *newpref);
    int		 replaceSuffix(const char *oldsuffix, 
			       const char *newsuffix);

    // expandArrays will expand a series of tokens of the
    // form prefix[pattern]suffix into the names array
    //
    // Note: Each names[i] must be free'd after use
    //       and label is used on the non-const parse method
    int		 expandArrays(char *names[], int max);

    // This routine will ensure no line is over the specified
    // number of columns.  Offending lines will be wrapped at
    // the first spaceChar or cut at exactly cols if spaceChar
    // is not found.
    // It returns one if any changes were done.
    // It currently treats tabs as single characters which should be
    // changed.
    // It will break words at hyphens if possible.
    int		format(int cols);

    // this method is similar to changeWord.. This method performs
    // a "dumb" substitution.   Return's the # of substitutions
    int		substitute( const char *find, const char *replacement,
			    bool all = true );

    // This function replaces the character found with another character.
    int		substitute( char find, char replacement, bool all = true );

    // this function removes the substring at pos and len, and inserts str
    // at pos. it returns the difference (new_length - old_length)
    int		replace( int pos, int len, const char *str );

    // remove the first len characters of this string
    int		eraseHead(int len)
		    { return replace(0, len, ""); }

    // remove the last len characters of this string
    int		eraseTail(int len)
		    { return replace(length() - len, len, ""); }

    // remove the substring start at pos for len characters
    int		erase(int pos = 0, int len = -1)
		    {
			if (len < 0)
			    len = length() - pos;			
			return replace(pos, len, "");
		    }

    // insert the given string at pos into this string
    int		insert(int pos, const char *str)
		    { return replace(pos, 0, str); }

    //  Does a "smart" string compare which will sort based on numbered names.
    //  That is "text20" is bigger than "text3".  In a strictly alphanumeric
    //  comparison, this would not be the case.  Zero is only returned if both
    //  strings are identical.
    static int	compareNumberedString(const char *s1, const char *s2,
				bool case_sensitive=true,
				bool allow_negatives=false);
    static int	qsortCmpNumberedString(const char *const*v1, const char *const*v2);

    //  Like compare numbered strings, but it sorts better when there are
    //  .ext extensions (i.e. it handles '.' as a special case)
    static int	compareNumberedFilename(const char *s1, const char *s2,
				bool case_sensitive=false);
    static int	qsortCmpNumberedFilename(const char *const*v1, const char *const*v2);

    /// Compare two version strings which have numbered components separated by
    /// dots. eg. "X.Y.Z". Assumes the components go from most to least
    /// significant in left to right order.
    static int	compareVersionString(const char *s1, const char *s2);

    /// Given a path, set the value of the string to the program name.  For
    /// example: @code
    ///   str.extractProgramName(argv[0]);
    ///   str.extractProgramName("c:/Path/program.exe");
    ///   str.extractProgramName("/usr/bin/program");
    /// @endcode
    /// This will extract the last path component.  Program names may also have
    /// their extensions stripped.  For example ".exe" on Windows and "-bin" to
    /// strip the Houdini wrappers on other platforms.
    ///
    /// @note The path should be normalized to have forward slashes as the path
    /// separator.
    void	extractProgramName(const char *path,
			    bool strip_extension=true,
			    bool normalize_path=true);

    /// Given a path, check to see whether the program name matches the
    /// expected.  For example: @code
    ///   if (UT_String::matchProgramname(argv[0], "houdini"))
    ///   if (UT_String::matchProgramname("c:/Path/houdini.exe", "houdini"))
    ///   if (UT_String::matchProgramname("/usr/bin/houdini", "houdini"))
    /// @endcode
    /// The matching is always case-insensitive.
    ///
    /// @note The path should be normalized to have forward slashes as the path
    /// separator.
    static bool	matchProgramName(const char *path, const char *expected,
			    bool normalize_path=false);

    /// Convert a path to a "normalized" path.  That is, all back-slashes will
    /// be converted to forward slashes.  On some operating systems, this will
    /// leave the string unchanged.
    void	normalizePath();

    // A very fast integer to string converter.  This is faster (at least on
    // SGI) than using sprintf("%d").  About two to three times as fast.  Both
    // of these methods return the length of the string generated.
    static int	itoa(char *str, int64 i);
    static int	utoa(char *str, uint64 i);

    // Versions of the above functions which set into this string object
    void	itoa(int64 i);
    void	utoa(uint64 i);

    // A reader-friendly version of itoa.  This places commas appropriately
    // to ensure the person can pick out the kilo points easily.
    // This can handle numbers up to 999,999,999,999,999,999.
    void	itoa_pretty(int64 val);

    /// Convert the given time delta (in milliseconds) 
    /// to a reader-friendly string.
    void	timeDeltaToPrettyString(double time_ms);

    // Do an sprintf into this string. This method will allocate exactly the
    // number of bytes required for the final string. If the format string is
    // bad, isstring() will return false afterwards.
    int		sprintf(const char *fmt, ...) SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    // This will change the string into a valid C style variable name.
    // All non-alpha numerics will be converted to _.
    // If the first letter is a digit, it is prefixed with an _.
    // This returns 0 if no changes occurred, 1 if something had to
    // be adjusted.
    // Note that this does NOT force the name to be non-zero in length.
    // The safechars parameter is a string containing extra characters
    // that should be considered safe. These characters are not
    // converted to underscores.
    int		forceValidVariableName(const char *safechars = NULL);
    // Returns true if the string matches a C-style varaible name.
    // The safechars are not allowed to be the start.
    // Matching forceValid, empty strings are considered valid!
    bool	isValidVariableName(const char *safechars = NULL) const;

    // This will force all non-alphanumeric characters to be underscores.
    // Returns true if any changes were required.
    bool	forceAlphaNumeric();

    // This function will calculate the relative path to get from src to dest.
    // If file_path is false, this method assume it is dealing with node paths.
    // If file_path is true, it will also deal with Windows drive letters and
    // UNC paths.
    void	getRelativePath(const char *src_fullpath,
				const char *dest_fullpath,
				bool file_path = false);

    // This function takes two absolute paths and returns the length of the
    // longest common path prefix, up to and including the last '/'.  This
    // means, for instance, that if fullpath1[len1-1] == '/' then all of 
    // fullpath1 is eligible as a common prefix.
    // NB: This function DOES NOT handle NT style drive names! It is currently
    //     only used for op paths. If you want to add support for this, you
    //     should add another default parameter to do this.
    static int	findLongestCommonPathPrefix(const char *fullpath1, int len1,
					    const char *fullpath2, int len2);

    // This function tests whether we are an absolute path, and returns true or
    // false depending on whether we are.
    bool	isAbsolutePath(bool file_path=false) const;

    // This function assumes that we are an absolute path and will remove all
    // un-necessary components from it as long as we remain an absolute path.
    // We return false if an error was encountered, in which case the results
    // are unpredictable.
    bool	collapseAbsolutePath(bool file_path=false);

    // This function will make sure that the string is at most max_length
    // characters long.  If the string is longer than that, it will
    // replace the middle of the string by "...".  Returns true if the string
    // has changed and false otherwise.  max_length must be greater than 3.
    bool	truncateMiddle(int max_length);

    // This function is an abomination when you can just write:
    //	    UT_String foo("");
    //	    ...
    //	    if (foo.isstring())
    //		...
    // Avoid using it and do not write functions that return "const UT_String&"
    static const UT_String	&getEmptyString();

    /// Count the number of valid characters in the : modifier for variable
    /// expansion.  For example, the string ":r" will return 2, the string
    /// ":r:t" will return 4, the string ":z" will return 0.  These use the csh
    /// expansion modifiers.
    ///
    /// If the string doesn't start with a ':', the method will return 0.
    static int	countCshModifiers(const char *src);

    /// Applies a "csh" style modifier string to this string.  For example, a
    /// modifier string of ":e" would replace the string with the file
    /// extension of the string.
    ///
    /// Returns true if any modifications were performed
    bool	applyCshModifiers(const char *modifiers);


    /// This will remove the range from a string of the form foo$Fbar.ext (#-#)
    /// and return the first number from the range. If there is only 1 range 
    /// number, it will be returned. If there is no range, 0 is returned.
    /// The returned string is hardened.
    UT_String	removeRange ();
    
    /// This will format a value to represent a given size in bytes, kilobytes,
    /// megabytes, etc.
    void	formatByteSize(exint size, int digits=2);

    // UTF-8 helpers

    /// Returns the number of Unicode codepoints in the string, assuming it's
    /// encoded as UTF-8.
    int		getCodePointCount() const;

    /// Returns a list of Unicode code points from this string.
    void	getAsCodePoints(UT_Int32Array &cp_list) const;

    /// Friend specialization of std::swap() to use UT_String::swap()
    /// @internal This is needed because standard std::swap() implementations
    /// will try to copy the UT_String objects, causing hardened strings to
    /// become weak.
    friend void	swap(UT_String& a, UT_String& b) { a.swap(b); }

private:
    template <typename OSTREAM>
    void	 saveInternal(OSTREAM &os, bool binary) const;

    void	 freeData();

    /// implements a few csh-style modifiers.
    /// @param mod pointer to a string starting with the modifier to apply.
    /// so, to apply a global substitute modifier :gs/l/r/ 
    /// mod should be: s/l/r
    /// @param all True if all possible modifications should be 
    /// (recursively) performed.
    /// Otherwise, at most one modification is applied.
    /// @return whether any modification was performed
    bool	applyNextModifier(const char *mod, bool all);


    /// Sets myIsReference to false and copies the other_string into myData,
    /// but attempts to avoid unnecessary memory reallocations. Frees up
    /// any previous data, if necessary. If other_string is NULL, the call
    /// is equivalent to freeData().
    void	 doSmartCopyFrom(const char* other_string);

    static int	compareNumberedStringInternal(const char *s1, const char *s2,
					      bool case_sensitive,
					      bool allow_negatives,
					      bool dot_first);

    static SYS_FORCE_INLINE void  utStrFree(char *str)
    {
#if defined(UT_DEBUG) && !defined(_WIN32)
	if (str)
	    ::memset((void *)str, 0xDD, ::strlen(str) + 1);
#endif
	::free((void *)str);
    }

    char	*myData;
    bool	 myIsReference:1,
		 myIsAlwaysDeep:1;

    /// This operator saves the string to the stream via the string's
    /// saveAscii() method, protecting any whitespace (by adding quotes),
    /// backslashes or quotes in the string.
    friend UT_API std::ostream	&operator<<(std::ostream &os, const UT_String &d);
    friend UT_API UT_OStream	&operator<<(UT_OStream &os, const UT_String &d);

    friend class UT_API UT_StringRef;
};

/// Creates a shallow wrapper around a string for calling UT_String's many
/// algorithms.
class UT_API UT_StringWrap : public UT_String
{
public:
    // We only have a single constructor which is always shallow.
    SYS_FORCE_INLINE
    UT_StringWrap(const char *str)
	: UT_String(str)
    {}
    // It seems necessary on MSVC to forceinline the empty constructor in order
    // to have it inlined.
    SYS_FORCE_INLINE
    ~UT_StringWrap()
    {}
};
		
SYS_FORCE_INLINE
UT_String::~UT_String()
{
    if (!myIsReference && myData)
	utStrFree(myData);
}

SYS_FORCE_INLINE
void
UT_String::freeData()
{
    if (myData)
    {
	if (!myIsReference)
	    utStrFree(myData);
	myData = 0;
    }
}

inline void
UT_String::swap( UT_String &other )
{
    // We can't use UTswap because it doesn't work with bit fields.
    bool temp = myIsReference;
    myIsReference = other.myIsReference;
    other.myIsReference = temp;

    char *tmp_data = myData;
    myData = other.myData;
    other.myData = tmp_data;

    if (myIsAlwaysDeep)
	harden();

    if (other.myIsAlwaysDeep)
	other.harden();
}

class UT_API UT_StringCshIO {
public:
    UT_String	 myOut;		// Points to argument following '>'
    UT_String	 myErr;		// Points to argument following '>&'
    UT_String	 myIn;		// Points to argument following '<'
    short	 myDoubleOut;	// If the argument is '>>' or '>>&'
    short	 myDoubleIn;	// If the argument is '<<'
};

UT_API std::ostream & do_setformat(std::ostream &os, const char fmt[]);

/// Does a "smart" string compare which will sort based on numbered names.
/// That is "text20" is bigger than "text3".  In a strictly alphanumeric
/// comparison, this would not be the case.
struct UT_API UTnumberedStringCompare
{
    bool operator()(const char *s1, const char *s2) const
    {
	return UT_String::compareNumberedString(s1, s2) < 0;
    }

    bool operator()(const std::string &s1, const std::string &s2) const
    {
	return operator()(s1.c_str(), s2.c_str());
    }
};

#endif
