/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_LIBRARY_STD_VACPP_H
#define HBOOST_PREDEF_LIBRARY_STD_VACPP_H

#include <hboost/predef/library/std/_prefix.h>

#include <hboost/predef/version_number.h>
#include <hboost/predef/make.h>

/*`
[heading `HBOOST_LIB_STD_IBM`]

[@http://www.ibm.com/software/awdtools/xlcpp/ IBM VACPP Standard C++] library.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__IBMCPP__`] [__predef_detection__]]
    ]
 */

#define HBOOST_LIB_STD_IBM HBOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__IBMCPP__)
#   undef HBOOST_LIB_STD_IBM
#   define HBOOST_LIB_STD_IBM HBOOST_VERSION_NUMBER_AVAILABLE
#endif

#if HBOOST_LIB_STD_IBM
#   define HBOOST_LIB_STD_IBM_AVAILABLE
#endif

#define HBOOST_LIB_STD_IBM_NAME "IBM VACPP"

#endif

#include <hboost/predef/detail/test.h>
HBOOST_PREDEF_DECLARE_TEST(HBOOST_LIB_STD_IBM,HBOOST_LIB_STD_IBM_NAME)
