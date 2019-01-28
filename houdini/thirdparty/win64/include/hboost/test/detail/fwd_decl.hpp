//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief contains forward eclarations for Boost.Test data types
// ***************************************************************************

#ifndef HBOOST_TEST_FWD_DECL_HPP_011605GER
#define HBOOST_TEST_FWD_DECL_HPP_011605GER

namespace hboost {

class  execution_monitor;
class  execution_exception;

namespace unit_test {

class  test_unit;
class  test_case;
class  test_suite;
class  master_test_suite_t;

class  test_tree_visitor;
class  test_observer;

// singletons
class  unit_test_monitor_t;
class  unit_test_log_t;

class  unit_test_log_formatter;
struct log_entry_data;
struct log_checkpoint_data;

class lazy_ostream;

} // namespace unit_test

} // namespace hboost

#endif // HBOOST_TEST_FWD_DECL_HPP_011605GER

