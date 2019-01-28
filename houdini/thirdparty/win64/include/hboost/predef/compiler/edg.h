/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_EDG_H
#define HBOOST_PREDEF_COMPILER_EDG_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_EDG`]

[@http://en.wikipedia.org/wiki/Edison_Design_Group EDG C++ Frontend] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__EDG__`] [__predef_detection__]]

    [[`__EDG_VERSION__`] [V.R.0]]
    ]
 */

#define HBOOST_COMP_EDG HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__EDG__)
#   define HBOOST_COMP_EDG_DETECTION HBOOST_PREDEF_MAKE_10_VRR(__EDG_VERSION__)
#endif

#ifdef HBOOST_COMP_EDG_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_EDG_EMULATED HBOOST_COMP_EDG_DETECTION
#   else
#       undef HBOOST_COMP_EDG
#       define HBOOST_COMP_EDG HBOOST_COMP_EDG_DETECTION
#   endif
#   define HBOOST_COMP_EDG_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_EDG_NAME "EDG C++ Frontend"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_EDG,HBOOST_COMP_EDG_NAME)

#ifdef HBOOST_COMP_EDG_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_EDG_EMULATED,HBOOST_COMP_EDG_NAME)
#endif
