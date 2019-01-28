/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_QTTablet.h (UT Library, C++)
 *
 * COMMENTS:
 *	Tablet support functions for Qt
 */
#ifndef __UT_QTTablet__
#define __UT_QTTablet__

#include "UT_API.h"

class UT_TabletState;

// Toggles whether there is a tablet installed.
UT_API void QTtabletSetInstalled(bool installed);
// Overwrites the current state with the given one.
UT_API void QTtabletSetTabletState(const UT_TabletState &state);

#endif
