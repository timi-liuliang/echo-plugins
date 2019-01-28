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
//  Description : defines the is_cstring type trait
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_IS_CSTRING_HPP
#define HBOOST_TEST_UTILS_IS_CSTRING_HPP

// Boost
#include <hboost/mpl/bool.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/decay.hpp>
#include <hboost/type_traits/remove_pointer.hpp>

//____________________________________________________________________________//

namespace hboost {
namespace unit_test {

// ************************************************************************** //
// **************                  is_cstring                  ************** //
// ************************************************************************** //

namespace ut_detail {

template<typename T>
struct is_cstring_impl : public mpl::false_ {};

template<typename T>
struct is_cstring_impl<T const*> : public is_cstring_impl<T*> {};

template<typename T>
struct is_cstring_impl<T const* const> : public is_cstring_impl<T*> {};

template<>
struct is_cstring_impl<char*> : public mpl::true_ {};

template<>
struct is_cstring_impl<wchar_t*> : public mpl::true_ {};

} // namespace ut_detail

template<typename T>
struct is_cstring : public ut_detail::is_cstring_impl<typename decay<T>::type> {};

} // namespace unit_test
} // namespace hboost

#endif // HBOOST_TEST_UTILS_IS_CSTRING_HPP
