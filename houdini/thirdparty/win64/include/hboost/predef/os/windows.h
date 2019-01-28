/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_WINDOWS_H
#define HBOOST_PREDEF_OS_WINDOWS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_WINDOWS`]

[@http://en.wikipedia.org/wiki/Category:Microsoft_Windows Microsoft Windows] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`_WIN32`] [__predef_detection__]]
    [[`_WIN64`] [__predef_detection__]]
    [[`__WIN32__`] [__predef_detection__]]
    [[`__TOS_WIN__`] [__predef_detection__]]
    [[`__WINDOWS__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_WINDOWS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(_WIN32) || defined(_WIN64) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || \
    defined(__WINDOWS__) \
    )
#   undef HBOOST_OS_WINDOWS
#   define HBOOST_OS_WINDOWS HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_WINDOWS
#   define HBOOST_OS_WINDOWS_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_WINDOWS_NAME "Microsoft Windows"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_WINDOWS,HBOOST_OS_WINDOWS_NAME)
