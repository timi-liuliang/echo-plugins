/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NTUtil.h (Utility Library, C++)
 *
 * COMMENTS:	Functions and defines for WinNT to make it look like Linux. 
 *
 */

#ifndef _UT_NTUtil_h_
#define _UT_NTUtil_h_

#include "UT_API.h"
#include "UT_Access.h"	    // UTaccess() used to be defined here
#include "UT_Defines.h"
#include <SYS/SYS_Time.h>

#ifdef WIN32

/*
    These functions replace the SGI forms with NT forms.
*/

#include <process.h>	    // for _getpid()
#include <io.h>		    // for open()/close() used by people who include us
	
typedef int mode_t;
typedef void * key_t;

struct timezone {
    long	tzd;
};

#define IPC_CREAT       0001000
#define IPC_EXCL        0002000
#define IPC_PRIVATE     (key_t)0
#define IPC_RMID        10

#define PR_SADDR         1
#define PR_SALL          2
#define PR_SFDS          3

#define MP_NPROCS 0

#define CONF_INITUSERS  1

UT_API void UTnap(int millisec);

// nap in hundredths of a second
static inline void sginap(int centiseconds)
{
    return UTnap(centiseconds * 10);
}
// nap in seconds
static inline void sleep(int seconds)
{
    return UTnap(seconds * 1000);
}


UT_API int mkdir( const char *, int );

// python/pyport.h also makes these exact same defines so guard against
// redefinition
#ifndef S_ISDIR
#define S_ISDIR(st_mode) (((st_mode) & S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(st_mode) (((st_mode) & S_IFMT) == S_IFREG)
#endif

UT_API int sysmp( int val, ... );
UT_API void *shmat( key_t h, void *addr, int flag );
UT_API int shmdt( void *mem );
UT_API key_t shmget( key_t key, int size, int flag );
UT_API int shmctl( key_t h, int cmd, int parm );
UT_API int shmctl( key_t h, int cmd );
UT_API key_t ftok( const char *path, int id );

UT_API int umask( char c );
UT_API void UTclearHostnameCache();
UT_API int  UTgethostname(char *host, int len);

// FBX SDK defines its own getpid macro, so we
// only define it here if it hasn't been defined
// in FBX already to avoid compiler warnings.
#ifndef getpid
#define getpid() _getpid()
#endif

UT_API int usconfig( int flag, int data );
UT_API char *dlerror( void );

UT_API void cftime( char *dest, char *fmt, time_t *now );

UT_API void closePerformanceKey(void *data);

#define snprintf _snprintf
#endif

#endif // _UT_NTUtil_h_
