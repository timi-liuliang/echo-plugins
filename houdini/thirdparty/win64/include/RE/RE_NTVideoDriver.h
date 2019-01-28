/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_NTVIDEODriver.h (RE Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RE_NTVIDEODRIVER_H_INCLUDED__
#define __RE_NTVIDEODRIVER_H_INCLUDED__

#ifdef _WIN32

#include "RE_API.h"
#include "RE_Types.h"
#include <UT/UT_WorkBuffer.h>

RE_API extern bool REvideoDriverDetermineVersion(
			RE_GraphicsDevice gl_device,
			int &major, int &minor,
			int &build_major, int &build_minor);

RE_API extern bool REvideoDriverDetermineOGLSoftwareDefault(
			int &ogl_software);

#endif

#endif // __RE_NTVIDEODRIVER_H_INCLUDED__
