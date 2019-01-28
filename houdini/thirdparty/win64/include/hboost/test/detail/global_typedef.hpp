//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief some trivial global typedefs
// ***************************************************************************

#ifndef HBOOST_TEST_GLOBAL_TYPEDEF_HPP_021005GER
#define HBOOST_TEST_GLOBAL_TYPEDEF_HPP_021005GER

#include <hboost/test/utils/basic_cstring/basic_cstring.hpp>
#include <hboost/test/detail/workaround.hpp>

#define HBOOST_TEST_L( s )         ::hboost::unit_test::const_string( s, sizeof( s ) - 1 )
#define HBOOST_TEST_STRINGIZE( s ) HBOOST_TEST_L( HBOOST_STRINGIZE( s ) )
#define HBOOST_TEST_EMPTY_STRING   HBOOST_TEST_L( "" )

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

typedef unsigned long   counter_t;

//____________________________________________________________________________//

enum report_level  { INV_REPORT_LEVEL, CONFIRMATION_REPORT, SHORT_REPORT, DETAILED_REPORT, NO_REPORT };

//____________________________________________________________________________//

enum output_format { OF_INVALID,
                     OF_CLF, ///< compiler log format
                     OF_XML, ///< XML format for report and log,
                     OF_DOT  ///< dot format for output content
};

//____________________________________________________________________________//

enum test_unit_type { TUT_CASE = 0x01, TUT_SUITE = 0x10, TUT_ANY = 0x11 };

//____________________________________________________________________________//

enum assertion_result { AR_FAILED, AR_PASSED, AR_TRIGGERED };

//____________________________________________________________________________//

typedef unsigned long   test_unit_id;

const test_unit_id INV_TEST_UNIT_ID  = 0xFFFFFFFF;
const test_unit_id MAX_TEST_CASE_ID  = 0xFFFFFFFE;
const test_unit_id MIN_TEST_CASE_ID  = 0x00010000;
const test_unit_id MAX_TEST_SUITE_ID = 0x0000FF00;
const test_unit_id MIN_TEST_SUITE_ID = 0x00000001;

//____________________________________________________________________________//

namespace ut_detail {

inline test_unit_type
test_id_2_unit_type( test_unit_id id )
{
    return (id & 0xFFFF0000) != 0 ? TUT_CASE : TUT_SUITE;
}

//____________________________________________________________________________//

} // namespace ut_detail

// helper templates to prevent ODR violations
template<class T>
struct static_constant {
    static T value;
};

template<class T>
T static_constant<T>::value;

//____________________________________________________________________________//

} // namespace unit_test
} // namespace hboost

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_GLOBAL_TYPEDEF_HPP_021005GER
