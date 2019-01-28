/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Time.h (SYS Library, C++)
 *
 * COMMENTS:	Platform-independent time structures
 */

#ifndef __SYS_TIME_H_INCLUDED__
#define __SYS_TIME_H_INCLUDED__

#include "SYS_API.h"
#include "SYS_Types.h"

#ifdef WIN32
struct SYS_TimeVal
{
    exint   tv_sec;         /* seconds */
    exint   tv_usec;        /* and microseconds */
};
#else

#include <sys/time.h>

typedef struct timeval SYS_TimeVal;

#endif

// SYSgettimeofday() fills in tval with the "current" time, which cannot be
// given any particular meaning. These values are only useful for comparing
// against each other.
SYS_API int SYSgettimeofday(SYS_TimeVal *tval);

// Return a number of milliseconds since the process started. This function
// will wrap values every 50 days or so.
SYS_API unsigned int SYSclock();

// Return the number of seconds since some arbitrary start time as a real
// number.  Returns -1 if there was an error computing the time.
SYS_API fpreal	SYStime();

// Return the number of seconds expressed in the given time value.
SYS_API fpreal	SYStime(const SYS_TimeVal &tval);

#endif // __SYS_TIME_H_INCLUDED__
