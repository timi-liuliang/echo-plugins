/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_COMPILER_GCC_XML_H
#define HBOOST_PREDEF_COMPILER_GCC_XML_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_COMP_GCCXML`]

[@http://www.gccxml.org/ GCC XML] compiler.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__GCCXML__`] [__predef_detection__]]
    ]
 */

#define HBOOST_COMP_GCCXML HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__GCCXML__)
#   define HBOOST_COMP_GCCXML_DETECTION HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#ifdef HBOOST_COMP_GCCXML_DETECTION
#   if defined(HBOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define HBOOST_COMP_GCCXML_EMULATED HBOOST_COMP_GCCXML_DETECTION
#   else
#       undef HBOOST_COMP_GCCXML
#       define HBOOST_COMP_GCCXML HBOOST_COMP_GCCXML_DETECTION
#   endif
#   define HBOOST_COMP_GCCXML_AVAILABLE
#   include <hboost/predef/detail/comp_detected.h>
#endif

#define HBOOST_COMP_GCCXML_NAME "GCC XML"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_GCCXML,HBOOST_COMP_GCCXML_NAME)

#ifdef HBOOST_COMP_GCCXML_EMULATED
#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_COMP_GCCXML_EMULATED,HBOOST_COMP_GCCXML_NAME)
#endif
