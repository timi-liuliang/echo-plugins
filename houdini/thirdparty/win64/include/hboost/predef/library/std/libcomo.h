/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_STD_LIBCOMO_H
#define HBOOST_PREDEF_LIBRARY_STD_LIBCOMO_H

#include <hboost/predef/library/std/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_STD_COMO`]

[@http://www.comeaucomputing.com/libcomo/ Comeau Computing] Standard C++ Library.
Version number available as major.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__LIBCOMO__`] [__predef_detection__]]

    [[`__LIBCOMO_VERSION__`] [V.0.0]]
    ]
 */

#define HBOOST_LIB_STD_COMO HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__LIBCOMO__)
#   undef HBOOST_LIB_STD_COMO
#   define HBOOST_LIB_STD_COMO HBOOST_VERSION_NUMBER(__LIBCOMO_VERSION__,0,0)
#endif

#if HBOOST_LIB_STD_COMO
#   define HBOOST_LIB_STD_COMO_AVAILABLE
#endif

#define HBOOST_LIB_STD_COMO_NAME "Comeau Computing"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_STD_COMO,HBOOST_LIB_STD_COMO_NAME)
