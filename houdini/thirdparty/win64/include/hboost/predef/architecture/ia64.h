/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_ARCHITECTURE_IA64_H
#define HBOOST_PREDEF_ARCHITECTURE_IA64_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_ARCH_IA64`]

[@http://en.wikipedia.org/wiki/Ia64 Intel Itanium 64] architecture.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__ia64__`] [__predef_detection__]]
    [[`_IA64`] [__predef_detection__]]
    [[`__IA64__`] [__predef_detection__]]
    [[`__ia64`] [__predef_detection__]]
    [[`_M_IA64`] [__predef_detection__]]
    [[`__itanium__`] [__predef_detection__]]
    ]
 */

#define HBOOST_ARCH_IA64 HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__ia64__) || defined(_IA64) || \
    defined(__IA64__) || defined(__ia64) || \
    defined(_M_IA64) || defined(__itanium__)
#   undef HBOOST_ARCH_IA64
#   define HBOOST_ARCH_IA64 HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_ARCH_IA64
#   define HBOOST_ARCH_IA64_AVAILABLE
#endif

#define HBOOST_ARCH_IA64_NAME "Intel Itanium 64"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_ARCH_IA64,HBOOST_ARCH_IA64_NAME)
