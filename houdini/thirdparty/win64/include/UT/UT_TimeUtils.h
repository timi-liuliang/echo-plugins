/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TimeUtils.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *  This is a collection of utilities useful for dealing with times and time
 *  ranges.
 *	
 */
#ifndef __UT_TimeUtils_h__
#define __UT_TimeUtils_h__

#include "UT_API.h"
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include "UT_Assert.h"

// Given a start frame, an end frame and a step, this method will return 
// the number of frames involved.  Be careful not to pass a zero step size.
// Optionally, this method can also compute the ending frame of the sequence.
template <typename T>
int UTgetNumberOfFrames(T start_frame, T end_frame, 
		        T step, T *new_end_frame=0)
{
    int		nframes = 0;
    T	        t, t1;
    T	        inc;

    UT_ASSERT(!SYSequalZero(step));	// step of 0 is invalid
    if (SYSequalZero(step))
       return 0;    // could also have returned INT_MAX

    if (new_end_frame)
       *new_end_frame = end_frame;

    // Check to see that we're going the right way (careful if s == e)
    if ((end_frame - start_frame) / step < 0.5)
       return 1;    // nframes + 1
    else
    {
       if (end_frame > start_frame)
       {
	   inc	= step;
	   t1	= end_frame + inc * 0.01;
	   t	= start_frame + inc;
       }
       else
       {
	   inc	= -step;
	   t1	= start_frame + inc * 0.01;
	   t	= end_frame + inc;
       }
       for (; t1 - t > 0; t += inc)
	   nframes++;

       // Remember to compute new_end_frame with the number of frames - 1.
       if (new_end_frame)
	   *new_end_frame = start_frame + step * (fpreal64)nframes;

       // Up to here we've computed nframes - 1;
    }
    return nframes + 1;
}


#endif
