/*
Copyright Franz Detro 2014
Copyright Rene Rivera 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_IOS_H
#define HBOOST_PREDEF_OS_IOS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_IOS`]

[@http://en.wikipedia.org/wiki/iOS iOS] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__APPLE__`] [__predef_detection__]]
    [[`__MACH__`] [__predef_detection__]]
    [[`__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__`] [__predef_detection__]]

    [[`__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__`] [__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__*1000]]
    ]
 */

#define HBOOST_OS_IOS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__APPLE__) && defined(__MACH__) && \
    defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) \
    )
#   undef HBOOST_OS_IOS
#   define HBOOST_OS_IOS (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__*1000)
#endif

#if HBOOST_OS_IOS
#   define HBOOST_OS_IOS_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_IOS_NAME "iOS"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_IOS,HBOOST_OS_IOS_NAME)
