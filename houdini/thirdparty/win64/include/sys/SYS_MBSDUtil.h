/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_MBSDUtil.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *		Mac OSX utility functions at the SYS level.
 *	
 */

#ifndef __SYS_MBSDUtil_h__
#define __SYS_MBSDUtil_h__

#include "SYS_API.h"

#if defined(MBSD)
SYS_API const char *SYSmbsdGetOSVersionString();

SYS_API void SYSmbsdGetOSVersion(int &major, int &minor, int &patch);

#endif // MBSD

#endif // __SYS_MBSDUtil_h__
