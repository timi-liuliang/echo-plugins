/*
Copyright Jessica Hamilton 2014
Copyright Rene Rivera 2014-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_HAIKU_H
#define HBOOST_PREDEF_OS_HAIKU_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_HAIKU`]

[@http://en.wikipedia.org/wiki/Haiku_(operating_system) Haiku] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__HAIKU__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_HAIKU HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__HAIKU__) \
    )
#   undef HBOOST_OS_HAIKU
#   define HBOOST_OS_HAIKU HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_HAIKU
#   define HBOOST_OS_HAIKU_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_HAIKU_NAME "Haiku"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_HAIKU,HBOOST_OS_HAIKU_NAME)
