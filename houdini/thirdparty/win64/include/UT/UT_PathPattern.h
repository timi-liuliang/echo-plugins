/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_PathPattern_h__
#define __UT_PathPattern_h__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_StringHolder.h"
#include "UT_StringArray.h"

// Matches a pattern against a USD path. The pattern uses roughly rsync style
// matching, where "*" only matches within a path component, and "**" matches
// across path components. Also supports standard Houdini "^" operator to
// exclude paths.
class UT_API UT_PathPattern
{
public:
			 UT_PathPattern(const UT_StringArray &pattern_tokens,
				 bool case_sensitive = true);
			 UT_PathPattern(const UT_StringRef &pattern,
				 bool case_sensitive = true);
    virtual		~UT_PathPattern();

    bool		 matches(const UT_StringRef &path) const;
    bool		 getExplicitList(UT_StringArray &tokens) const;
    bool		 getCaseSensitive() const
			 { return myCaseSensitive; }

protected:
    class Token {
    public:
			 Token(const UT_StringHolder &str,
				 bool is_exclude,
				 bool do_path_matching)
			    : myString(str),
			      mySpecialTokenData(nullptr),
			      myIsExclude(is_exclude),
			      myDoPathMatching(do_path_matching),
			      myIsSpecialToken(false)
			 { }
			~Token()
			 { }

	UT_StringHolder		 myString;
	void			*mySpecialTokenData;
	bool			 myIsExclude;
	bool			 myDoPathMatching;
	bool			 myIsSpecialToken;
    };
    virtual bool	 matchSpecialToken(
				const UT_StringRef &path,
				const Token &token) const;
    void		 testForExplicitList();

    UT_Array<Token>	 myTokens;

private:
    void		 init(const UT_StringArray &pattern_tokens);

    int			 myLastExcludeToken;
    bool		 myIsExplicitList;
    bool		 myCaseSensitive;
};

#endif

