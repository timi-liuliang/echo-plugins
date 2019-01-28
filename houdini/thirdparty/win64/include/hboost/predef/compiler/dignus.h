/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_DIGNUS_H
#define HBOOST_PREDEF_COMPILER_DIGNUS_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_SYSC`]

[@http://www.dignus.com/dcxx/ Dignus Systems/C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__SYSC__`] [__predef_detection__]]

    [[`__SYSC_VER__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_SYSC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__SYSC__)
#   define HBOOST_COMP_SYSC_DETECTION HBOOST_PREDEF_MAKE_10_VRRPP(__SYSC_VER__)
#endif

#ifdef HBOOST_COMP_SYSC_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_SYSC_EMULATED HBOOST_COMP_SYSC_DETECTION
#   else
#       undef HBOOST_COMP_SYSC
#       define HBOOST_COMP_SYSC HBOOST_COMP_SYSC_DETECTION
#   endif
#   define HBOOST_COMP_SYSC_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_SYSC_NAME "Dignus Systems/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_SYSC,HBOOST_COMP_SYSC_NAME)

#ifdef HBOOST_COMP_SYSC_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_SYSC_EMULATED,HBOOST_COMP_SYSC_NAME)
#endif
