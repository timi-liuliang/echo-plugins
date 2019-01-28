/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SysResource.h ( UT Library, C++)
 *
 * COMMENTS:	SysResource class to retrieve system specific information
 */

#ifndef __UT_SysResource__
#define __UT_SysResource__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

class UT_API UT_SysResource {
public:
     UT_SysResource();
    ~UT_SysResource();

    // Do not update the CPU idle if you're calling this from a static
    // construction code path or else it will hang houdini on Windows!
    void	update( bool skip_update_idle = false );

    // Return information about memory on the system.  All results are Kb.  
    int64	getPhysMemory() const	{ return myPhysMemory; }
    int64	getFreeMemory() const	{ return myFreeMemory; }

    // If not supported, returns 0
    int64	getTotalSwap() const	{ return myTotalSwap; }
    int64	getFreeSwap() const	{ return myFreeSwap; }

    // This is a number between 0 (fully loaded) and 1 (fully idle).
    // If not supported, returns -1
    fpreal	getCPUIdle() const	{ return myCPUIdle; }

    const char	*getSystemName() const	{ return mySystemName; }

private:
    int64	 myPhysMemory;
    int64	 myFreeMemory;
    int64	 myTotalSwap;
    int64	 myFreeSwap;
    const char	*mySystemName;

    float	 myCPUIdle;
};

#endif
