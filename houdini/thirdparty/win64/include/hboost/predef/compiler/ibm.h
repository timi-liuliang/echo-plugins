/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_IBM_H
#define HBOOST_PREDEF_COMPILER_IBM_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_IBM`]

[@http://en.wikipedia.org/wiki/VisualAge IBM XL C/C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__IBMCPP__`] [__predef_detection__]]
    [[`__xlC__`] [__predef_detection__]]
    [[`__xlc__`] [__predef_detection__]]

    [[`__COMPILER_VER__`] [V.R.P]]
    [[`__xlC__`] [V.R.P]]
    [[`__xlc__`] [V.R.P]]
    [[`__IBMCPP__`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_IBM HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__IBMCPP__) || defined(__xlC__) || defined(__xlc__)
#   if !defined(HBOOST_COMP_IBM_DETECTION) && defined(__COMPILER_VER__)
#       define HBOOST_COMP_IBM_DETECTION HBOOST_PREDEF_MAKE_0X_VRRPPPP(__COMPILER_VER__)
#   endif
#   if !defined(HBOOST_COMP_IBM_DETECTION) && defined(__xlC__)
#       define HBOOST_COMP_IBM_DETECTION HBOOST_PREDEF_MAKE_0X_VVRR(__xlC__)
#   endif
#   if !defined(HBOOST_COMP_IBM_DETECTION) && defined(__xlc__)
#       define HBOOST_COMP_IBM_DETECTION HBOOST_PREDEF_MAKE_0X_VVRR(__xlc__)
#   endif
#   if !defined(HBOOST_COMP_IBM_DETECTION)
#       define HBOOST_COMP_IBM_DETECTION HBOOST_PREDEF_MAKE_10_VRP(__IBMCPP__)
#   endif
#endif

#ifdef HBOOST_COMP_IBM_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_IBM_EMULATED HBOOST_COMP_IBM_DETECTION
#   else
#       undef HBOOST_COMP_IBM
#       define HBOOST_COMP_IBM HBOOST_COMP_IBM_DETECTION
#   endif
#   define HBOOST_COMP_IBM_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_IBM_NAME "IBM XL C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_IBM,HBOOST_COMP_IBM_NAME)

#ifdef HBOOST_COMP_IBM_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_IBM_EMULATED,HBOOST_COMP_IBM_NAME)
#endif
