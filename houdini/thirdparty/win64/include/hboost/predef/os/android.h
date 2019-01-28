/*
Copyright Rene Rivera 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_ADROID_H
#define HBOOST_PREDEF_OS_ADROID_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_ANDROID`]

[@http://en.wikipedia.org/wiki/Android_%28operating_system%29 Android] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__ANDROID__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_ANDROID HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__ANDROID__) \
    )
#   undef HBOOST_OS_ANDROID
#   define HBOOST_OS_ANDROID HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_ANDROID
#   define HBOOST_OS_ANDROID_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_ANDROID_NAME "Android"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_ANDROID,HBOOST_OS_ANDROID_NAME)
