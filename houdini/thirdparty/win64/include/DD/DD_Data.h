/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DD_Data.h 
 *
 * COMMENTS:
 * 		This class is a simple wrapper
 * 		to hide the data matching routine
 */

#ifndef __DD_Data__
#define __DD_Data__

#include "DD_API.h"

class DD_API DD_Data
{
public:

    // built from common defines 

    static bool		isText(const char *label);
    static bool		isChannelPath(const char *label);
    static bool		isParmPath(const char *label);
    static bool		isOpPath(const char *label);
    static bool		isPrimPath(const char *label);
    static bool		isItemPath(const char *label);
    static bool		isTrackPath(const char *label);
    static bool		isPersistentHandlePath(const char *label);
    static bool		isChannelGroupList(const char *label);
    static bool		isSITreeNodes(const char *label);
    static bool		isFlagPath(const char *label);
    static bool		isTakeName(const char *label);
    static bool		isGalleryEntry(const char *label);
    static bool		isGalleryEntryName(const char *label);
    static bool		isShelfTool(const char *label);
    static bool		isShelfToolName(const char *label);
    static bool		isShelfToolButton(const char *label);
    static bool		isShelfToolbar(const char *label);
    static bool		isShelfToolbarName(const char *label);
    static bool		isDesktopNode(const char *label);
    static bool		isDesktopNodeName(const char *label);
    static bool		isDopDataPython(const char *label);
    static bool		isDopDataHscriptFloat(const char *label);
    static bool		isDopDataHscriptString(const char *label);
    static bool		isAssetGuidType(const char *label);
    static bool		isAssetToolNameType(const char *label);
    static bool		isFolderGuidType(const char *label);
    static bool		isVisualizer(const char *label);
    static bool		isUsdPrimPath(const char *label);

    // general match routine
    static bool		isMatch(const char *label, const char *test);
};

#endif

