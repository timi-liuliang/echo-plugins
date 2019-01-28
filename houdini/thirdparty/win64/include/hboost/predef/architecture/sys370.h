/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_ARCHITECTURE_SYS370_H
#define HBOOST_PREDEF_ARCHITECTURE_SYS370_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_ARCH_SYS370`]

[@http://en.wikipedia.org/wiki/System/370 System/370] architecture.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__370__`] [__predef_detection__]]
    [[`__THW_370__`] [__predef_detection__]]
    ]
 */

#define HBOOST_ARCH_SYS370 HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__370__) || defined(__THW_370__)
#   undef HBOOST_ARCH_SYS370
#   define HBOOST_ARCH_SYS370 HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_ARCH_SYS370
#   define HBOOST_ARCH_SYS370_AVAILABLE
#endif

#define HBOOST_ARCH_SYS370_NAME "System/370"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_ARCH_SYS370,HBOOST_ARCH_SYS370_NAME)
