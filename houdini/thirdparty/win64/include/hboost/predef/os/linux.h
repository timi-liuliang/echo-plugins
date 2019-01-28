/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_LINUX_H
#define HBOOST_PREDEF_OS_LINUX_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_LINUX`]

[@http://en.wikipedia.org/wiki/Linux Linux] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`linux`] [__predef_detection__]]
    [[`__linux`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_LINUX HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(linux) || defined(__linux) \
    )
#   undef HBOOST_OS_LINUX
#   define HBOOST_OS_LINUX HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_LINUX
#   define HBOOST_OS_LINUX_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_LINUX_NAME "Linux"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_LINUX,HBOOST_OS_LINUX_NAME)
