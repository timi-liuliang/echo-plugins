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
//  Description : main function implementation for Unit Test Framework
// ***************************************************************************

#ifndef HBOOST_TEST_UNIT_TEST_MAIN_IPP_012205GER
#define HBOOST_TEST_UNIT_TEST_MAIN_IPP_012205GER

// Boost.Test
#include <hboost/test/framework.hpp>
#include <hboost/test/results_collector.hpp>
#include <hboost/test/results_reporter.hpp>

#include <hboost/test/tree/visitor.hpp>
#include <hboost/test/tree/test_unit.hpp>
#include <hboost/test/tree/traverse.hpp>

#include <hboost/test/unit_test_parameters.hpp>

#include <hboost/test/utils/foreach.hpp>
#include <hboost/test/utils/basic_cstring/io.hpp>

// Boost
#include <hboost/cstdlib.hpp>

// STL
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <set>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

namespace ut_detail {

// ************************************************************************** //
// **************             hrf_content_reporter             ************** //
// ************************************************************************** //

struct hrf_content_reporter : test_tree_visitor {
    explicit        hrf_content_reporter( std::ostream& os ) : m_os( os ), m_indent( -4 ) {} // skip master test suite

private:
    void            report_test_unit( test_unit const& tu )
    {
        m_os << std::setw( m_indent ) << "" << tu.p_name;
        m_os << (tu.p_default_status == test_unit::RS_ENABLED ? "*" : " ");
        //m_os << '[' << tu.p_sibling_rank << ']';
        if( !tu.p_description->empty() )
            m_os << ": " << tu.p_description;

        m_os << "\n";
    }
    virtual void    visit( test_case const& tc ) { report_test_unit( tc ); }
    virtual bool    test_suite_start( test_suite const& ts )
    {
        if( m_indent >= 0 )
            report_test_unit( ts );
        m_indent += 4;
        return true;
    }
    virtual void    test_suite_finish( test_suite const& )
    {
        m_indent -= 4;
    }

    // Data members
    std::ostream&   m_os;
    int             m_indent;
};

// ************************************************************************** //
// **************             dot_content_reporter             ************** //
// ************************************************************************** //

struct dot_content_reporter : test_tree_visitor {
    explicit        dot_content_reporter( std::ostream& os ) : m_os( os ) {}

private:
    void            report_test_unit( test_unit const& tu )
    {
        bool master_ts = tu.p_parent_id == INV_TEST_UNIT_ID;

        m_os << "tu" << tu.p_id;

        m_os << (master_ts ? "[shape=ellipse,peripheries=2" : "[shape=Mrecord" );

        m_os << ",fontname=Helvetica";

        m_os << (tu.p_default_status == test_unit::RS_ENABLED ? ",color=green" : ",color=yellow");

        if( master_ts )
            m_os << ",label=\"" << tu.p_name << "\"];\n";
        else {
            m_os << ",label=\"" << tu.p_name << "|" << tu.p_file_name << "(" << tu.p_line_num << ")";
            if( tu.p_timeout > 0  )
                m_os << "|timeout=" << tu.p_timeout;
            if( tu.p_expected_failures != 0  )
                m_os << "|expected failures=" << tu.p_expected_failures;
            if( !tu.p_labels->empty() ) {
                m_os << "|labels:";

                HBOOST_TEST_FOREACH( std::string const&, l, tu.p_labels.get() )
                    m_os << " @" << l;
            }
            m_os << "\"];\n";
        }

        if( !master_ts )
            m_os << "tu" << tu.p_parent_id << " -> " << "tu" << tu.p_id << ";\n";

        HBOOST_TEST_FOREACH( test_unit_id, dep_id, tu.p_dependencies.get() ) {
            test_unit const& dep = framework::get( dep_id, TUT_ANY );

            m_os << "tu" << tu.p_id << " -> " << "tu" << dep.p_id << "[color=red,style=dotted,constraint=false];\n";
        }

    }
    virtual void    visit( test_case const& tc )
    { 
        report_test_unit( tc );
    }
    virtual bool    test_suite_start( test_suite const& ts )
    {
        if( ts.p_parent_id == INV_TEST_UNIT_ID )
            m_os << "digraph G {rankdir=LR;\n";

        report_test_unit( ts );

        m_os << "{\n";

        return true;
    }
    virtual void    test_suite_finish( test_suite const& ts )
    {
        m_os << "}\n";
        if( ts.p_parent_id == INV_TEST_UNIT_ID )
            m_os << "}\n";
    }

    std::ostream&   m_os;
};

// ************************************************************************** //
// **************               labels_collector               ************** //
// ************************************************************************** //

struct labels_collector : test_tree_visitor {
    std::set<std::string> const& labels() const { return m_labels; }

private:
    virtual bool            visit( test_unit const& tu ) 
    {
        m_labels.insert( tu.p_labels->begin(), tu.p_labels->end() );
        return true;
    }

    // Data members
    std::set<std::string>   m_labels;
};

} // namespace ut_detail

// ************************************************************************** //
// **************                  unit_test_main              ************** //
// ************************************************************************** //

int HBOOST_TEST_DECL
unit_test_main( init_unit_test_func init_func, int argc, char* argv[] )
{
    int result_code = 0;

    HBOOST_TEST_I_TRY {
        framework::init( init_func, argc, argv );

        if( runtime_config::get<bool>( runtime_config::WAIT_FOR_DEBUGGER ) ) {
            results_reporter::get_stream() << "Press any key to continue..." << std::endl;

            std::getchar();
            results_reporter::get_stream() << "Continuing..." << std::endl;
        }

        framework::finalize_setup_phase();

        output_format list_cont = runtime_config::get<output_format>( runtime_config::LIST_CONTENT );
        if( list_cont != unit_test::OF_INVALID ) {
            if( list_cont == unit_test::OF_DOT ) {
                ut_detail::dot_content_reporter reporter( results_reporter::get_stream() );

                traverse_test_tree( framework::master_test_suite().p_id, reporter, true );
            }
            else {
                ut_detail::hrf_content_reporter reporter( results_reporter::get_stream() );

                traverse_test_tree( framework::master_test_suite().p_id, reporter, true );
            }

            return hboost::exit_success;
        }

        if( runtime_config::get<bool>( runtime_config::LIST_LABELS ) ) {
            ut_detail::labels_collector collector;

            traverse_test_tree( framework::master_test_suite().p_id, collector, true );

            results_reporter::get_stream() << "Available labels:\n  ";
            std::copy( collector.labels().begin(), collector.labels().end(), 
                       std::ostream_iterator<std::string>( results_reporter::get_stream(), "\n  " ) );
            results_reporter::get_stream() << "\n";

            return hboost::exit_success;
        }

        framework::run();

        results_reporter::make_report();

        result_code = !runtime_config::get<bool>( runtime_config::RESULT_CODE )
                        ? hboost::exit_success
                        : results_collector.results( framework::master_test_suite().p_id ).result_code();
    }
    HBOOST_TEST_I_CATCH( framework::nothing_to_test, ex ) {
        result_code = ex.m_result_code;
    }
    HBOOST_TEST_I_CATCH( framework::internal_error, ex ) {
        results_reporter::get_stream() << "Boost.Test framework internal error: " << ex.what() << std::endl;

        result_code = hboost::exit_exception_failure;
    }
    HBOOST_TEST_I_CATCH( framework::setup_error, ex ) {
        results_reporter::get_stream() << "Test setup error: " << ex.what() << std::endl;

        result_code = hboost::exit_exception_failure;
    }
    HBOOST_TEST_I_CATCHALL() {
        results_reporter::get_stream() << "Boost.Test framework internal error: unknown reason" << std::endl;

        result_code = hboost::exit_exception_failure;
    }

    framework::shutdown();

    return result_code;
}

} // namespace unit_test
} // namespace hboost

#if !defined(HBOOST_TEST_DYN_LINK) && !defined(HBOOST_TEST_NO_MAIN)

// ************************************************************************** //
// **************        main function for tests using lib     ************** //
// ************************************************************************** //

int HBOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    // prototype for user's unit test init function
#ifdef HBOOST_TEST_ALTERNATIVE_INIT_API
    extern bool init_unit_test();

    hboost::unit_test::init_unit_test_func init_func = &init_unit_test;
#else
    extern ::hboost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] );

    hboost::unit_test::init_unit_test_func init_func = &init_unit_test_suite;
#endif

    return ::hboost::unit_test::unit_test_main( init_func, argc, argv );
}

#endif // !HBOOST_TEST_DYN_LINK && !HBOOST_TEST_NO_MAIN

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_UNIT_TEST_MAIN_IPP_012205GER
