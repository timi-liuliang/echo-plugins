/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OPUI_GraphDisplayOptions.h (User Interface Library, C++)
 *
 * COMMENTS:
 *		A class to control the way items are displayed in the graph.
 *
 */

#ifndef __OPUI_GraphDisplayOptions__
#define __OPUI_GraphDisplayOptions__

#include "OPUI_API.h"
#include "OPUI_GraphBadge.h"
#include "OPUI_GraphTextBadge.h"
#include <OP/OP_Context.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Color.h>
#include <UT/UT_Array.h>
#include <UT/UT_Map.h>

class UT_OptionFile;
class AP_Interface;
class CV_Canvas;
class OPUI_GraphDisplayOptions;

typedef UT_Map<OPUI_GraphDisplayOptions *, CV_Canvas *> OPUI_OptionsCanvasMap;

extern "C" {
    SYS_VISIBILITY_EXPORT extern
    void OPUIaddBadges(OPUI_GraphBadgeArray *add_badges);
}
extern "C" {
    SYS_VISIBILITY_EXPORT extern
    void OPUIaddTextBadges(OPUI_GraphTextBadgeArray *add_textbadges);
}

class OPUI_API OPUI_GraphDisplayOptions
{
public:
				 OPUI_GraphDisplayOptions();
    virtual			~OPUI_GraphDisplayOptions();

    // Add our preference key/values to the canvas.
    void			 registerPreferences(CV_Canvas &canvas);

    // Returns true if one of our options was changed by this call.
    void			 setPref(const UT_StringHolder &pref,
					const UT_StringHolder &value);

    const UT_StringHolder	&perfStatName() const;
    bool			 showPerfStats() const;
    const UT_StringHolder	&taskGraphPerfStatName() const;
    bool			 showTaskGraphPerfStats() const;
    const UT_StringHolder	&taskGraphSortCriteria() const;
    const UT_StringHolder	&taskGraphCollapseMode() const;
    bool			 showAllDependencies() const;
    bool			 showSelectedDependencies() const;
    bool			 showChildDependencies() const;
    const UT_Array<UT_Color>	&paletteColors() const;

    bool			 showNodeTypes() const;
    bool			 showNodeShapes() const;
    bool			 showCopPreviews() const;
    bool			 showVopPreviews() const;
    bool			 showTaskGraph() const;
    bool			 showCookedWorkItems() const;
    bool			 showCookingWorkItems() const;
    bool			 showDirtyWorkItems() const;
    bool			 showCancelledWorkItems() const;
    bool			 showFailedWorkItems() const;
    bool			 showRootWorkItems() const;
    bool			 showPartitionWorkItems() const;
    bool			 soloHighlightedWorkItems() const;
    bool			 taskGraphLimitRows() const;
    bool			 showPromptText() const;
    bool			 showSimpleShape() const;
    bool			 showVopInOutLabels() const;
    bool			 showStackedLook() const;

    fpreal			 maxFlyoutScale() const;
    fpreal			 maxNameWidth() const;
    int				 minNameHeight() const;
    int				 maxNameHeight() const;
    fpreal			 wireFadeFactor() const;

    int				 maxTaskGraphRows() const;
    int				 maxWorkItemsPerRow() const;

    int				 badgeCount() const
				 { return theRegisteredBadges.entries(); }
    const UT_IntArray		&badgeNormalIndexes() const
				 { return myBadgeNormalIndexes; }
    const UT_IntArray		&badgeLargeIndexes() const
				 { return myBadgeLargeIndexes; }
    const OPUI_GraphBadge	&badge(int badgeidx) const;
    bool			 badgeVisible(int badgeidx,
					OPUI_GraphBadgeVisibility vis,
					const OPUI_GraphProxyDescriptor &desc,
					UT_StringHolder &icon,
					UT_Color &color) const;
    int				 textBadgeCount() const
				 { return theRegisteredTextBadges.entries(); }
    const UT_IntArray		&textBadgeIndexes() const
				 { return myTextBadgeIndexes; }
    const OPUI_GraphTextBadge	&textBadge(int textbadgeidx) const;
    bool			 textBadgeVisible(int textbadgeidx,
					const OPUI_GraphProxyDescriptor &desc,
					UT_StringHolder &text,
					bool &full_text,
					fpreal &full_text_max_width,
					UT_Color &color) const;
    bool			 prepareSingleThreadedBadges(int num_nodes);
    void			 calculateSingleThreadedBadges(
					OPUI_GraphProxyDescriptor &desc);

    static const UT_StringArray	 topInOutColorTags();
    static const UT_StringArray	 topInOutColors();
    static const UT_StringHolder topInOutColor(UT_StringHolder tag);

    static void			 installBadges();
    static void			 installTextBadges();

    static void			 installTopInOutColors();

    static void			 saveGlobalPrefs(AP_Interface &app);
    static void			 loadGlobalPrefs(AP_Interface &app);
    static void			 importGlobalPrefs(AP_Interface &app);
    static void			 exportGlobalPrefs(AP_Interface &app);
    static void			 addGlobalPref(const UT_StringHolder &pref);
    static bool			 isGlobalPref(const UT_StringHolder &pref);

    static void			 clearAllContexts();

private:
    static void			 setGlobalPref(const UT_StringHolder &pref,
					const UT_StringHolder &value);
    static void			 getGlobalPref(const UT_StringHolder &pref,
					UT_StringHolder &value);

    static bool			 loadPaletteColors(
					const char *json_buf,
					UT_Array<UT_Color> &colors);
    static void			 loadDefaultPaletteColors(
					UT_Array<UT_Color> &colors);
    static void			 savePaletteColors(
					const UT_Array<UT_Color> &colors,
					UT_WorkBuffer &json_buf);

    static int			 findBadgeWithPref(
					const UT_StringHolder &pref);
    static int			 findTextBadgeWithPref(
					const UT_StringHolder &pref);

    UT_StringHolder			 myPerfStatName;
    bool				 myShowPerfStats;
    UT_StringHolder			 myTaskGraphPerfStatName;
    bool				 myShowTaskGraphPerfStats;
    UT_StringHolder			 myTaskGraphSortCriteria;
    UT_StringHolder			 myTaskGraphCollapseMode;
    bool				 myShowAllDependencies;
    bool				 myShowSelectedDependencies;
    bool				 myShowChildDependencies;
    bool				 myPaletteColorsAreDefault;
    UT_Array<UT_Color>			 myPaletteColors;

    UT_Array<UT_Array<bool> >		 mySingleThreadedBadgeData;
    UT_Array<UT_StringArray>		 mySingleThreadedBadgeIcons;
    UT_Array<UT_Array<UT_Color> >	 mySingleThreadedBadgeColors;
    UT_IntArray				 mySingleThreadedBadgeIndexes;
    UT_Array<UT_StringArray>		 mySingleThreadedTextBadgeData;
    UT_Array<UT_Array<UT_Color> >	 mySingleThreadedTextBadgeColors;
    UT_IntArray				 mySingleThreadedTextBadgeIndexes;
    int					 mySingleThreadedDataId;
    UT_IntArray				 myBadgeNormalIndexes;
    UT_IntArray				 myBadgeLargeIndexes;
    UT_IntArray				 myTextBadgeIndexes;
    OP_Context				 myContext;

    static fpreal			 theMaxFlyoutScale;
    static fpreal			 theMaxNameWidth;
    static fpreal			 theMinNameHeight;
    static fpreal			 theMaxNameHeight;
    static fpreal			 theWireFadeFactor;
    static fpreal			 theSnapRadius;
    static fpreal			 theConnectorSnapRadius;
    static fpreal			 theShakeSensitivity;
    static bool				 theShowNodeTypes;
    static bool				 theShowNodeShapes;
    static bool				 theShowPreviews;
    static bool				 theShowTaskGraph;
    static bool				 theShowCookedWorkItems;
    static bool				 theShowCookingWorkItems;
    static bool				 theShowDirtyWorkItems;
    static bool				 theShowCancelledWorkItems;
    static bool				 theShowFailedWorkItems;
    static bool				 theShowRootWorkItems;
    static bool				 theShowPartitionWorkItems;
    static bool				 theSoloHighlightedWorkItems;
    static bool				 theTaskGraphLimitRows;
    static int				 theMaxTaskGraphRows;
    static int				 theMaxWorkItemsPerRow;
    static UT_StringArray		 theTopInOutColorTags;
    static UT_StringArray		 theTopInOutColors;
    static bool				 theShowPromptText;
    static bool				 theShowSimpleShape;
    static bool				 theShowVopInOutLabels;
    static bool				 theShowStackedLook;
    static bool				 theShowAnimations;
    static bool				 theTransientInfo;
    static bool				 theFlagsAllowPreSelect;
    static bool				 theAllowDiveIntoHDAs;
    static bool				 theAutoScroll;
    static OPUI_OptionsCanvasMap	 theRegisteredCanvases;
    static OPUI_GraphBadgeArray		 theRegisteredBadges;
    static OPUI_GraphTextBadgeArray	 theRegisteredTextBadges;
};

#endif

