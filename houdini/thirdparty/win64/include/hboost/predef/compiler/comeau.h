/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_COMEAU_H
#define HBOOST_PREDEF_COMPILER_COMEAU_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

#define HBOOST_COMP_COMO HBOOST_VERSION_NUMBER_NOT_AVAILABLE

/*`
[heading `HBOOST_COMP_COMO`]

[@http://en.wikipedia.org/wiki/Comeau_C/C%2B%2B Comeau C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__COMO__`] [__predef_detection__]]

    [[`__COMO_VERSION__`] [V.R.P]]
    ]
 */

#if defined(__COMO__)
#   if !defined(HBOOST_COMP_COMO_DETECTION) && defined(__COMO_VERSION__)
#       define HBOOST_COMP_COMO_DETECTION HBOOST_PREDEF_MAKE_0X_VRP(__COMO_VERSION__)
#   endif
#   if !defined(HBOOST_COMP_COMO_DETECTION)
#       define HBOOST_COMP_COMO_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef HBOOST_COMP_COMO_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_COMO_EMULATED HBOOST_COMP_COMO_DETECTION
#   else
#       undef HBOOST_COMP_COMO
#       define HBOOST_COMP_COMO HBOOST_COMP_COMO_DETECTION
#   endif
#   define HBOOST_COMP_COMO_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_COMO_NAME "Comeau C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_COMO,HBOOST_COMP_COMO_NAME)

#ifdef HBOOST_COMP_COMO_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_COMO_EMULATED,HBOOST_COMP_COMO_NAME)
#endif
