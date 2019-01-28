/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_C_UC_H
#define HBOOST_PREDEF_LIBRARY_C_UC_H

#include <hboost/predef/library/c/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_C_UC`]

[@http://en.wikipedia.org/wiki/Uclibc uClibc] Standard C library.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__UCLIBC__`] [__predef_detection__]]

    [[`__UCLIBC_MAJOR__`, `__UCLIBC_MINOR__`, `__UCLIBC_SUBLEVEL__`] [V.R.P]]
    ]
 */

#define HBOOST_LIB_C_UC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__UCLIBC__)
#   undef HBOOST_LIB_C_UC
#   define HBOOST_LIB_C_UC HBOOST_VERSION_NUMBER(\
        __UCLIBC_MAJOR__,__UCLIBC_MINOR__,__UCLIBC_SUBLEVEL__)
#endif

#if HBOOST_LIB_C_UC
#   define HBOOST_LIB_C_UC_AVAILABLE
#endif

#define HBOOST_LIB_C_UC_NAME "uClibc"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_C_UC,HBOOST_LIB_C_UC_NAME)
