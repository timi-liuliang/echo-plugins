/* Dummy includy file for Linux and NT */

#ifdef WIN32
    #include <direct.h>
    #include <UT/UT_DirUtil.h>
#elif defined(LINUX) || defined(MBSD)
    #ifndef __SESI_DIRENT_H__
    #define __SESI_DIRENT_H__

    #ifndef GCC3
	/* Use the 64 bit versions of directory functions because the default
	 * versions don't work for very large file systems. */
	#define __USE_LARGEFILE64
	#define __USE_FILE_OFFSET64
    #endif

    #include <sys/types.h>
    #include "/usr/include/dirent.h"

    #ifndef GCC3
	#define dirent dirent64
	#define readdir readdir64
	#define scandir scandir64
	#define alphasort alphasort64
	#define getdents getdents64
    #endif

    #endif
#endif
