//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief few helpers for working with variadic macros
// ***************************************************************************

#ifndef HBOOST_TEST_PP_VARIADIC_HPP_021515GER
#define HBOOST_TEST_PP_VARIADIC_HPP_021515GER

// Boost
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/variadic/size.hpp>

//____________________________________________________________________________//

#if HBOOST_PP_VARIADICS

#if HBOOST_PP_VARIADICS_MSVC
#  define HBOOST_TEST_INVOKE_VARIADIC( tool, ... ) HBOOST_PP_CAT( tool (__VA_ARGS__), )
#else
#  define HBOOST_TEST_INVOKE_VARIADIC( tool, ... ) tool (__VA_ARGS__)
#endif

//____________________________________________________________________________//

/// if sizeof(__VA_ARGS__) == N: F1(__VA_ARGS__)
/// else:                        F2(__VA_ARGS__)
#define HBOOST_TEST_INVOKE_IF_N_ARGS( N, F1, F2, ... )               \
    HBOOST_TEST_INVOKE_VARIADIC(                                     \
        HBOOST_PP_IIF(                                               \
            HBOOST_PP_EQUAL(HBOOST_PP_VARIADIC_SIZE(__VA_ARGS__), N), \
            F1,                                                     \
            F2),                                                    \
        __VA_ARGS__ )                                               \
/**/

//____________________________________________________________________________//

#endif  /* HBOOST_PP_VARIADICS */

#endif // HBOOST_TEST_PP_VARIADIC_HPP_021515GER

// EOF
