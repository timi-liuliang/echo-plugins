//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief Provides access to various Unit Test Framework runtime parameters
///
/// Primarily for use by the framework itself
// ***************************************************************************

#ifndef HBOOST_TEST_UNIT_TEST_PARAMETERS_HPP_071894GER
#define HBOOST_TEST_UNIT_TEST_PARAMETERS_HPP_071894GER

// Boost.Test
#include <hboost/test/detail/global_typedef.hpp>
#include <hboost/test/utils/runtime/argument.hpp>

// STL
#include <iostream>
#include <fstream>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {
namespace runtime_config {

// ************************************************************************** //
// **************                 runtime_config               ************** //
// ************************************************************************** //

// UTF parameters 
HBOOST_TEST_DECL extern std::string AUTO_START_DBG;
HBOOST_TEST_DECL extern std::string BREAK_EXEC_PATH;
HBOOST_TEST_DECL extern std::string BUILD_INFO;
HBOOST_TEST_DECL extern std::string CATCH_SYS_ERRORS;
HBOOST_TEST_DECL extern std::string COLOR_OUTPUT;
HBOOST_TEST_DECL extern std::string DETECT_FP_EXCEPT;
HBOOST_TEST_DECL extern std::string DETECT_MEM_LEAKS;
HBOOST_TEST_DECL extern std::string LIST_CONTENT;
HBOOST_TEST_DECL extern std::string LIST_LABELS;
HBOOST_TEST_DECL extern std::string LOG_FORMAT;
HBOOST_TEST_DECL extern std::string LOG_LEVEL;
HBOOST_TEST_DECL extern std::string LOG_SINK;
HBOOST_TEST_DECL extern std::string OUTPUT_FORMAT;
HBOOST_TEST_DECL extern std::string RANDOM_SEED;
HBOOST_TEST_DECL extern std::string REPORT_FORMAT;
HBOOST_TEST_DECL extern std::string REPORT_LEVEL;
HBOOST_TEST_DECL extern std::string REPORT_MEM_LEAKS;
HBOOST_TEST_DECL extern std::string REPORT_SINK;
HBOOST_TEST_DECL extern std::string RESULT_CODE;
HBOOST_TEST_DECL extern std::string RUN_FILTERS;
HBOOST_TEST_DECL extern std::string SAVE_TEST_PATTERN;
HBOOST_TEST_DECL extern std::string SHOW_PROGRESS;
HBOOST_TEST_DECL extern std::string USE_ALT_STACK;
HBOOST_TEST_DECL extern std::string WAIT_FOR_DEBUGGER;

HBOOST_TEST_DECL void init( int& argc, char** argv );

// ************************************************************************** //
// **************              runtime_param::get              ************** //
// ************************************************************************** //

/// Access to arguments
HBOOST_TEST_DECL runtime::arguments_store const& argument_store();

template<typename T>
inline T const&
get( runtime::cstring parameter_name )
{
    return argument_store().get<T>( parameter_name );
}

/// For public access
HBOOST_TEST_DECL bool save_pattern();

// ************************************************************************** //
// **************                  stream_holder               ************** //
// ************************************************************************** //

class stream_holder {
public:
    // Constructor
    explicit        stream_holder( std::ostream& default_stream )
    : m_stream( &default_stream )
    {
    }

    void            setup( runtime::cstring param_name )
    {
        if( !runtime_config::argument_store().has( param_name ) )
            return;

        std::string const& file_name = runtime_config::get<std::string>( param_name );

        if( file_name == "stderr" )
            m_stream = &std::cerr;
        else if( file_name == "stdout" )
            m_stream = &std::cout;
        else {
            m_file.open( file_name.c_str() );
            m_stream = &m_file;
        }
    }

    // Access methods
    std::ostream&   ref() const { return *m_stream; }  

private:
    // Data members
    std::ofstream   m_file;
    std::ostream*   m_stream;  
};

} // namespace runtime_config
} // namespace unit_test
} // namespace hboost

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_UNIT_TEST_PARAMETERS_HPP_071894GER
