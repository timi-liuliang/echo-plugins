/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TS_Ray (C++)
 *
 * COMMENTS:
 *
 */

#ifndef _TS_Ray_h_
#define _TS_Ray_h_

#include "TS_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Interval.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_ThreadSpecificValue.h>

class TS_API TS_Ray
{
  public:
    inline	TS_Ray(const UT_Vector3 &xdir, const UT_Vector3 &xanchor,
		       int threadNum = -1);
    int		rayBoxHit(const UT_BoundingBox &box, UT_Interval &t) const;

    // Sets up multiple ray counts for use by seperate threads
    static void	setupThreads(int numThreads);
  public:
    UT_Vector3		 dir;
    UT_Vector3		 anchor;
    int			 index;
    static UT_ThreadSpecificValue<int>		 ourRaySerial;
};

inline TS_Ray::TS_Ray(const UT_Vector3 &pdir, const UT_Vector3 &panchor, 
		      int threadNum) 
{
  this->dir = pdir;  this->anchor = panchor; 
  
  index = ourRaySerial.get()+1;
  ourRaySerial.get() = index;
}

#endif
