/*
Copyright Rene Rivera 2011-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_STD_CXX_H
#define HBOOST_PREDEF_LIBRARY_STD_CXX_H

#include <hboost/predef/library/std/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_STD_CXX`]

[@http://libcxx.llvm.org/ libc++] C++ Standard Library.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`_LIBCPP_VERSION`] [__predef_detection__]]

    [[`_LIBCPP_VERSION`] [V.0.P]]
    ]
 */

#define HBOOST_LIB_STD_CXX HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(_LIBCPP_VERSION)
#   undef HBOOST_LIB_STD_CXX
#   define HBOOST_LIB_STD_CXX HBOOST_PREDEF_MAKE_10_VPPP(_LIBCPP_VERSION)
#endif

#if HBOOST_LIB_STD_CXX
#   define HBOOST_LIB_STD_CXX_AVAILABLE
#endif

#define HBOOST_LIB_STD_CXX_NAME "libc++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_STD_CXX,HBOOST_LIB_STD_CXX_NAME)
