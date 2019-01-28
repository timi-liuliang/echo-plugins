/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_C_ZOS_H
#define HBOOST_PREDEF_LIBRARY_C_ZOS_H

#include <hboost/predef/library/c/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_C_ZOS`]

z/OS libc Standard C library.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__LIBREL__`] [__predef_detection__]]

    [[`__LIBREL__`] [V.R.P]]
    [[`__TARGET_LIB__`] [V.R.P]]
    ]
 */

#define HBOOST_LIB_C_ZOS HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__LIBREL__)
#   undef HBOOST_LIB_C_ZOS
#   if !defined(HBOOST_LIB_C_ZOS) && defined(__LIBREL__)
#       define HBOOST_LIB_C_ZOS HBOOST_PREDEF_MAKE_0X_VRRPPPP(__LIBREL__)
#   endif
#   if !defined(HBOOST_LIB_C_ZOS) && defined(__TARGET_LIB__)
#       define HBOOST_LIB_C_ZOS HBOOST_PREDEF_MAKE_0X_VRRPPPP(__TARGET_LIB__)
#   endif
#   if !defined(HBOOST_LIB_C_ZOS)
#       define HBOOST_LIB_C_ZOS HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_LIB_C_ZOS
#   define HBOOST_LIB_C_ZOS_AVAILABLE
#endif

#define HBOOST_LIB_C_ZOS_NAME "z/OS"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_C_ZOS,HBOOST_LIB_C_ZOS_NAME)
