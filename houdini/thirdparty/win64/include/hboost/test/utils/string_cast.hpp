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
//  Description : trivial utility to cast to/from strings
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_STRING_CAST_HPP
#define HBOOST_TEST_UTILS_STRING_CAST_HPP

// Boost.Test
#include <hboost/test/utils/basic_cstring/basic_cstring.hpp>

// STL
#include <sstream>

#include <hboost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {
namespace utils {

// ************************************************************************** //
// **************                  string_cast                 ************** //
// ************************************************************************** //

template<typename T>
inline std::string
string_cast( T const& t )
{
    std::ostringstream buff;
    buff << t;
    return buff.str();
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                  string_as                 ************** //
// ************************************************************************** //

template<typename T>
inline bool
string_as( const_string str, T& res )
{
    std::istringstream buff( std::string( str.begin(), str.end() ) );
    buff >> res;

    return !buff.fail() && buff.eof();
}

//____________________________________________________________________________//

} // namespace utils
} // namespace unit_test
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_UTILS_STRING_CAST_HPP