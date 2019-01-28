/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ChildProcessTracker.h ( UT Library, C++)
 *
 * COMMENTS:
 *     For child processes that have an indeterminant exit time (like a
 *     spawned viewer), this class will periodically do a non-blocking wait on
 *     them.
 *	
 */
#ifndef UT_CHILD_PROCESS_TRACKER_H
#define UT_CHILD_PROCESS_TRACKER_H

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_Defines.h"

class UT_API UT_ChildProcessTracker
{
public:
	     UT_ChildProcessTracker();
    virtual ~UT_ChildProcessTracker();

    virtual void addNewChild(pid_t pid);
    virtual bool pollChildren();

protected:
    int		 myNumPIDs;
    pid_t	*myPIDs;
};

UT_API extern void UTsetChildProcessTracker(UT_ChildProcessTracker *);
UT_API extern UT_ChildProcessTracker *UTgetChildProcessTracker();

#endif
