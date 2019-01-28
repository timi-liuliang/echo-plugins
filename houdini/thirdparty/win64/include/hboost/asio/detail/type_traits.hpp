//
// detail/type_traits.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_TYPE_TRAITS_HPP
#define HBOOST_ASIO_DETAIL_TYPE_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_STD_TYPE_TRAITS)
# include <type_traits>
#else // defined(HBOOST_ASIO_HAS_TYPE_TRAITS)
# include <hboost/type_traits/add_const.hpp>
# include <hboost/type_traits/is_const.hpp>
# include <hboost/type_traits/is_convertible.hpp>
# include <hboost/type_traits/is_function.hpp>
# include <hboost/type_traits/is_same.hpp>
# include <hboost/type_traits/remove_pointer.hpp>
# include <hboost/type_traits/remove_reference.hpp>
# include <hboost/utility/enable_if.hpp>
#endif // defined(HBOOST_ASIO_HAS_TYPE_TRAITS)

namespace hboost {
namespace asio {

#if defined(HBOOST_ASIO_HAS_STD_TYPE_TRAITS)
using std::add_const;
using std::enable_if;
using std::is_const;
using std::is_convertible;
using std::is_function;
using std::is_same;
using std::remove_pointer;
using std::remove_reference;
#else // defined(HBOOST_ASIO_HAS_STD_TYPE_TRAITS)
using hboost::add_const;
template <bool Condition, typename Type = void>
struct enable_if : hboost::enable_if_c<Condition, Type> {};
using hboost::is_const;
using hboost::is_convertible;
using hboost::is_function;
using hboost::is_same;
using hboost::remove_pointer;
using hboost::remove_reference;
#endif // defined(HBOOST_ASIO_HAS_STD_TYPE_TRAITS)

} // namespace asio
} // namespace hboost

#endif // HBOOST_ASIO_DETAIL_TYPE_TRAITS_HPP
