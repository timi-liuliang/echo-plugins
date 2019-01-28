/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_STD_MODENA_H
#define HBOOST_PREDEF_LIBRARY_STD_MODENA_H

#include <hboost/predef/library/std/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_STD_MSIPL`]

[@http://modena.us/ Modena Software Lib++] Standard C++ Library.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`MSIPL_COMPILE_H`] [__predef_detection__]]
    [[`__MSIPL_COMPILE_H`] [__predef_detection__]]
    ]
 */

#define HBOOST_LIB_STD_MSIPL HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(MSIPL_COMPILE_H) || defined(__MSIPL_COMPILE_H)
#   undef HBOOST_LIB_STD_MSIPL
#   define HBOOST_LIB_STD_MSIPL HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_LIB_STD_MSIPL
#   define HBOOST_LIB_STD_MSIPL_AVAILABLE
#endif

#define HBOOST_LIB_STD_MSIPL_NAME "Modena Software Lib++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_STD_MSIPL,HBOOST_LIB_STD_MSIPL_NAME)
