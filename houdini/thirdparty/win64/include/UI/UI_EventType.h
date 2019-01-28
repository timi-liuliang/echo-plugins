/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UI_EventType.h
 *
 * COMMENTS:
 *	
 *  The various event types generated by objects and event generators.
 *
 */
#ifndef __UI_EventType__
#define __UI_EventType__

#define UI_REDRAW_FENCE_EVENT 0

// All events, except specifically noted, have keyboard modifier and cursor 
// position -- relative to window -- in altFlags, and values[X/Y]. 
enum UI_EventType
{
    UI_EVENT_NO_EVENT	    = 0,
    
    // data = New window, context = old window 
    UI_EVENT_FOCUS_OFF	    = 1,
    
    // data = Old window, context = New window
    UI_EVENT_FOCUS_ON	    = 2,
    
    //  values[W] = buttons, values[TUVZ] = tablet data 
    UI_EVENT_BUTTON	    = 3,
    
    // data = UI_Keyboard key  
    UI_EVENT_KEYBOARD	    = 4,
    UI_EVENT_KEYUPDOWN	    = 5,
    UI_EVENT_FUNCTION_KEY   = 6,
    UI_EVENT_ARROW_KEY	    = 7,
    UI_EVENT_MOTION	    = 8,
    UI_EVENT_REDRAW	    = 9,
    UI_EVENT_MINIMIZE_RESTORE = 10,
    UI_EVENT_OPEN	    = 11,
    UI_EVENT_CLOSE	    = 12,
    UI_EVENT_DELETE	    = 13,
    UI_EVENT_QUIT	    = 14,
    UI_EVENT_VALUE_CHANGE   = 15,
    UI_EVENT_TIMER	    = 16,
    UI_EVENT_PAUSE	    = 17,
    UI_EVENT_ENABLE	    = 18,
    UI_EVENT_APP_SPECIFIC   = 19,
    UI_EVENT_RESIZE         = 24,
    UI_EVENT_KEYOTHER	    = 25,
    UI_EVENT_OVERRIDE	    = 26,
    UI_EVENT_MOUSEWHEEL     = 27,
    UI_EVENT_DBLCLK	    = 28,
    UI_EVENT_REFRESH	    = 29,
    UI_EVENT_MENU	    = 30,
    UI_EVENT_OVERLAY_REDRAW = 32,
    UI_EVENT_OVERLAY_CLEAR  = 33,
    UI_EVENT_VISIBLE	    = 34,
    
    // Only fired when drag-n-drop a file from outside Houdini
    // context = active window, key = file path
    UI_EVENT_DROP           = 35

};


static inline bool
UIisExclusiveEvent(UI_EventType e)
{
    switch(e)
    {
    case UI_EVENT_BUTTON:
    case UI_EVENT_KEYBOARD:
    case UI_EVENT_KEYUPDOWN:
    case UI_EVENT_FUNCTION_KEY:
    case UI_EVENT_ARROW_KEY:
    case UI_EVENT_MOTION:
    case UI_EVENT_QUIT:
    case UI_EVENT_KEYOTHER:
    case UI_EVENT_MOUSEWHEEL:
    case UI_EVENT_DBLCLK:
    case UI_EVENT_MENU:
	return true;
    default:
	return false;
    }
}

static inline bool
UIisRedrawFenceEvent(UI_EventType e)
{
    switch(e)
    {
    case UI_EVENT_BUTTON:
    case UI_EVENT_KEYBOARD:
    case UI_EVENT_KEYUPDOWN:
    case UI_EVENT_FUNCTION_KEY:
    case UI_EVENT_ARROW_KEY:
    case UI_EVENT_MOTION:
    case UI_EVENT_RESIZE:
    case UI_EVENT_KEYOTHER:
    case UI_EVENT_MOUSEWHEEL:
    case UI_EVENT_DBLCLK:
    case UI_EVENT_REFRESH:
    case UI_EVENT_MENU:
    case UI_EVENT_OVERLAY_REDRAW:
    case UI_EVENT_OVERLAY_CLEAR:
	return true;
    default:
	return false;
    }
}

#endif