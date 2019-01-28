/*
Copyright Rene Rivera 2011-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_ARCHITECTURE_PYRAMID_H
#define HBOOST_PREDEF_ARCHITECTURE_PYRAMID_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_ARCH_PYRAMID`]

Pyramid 9810 architecture.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`pyr`] [__predef_detection__]]
    ]
 */

#define HBOOST_ARCH_PYRAMID HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(pyr)
#   undef HBOOST_ARCH_PYRAMID
#   define HBOOST_ARCH_PYRAMID HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_ARCH_PYRAMID
#   define HBOOST_ARCH_PYRAMID_AVAILABLE
#endif

#define HBOOST_ARCH_PYRAMID_NAME "Pyramid 9810"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_ARCH_PYRAMID,HBOOST_ARCH_PYRAMID_NAME)
