/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_SourceLocation.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_SourceLocation__
#define __STY_SourceLocation__

#include "STY_API.h"
#include <UT/UT_StringHolder.h>

/// Fully describes a stye sheet source location.
class STY_SourceLocation
{
public:
    STY_SourceLocation()
	: myLine(-1), myOffset(-1)
    { }

    UT_StringHolder	 myFile;
    int			 myLine;
    int			 myOffset;
};

#endif

