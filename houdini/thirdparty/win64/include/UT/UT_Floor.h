/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Floor (C++)
 *
 */

#ifndef __UT_Floor_h__
#define __UT_Floor_h__

#include <SYS/SYS_Floor.h>

#if defined(_MSC_VER) 
    #pragma message "Deprecated in Houdini 13.0. Use SYS_Floor.h instead."
#else
    #warning "Deprecated in Houdini 13.0. Use SYS_Floor.h instead."
#endif

// Do the appropriate re-mapping
#define UTfloor		SYSfloor
#define UTceil		SYSceil
#define UTfrac		SYSfrac
#define UTsplitFloat	SYSsplitFloat
#define UTfloorIL	SYSfloorIL
#define UTceilIL	SYSceilIL
#define UTfracIL	SYSfracIL
#define UTrint		SYSrint
#define UTniceNumber	SYSniceNumber

#endif
