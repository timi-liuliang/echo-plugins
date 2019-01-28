/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Regex.h ( UT Library, C++)
 *
 * COMMENTS:	A simple wrapper for hboost::regex
 */

#ifndef __UT_Regex__
#define __UT_Regex__

#include "UT_WorkBuffer.h"
#include "UT_StringArray.h"
#include "UT_StringView.h"
#include "UT_Pair.h"

#include <hboost/xpressive/xpressive.hpp>

class UT_API UT_Regex
{
public:
    struct FromGlobTag {};
    static FromGlobTag FromGlob;
    struct FromBundleGlobTag {};
    static FromBundleGlobTag FromBundleGlob;

    /// Constructs an empty regular expression. It will match nothing and
    /// isValid will return false.
			 UT_Regex() {}

    /// Initializes the regular expression from the expression string. Use
    /// isValid to test for validity.
    explicit		 UT_Regex(const char *expr)
			 {
			     init(expr);
			 }

    /// Initializes the regular expression from a glob pattern. Use
    /// isValid to test for validity.
    /// @see convertGlobToExpr() for exact accepted rules
			 UT_Regex(const char *pattern, FromGlobTag)
			 {
			     initFromGlob(pattern);
			 }
			 
    /// Initializes the regular expression from a glob pattern. Use
    /// isValid to test for validity.
    /// @see convertGlobToExpr() for exact accepted rules
			 UT_Regex(const char *pattern, FromBundleGlobTag)
			 {
			     initFromGlob(pattern, true, true);
			 }
						 
    /// Initializes the regular expression from the expression string. If the
    /// expression parses correctly, this function returns true.
    bool		 init(const char *expr, bool case_sensitive = true);

    /// Initializes the regular expression from the glob pattern. If the
    /// expression parses correctly, this function returns true. See
    /// convertGlobToExpr() for syntax.
    bool		 initFromGlob(const char *pattern,
				      bool case_sensitive=true,
				      bool is_bundle_glob=false);
    
    /// The accepted glob pattern rules are as follows:
    ///   - a '*' matches any path component, but it stops at slashes.
    ///   - use '**' to match anything, including slashes.
    ///   - a '?' matches any character except a slash (/).
    ///   - '[',']' matches one of the enclosed characters, eg. [abc]
    ///     - '-' within '[',']' denotes a character range, eg. [a-c]
    ///	  - If first character after '[' is ! or ^, then it matches anything
    ///	    except the enclosed characters.
    ///   - '{','}' matches the any of the comma(',') separated patterns within
    ///   - If is_bundle_glob is true, then the following rules apply as well:
    ///   - '*' matches anything, including slashes. 
    ///   - '%' to match any path component, but stops at slashes. 
    ///   - '(', '|', ')' for grouping.
    /// @return false if glob is NULL or the empty string.
    static bool		 convertGlobToExpr(
			    UT_WorkBuffer &expr,
			    const char *glob,
			    bool is_bundle_glob = false);

    /// Returns true if the regular expression parses correctly. If the return
    /// value is false, use getErrorString to get a human readable error string.
    bool 		 isValid() const;
    const char 		*getErrorString() const;

    /// Returns true if the entire input string matches the regular expression, 
    /// false otherwise.
    bool		 match(const char *string) const;

    /// Returns true if the entire input string matches the regular expression, 
    /// false otherwise. If successful, returns the list of the captured 
    /// sub-strings through the captured string array, excluding the string
    /// itself.
    bool		 match(const char *string,
			       UT_StringArray &captured) const;

    /// Returns true if the entire input string matches the regular expression, 
    /// false otherwise.
    bool		 match(const UT_StringView &str) const;

    /// Returns true if the entire input string matches the regular expression, 
    /// false otherwise. If successful, returns the list of the captured 
    /// sub-strings through the captured string array, excluding the string
    /// itself.
    bool		 match(const UT_StringView &string,
			       UT_StringViewArray &captured) const;
    
    /// Returns true if the regular expression is found somewhere in the input 
    /// string, false otherwise.
    bool		 search(const char *string,
				    exint start = 0, exint end = -1) const;

    /// Returns true if the regular expression is found somewhere in the input
    /// string, false otherwise. If successful, returns the list of the 
    /// captured sub-strings through the captured string array.
    bool		 search(const char *string,
			       UT_StringArray &captured,
				exint start = 0, exint end = -1) const;

    /// Returns true if the regular expression is found somewhere in the input
    /// string, false otherwise. If successful, returns an array of 
    /// pairs of start and end indicies the items where found at.
    bool		 search(const char* string, UT_Array< UT_Pair<exint, exint> >& indicies,
				 exint start = 0, exint end = -1) const;

    /// Returns true if the regular expression is found somewhere in the input
    /// string, false otherwise. If successful, fills results array
    /// with strings that match the entire regex expression (no capture
    /// groups)
    bool		 searchall(const char* string, UT_StringArray& results,
				exint start = 0, exint end = -1) const;

    /// Splits the given string into substrings when the regular expression
    /// is found. Returns true if at least one split has occoured. If no
    /// splits occour, results will contain one entry which is the 
    /// original string.
    bool                 split(const char* string, UT_StringArray& results,
				exint maxsplits = 0) const;
    
    bool		 split(const UT_StringView &str,
			       UT_StringViewArray &results,
			       exint maxsplits = 0) const;
                                                                                                                                   
    /// Replaces the first occurence of the regular expression in the source
    /// string with the replacement value from replace, and writes out the
    /// result. If replace_all is set to true, all occurences are replaced.
    /// If there is no match, the result buffer is set to identical to the 
    /// input string, unless copy_on_fail is false, in which case it is left
    /// untouched.
    bool		 replace(UT_WorkBuffer &result,
				 const char *string,
				 const char *replace,
				 bool replace_all = false,
				 bool copy_on_fail = true
				) const;
    
    /// Same as above except a fixed number of replacements can be set
    /// instead of just all or just one. 0 means replace all. 
    bool		 replace(UT_WorkBuffer &result,
				 const char *string,
				 const char *replace,
				 exint num_replacements,
				 bool copy_on_fail = true
				) const;
    
private:
    hboost::xpressive::cregex myExpr;
    UT_String		 myError;
};

#endif
