/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SysClone.h ( UT Library, C++)
 *
 * COMMENTS:	Functions and defines for WinNT or Linux to make it look
 *		like IRIX. The difference between this file and UT_SysSpecific
 *		is that this one simply replaces SGI system calls with
 *		custom calls of the same name, while SysSpecific is a true
 *		wrapper that hides even the system call name under a "UT"-
 *		prefixed name.	
 *	
 */

#ifndef _UT_SysClone_h_
#define _UT_SysClone_h_

#include "UT_API.h"
#include <SYS/SYS_Math.h>

#if defined(GAMEOS)
    #include "UT_GameOsUtil.h"
#endif

#ifdef WIN32
    #include "UT_NTUtil.h"
    #include "UT_NTGetOpt.h"
#elif defined(MBSD)
    #include "UT_MBSDUtil.h"
#else
    #include <unistd.h>

    static inline void UTclearHostnameCache()
    {
	// Nothing to do here since we don't cache the hostname
    }

    static inline int
    UTgethostname(char *host, int len)
    {
	return gethostname(host, len);
    }
#endif

class UT_String;

UT_API extern int UTgethostname(UT_String &host);

#if defined(MBSD)
    #include "UT_MBSDUtil.h"
#endif

#if defined(LINUX) 
    #include "UT_LinuxUtil.h"
#endif

#include "UT_Access.h"
#include "UT_DirUtil.h"
#include "UT_NTStreamUtil.h"

#endif // _UT_SysClone_h_
