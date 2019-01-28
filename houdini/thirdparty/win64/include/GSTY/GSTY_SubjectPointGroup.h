/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GSTY_SubjectPointGroup.h ( GSTY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GSTY_SubjectPointGroup__
#define __GSTY_SubjectPointGroup__

#include "GSTY_API.h"
#include "GSTY_SubjectPoint.h"
#include <GOP/GOP_Manager.h>
#include <STY/STY_SubjectGroup.h>
#include <UT/UT_ConcurrentHashMap.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Array.h>
#include <UT/UT_Lock.h>

class GA_ElementGroup;
class GEO_Detail;

/// Represents a group of primitives that can be affected by style sheets.
class GSTY_API GSTY_SubjectPointGroup : public STY_SubjectGroup
{
public:
			 GSTY_SubjectPointGroup(
				const GEO_Detail &gdp,
				int start_idx, int end_idx,
				int numeric_point_group_offset = 0,
				bool apply_detail_style_attribute = true,
				bool apply_point_style_attribute = true);
    virtual		~GSTY_SubjectPointGroup();

    virtual int			 getNumSubjects() const;
    virtual const STY_Subject	&getSubject(int idx) const;

    const GEO_Detail		&getGdp(int idx) const;
    GA_Offset			 getOffset(int idx) const;
    const GA_ElementGroup	*getGroup(const char *group_str);
    const STY_StyleSheetHandle	&getStyleSheet(const UT_StringHolder &ss_str);

private:
    typedef UT_ConcurrentHashMap<unsigned,
				 const GA_ElementGroup *> gsty_GroupMap;
    typedef UT_ConcurrentHashMap<unsigned,
				 STY_StyleSheetHandle> gsty_StyleSheetMap;

    UT_Array<GSTY_SubjectPoint>			 mySubjects;
    GOP_Manager					 myGopManager;
    const GEO_Detail				*myGdp;
    gsty_GroupMap				 myParsedGroups;
    gsty_StyleSheetMap				 myStyleSheets;
    UT_Lock					 myLock;
    int						 myNumericPointGroupOffset;
};

#endif

