/*
Copyright Rene Rivera 2012-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_BSD_BSDI_H
#define HBOOST_PREDEF_OS_BSD_BSDI_H

#include <hboost/predef/os/bsd.h>

/*`
[heading `HBOOST_OS_BSD_BSDI`]

[@http://en.wikipedia.org/wiki/BSD/OS BSDi BSD/OS] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__bsdi__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_BSD_BSDI HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__bsdi__) \
    )
#   ifndef HBOOST_OS_BSD_AVAILABLE
#       define HBOOST_OS_BSD HBOOST_VERSION_NUMBER_AVAILABLE
#       define HBOOST_OS_BSD_AVAILABLE
#   endif
#   undef HBOOST_OS_BSD_BSDI
#   define HBOOST_OS_BSD_BSDI HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_BSD_BSDI
#   define HBOOST_OS_BSD_BSDI_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_BSD_BSDI_NAME "BSDi BSD/OS"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_BSD_BSDI,HBOOST_OS_BSD_BSDI_NAME)
