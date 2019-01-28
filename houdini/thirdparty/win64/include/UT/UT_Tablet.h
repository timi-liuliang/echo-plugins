/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Tablet.h ( UT Library, C++)
 *
 * COMMENTS:	This has the system independent interface to tablets such 
 *		as the Wacom Tablet.
 */

#ifndef __UT_Tablet__
#define __UT_Tablet__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

enum UT_TABLET_CURSOR
{
    UT_TABLET_NONE = -1,
    UT_TABLET_MOUSE = 0,
    UT_TABLET_STYLUS,
    UT_TABLET_ERASER
};

class UT_API UT_TabletState
{
public:
    float		myPressure;	// 0..1
    float		myAngle;	// In the X/Y plane, in degrees.
    float		myTilt;		// Off horizontal (horizontal = 0)
					// This can be negative (ie: eraser)
    float		myRoll;		// Around own axis, in degrees.
    UT_TABLET_CURSOR	myCursor;	// What cursor type generated this
};

// Is a tablet installed?
UT_API bool UThasTablet();
// Get the most recent tablet status.
UT_API void UTgetTabletState(UT_TabletState &stat);

#endif

