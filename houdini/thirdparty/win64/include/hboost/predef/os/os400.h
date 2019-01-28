/*
Copyright Rene Rivera 2011-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_OS400_H
#define HBOOST_PREDEF_OS_OS400_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_OS400`]

[@http://en.wikipedia.org/wiki/IBM_i IBM OS/400] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__OS400__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_OS400 HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__OS400__) \
    )
#   undef HBOOST_OS_OS400
#   define HBOOST_OS_OS400 HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_OS400
#   define HBOOST_OS_OS400_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_OS400_NAME "IBM OS/400"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_OS400,HBOOST_OS_OS400_NAME)
