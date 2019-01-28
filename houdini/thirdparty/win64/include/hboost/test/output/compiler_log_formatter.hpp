//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : contains compiler like Log formatter definition
// ***************************************************************************

#ifndef HBOOST_TEST_COMPILER_LOG_FORMATTER_HPP_020105GER
#define HBOOST_TEST_COMPILER_LOG_FORMATTER_HPP_020105GER

// Boost.Test
#include <hboost/test/detail/global_typedef.hpp>
#include <hboost/test/unit_test_log_formatter.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {
namespace output {

// ************************************************************************** //
// **************             compiler_log_formatter           ************** //
// ************************************************************************** //

class HBOOST_TEST_DECL compiler_log_formatter : public unit_test_log_formatter {
public:
    compiler_log_formatter() : m_color_output( false ) {}

    // Formatter interface
    void    log_start( std::ostream&, counter_t test_cases_amount );
    void    log_finish( std::ostream& );
    void    log_build_info( std::ostream& );

    void    test_unit_start( std::ostream&, test_unit const& tu );
    void    test_unit_finish( std::ostream&, test_unit const& tu, unsigned long elapsed );
    void    test_unit_skipped( std::ostream&, test_unit const& tu, const_string reason );

    void    log_exception_start( std::ostream&, log_checkpoint_data const&, execution_exception const& ex );
    void    log_exception_finish( std::ostream& );

    void    log_entry_start( std::ostream&, log_entry_data const&, log_entry_types let );
    void    log_entry_value( std::ostream&, const_string value );
    void    log_entry_value( std::ostream&, lazy_ostream const& value );
    void    log_entry_finish( std::ostream& );

    void    entry_context_start( std::ostream&, log_level );
    void    log_entry_context( std::ostream&, const_string );
    void    entry_context_finish( std::ostream& );

protected:
    virtual void    print_prefix( std::ostream&, const_string file, std::size_t line );

    // Data members
    bool    m_color_output;
};

} // namespace output
} // namespace unit_test
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_COMPILER_LOG_FORMATTER_HPP_020105GER