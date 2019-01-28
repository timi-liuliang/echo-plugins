/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RAY_IO.h ( RAY Library, C++)
 *
 * COMMENTS:	Map mantra error reporting methods to the ones in UT
 */

#ifndef __RAY_IO__
#define __RAY_IO__

#include <UT/UT_ErrorLog.h>

#define RAYprintf	UT_ErrorLog::mantraPrintf
#define RAYerror	UT_ErrorLog::mantraError
#define RAYwarning	UT_ErrorLog::mantraWarning

#define RAYprintfOnce	UT_ErrorLog::mantraPrintfOnce
#define RAYerrorOnce	UT_ErrorLog::mantraErrorOnce
#define RAYwarningOnce	UT_ErrorLog::mantraWarningOnce

#endif
