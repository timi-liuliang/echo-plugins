//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief Deprecated implementation of Test Execution Monitor
///
/// To convert to Unit Test Framework simply rewrite:
/// @code
/// #include <hboost/test/test_exec_monitor.hpp>
///
/// int test_main( int, char *[] )
/// {
///   ...
/// }
/// @endcode
/// as
/// @code
/// #include <hboost/test/unit_test.hpp>
///
/// HBOOST_AUTO_TEST_CASE(test_main)
/// {
///   ...
/// }
/// @endcode
/// and link with hboost_unit_test_framework library *instead of* hboost_test_exec_monitor
// ***************************************************************************

#ifndef HBOOST_TEST_EXEC_MONITOR_HPP_071894GER
#define HBOOST_TEST_EXEC_MONITOR_HPP_071894GER

// Boost.Test
#include <hboost/test/test_tools.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 Auto Linking                 ************** //
// ************************************************************************** //

// Automatically link to the correct build variant where possible.
#if !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_TEST_NO_LIB) && \
    !defined(HBOOST_TEST_SOURCE) && !defined(HBOOST_TEST_INCLUDED)

#  define HBOOST_LIB_NAME hboost_test_exec_monitor
#  include <hboost/config/auto_link.hpp>

#endif  // auto-linking disabled

#endif // HBOOST_TEST_EXEC_MONITOR_HPP_071894GER
