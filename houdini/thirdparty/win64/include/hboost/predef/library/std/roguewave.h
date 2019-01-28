/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_STD_ROGUEWAVE_H
#define HBOOST_PREDEF_LIBRARY_STD_ROGUEWAVE_H

#include <hboost/predef/library/std/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_STD_RW`]

[@http://stdcxx.apache.org/ Roguewave] Standard C++ library.
If available version number as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__STD_RWCOMPILER_H__`] [__predef_detection__]]
    [[`_RWSTD_VER`] [__predef_detection__]]

    [[`_RWSTD_VER`] [V.R.P]]
    ]
 */

#define HBOOST_LIB_STD_RW HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__STD_RWCOMPILER_H__) || defined(_RWSTD_VER)
#   undef HBOOST_LIB_STD_RW
#   if defined(_RWSTD_VER)
#       if _RWSTD_VER < 0x010000
#           define HBOOST_LIB_STD_RW HBOOST_PREDEF_MAKE_0X_VVRRP(_RWSTD_VER)
#       else
#           define HBOOST_LIB_STD_RW HBOOST_PREDEF_MAKE_0X_VVRRPP(_RWSTD_VER)
#       endif
#   else
#       define HBOOST_LIB_STD_RW HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_LIB_STD_RW
#   define HBOOST_LIB_STD_RW_AVAILABLE
#endif

#define HBOOST_LIB_STD_RW_NAME "Roguewave"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_STD_RW,HBOOST_LIB_STD_RW_NAME)
