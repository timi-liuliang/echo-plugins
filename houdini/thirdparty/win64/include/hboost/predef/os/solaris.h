/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_SOLARIS_H
#define HBOOST_PREDEF_OS_SOLARIS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_SOLARIS`]

[@http://en.wikipedia.org/wiki/Solaris_Operating_Environment Solaris] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`sun`] [__predef_detection__]]
    [[`__sun`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_SOLARIS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(sun) || defined(__sun) \
    )
#   undef HBOOST_OS_SOLARIS
#   define HBOOST_OS_SOLARIS HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_SOLARIS
#   define HBOOST_OS_SOLARIS_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_SOLARIS_NAME "Solaris"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_SOLARIS,HBOOST_OS_SOLARIS_NAME)
