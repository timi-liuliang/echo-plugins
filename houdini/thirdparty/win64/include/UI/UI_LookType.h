/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UI_LookType.h ( UI Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UI_LookType__
#define __UI_LookType__

#include "UI_API.h"

#include "UI_Types.h"

class UI_Look;

UI_API extern UI_Look	*UIgetLook (UI_LookType type, const char *label = 0,
				    const char *iconfile = 0);
UI_API extern UI_Look	*UIgetLook (const char *name,
				    UI_BorderType t=UI_BORDER_BEVEL,
				    int locate=0, int warnflag = 1,
				    UI_IconScalability scale = UI_ICON_FIXED);

//
//  Use UIisSharedLook to find out if one of the above enumerated looks
//  is a shared resource or if UIgetLook will return a unique instance.
//
UI_API extern int		 UIisSharedLook(UI_LookType type);

#endif

