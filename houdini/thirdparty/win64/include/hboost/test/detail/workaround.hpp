//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief contains mics. workarounds
// ***************************************************************************

#ifndef HBOOST_TEST_WORKAROUND_HPP_021005GER
#define HBOOST_TEST_WORKAROUND_HPP_021005GER

// Boost
#include <hboost/config.hpp> // compilers workarounds and std::ptrdiff_t

// STL
#include <iterator>     // for std::distance

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {
namespace ut_detail {

#ifdef HBOOST_NO_STD_DISTANCE
template <class T>
std::ptrdiff_t distance( T const& x_, T const& y_ )
{
    std::ptrdiff_t res = 0;

    std::distance( x_, y_, res );

    return res;
}

//____________________________________________________________________________//

#else
using std::distance;
#endif

template <class T> inline void ignore_unused_variable_warning(const T&) {}

} // namespace ut_detail
} // namespace unit_test
} // namespace hboost

//____________________________________________________________________________//

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_WORKAROUND_HPP_021005GER
