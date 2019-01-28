/*
Copyright Rene Rivera 2008-2015
Copyright Franz Detro 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_MACOS_H
#define HBOOST_PREDEF_OS_MACOS_H

/* Special case: iOS will define the same predefs as MacOS, and additionally
 '__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__'. We can guard against that,
 but only if we detect iOS first. Hence we will force include iOS detection
 * before doing any MacOS detection.
 */
#include <hboost/predef/os/ios.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_MACOS`]

[@http://en.wikipedia.org/wiki/Mac_OS Mac OS] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`macintosh`] [__predef_detection__]]
    [[`Macintosh`] [__predef_detection__]]
    [[`__APPLE__`] [__predef_detection__]]
    [[`__MACH__`] [__predef_detection__]]

    [[`__APPLE__`, `__MACH__`] [10.0.0]]
    [[ /otherwise/ ] [9.0.0]]
    ]
 */

#define HBOOST_OS_MACOS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(macintosh) || defined(Macintosh) || \
    (defined(__APPLE__) && defined(__MACH__)) \
    )
#   undef HBOOST_OS_MACOS
#   if !defined(HBOOST_OS_MACOS) && defined(__APPLE__) && defined(__MACH__)
#       define HBOOST_OS_MACOS HBOOST_VERSION_NUMBER(10,0,0)
#   endif
#   if !defined(HBOOST_OS_MACOS)
#       define HBOOST_OS_MACOS HBOOST_VERSION_NUMBER(9,0,0)
#   endif
#endif

#if HBOOST_OS_MACOS
#   define HBOOST_OS_MACOS_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_MACOS_NAME "Mac OS"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_MACOS,HBOOST_OS_MACOS_NAME)
