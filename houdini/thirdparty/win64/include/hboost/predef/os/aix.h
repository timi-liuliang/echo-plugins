/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_OS_AIX_H
#define HBOOST_PREDEF_OS_AIX_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_OS_AIX`]

[@http://en.wikipedia.org/wiki/AIX_operating_system IBM AIX] operating system.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`_AIX`] [__predef_detection__]]
    [[`__TOS_AIX__`] [__predef_detection__]]

    [[`_AIX43`] [4.3.0]]
    [[`_AIX41`] [4.1.0]]
    [[`_AIX32`] [3.2.0]]
    [[`_AIX3`] [3.0.0]]
    ]
 */

#define HBOOST_OS_AIX HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(HBOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(_AIX) || defined(__TOS_AIX__) \
    )
#   undef HBOOST_OS_AIX
#   if !defined(HBOOST_OS_AIX) && defined(_AIX43)
#       define HBOOST_OS_AIX HBOOST_VERSION_NUMBER(4,3,0)
#   endif
#   if !defined(HBOOST_OS_AIX) && defined(_AIX41)
#       define HBOOST_OS_AIX HBOOST_VERSION_NUMBER(4,1,0)
#   endif
#   if !defined(HBOOST_OS_AIX) && defined(_AIX32)
#       define HBOOST_OS_AIX HBOOST_VERSION_NUMBER(3,2,0)
#   endif
#   if !defined(HBOOST_OS_AIX) && defined(_AIX3)
#       define HBOOST_OS_AIX HBOOST_VERSION_NUMBER(3,0,0)
#   endif
#   if !defined(HBOOST_OS_AIX)
#       define HBOOST_OS_AIX HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_OS_AIX
#   define HBOOST_OS_AIX_AVAILABLE
#   include <hboost/predef/detail/os_detected.h>
#endif

#define HBOOST_OS_AIX_NAME "IBM AIX"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_OS_AIX,HBOOST_OS_AIX_NAME)
