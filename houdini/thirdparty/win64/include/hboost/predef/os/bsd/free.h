/*
Copyright Rene Rivera 2012-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_BSD_FREE_H
#define HBOOST_PREDEF_OS_BSD_FREE_H

#include <hboost/predef/os/bsd.h>

/*`
[heading `HBOOST_OS_BSD_FREE`]

[@http://en.wikipedia.org/wiki/Freebsd FreeBSD] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__FreeBSD__`] [__predef_detection__]]

    [[`__FreeBSD_version`] [V.R.P]]
    ]
 */

#define HBOOST_OS_BSD_FREE HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__FreeBSD__) \
    )
#   ifndef HBOOST_OS_BSD_AVAILABLE
#       define HBOOST_OS_BSD HBOOST_VERSION_NUMBER_AVAILABLE
#       define HBOOST_OS_BSD_AVAILABLE
#   endif
#   undef HBOOST_OS_BSD_FREE
#   if defined(__FreeBSD_version)
#       if __FreeBSD_version < 500000
#           define HBOOST_OS_BSD_FREE \
                HBOOST_PREDEF_MAKE_10_VRP000(__FreeBSD_version)
#       else
#           define HBOOST_OS_BSD_FREE \
                HBOOST_PREDEF_MAKE_10_VRR000(__FreeBSD_version)
#       endif
#   else
#       define HBOOST_OS_BSD_FREE HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_OS_BSD_FREE
#   define HBOOST_OS_BSD_FREE_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_BSD_FREE_NAME "Free BSD"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_BSD_FREE,HBOOST_OS_BSD_FREE_NAME)
