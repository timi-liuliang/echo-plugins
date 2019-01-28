/*
Copyright Rene Rivera 2011-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_VMS_H
#define HBOOST_PREDEF_OS_VMS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_VMS`]

[@http://en.wikipedia.org/wiki/Vms VMS] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`VMS`] [__predef_detection__]]
    [[`__VMS`] [__predef_detection__]]

    [[`__VMS_VER`] [V.R.P]]
    ]
 */

#define HBOOST_OS_VMS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(VMS) || defined(__VMS) \
    )
#   undef HBOOST_OS_VMS
#   if defined(__VMS_VER)
#       define HBOOST_OS_VMS HBOOST_PREDEF_MAKE_10_VVRR00PP00(__VMS_VER)
#   else
#       define HBOOST_OS_VMS HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_OS_VMS
#   define HBOOST_OS_VMS_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_VMS_NAME "VMS"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_VMS,HBOOST_OS_VMS_NAME)
