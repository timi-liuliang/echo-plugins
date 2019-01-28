/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_ViewportHookCollector.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_ViewportHookCollector__
#define __GT_ViewportHookCollector__

#include "GT_API.h"
#include <GA/GA_PrimitiveTypeId.h>

class GT_API GT_ViewportHookCollector
{
public:
    static void	hookPrimitive(const GA_PrimitiveTypeId &prim_type,
				bool pass_through);
    static void	unhookPrimitive(const GA_PrimitiveTypeId &prim_type);
    static void	enable();	// Enable hooks
    static void	disable();	// Disable hooks
};

/// Auto-enable/disable viewport hooks.
class GT_API GT_EnableViewportHook
{
public:
    GT_EnableViewportHook()
    {
	GT_ViewportHookCollector::enable();
    }
    ~GT_EnableViewportHook()
    {
	GT_ViewportHookCollector::disable();
    }
};

#endif

