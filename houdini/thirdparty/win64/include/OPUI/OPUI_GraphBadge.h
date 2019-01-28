/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OPUI_GraphBadge.h (User Interface Library, C++)
 *
 * COMMENTS:
 *		A class to describe a node badge.
 *
 */

#ifndef __OPUI_GraphBadge__
#define __OPUI_GraphBadge__

#include "OPUI_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_Color.h>
#include <UT/UT_StringHolder.h>

class OPUI_GraphProxyDescriptor;
class OPUI_GraphBadge;
class OP_Context;

enum OPUI_GraphBadgeVisibility {
    BADGEVIS_HIDE,
    BADGEVIS_NORMAL,
    BADGEVIS_LARGE,
    BADGEVIS_NUM_ENTRIES
};

enum OPUI_GraphBadgeThreading {
    BADGE_SINGLE_THREADED,
    BADGE_MULTI_THREADED
};

typedef bool (*OPUI_GraphBadgeTest)(const OPUI_GraphProxyDescriptor &desc,
				    OPUI_GraphBadgeVisibility visibility,
				    OP_Context &context,
				    UT_StringHolder &icon,
				    UT_Color &clr);
typedef UT_Array<OPUI_GraphBadge> OPUI_GraphBadgeArray;

class OPUI_API OPUI_GraphBadge
{
public:
				 OPUI_GraphBadge(const UT_StringHolder &pref,
					const UT_StringHolder &category,
					const UT_StringHolder &description,
					const UT_StringHolder &icon,
					OPUI_GraphBadgeVisibility vis,
					OPUI_GraphBadgeTest test,
					OPUI_GraphBadgeThreading threading);
				~OPUI_GraphBadge();

    const UT_StringHolder	&getPreferenceName() const
				 {  return myPreferenceName; }
    const UT_StringHolder	&getPreferenceCategory() const
				 {  return myPreferenceCategory; }
    const UT_StringHolder	&getDescription() const
				 {  return myDescription; }
    const UT_StringHolder	&getIcon() const
				 {  return myIcon; }
    OPUI_GraphBadgeVisibility	 getDefaultVisibility() const
				 { return myDefaultVisibility; }
    OPUI_GraphBadgeTest		 getBadgeTest() const
				 { return myBadgeTest; }
    bool			 getThreadSafe() const
				 { return myThreadSafe; }

    OPUI_GraphBadgeVisibility	 getVisibility() const
				 { return myVisibility; }
    void			 setVisibility(OPUI_GraphBadgeVisibility vis)
				 { myVisibility = vis; }

    static void			 addBadges(OPUI_GraphBadgeArray &add_badges);

    static const UT_StringHolder theErrorPref;
    static const UT_StringHolder theWarningPref;
    static const UT_StringHolder theCommentPref;
    static const UT_StringHolder theNodeLockedPref;
    static const UT_StringHolder theNodeUnloadPref;
    static const UT_StringHolder theNodeHasDataPref;
    static const UT_StringHolder theNeedsCookPref;
    static const UT_StringHolder theHDALockedPref;
    static const UT_StringHolder theHDAUnlockedPref;
    static const UT_StringHolder theTimeDepPref;
    static const UT_StringHolder theVexCachePref;
    static const UT_StringHolder theNotCompilablePref;
    static const UT_StringHolder theWorkItemFilterPref;
    static const UT_StringHolder theLopDebugPref;

    static const UT_StringHolder theConstraintsPref;
    static const UT_StringHolder theKinematicsPref;
    static const UT_StringHolder theHasTakeDataPref;

    static const UT_StringHolder theMainBadgeCategory;
    static const UT_StringHolder theParametersBadgeCategory;

private:
    UT_StringHolder		 myPreferenceName;
    UT_StringHolder		 myPreferenceCategory;
    UT_StringHolder		 myDescription;
    UT_StringHolder		 myIcon;
    OPUI_GraphBadgeVisibility	 myDefaultVisibility;
    OPUI_GraphBadgeVisibility	 myVisibility;
    OPUI_GraphBadgeTest		 myBadgeTest;
    bool			 myThreadSafe;
};

#endif

