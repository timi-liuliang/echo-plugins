/*
Copyright Rene Rivera 2011-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LANGUAGE_STDCPP_H
#define HBOOST_PREDEF_LANGUAGE_STDCPP_H

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LANG_STDCPP`]

[@http://en.wikipedia.org/wiki/C%2B%2B Standard C++] language.
If available, the year of the standard is detected as YYYY.MM.1 from the Epoc date.
Because of the way the C++ standardization process works the
defined version year will not be the commonly known year of the standard.
Specifically the defined versions are:

[table Detected Version Number vs. C++ Standard Year
  [[Detected Version Number] [Standard Year] [C++ Standard]]
  [[27.11.1] [1998] [ISO/IEC 14882:1998]]
  [[41.12.1] [2011] [ISO/IEC 14882:2011]]
]

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__cplusplus`] [__predef_detection__]]

    [[`__cplusplus`] [YYYY.MM.1]]
    ]
 */

#define HBOOST_LANG_STDCPP HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__cplusplus)
#   undef HBOOST_LANG_STDCPP
#   if (__cplusplus > 100)
#       define HBOOST_LANG_STDCPP HBOOST_PREDEF_MAKE_YYYYMM(__cplusplus)
#   else
#       define HBOOST_LANG_STDCPP HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_LANG_STDCPP
#   define HBOOST_LANG_STDCPP_AVAILABLE
#endif

#define HBOOST_LANG_STDCPP_NAME "Standard C++"

/*`
[heading `HBOOST_LANG_STDCPPCLI`]

[@http://en.wikipedia.org/wiki/C%2B%2B/CLI Standard C++/CLI] language.
If available, the year of the standard is detected as YYYY.MM.1 from the Epoc date.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__cplusplus_cli`] [__predef_detection__]]

    [[`__cplusplus_cli`] [YYYY.MM.1]]
    ]
 */

#define HBOOST_LANG_STDCPPCLI HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__cplusplus_cli)
#   undef HBOOST_LANG_STDCPPCLI
#   if (__cplusplus_cli > 100)
#       define HBOOST_LANG_STDCPPCLI HBOOST_PREDEF_MAKE_YYYYMM(__cplusplus_cli)
#   else
#       define HBOOST_LANG_STDCPPCLI HBOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if HBOOST_LANG_STDCPPCLI
#   define HBOOST_LANG_STDCPPCLI_AVAILABLE
#endif

#define HBOOST_LANG_STDCPPCLI_NAME "Standard C++/CLI"

/*`
[heading `HBOOST_LANG_STDECPP`]

[@http://en.wikipedia.org/wiki/Embedded_C%2B%2B Standard Embedded C++] language.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__embedded_cplusplus`] [__predef_detection__]]
    ]
 */

#define HBOOST_LANG_STDECPP HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__embedded_cplusplus)
#   undef HBOOST_LANG_STDECPP
#   define HBOOST_LANG_STDECPP HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_LANG_STDECPP
#   define HBOOST_LANG_STDECPP_AVAILABLE
#endif

#define HBOOST_LANG_STDECPP_NAME "Standard Embedded C++"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LANG_STDCPP,HBOOST_LANG_STDCPP_NAME)

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LANG_STDCPPCLI,HBOOST_LANG_STDCPPCLI_NAME)

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LANG_STDECPP,HBOOST_LANG_STDECPP_NAME)
