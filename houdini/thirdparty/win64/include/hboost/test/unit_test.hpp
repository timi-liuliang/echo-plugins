//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief Entry point into the Unit Test Framework
///
/// This header should be the only header necessary to include to start using the framework
// ***************************************************************************

#ifndef HBOOST_TEST_UNIT_TEST_HPP_071894GER
#define HBOOST_TEST_UNIT_TEST_HPP_071894GER

// Boost.Test
#include <hboost/test/test_tools.hpp>
#include <hboost/test/unit_test_suite.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 Auto Linking                 ************** //
// ************************************************************************** //

#if !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_TEST_NO_LIB) && \
    !defined(HBOOST_TEST_SOURCE) && !defined(HBOOST_TEST_INCLUDED)
#  define HBOOST_LIB_NAME hboost_unit_test_framework

#  if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_TEST_DYN_LINK)
#    define HBOOST_DYN_LINK
#  endif

#  include <hboost/config/auto_link.hpp>

#endif  // auto-linking disabled

// ************************************************************************** //
// **************                  unit_test_main              ************** //
// ************************************************************************** //

namespace hboost { namespace unit_test {

int HBOOST_TEST_DECL unit_test_main( init_unit_test_func init_func, int argc, char* argv[] );

}

// !! ?? to remove
namespace unit_test_framework=unit_test;

}

#if defined(HBOOST_TEST_DYN_LINK) && defined(HBOOST_TEST_MAIN) && !defined(HBOOST_TEST_NO_MAIN)

// ************************************************************************** //
// **************        main function for tests using dll     ************** //
// ************************************************************************** //

int HBOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    return ::hboost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

//____________________________________________________________________________//

#endif // HBOOST_TEST_DYN_LINK && HBOOST_TEST_MAIN && !HBOOST_TEST_NO_MAIN

#endif // HBOOST_TEST_UNIT_TEST_HPP_071894GER
