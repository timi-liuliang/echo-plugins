/*
Copyright (c) Microsoft Corporation 2014
Copyright Rene Rivera 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_PLAT_WINDOWS_RUNTIME_H
#define HBOOST_PREDEF_PLAT_WINDOWS_RUNTIME_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>
#include <hboost/predef/os/windows.h>

/*`
[heading `HBOOST_PLAT_WINDOWS_RUNTIME`]

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`WINAPI_FAMILY == WINAPI_FAMILY_APP`] [__predef_detection__]]
    [[`WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP`] [__predef_detection__]]
    ]
 */

#define HBOOST_PLAT_WINDOWS_RUNTIME HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if HBOOST_OS_WINDOWS && defined(WINAPI_FAMILY) && \
    ( WINAPI_FAMILY == WINAPI_FAMILY_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP )
#   undef HBOOST_PLAT_WINDOWS_RUNTIME
#   define HBOOST_PLAT_WINDOWS_RUNTIME HBOOST_VERSION_NUMBER_AVAILABLE
#endif
 
#if HBOOST_PLAT_WINDOWS_RUNTIME
#   define HBOOST_PLAT_WINDOWS_RUNTIME_AVAILABLE
#   include <hboost/predef/detail/platform_detected.h>
#endif

#define HBOOST_PLAT_WINDOWS_RUNTIME_NAME "Windows Runtime"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_PLAT_WINDOWS_RUNTIME,HBOOST_PLAT_WINDOWS_RUNTIME_NAME)