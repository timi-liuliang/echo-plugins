/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GOP_GroupParse_h__
#define __GOP_GroupParse_h__

#include "GOP_API.h"

#include <GA/GA_ElementGroup.h>
#include <GA/GA_Types.h>
#include <UT/UT_Assert.h>
#include <SYS/SYS_Types.h>
#include <stddef.h>
#include <string.h>

class UT_String;
class UT_WorkBuffer;
class GA_Attribute;
class GA_AttributeDict;
class GA_Detail;
class GA_EdgeGroup;
class GA_ElementGroup;
class GA_Group;
class GA_GroupTable;

//___________________________________________________________________________

class gopAddData;
class gop_PendingParse;
class GEO_Detail;
class GA_PrimitiveGroup;
class GA_PointGroup;
class GOP_Manager;

class GOP_API GOP_GroupParse
{
    friend class gopAddData;

public:

    // A class for requesting ancillary information about a parse operation.
    class GOP_API ParseInfo
    {
    public:
	ParseInfo()
	    : myNumericTokensFlag(false),
	      myAttributeTokensFlag(false),
	      myNameTokensFlag(false)
	{
	}

	bool	 getParsedNameTokens() const { return myNameTokensFlag; }
	bool	 getParsedAdhocTokens() const
		    { return myNumericTokensFlag || myAttributeTokensFlag; }

	void	 setParsedNameToken(bool value = true)
		    { myNameTokensFlag = value; }
	void	 setParsedNumericToken(bool value = true)
		    { myNumericTokensFlag = value; }
	void	 setParsedAttributeToken(bool value = true)
		    { myAttributeTokensFlag = value; }

    private:
	bool	 myNumericTokensFlag;
	bool	 myAttributeTokensFlag;
	bool	 myNameTokensFlag;
    };

    /// This wrapper of a detail allows parsing of groups
    /// on either a const or non-const GA_Detail using the
    /// same interface for both.  If the detail is const,
    /// any new group will be detached.  If the detail is
    /// non-const, they will be optionally detached.
    class GOP_API GroupCreator
    {
    public:
        GroupCreator() {}
        explicit GroupCreator(const GEO_Detail *detail)
            : myConstDetail(detail)
            , myNonConstDetail(NULL)
        {}
        GroupCreator(GEO_Detail *detail,bool detached)
            : myConstDetail(detail)
            , myNonConstDetail(detached ? NULL : detail)
        {}

        GA_Group *createGroup(GA_GroupType type) const;

        const GEO_Detail &getDetail() const
        {
            return *myConstDetail;
        }

        const GEO_Detail *myConstDetail;
        GEO_Detail *myNonConstDetail;
    };


    GOP_GroupParse(GOP_Manager *manager)
    {
        myManager = manager;
    };
    ~GOP_GroupParse() {};

    // Build a string that contains the id's of the primitives/points in a
    // primitive/point group or a primitive/point selection.  The offset is
    // the amount to add to the id of each primitive/point in the selection.
    static void		 buildPrimGroupToken(const GEO_Detail *gdp,
				const GA_PrimitiveGroup *primgrp,
				UT_String &token,
				GA_Index offset = GA_Index(0),
				bool ordered=true);
    static void		 buildPointGroupToken(const GEO_Detail *gdp,
				const GA_PointGroup *ptgrp,
				UT_String &token,
				GA_Index offset = GA_Index(0),
				bool ordered=true);

    // Parse an attribute such as 'P.z' or 'P[2]'. Returns the component of the
    // vector, or -1 if the index is invalid.
    static int		 parseVectorAttributeComponent(
				const GA_AttributeDict *dict,
                                UT_WorkBuffer &attribname,
                                const GA_Attribute *&attrib);

    const GA_PrimitiveGroup *parsePrimitiveGroups(const char *pat,
				const GroupCreator &creator,
				bool numok,
				bool ordered,
				bool strict,
				GA_Index offset = GA_Index(0),
				ParseInfo *info = NULL)
    {
	return static_cast<const GA_PrimitiveGroup *>
		(parseGroups(pat, GA_GROUP_PRIMITIVE, creator,
			     numok, true, ordered, strict, offset, info));
    }
    const GA_PointGroup	*parsePointGroups(const char *pat,
				const GroupCreator &creator,
				bool numok,
				bool ordered,
				bool strict,
				GA_Index offset = GA_Index(0),
				ParseInfo *info = NULL)
    {
	return static_cast<const GA_PointGroup *>
		(parseGroups(pat, GA_GROUP_POINT, creator,
			     numok, true, ordered, strict, offset, info));
    }

    const GA_EdgeGroup	*parseEdgeGroups(const char *pat,
				const GroupCreator &creator,
				bool strict,
				GA_Index prim_offset = GA_Index(0),
				GA_Index point_offset = GA_Index(0),
				ParseInfo *info = NULL);

    //
    // Similar to above, except the caller is free to modify its contents
    //

    GA_PrimitiveGroup	*parsePrimitiveGroupsCopy(const char *pat,
				const GroupCreator &creator,
				bool numok,
				bool ordered,
				bool strict,
				GA_Index offset = GA_Index(0))
    {
	return static_cast<GA_PrimitiveGroup *>
		(constCastParseGroups(pat, GA_GROUP_PRIMITIVE, creator,
				      numok, false, ordered, strict, offset));
    }

    GA_PointGroup	*parsePointGroupsCopy(const char *pat,
				const GroupCreator &creator,
				bool numok,
				bool ordered,
				bool strict,
				GA_Index offset = GA_Index(0))
    {
	return static_cast<GA_PointGroup *>
		(constCastParseGroups(pat, GA_GROUP_POINT, creator,
				      numok, false, ordered, strict, offset));
    }

    GA_EdgeGroup	*parseEdgeGroupsCopy(const char *pat,
				const GroupCreator &creator,
				bool strict,
				GA_Index prim_offset = GA_Index(0),
				GA_Index point_offset = GA_Index(0),
				ParseInfo *info = 0);

    void		 expandGroupMask(const char *pattern,
				UT_String &outNames,
				GA_GroupType type,
				const GEO_Detail *gdp);
    
    /// Simple helpers to check if the string given is a group string literal 
    /// of a certain type, as output by build*GroupToken. This is call is
    /// not a definitive arbiter of which group literal string is which type,
    /// since their definitions are ambiguous.
    /// Points and primitives: "<num> ..."
    /// Edges: "[<num>e<num>|p<num>[-<num>...]] ..."
    /// Vertices: "[<num>|<num>v<num>[-<num>]] ..."
    /// Breakpoints: "[<num>b[<num>|\[<num>,<num>\]]] ..."
    
    static bool isGroupLiteral(const char *pattern, GA_GroupType type);
    
    static bool isGroupNumeric(const char* pattern);

    static void matchGroups(
        const char *pattern,
        const GA_GroupTable &group_table,
        UT_ValArray<const GA_Group *> &match_list);

    static bool hasAttributePattern(const char *pattern)
	{ return (::strchr(pattern, '@') != nullptr); }

private:

    GOP_Manager		*myManager;

    const GA_Group	*parseGroups(const char *inpattern, GA_GroupType type,
				const GroupCreator &creator,
				bool allow_numeric,
				bool allow_reference,
				bool ordered,
				bool strict,
				GA_Index offset,
				ParseInfo *info = NULL);
    GA_Group		*constCastParseGroups(const char *inpattern,
				GA_GroupType type,
				const GroupCreator &creator,
				bool allow_numeric,
				bool allow_reference,
				bool ordered,
				bool strict,
				GA_Index offset,
				ParseInfo *info = NULL)
    {
        const GA_Group *group = parseGroups(
                inpattern, type, creator, allow_numeric,
                allow_reference, ordered, strict, offset, info);
        UT_ASSERT_MSG_P(!group || group->getInternal(),
			"This group must be a newly-created, ad-hoc group!");
        return SYSconst_cast(group);
    }

    // Parse a block of consecutive numeric patterns, updating group.  Returns
    // a pointer to the first non-whitespace character in pattern following
    // the parsed block.
    //
    // An optional pointer to gop_PendingParse object may be provided to have
    // that parse be processed before any numeric patterns.
    //
    // PRE: *pattern is 0 or a non-whitespace character.
    char		*parseNumericBlock(char *pattern, bool is_prim,
				const GroupCreator &creator,
				GA_ElementGroup *&group,
				bool &adhoc_flag,
				bool ordered, GA_Index offset,
				gop_PendingParse *pending_parse);

    // Parse a single attribute pattern, updating group.  Returns a pointer
    // to the character in pattern immediately following the block.
    //
    // PRE: *pattern == '@' && group != NULL
    char		*parseAttributeGroup(GA_Group *group,
				char *pattern, bool isPrim,
				const GA_Detail &gdp, bool remove, bool invert,
				bool ordered);

    GA_Group		*createAdhocGroup(const GroupCreator &creator,
				GA_GroupType type);
    void		 appendAdhocGroup(GA_Group *group, bool ismetheowner);
    void		 destroyAdhocGroup(const GA_Group *group);

    friend class gop_PendingNamePatternParse;
};

#endif
