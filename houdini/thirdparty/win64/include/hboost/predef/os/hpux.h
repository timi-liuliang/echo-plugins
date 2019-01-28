/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_HPUX_H
#define HBOOST_PREDEF_OS_HPUX_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_HPUX`]

[@http://en.wikipedia.org/wiki/HP-UX HP-UX] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`hpux`] [__predef_detection__]]
    [[`_hpux`] [__predef_detection__]]
    [[`__hpux`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_HPUX HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(hpux) || defined(_hpux) || defined(__hpux) \
    )
#   undef HBOOST_OS_HPUX
#   define HBOOST_OS_HPUX HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_HPUX
#   define HBOOST_OS_HPUX_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_HPUX_NAME "HP-UX"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_HPUX,HBOOST_OS_HPUX_NAME)
