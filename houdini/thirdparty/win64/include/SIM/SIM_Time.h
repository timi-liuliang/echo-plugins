/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Time_h__
#define __SIM_Time_h__

#include "SIM_API.h"
#include <SYS/SYS_Math.h>
#include <UT/UT_Array.h>

#define SIM_TIME_TOL (0.00001)

/// This class is used to represent simulation times. Time is stored with
/// fpreal64 precision, and all comparisons are performed with a tolerance.
class SIM_API SIM_Time
{
public:
     SIM_Time()
	 : myTime(0.0)		{ }
     SIM_Time(const fpreal64 &t)
	 : myTime(t)		{ }
     SIM_Time(const SIM_Time &t)
	 : myTime(t.myTime)	{ }
    ~SIM_Time()
	 { }

    operator		 fpreal64 () const
			 { return myTime; }
    const SIM_Time	&operator=(const fpreal64 &t)
			 { myTime = t; return *this; }
    const SIM_Time	&operator=(const SIM_Time &t)
			 { return (*this = t.myTime); }
    bool		 operator==(const fpreal64 &t) const
			 { return SYSisEqual(myTime, t, SIM_TIME_TOL); }
    bool		 operator==(const SIM_Time &t) const
			 { return (*this == t.myTime); }
    bool		 operator!=(const fpreal64 &t) const
			 { return !SYSisEqual(myTime, t, SIM_TIME_TOL); }
    bool		 operator!=(const SIM_Time &t) const
			 { return (*this != t.myTime); }
    bool		 operator>(const fpreal64 &t) const
			 { return SYSisGreater(myTime, t, SIM_TIME_TOL); }
    bool		 operator>(const SIM_Time &t) const
			 { return (*this > t.myTime); }
    bool		 operator<(const fpreal64 &t) const
			 { return SYSisLess(myTime, t, SIM_TIME_TOL); }
    bool		 operator<(const SIM_Time &t) const
			 { return (*this < t.myTime); }
    bool		 operator>=(const fpreal64 &t) const
			 { return SYSisGreaterOrEqual(myTime, t, SIM_TIME_TOL); }
    bool		 operator>=(const SIM_Time &t) const
			 { return (*this >= t.myTime); }
    bool		 operator<=(const fpreal64 &t) const
			 { return SYSisLessOrEqual(myTime, t, SIM_TIME_TOL); }
    bool		 operator<=(const SIM_Time &t) const
			 { return (*this <= t.myTime); }

private:
    fpreal64		 myTime;
};

SIM_API size_t format(char *buffer, size_t buffer_Size, const SIM_Time &v);

#endif

