/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_CYGWIN_H
#define HBOOST_PREDEF_OS_CYGWIN_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_CYGWIN`]

[@http://en.wikipedia.org/wiki/Cygwin Cygwin] evironment.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__CYGWIN__`] [__predef_detection__]]
    ]
 */

#define HBOOST_OS_CYGWIN HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__CYGWIN__) \
    )
#   undef HBOOST_OS_CYGWIN
#   define HBOOST_OS_CYGWIN HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_OS_CYGWIN
#   define HBOOST_OS_CYGWIN_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_CYGWIN_NAME "Cygwin"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_CYGWIN,HBOOST_OS_CYGWIN_NAME)
