/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_SUNPRO_H
#define HBOOST_PREDEF_COMPILER_SUNPRO_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_SUNPRO`]

[@http://en.wikipedia.org/wiki/Oracle_Solaris_Studio Oracle Solaris Studio] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__SUNPRO_CC`] [__predef_detection__]]
    [[`__SUNPRO_C`] [__predef_detection__]]

    [[`__SUNPRO_CC`] [V.R.P]]
    [[`__SUNPRO_C`] [V.R.P]]
    [[`__SUNPRO_CC`] [VV.RR.P]]
    [[`__SUNPRO_C`] [VV.RR.P]]
    ]
 */

#define HBOOST_COMP_SUNPRO HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#   if !defined(HBOOST_COMP_SUNPRO_DETECTION) && defined(__SUNPRO_CC)
#       if (__SUNPRO_CC < 0x5100)
#           define HBOOST_COMP_SUNPRO_DETECTION HBOOST_PREDEF_MAKE_0X_VRP(__SUNPRO_CC)
#       else
#           define HBOOST_COMP_SUNPRO_DETECTION HBOOST_PREDEF_MAKE_0X_VVRRP(__SUNPRO_CC)
#       endif
#   endif
#   if !defined(HBOOST_COMP_SUNPRO_DETECTION) && defined(__SUNPRO_C)
#       if (__SUNPRO_C < 0x5100)
#           define HBOOST_COMP_SUNPRO_DETECTION HBOOST_PREDEF_MAKE_0X_VRP(__SUNPRO_C)
#       else
#           define HBOOST_COMP_SUNPRO_DETECTION HBOOST_PREDEF_MAKE_0X_VVRRP(__SUNPRO_C)
#       endif
#   endif
#   if !defined(HBOOST_COMP_SUNPRO_DETECTION)
#       define HBOOST_COMP_SUNPRO_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef HBOOST_COMP_SUNPRO_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_SUNPRO_EMULATED HBOOST_COMP_SUNPRO_DETECTION
#   else
#       undef HBOOST_COMP_SUNPRO
#       define HBOOST_COMP_SUNPRO HBOOST_COMP_SUNPRO_DETECTION
#   endif
#   define HBOOST_COMP_SUNPRO_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_SUNPRO_NAME "Oracle Solaris Studio"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_SUNPRO,HBOOST_COMP_SUNPRO_NAME)

#ifdef HBOOST_COMP_SUNPRO_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_SUNPRO_EMULATED,HBOOST_COMP_SUNPRO_NAME)
#endif
