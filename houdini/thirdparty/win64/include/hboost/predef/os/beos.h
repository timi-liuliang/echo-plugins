/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_BEOS_H
#define HBOOST_PREDEF_OS_BEOS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_BEOS`]

[@http://en.wikipedia.org/wiki/BeOS BeOS] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__BEOS__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_BEOS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__BEOS__) \
    )
#   undef HBOOST_OS_BEOS
#   define HBOOST_OS_BEOS HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_BEOS
#   define HBOOST_OS_BEOS_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_BEOS_NAME "BeOS"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_BEOS,HBOOST_OS_BEOS_NAME)
