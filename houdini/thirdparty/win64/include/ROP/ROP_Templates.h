/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_Templates.h ( ROP Library, C++)
 *
 * COMMENTS:	Common templates defined for ROPs.
 */

#ifndef __ROP_Templates__
#define __ROP_Templates__

#include "ROP_API.h"

class PRM_Template;
class PRM_ChoiceList;
class PRM_SpareData;

enum {
    // These parameters must appear in each and every ROP.
    ROP_RENDER_TPLATE = 0,
    ROP_PREVIEW_TPLATE,			// Optional
    ROP_VIEWPORT_TPLATE,		// Optional
    ROP_RENDERBACKGROUND_TPLATE,	// Optional
    ROP_RENDERDIALOG_TPLATE,
    ROP_TRANGE_TPLATE,
    ROP_FRAMERANGE_TPLATE,
    ROP_TAKENAME_TPLATE,

    // Pre/post render/frame scripts
    // NOTE: These must be contiguous!
    ROP_TPRERENDER_TPLATE,
    ROP_SCRIPT_START_TPLATE = ROP_TPRERENDER_TPLATE,
    ROP_PRERENDER_TPLATE,
    ROP_LPRERENDER_TPLATE,
    ROP_TPREFRAME_TPLATE,
    ROP_PREFRAME_TPLATE,
    ROP_LPREFRAME_TPLATE,
    ROP_TPOSTFRAME_TPLATE,
    ROP_POSTFRAME_TPLATE,
    ROP_LPOSTFRAME_TPLATE,
    ROP_TPOSTWRITE_TPLATE,
    ROP_POSTWRITE_TPLATE,
    ROP_LPOSTWRITE_TPLATE,
    ROP_TPOSTRENDER_TPLATE,
    ROP_POSTRENDER_TPLATE,
    ROP_LPOSTRENDER_TPLATE,
    ROP_SCRIPT_END_TPLATE = ROP_LPOSTRENDER_TPLATE,
    
    // Other common templates
    ROP_INITSIM_TPLATE,
    ROP_MKPATH_TPLATE,

    ROP_MAX_TPLATE_DEFS
};

ROP_API extern PRM_Template	theRopTemplates[];
ROP_API extern PRM_Template	theRopITemplates[];

ROP_API extern PRM_ChoiceList	theRopBundleMenu;

ROP_API extern PRM_SpareData	theRopLightList;
ROP_API extern PRM_SpareData	theRopGeometryList;
ROP_API extern PRM_SpareData	theRopFogList;
ROP_API extern PRM_SpareData	theRopTakeAlways;

#endif
