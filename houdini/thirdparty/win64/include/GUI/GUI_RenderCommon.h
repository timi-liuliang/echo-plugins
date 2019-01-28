/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_RenderCommon.h ( GUI Library, C++)
 *
 * COMMENTS:
 */
#ifndef GUI_RenderCommon_h
#define GUI_RenderCommon_h

#include <GR/GR_Defines.h>
#include <UT/UT_Array.h>

class GUI_DetailLook;

// setup can produce a variety of required update actions
enum GUI_SetupResult
{
    GUI_SETUP_NO_CHANGE		= 0x0,
    
    // GR_UpdateReason occupies the lower 24b.
    GUI_SETUP_GR_MASK		= 0x0FFFFFF,
};

// bitfield for specifying additional passes
enum GUI_RenderPassFlags
{
    GUI_RENDER_NONE	  = 0x0,
    GUI_RENDER_WIRE_PASS  = 0x1,
    GUI_RENDER_MATTE_PASS = 0x2
};


/// Viewport type for Rotoscope settings
enum GUI_RotoView
{
    GUI_ROTO_PERSPECTIVE,
    GUI_ROTO_CAMERA,
    GUI_ROTO_TOP,
    GUI_ROTO_FRONT,
    GUI_ROTO_RIGHT,
    GUI_ROTO_BOTTOM,
    GUI_ROTO_BACK,
    GUI_ROTO_LEFT,
    GUI_ROTO_UV
};
const int GUI_NUM_ROTO_VIEWS = (GUI_ROTO_UV+1);

class GUI_DetailLook;
typedef UT_Array<GUI_DetailLook *> GUI_DetailList;

#endif
