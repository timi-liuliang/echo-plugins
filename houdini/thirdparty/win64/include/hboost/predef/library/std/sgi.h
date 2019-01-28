/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_STD_SGI_H
#define HBOOST_PREDEF_LIBRARY_STD_SGI_H

#include <hboost/predef/library/std/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_STD_SGI`]

[@http://www.sgi.com/tech/stl/ SGI] Standard C++ library.
If available version number as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__STL_CONFIG_H`] [__predef_detection__]]

    [[`__SGI_STL`] [V.R.P]]
    ]
 */

#define HBOOST_LIB_STD_SGI HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__STL_CONFIG_H)
#   undef HBOOST_LIB_STD_SGI
#   if defined(__SGI_STL)
#       define HBOOST_LIB_STD_SGI HBOOST_PREDEF_MAKE_0X_VRP(__SGI_STL)
#   else
#       define HBOOST_LIB_STD_SGI HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_LIB_STD_SGI
#   define HBOOST_LIB_STD_SGI_AVAILABLE
#endif

#define HBOOST_LIB_STD_SGI_NAME "SGI"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_STD_SGI,HBOOST_LIB_STD_SGI_NAME)
