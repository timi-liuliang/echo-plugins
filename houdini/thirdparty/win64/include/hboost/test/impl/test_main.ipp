//  (C) Copyright Gennadiy Rozental 2001.
//  (C) Copyright Beman Dawes 1995-2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief Implements main function for Test Execution Monitor.
// ***************************************************************************

#ifndef HBOOST_TEST_TEST_MAIN_IPP_012205GER
#define HBOOST_TEST_TEST_MAIN_IPP_012205GER

// Boost.Test
#include <hboost/test/framework.hpp>
#include <hboost/test/test_tools.hpp>
#include <hboost/test/unit_test_suite.hpp>

// Boost
#include <hboost/cstdlib.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

extern int test_main( int argc, char* argv[] );    // prototype for user's test_main()

struct test_main_caller {
    test_main_caller( int argc, char** argv ) : m_argc( argc ), m_argv( argv ) {}

    void operator()() {
        int test_main_result = test_main( m_argc, m_argv );

        // translate a test_main non-success return into a test error
        HBOOST_CHECK( test_main_result == 0 || test_main_result == hboost::exit_success );
    }

private:
    // Data members
    int      m_argc;
    char**   m_argv;
};

// ************************************************************************** //
// **************                   test main                  ************** //
// ************************************************************************** //

::hboost::unit_test::test_suite*
init_unit_test_suite( int argc, char* argv[] ) {
    using namespace ::hboost::unit_test;

    framework::master_test_suite().p_name.value = "Test Program";

    framework::master_test_suite().add( HBOOST_TEST_CASE( test_main_caller( argc, argv ) ) );

    return 0;
}

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TEST_MAIN_IPP_012205GER
