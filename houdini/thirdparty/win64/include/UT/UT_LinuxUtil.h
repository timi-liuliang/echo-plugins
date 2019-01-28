/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_LinuxUtil.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	These functions replace the SGI forms with Linux forms.
 *	
 */

#ifndef _UT_LinuxUtil_h_
#define _UT_LinuxUtil_h_

#include "UT_API.h"
#ifdef LINUX

#include <sched.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <SYS/SYS_Types.h>

//
// Defines:
//
#define PR_SADDR	1 
#define MP_NPROCS	0 
#if !defined(SO_REUSEPORT)
    #define SO_REUSEPORT	SO_REUSEADDR
#endif

#define CONF_INITUSERS  1

#define FNONBLK		FNONBLOCK

#define sigset(a,b)	signal(a,b)
#define	sighold(a)
#define	sigrelse(a)
#define	sigignore(a)
#define oserror()	errno

#ifndef CLK_TCK
#define CLK_TCK CLOCKS_PER_SEC
#endif

#define sginap(a) UTnap(a*10)

//
// Functions:
//
UT_API long UTnap(long ms);

UT_API void cftime (char *dest, char *fmt, time_t *now);

UT_API pid_t sproc (void (*)(void *), unsigned int flags = 0, void *args = 0);

UT_API int sysmp (int val, ...);

UT_API int usconfig (int flag, int data);

#endif // LINUX

#endif // _UT_LinuxUtil_h_
