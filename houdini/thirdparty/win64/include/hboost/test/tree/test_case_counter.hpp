//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 74640 $
//
//  Description : defines test_case_counter
// ***************************************************************************

#ifndef HBOOST_TEST_TREE_TEST_CASE_COUNTER_HPP_100211GER
#define HBOOST_TEST_TREE_TEST_CASE_COUNTER_HPP_100211GER

// Boost.Test
#include <hboost/test/detail/config.hpp>
#include <hboost/test/utils/class_properties.hpp>

#include <hboost/test/tree/test_unit.hpp>
#include <hboost/test/tree/visitor.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

// ************************************************************************** //
// **************                test_case_counter             ************** //
// ************************************************************************** //

class test_case_counter : public test_tree_visitor {
public:
    // Constructor
    test_case_counter() : p_count( 0 ) {}

    HBOOST_READONLY_PROPERTY( counter_t, (test_case_counter)) p_count;
private:
    // test tree visitor interface
    virtual void    visit( test_case const& tc )                { if( tc.is_enabled() ) ++p_count.value; }
    virtual bool    test_suite_start( test_suite const& ts )    { return ts.is_enabled(); }
};

} // namespace unit_test
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_TREE_TEST_CASE_COUNTER_HPP_100211GER

