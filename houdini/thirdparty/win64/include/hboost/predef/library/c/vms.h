/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_C_VMS_H
#define HBOOST_PREDEF_LIBRARY_C_VMS_H

#include <hboost/predef/library/c/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_C_VMS`]

VMS libc Standard C library.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__CRTL_VER`] [__predef_detection__]]

    [[`__CRTL_VER`] [V.R.P]]
    ]
 */

#define HBOOST_LIB_C_VMS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__CRTL_VER)
#   undef HBOOST_LIB_C_VMS
#   define HBOOST_LIB_C_VMS HBOOST_PREDEF_MAKE_10_VVRR0PP00(__CRTL_VER)
#endif

#if HBOOST_LIB_C_VMS
#   define HBOOST_LIB_C_VMS_AVAILABLE
#endif

#define HBOOST_LIB_C_VMS_NAME "VMS"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_C_VMS,HBOOST_LIB_C_VMS_NAME)
