/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_STD_MSL_H
#define HBOOST_PREDEF_LIBRARY_STD_MSL_H

#include <hboost/predef/library/std/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_STD_MSL`]

[@http://www.freescale.com/ Metrowerks] Standard C++ Library.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__MSL_CPP__`] [__predef_detection__]]
    [[`__MSL__`] [__predef_detection__]]

    [[`__MSL_CPP__`] [V.R.P]]
    [[`__MSL__`] [V.R.P]]
    ]
 */

#define HBOOST_LIB_STD_MSL HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__MSL_CPP__) || defined(__MSL__)
#   undef HBOOST_LIB_STD_MSL
#   if defined(__MSL_CPP__)
#       define HBOOST_LIB_STD_MSL HBOOST_PREDEF_MAKE_0X_VRPP(__MSL_CPP__)
#   else
#       define HBOOST_LIB_STD_MSL HBOOST_PREDEF_MAKE_0X_VRPP(__MSL__)
#   endif
#endif

#if HBOOST_LIB_STD_MSL
#   define HBOOST_LIB_STD_MSL_AVAILABLE
#endif

#define HBOOST_LIB_STD_MSL_NAME "Metrowerks"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_STD_MSL,HBOOST_LIB_STD_MSL_NAME)
