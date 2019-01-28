/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MBSDUtil.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	These functions replace the SGI forms with BSD forms.
 *	
 */

#ifndef __UT_MBSDUtil_h__
#define __UT_MBSDUtil_h__

#include "UT_API.h"

// Everything after this point will only be included if on MacOS.

#if defined(MBSD)

#include <sched.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <SYS/SYS_Types.h>

#include "UT_Assert.h"
#include "UT_StringMap.h"
#include "UT_UniquePtr.h"
#include "UT_WorkBuffer.h"

//
// Defines:
//
#define PR_SADDR	1 
#define MP_NPROCS	0 
#ifndef SO_REUSEPORT
#define SO_REUSEPORT	SO_REUSEADDR
#endif
#ifndef SIGCLD
#define SIGCLD 		SIGCHLD
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

// Forward declares
class UT_WorkArgs;
class UT_ExtensionList;

//
// Functions:
//
UT_API long UTnap(long ms);

UT_API void cftime (char *dest, char *fmt, time_t *now);

UT_API pid_t sproc (void (*)(void *), unsigned int flags = 0, void *args = 0);

UT_API int sysmp (int val, ...);

UT_API int usconfig (int flag, int data);

UT_API extern void UTclearHostnameCache();
UT_API extern int  UTgethostname(char *host, int len);

UT_API void MBSDregisterThread();

UT_API int MBSDLaunchApp(UT_WorkArgs& args);

UT_API void MBSDLoadURL(const char* url);

UT_API std::string MBSDurlToLocalPathStdString(const std::string &url);

UT_API void MBSDAddImageAndMovieExtensions(UT_ExtensionList & extension_list);

UT_API UT_AssertResponse MBSDgetAssertResponse(UT_WorkBuffer &info);

UT_API void MBSDalert(const char *msg, const char *details);

UT_API float MBSDgetBackingScaleFactor(int screen_idx);

/// This class works around a bug in OSX Yosemite where duplicate environment
/// variable entries can exist in the global `environ`.  Duplicate entries cause
/// spawned bash/sh subprocesses to inherit the wrong variable values if the
/// parent process was launched from launchd.  This bug is logged with Apple as
/// bug 18724538 and submitted by rvinluan@sidefx.com.  When that bug is fixed,
/// then we can get rid of UT_MBSDEnvironFix.
///
/// This class works around the bug by recreating `environ` without duplicate
/// entries.
///
/// To use this class, create an instance of it before spawning a process.
/// For example:
/// {
///     #if defined(MBSD)
///     UT_MBSDEnvironFix tmp_env;
///     #endif
///     ::execlp("/bin/bsh", "sh", "-c", command, NULL);
/// }
class UT_API UT_MBSDEnvironFix
{
public:
    UT_MBSDEnvironFix();
    ~UT_MBSDEnvironFix();
private:
    char **myOrigEnviron;
    UT_UniquePtr<char*[]> myFixedEnviron;
    UT_StringMap<UT_String> myEnvironMap;
};

#ifdef __OBJC__

#import <Foundation/Foundation.h>

class UT_String;

NSString * MBSDconvertString(const UT_String & ut_string);
UT_String MBSDconvertString(NSString * string);

NSString * UTstackTraceForException(NSException * exception);

#endif

#endif // MBSD

#endif // __UT_MBSDUtil_h__
