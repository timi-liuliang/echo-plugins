/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CumulativeTimer.h ( UT Library, C++)
 *
 *	Measures the amount of time between start() and stop() calls,
 *      accumulating the time in a threadsafe manner.
 */
#ifndef UT_CumulativeTimer_H
#define UT_CumulativeTimer_H

#include "UT_String.h"
#include "UT_SpinLock.h"
#include "UT_StopWatch.h"
#include "UT_ThreadSpecificValue.h"


class UT_API UT_CumulativeTimer
{
public:
    UT_CumulativeTimer(const char *name, bool report_on_destroy = false);
    ~UT_CumulativeTimer();

    // Print the time when this object is destroyed if true.
    void	reportWhenDestroyed(bool report = true);

    // Start and stop the timer. Can be used safely in multiple threads
    // simultaneously.
    void	start();
    void	stop();

    // Reset accumulated time to zero.
    void	reset();

    const char *getName() const;

    // Total time between all threads.
    double	getTime() const;

    // Time when more than one thread is timing at once.
    double	getOverlapTime() const;
    
    // Wall clock time, from the time the first thread starts, til the time
    // the last ends.
    double	getTotalTime() const;
    
private:
    UT_String	myName;

    // Per-thread timing state.
    UT_ThreadSpecificValue<UT_StopWatch> myTimer;
    UT_ThreadSpecificValue<int>		 myRunning;
    UT_StopWatch			 myOverlapTimer;
    UT_StopWatch			 myTotalTimer;
    
    // A spin lock is used because the critical sections are extremely short.
    UT_SpinLock	myLock;

    // Total time for all threads
    double	myTime;
    double	myOverlapTime;
    double	myTotalTime;
    int		myTimingCount;
    bool	myReportOnDestroy;
};


#endif
