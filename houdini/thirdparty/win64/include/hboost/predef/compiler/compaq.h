/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_COMPAQ_H
#define HBOOST_PREDEF_COMPILER_COMPAQ_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_DEC`]

[@http://www.openvms.compaq.com/openvms/brochures/deccplus/ Compaq C/C++] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__DECCXX`] [__predef_detection__]]
    [[`__DECC`] [__predef_detection__]]

    [[`__DECCXX_VER`] [V.R.P]]
    [[`__DECC_VER`] [V.R.P]]
    ]
 */

#define HBOOST_COMP_DEC HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__DECC) || defined(__DECCXX)
#   if !defined(HBOOST_COMP_DEC_DETECTION) && defined(__DECCXX_VER)
#       define HBOOST_COMP_DEC_DETECTION HBOOST_PREDEF_MAKE_10_VVRR0PP00(__DECCXX_VER)
#   endif
#   if !defined(HBOOST_COMP_DEC_DETECTION) && defined(__DECC_VER)
#       define HBOOST_COMP_DEC_DETECTION HBOOST_PREDEF_MAKE_10_VVRR0PP00(__DECC_VER)
#   endif
#   if !defined(HBOOST_COMP_DEC_DETECTION)
#       define HBOOST_COM_DEC_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef HBOOST_COMP_DEC_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_DEC_EMULATED HBOOST_COMP_DEC_DETECTION
#   else
#       undef HBOOST_COMP_DEC
#       define HBOOST_COMP_DEC HBOOST_COMP_DEC_DETECTION
#   endif
#   define HBOOST_COMP_DEC_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_DEC_NAME "Compaq C/C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_DEC,HBOOST_COMP_DEC_NAME)

#ifdef HBOOST_COMP_DEC_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_DEC_EMULATED,HBOOST_COMP_DEC_NAME)
#endif
