/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TimerTable.h ( UT Library, C++)
 *
 * COMMENTS:	Class to keep track of cumulative time spent in different
 *		algorithms.
 */

#ifndef __UT_TimerTable__
#define __UT_TimerTable__

#include "UT_API.h"

class UT_WorkBuffer;

// Add new algorithms here.  There must be a matching entry in theUsers in
// the UT_TimerTable.C
enum UT_TimerTableUser {
    UT_TIMER_DEBUG = 0,

    UT_TIMER_RAY_SAMPLING,
    UT_TIMER_RAY_RAYTRACING,
    UT_TIMER_VMAT_OCCLUSION,
    UT_TIMER_RAY_DUMPSAMPLES,
    UT_TIMER_RAY_SPUSAMPLING,

    UT_MAX_TIMER_USERS	// Sentinal
};

class UT_API UT_TimerTable {
public:
    // Start a timer.  Nested timers are ignored.
    static void		start(UT_TimerTableUser user);
    static void		stop(UT_TimerTableUser user);

    static void		printUsage(UT_WorkBuffer &buf, int indent=3);
};

// Automatically starts and stops a timer within the scope of a
// UT_AutoTimerTable object.
class UT_API UT_AutoTimerTable {
public:
    UT_AutoTimerTable(UT_TimerTableUser user)
	: myUser(user)
    {
	UT_TimerTable::start(myUser);
    }
    ~UT_AutoTimerTable()
    {
	UT_TimerTable::stop(myUser);
    }

private:
    UT_TimerTableUser	 myUser;
};

#endif
