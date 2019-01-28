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
//  Description : runtime parameters forward declaration
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_RUNTIME_FWD_HPP
#define HBOOST_TEST_UTILS_RUNTIME_FWD_HPP

// Boost.Test
#include <hboost/test/detail/config.hpp>
#include <hboost/test/utils/basic_cstring/basic_cstring.hpp>
#include <hboost/test/utils/basic_cstring/io.hpp> // operator<<(hboost::runtime::cstring)

// Boost
#include <hboost/shared_ptr.hpp>

// STL
#include <map>

namespace hboost {
namespace runtime {

typedef unit_test::const_string     cstring;

class argument;
typedef shared_ptr<argument> argument_ptr;

template<typename T> class typed_argument;

class basic_param;
typedef shared_ptr<basic_param> basic_param_ptr;

} // namespace runtime
} // namespace hboost

#endif // HBOOST_TEST_UTILS_RUNTIME_FWD_HPP
