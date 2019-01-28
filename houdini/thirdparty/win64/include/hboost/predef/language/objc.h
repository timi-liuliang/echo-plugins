/*
Copyright Rene Rivera 2011-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LANGUAGE_OBJC_H
#define HBOOST_PREDEF_LANGUAGE_OBJC_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LANG_OBJC`]

[@http://en.wikipedia.org/wiki/Objective-C Objective-C] language.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__OBJC__`] [__predef_detection__]]
    ]
 */

#define HBOOST_LANG_OBJC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__OBJC__)
#   undef HBOOST_LANG_OBJC
#   define HBOOST_LANG_OBJC HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_LANG_OBJC
#   define HBOOST_LANG_OBJC_AVAILABLE
#endif

#define HBOOST_LANG_OBJC_NAME "Objective-C"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LANG_OBJC,HBOOST_LANG_OBJC_NAME)
