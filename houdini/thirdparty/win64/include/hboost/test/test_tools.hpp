//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief test tools compatibility header
///
/// This file is used to select the test tools implementation and includes all the necessary headers
// ***************************************************************************

#ifndef HBOOST_TEST_TOOLS_HPP_111812GER
#define HBOOST_TEST_TOOLS_HPP_111812GER

#include <hboost/config.hpp>

// brings some compiler configuration like HBOOST_PP_VARIADICS
#include <hboost/test/detail/config.hpp>

#include <hboost/preprocessor/config/config.hpp>

#if    defined(HBOOST_NO_CXX11_VARIADIC_MACROS) \
    || defined(HBOOST_NO_CXX11_AUTO_DECLARATIONS) \
    || defined(HBOOST_NO_CXX11_DECLTYPE)
#  define HBOOST_TEST_MACRO_LIMITED_SUPPORT
#endif

// Boost.Test
// #define HBOOST_TEST_NO_OLD_TOOLS

#if     defined(HBOOST_TEST_MACRO_LIMITED_SUPPORT) \
    &&  (   !HBOOST_PP_VARIADICS \
         || !(__cplusplus >= 201103L) && defined(HBOOST_NO_CXX11_VARIADIC_MACROS))
#  define HBOOST_TEST_NO_NEW_TOOLS
#endif

// #define HBOOST_TEST_TOOLS_UNDER_DEBUGGER
// #define HBOOST_TEST_TOOLS_DEBUGGABLE

#include <hboost/test/tools/context.hpp>

#ifndef HBOOST_TEST_NO_OLD_TOOLS
#  include <hboost/test/tools/old/interface.hpp>
#  include <hboost/test/tools/old/impl.hpp>

#  include <hboost/test/tools/detail/print_helper.hpp>
#endif

#ifndef HBOOST_TEST_NO_NEW_TOOLS
#  include <hboost/test/tools/interface.hpp>
#  include <hboost/test/tools/assertion.hpp>
#  include <hboost/test/tools/fpc_op.hpp>
#  include <hboost/test/tools/collection_comparison_op.hpp>
#  include <hboost/test/tools/cstring_comparison_op.hpp>

#  include <hboost/test/tools/detail/fwd.hpp>
#  include <hboost/test/tools/detail/print_helper.hpp>
#  include <hboost/test/tools/detail/it_pair.hpp>

#  include <hboost/test/tools/detail/bitwise_manip.hpp>
#  include <hboost/test/tools/detail/tolerance_manip.hpp>
#  include <hboost/test/tools/detail/per_element_manip.hpp>
#  include <hboost/test/tools/detail/lexicographic_manip.hpp>
#endif

#endif // HBOOST_TEST_TOOLS_HPP_111812GER
