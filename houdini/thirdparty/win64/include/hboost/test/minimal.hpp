//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief Deprecated implementation of simple minimal testing
/// @deprecated
/// To convert to Unit Test Framework simply rewrite:
/// @code
/// #include <hboost/test/minimal.hpp>
///
/// int test_main( int, char *[] )
/// {
///   ...
/// }
/// @endcode
/// as
/// @code
/// #include <hboost/test/included/unit_test.hpp>
///
/// HBOOST_AUTO_TEST_CASE(test_main)
/// {
///   ...
/// }
/// @endcode
// ***************************************************************************

#ifndef HBOOST_TEST_MINIMAL_HPP_071894GER
#define HBOOST_TEST_MINIMAL_HPP_071894GER

#define HBOOST_CHECK(exp)       \
  ( (exp)                      \
      ? static_cast<void>(0)   \
      : hboost::minimal_test::report_error(#exp,__FILE__,__LINE__, HBOOST_CURRENT_FUNCTION) )

#define HBOOST_REQUIRE(exp)     \
  ( (exp)                      \
      ? static_cast<void>(0)   \
      : hboost::minimal_test::report_critical_error(#exp,__FILE__,__LINE__,HBOOST_CURRENT_FUNCTION))

#define HBOOST_ERROR( msg_ )    \
        hboost::minimal_test::report_error( (msg_),__FILE__,__LINE__, HBOOST_CURRENT_FUNCTION, true )
#define HBOOST_FAIL( msg_ )     \
        hboost::minimal_test::report_critical_error( (msg_),__FILE__,__LINE__, HBOOST_CURRENT_FUNCTION, true )

//____________________________________________________________________________//

// Boost.Test
#include <hboost/test/detail/global_typedef.hpp>
#include <hboost/test/impl/execution_monitor.ipp>
#include <hboost/test/impl/debug.ipp>
#include <hboost/test/utils/class_properties.hpp>
#include <hboost/test/utils/basic_cstring/io.hpp>

// Boost
#include <hboost/cstdlib.hpp>            // for exit codes
#include <hboost/current_function.hpp>   // for HBOOST_CURRENT_FUNCTION

// STL
#include <iostream>                     // std::cerr, std::endl
#include <string>                       // std::string

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

int test_main( int argc, char* argv[] );  // prototype for users test_main()

namespace hboost {
namespace minimal_test {

typedef hboost::unit_test::const_string const_string;

inline unit_test::counter_t& errors_counter() { static unit_test::counter_t ec = 0; return ec; }

inline void
report_error( const char* msg, const char* file, int line, const_string func_name, bool is_msg = false )
{
    ++errors_counter();
    std::cerr << file << "(" << line << "): ";

    if( is_msg )
        std::cerr << msg;
    else
        std::cerr << "test " << msg << " failed";

    if( func_name != "(unknown)" )
        std::cerr << " in function: '" << func_name << "'";

    std::cerr << std::endl;
}

inline void
report_critical_error( const char* msg, const char* file, int line, const_string func_name, bool is_msg = false )
{
    report_error( msg, file, line, func_name, is_msg );

    throw hboost::execution_aborted();
}

class caller {
public:
    // constructor
    caller( int argc, char** argv )
    : m_argc( argc ), m_argv( argv ) {}

    // execution monitor hook implementation
    int operator()() { return test_main( m_argc, m_argv ); }

private:
    // Data members
    int         m_argc;
    char**      m_argv;
}; // monitor

} // namespace minimal_test
} // namespace hboost

//____________________________________________________________________________//

int HBOOST_TEST_CALL_DECL main( int argc, char* argv[] )
{
    using namespace hboost::minimal_test;

    try {
        ::hboost::execution_monitor ex_mon;
        int run_result = ex_mon.execute( caller( argc, argv ) );

        HBOOST_CHECK( run_result == 0 || run_result == hboost::exit_success );
    }
    catch( hboost::execution_exception const& exex ) {
        if( exex.code() != hboost::execution_exception::no_error )
            HBOOST_ERROR( (std::string( "exception \"" ) + exex.what() + "\" caught").c_str() );
        std::cerr << "\n**** Testing aborted.";
    }

    if( hboost::minimal_test::errors_counter() != 0 ) {
        std::cerr << "\n**** " << errors_counter()
                  << " error" << (errors_counter() > 1 ? "s" : "" ) << " detected\n";

        return hboost::exit_test_failure;
    }

    std::cout << "\n**** no errors detected\n";

    return hboost::exit_success;
}

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_MINIMAL_HPP_071894GER
