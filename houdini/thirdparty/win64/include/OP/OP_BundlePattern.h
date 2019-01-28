/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_BundlePattern.h ( OP Library, C++)
 *
 * COMMENTS:	Pattern tokens for a bundle.  This breaks up the pattern string
 *		into managable tokens which can be processed easily
 */

#ifndef __OP_BundlePattern__
#define __OP_BundlePattern__

#include "OP_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>

class OP_Node;
class OP_BundleFilter;
class UT_Regex;

class OP_API OP_BundlePattern {
public:
    static OP_BundlePattern	*allocPattern(const char *str);
    static void			 freePattern(OP_BundlePattern *pattern);

    int		 argc() const	{ return myPatterns.entries(); }
    const char	*argv(int i) const
		 {
		     return myPatterns(i).org_str;
		 }

    bool	 isAllPattern() const		{ return myAllPattern; }
    bool	 isNullPattern() const		{ return myNullPattern; }

    bool	 hasPattern(const char *str) const;

    bool	 match(const OP_Node *node, const char *relativepath,
		       bool assume_kids_included) const;

    //
    // Raw, single match function.  This can be used in filter traversal.
    bool	 matchNode(const OP_Node *node,
			   const char *relativetopath,
			   bool result_if_no_match,
			   int *excluded) const;

protected:
     OP_BundlePattern(const char *str);
    ~OP_BundlePattern();

private:
    struct Pattern
    {
	bool		 is_pattern;	// false if just a plain string
	bool		 is_negated;	// negated pattern (starts with a ^)
	bool		 is_relative;	// Relative pattern (starts with './')
	const char	*org_str;	// The original string
	const char	*str;		// Adjusted string (for carat)
	const char	*rel_str;	// Relative string (incl carat)
	UT_SharedPtr<UT_Regex> pattern;
	UT_SharedPtr<UT_Regex> rel_pattern;
    };
    
    const char	 	*myPatternToken;
    
    // A copy of the original string. Just used as a storage for the tokenized
    // parts.
    UT_String		 myPattern;
    
    // The list of sub-patterns.
    UT_Array<Pattern>	 myPatterns;
    
    // Here we store a map into the pattern array of strings that are
    // "plain" and contain no glob/regex characters.
    typedef UT_StringMap<exint> PlainPatternMap;
    
    PlainPatternMap 	 myPlainPatterns;
    PlainPatternMap 	 myRelPatterns;
    
    int		  	 myRefCount;
    bool	  	 myAllPattern;
    bool	  	 myNullPattern;
};
#endif

