/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_IRIX_H
#define HBOOST_PREDEF_OS_IRIX_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_IRIX`]

[@http://en.wikipedia.org/wiki/Irix IRIX] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`sgi`] [__predef_detection__]]
    [[`__sgi`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_IRIX HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(sgi) || defined(__sgi) \
    )
#   undef HBOOST_OS_IRIX
#   define HBOOST_OS_IRIX HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_IRIX
#   define HBOOST_OS_IRIX_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_IRIX_NAME "IRIX"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_IRIX,HBOOST_OS_IRIX_NAME)
