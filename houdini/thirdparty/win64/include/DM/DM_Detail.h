/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Direct manipulation library (C++)
 *
 * COMMENTS:
 *	Class with a very simple interface for drawing a GU_Detail.
 *
 */

#ifndef __DM_Detail_H__
#define __DM_Detail_H__

#include "DM_API.h"
#include <GR/GR_Defines.h>
#include <GU/GU_Detail.h>
#include <GU/GU_DetailHandle.h>
#include <UT/UT_UniquePtr.h>

class UT_Color;
class RE_Render;
class DM_SceneManager;
class GUI_DetailLook;

class DM_API DM_Detail
{
public:
    DM_Detail(DM_SceneManager &scene_manager, const char *name);
    ~DM_Detail();

    GU_Detail	&getDetail();
    void 	 renderWire(RE_Render &r,
			int pickflag, uint id1, uint id2,
			const UT_Color &color,
			GU_Detail *gdp = NULL,
			const UT_DMatrix4 *xform = NULL);

private:
    DM_SceneManager		&mySceneManager;
    GU_Detail			 myDetail;
    GU_DetailHandle		 myDetailHandle;
    GUI_DetailLook		*myDetailLook;
};

#endif

