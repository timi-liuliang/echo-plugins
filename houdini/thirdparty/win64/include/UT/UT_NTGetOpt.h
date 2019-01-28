#ifndef __UT_NTGETOPT_H__
#define __UT_NTGETOPT_H__

#include "UT_API.h"
#ifdef WIN32
    UT_API extern const char *optarg;
    UT_API extern int optind, opterr, optopt;
    UT_API void getoptreset( void );
    UT_API int getopt( int argc, char *const *argv, const char *optstring );
#endif

#endif
