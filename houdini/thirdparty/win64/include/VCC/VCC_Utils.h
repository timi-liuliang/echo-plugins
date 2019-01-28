/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VCC_Utils.h ( CVEX Library, C++)
 *
 * COMMENTS:	This file contains utility functions for compiling VEX code.
 */

#ifndef __VCC_Utils__
#define __VCC_Utils__

#include "VCC_API.h"
#include <CVEX/CVEX_Function.h>

class CVEX_Context;
class UT_WorkBuffer;

class VCC_API VCC_Utils
{
public:
    static CVEX_Function	 preloadLocalFunction(CVEX_Context &context,
					const UT_WorkBuffer &source_code,
					int node_id);
};

#endif

