/*
Copyright Rene Rivera 2013-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_ARCHITECTURE_BLACKFIN_H
#define HBOOST_PREDEF_ARCHITECTURE_BLACKFIN_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_ARCH_BLACKFIN`]

Blackfin Processors from Analog Devices.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__bfin__`] [__predef_detection__]]
    [[`__BFIN__`] [__predef_detection__]]
    [[`bfin`] [__predef_detection__]]
    [[`BFIN`] [__predef_detection__]]
    ]
 */

#define HBOOST_ARCH_BLACKFIN HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__bfin__) || defined(__BFIN__) || \
    defined(bfin) || defined(BFIN)
#   undef HBOOST_ARCH_BLACKFIN
#   define HBOOST_ARCH_BLACKFIN HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_ARCH_BLACKFIN
#   define HBOOST_ARCH_BLACKFIN_AVAILABLE
#endif

#define HBOOST_ARCH_BLACKFIN_NAME "Blackfin"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_ARCH_BLACKFIN,HBOOST_ARCH_BLACKFIN_NAME)
