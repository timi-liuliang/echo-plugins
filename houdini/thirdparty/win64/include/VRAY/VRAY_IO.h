/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VRAY_IO.h
 *
 * COMMENTS:	Backward compatibility for previously named file
 */

#ifndef __VRAY_IO_h__
#define __VRAY_IO_h__

#include <RAY/RAY_IO.h>

#define VRAYprintf RAYprintf
#define VRAYerror RAYerror
#define VRAYwarning RAYwarning

#define VRAYprintfOnce RAYprintfOnce
#define VRAYerrorOnce RAYerrorOnce
#define VRAYwarningOnce RAYwarningOnce

#endif
