/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_ErrorScope_h__
#define __HUSD_ErrorScope_h__

#include "HUSD_API.h"
#include <UT/UT_Error.h>
#include <UT/UT_UniquePtr.h>

class UT_ErrorManager;

class HUSD_API HUSD_ErrorScope
{
public:
    // Use the passed error manager if non-null, otherwise use the global one.
		 HUSD_ErrorScope(UT_ErrorManager *mgr = nullptr);
		~HUSD_ErrorScope();

private:
    class husd_ErrorScopePrivate;

    UT_UniquePtr<husd_ErrorScopePrivate>	 myPrivate;
    
};

#endif

