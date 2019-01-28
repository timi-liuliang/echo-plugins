/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OPUI_GraphTextBadge.h (User Interface Library, C++)
 *
 * COMMENTS:
 *		A class to describe a node badge.
 *
 */

#ifndef __OPUI_GraphTextBadge__
#define __OPUI_GraphTextBadge__

#include "OPUI_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_Color.h>
#include <UT/UT_StringHolder.h>

class OPUI_GraphProxyDescriptor;
class OPUI_GraphTextBadge;
class OP_Context;

enum OPUI_GraphTextBadgeVisibility {
    TEXTBADGEVIS_HIDE,
    TEXTBADGEVIS_TRUNCATED,
    TEXTBADGEVIS_FULL,
    TEXTBADGEVIS_SELECTED_TRUNCATED,
    TEXTBADGEVIS_SELECTED_FULL,
    TEXTBADGEVIS_SELECTED_FULL_ELSE_TRUNCATED,
    TEXTBADGEVIS_NUM_ENTRIES
};

enum OPUI_GraphTextBadgeThreading {
    TEXTBADGE_SINGLE_THREADED,
    TEXTBADGE_MULTI_THREADED
};

typedef bool (*OPUI_GraphTextBadgeTest)(const OPUI_GraphProxyDescriptor &desc,
				    OP_Context &context,
				    UT_StringHolder &text,
				    UT_Color &clr);
typedef UT_Array<OPUI_GraphTextBadge> OPUI_GraphTextBadgeArray;

class OPUI_API OPUI_GraphTextBadge
{
public:
				 OPUI_GraphTextBadge(
					const UT_StringHolder &pref,
					const UT_StringHolder &category,
					const UT_StringHolder &description,
					const UT_StringHolder &icon,
					fpreal full_text_max_width,
					OPUI_GraphTextBadgeVisibility vis,
					OPUI_GraphTextBadgeTest test,
					OPUI_GraphTextBadgeThreading threading);
				~OPUI_GraphTextBadge();

    const UT_StringHolder	&getPreferenceName() const
				 {  return myPreferenceName; }
    const UT_StringHolder	&getPreferenceCategory() const
				 {  return myPreferenceCategory; }
    const UT_StringHolder	&getDescription() const
				 {  return myDescription; }
    const UT_StringHolder	&getIcon() const
				 {  return myIcon; }
    fpreal			 getFullTextMaxWidth() const
				 { return myFullTextMaxWidth; }
    OPUI_GraphTextBadgeVisibility getDefaultVisibility() const
				 { return myDefaultVisibility; }
    OPUI_GraphTextBadgeTest	 getTextBadgeTest() const
				 { return myTextBadgeTest; }
    bool			 getThreadSafe() const
				 { return myThreadSafe; }

    OPUI_GraphTextBadgeVisibility getVisibility() const
				 { return myVisibility; }
    void			 setVisibility(OPUI_GraphTextBadgeVisibility v)
				 { myVisibility = v; }

    static void			 addText(OPUI_GraphTextBadgeArray &add_text);

    static const UT_StringHolder theCommentPref;
    static const UT_StringHolder theDescriptiveParmPref;
    static const UT_StringHolder theContextOptionDepsPref;
    static const UT_StringHolder theLopLastModifiedPrimPref;

    static const UT_StringHolder theMainTextBadgeCategory;

private:
    UT_StringHolder			 myPreferenceName;
    UT_StringHolder			 myPreferenceCategory;
    UT_StringHolder			 myDescription;
    UT_StringHolder			 myIcon;
    fpreal				 myFullTextMaxWidth;
    OPUI_GraphTextBadgeVisibility	 myDefaultVisibility;
    OPUI_GraphTextBadgeVisibility	 myVisibility;
    OPUI_GraphTextBadgeTest		 myTextBadgeTest;
    bool				 myThreadSafe;
};

#endif

