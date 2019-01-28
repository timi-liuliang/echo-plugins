/*
Copyright Rene Rivera 2012-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_BSD_DRAGONFLY_H
#define HBOOST_PREDEF_OS_BSD_DRAGONFLY_H

#include <hboost/predef/os/bsd.h>

/*`
[heading `HBOOST_OS_BSD_DRAGONFLY`]

[@http://en.wikipedia.org/wiki/DragonFly_BSD DragonFly BSD] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__DragonFly__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_BSD_DRAGONFLY HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__DragonFly__) \
    )
#   ifndef HBOOST_OS_BSD_AVAILABLE
#       define HBOOST_OS_BSD HBOOST_VERSION_NUMBER_AVAILABLE
#       define HBOOST_OS_BSD_AVAILABLE
#   endif
#   undef HBOOST_OS_BSD_DRAGONFLY
#   if defined(__DragonFly__)
#       define HBOOST_OS_DRAGONFLY_BSD HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_OS_BSD_DRAGONFLY
#   define HBOOST_OS_BSD_DRAGONFLY_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_BSD_DRAGONFLY_NAME "DragonFly BSD"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_BSD_DRAGONFLY,HBOOST_OS_BSD_DRAGONFLY_NAME)
