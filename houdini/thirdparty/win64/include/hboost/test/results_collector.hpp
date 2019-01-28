//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file results_collector.hpp @brief defines testing result collector components
///
/// Defines class results_collector_t that is responsible for
/// gathering test results and class test_results for presenting this information to end-user
// ***************************************************************************

#ifndef HBOOST_TEST_RESULTS_COLLECTOR_HPP_071894GER
#define HBOOST_TEST_RESULTS_COLLECTOR_HPP_071894GER

// Boost.Test
#include <hboost/test/tree/observer.hpp>

#include <hboost/test/detail/global_typedef.hpp>
#include <hboost/test/detail/fwd_decl.hpp>

#include <hboost/test/utils/trivial_singleton.hpp>
#include <hboost/test/utils/class_properties.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

namespace {

// ************************************************************************** //
/// First failed assertion debugger hook
///
/// This function is a placeholder where user can set a breakpoint in debugger to catch the
/// very first assertion failure in each test case
// ************************************************************************** //
inline void first_failed_assertion() {}
}

// ************************************************************************** //
/// @brief Collection of attributes constituting test unit results
///
/// This class is a collection of attributes describing testing results. The atributes presented as public properties on
/// an instance of the class. In addition summary conclusion methods are presented to generate simple answer to pass/fail question
// ************************************************************************** //

class HBOOST_TEST_DECL test_results {
public:
    test_results();

    /// Type representing counter like public property
    typedef HBOOST_READONLY_PROPERTY( counter_t, (results_collector_t)(test_results)(results_collect_helper) ) counter_prop;
    /// Type representing boolean like public property
    typedef HBOOST_READONLY_PROPERTY( bool,      (results_collector_t)(test_results)(results_collect_helper) ) bool_prop;

    /// @name Public properties
    counter_prop    p_assertions_passed;
    counter_prop    p_assertions_failed;
    counter_prop    p_warnings_failed;
    counter_prop    p_expected_failures;
    counter_prop    p_test_cases_passed;
    counter_prop    p_test_cases_warned;
    counter_prop    p_test_cases_failed;
    counter_prop    p_test_cases_skipped;
    counter_prop    p_test_cases_aborted;
    bool_prop       p_aborted;
    bool_prop       p_skipped;
    /// @}

    /// @name Summary conclusion

    /// Returns true if test unit passed
    bool            passed() const;
    /// Produces result code for the test unit execution

    /// This methhod return one of the result codes defined in hboost/cstdlib.hpp
    /// @returns hboost::exit_success on success, hboost::exit_exception_failure in case test unit was aborted for any reason
    ///          (incuding uncausght exception) and hboost::exit_test_failure otherwise
    int             result_code() const;
    /// @}

    // collection helper
    void            operator+=( test_results const& );

    void            clear();
};

// ************************************************************************** //
/// This class implements test observer interface to collect the result of test unit execution
// ************************************************************************** //

class HBOOST_TEST_DECL results_collector_t : public test_observer, public singleton<results_collector_t> {
public:

    virtual void        test_start( counter_t test_cases_amount );

    virtual void        test_unit_start( test_unit const& );
    virtual void        test_unit_finish( test_unit const&, unsigned long );
    virtual void        test_unit_skipped( test_unit const&, const_string );
    virtual void        test_unit_aborted( test_unit const& );

    virtual void        assertion_result( unit_test::assertion_result );
    virtual void        exception_caught( execution_exception const& );

    virtual int         priority() { return 2; }

    /// Results access per test unit

    /// @param[in] tu_id id of a test unit
    test_results const& results( test_unit_id tu_id ) const;

private:
    HBOOST_TEST_SINGLETON_CONS( results_collector_t )
};

HBOOST_TEST_SINGLETON_INST( results_collector )

} // namespace unit_test
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_RESULTS_COLLECTOR_HPP_071894GER
