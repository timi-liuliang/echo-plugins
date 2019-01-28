//
// detail/function.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_FUNCTION_HPP
#define HBOOST_ASIO_DETAIL_FUNCTION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_STD_FUNCTION)
# include <functional>
#else // defined(HBOOST_ASIO_HAS_STD_FUNCTION)
# include <hboost/function.hpp>
#endif // defined(HBOOST_ASIO_HAS_STD_FUNCTION)

namespace hboost {
namespace asio {
namespace detail {

#if defined(HBOOST_ASIO_HAS_STD_FUNCTION)
using std::function;
#else // defined(HBOOST_ASIO_HAS_STD_FUNCTION)
using hboost::function;
#endif // defined(HBOOST_ASIO_HAS_STD_FUNCTION)

} // namespace detail
} // namespace asio
} // namespace hboost

#endif // HBOOST_ASIO_DETAIL_FUNCTION_HPP
