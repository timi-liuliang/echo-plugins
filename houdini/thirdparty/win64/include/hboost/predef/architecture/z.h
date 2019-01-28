/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_ARCHITECTURE_Z_H
#define HBOOST_PREDEF_ARCHITECTURE_Z_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_ARCH_Z`]

[@http://en.wikipedia.org/wiki/Z/Architecture z/Architecture] architecture.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__SYSC_ZARCH__`] [__predef_detection__]]
    ]
 */

#define HBOOST_ARCH_Z HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__SYSC_ZARCH__)
#   undef HBOOST_ARCH_Z
#   define HBOOST_ARCH_Z HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_ARCH_Z
#   define HBOOST_ARCH_Z_AVAILABLE
#endif

#define HBOOST_ARCH_Z_NAME "z/Architecture"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_ARCH_Z,HBOOST_ARCH_Z_NAME)
