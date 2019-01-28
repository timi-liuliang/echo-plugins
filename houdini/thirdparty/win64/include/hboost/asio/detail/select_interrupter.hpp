//
// detail/select_interrupter.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_SELECT_INTERRUPTER_HPP
#define HBOOST_ASIO_DETAIL_SELECT_INTERRUPTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if !defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
# include <hboost/asio/detail/socket_select_interrupter.hpp>
#elif defined(HBOOST_ASIO_HAS_EVENTFD)
# include <hboost/asio/detail/eventfd_select_interrupter.hpp>
#else
# include <hboost/asio/detail/pipe_select_interrupter.hpp>
#endif

namespace hboost {
namespace asio {
namespace detail {

#if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
typedef socket_select_interrupter select_interrupter;
#elif defined(HBOOST_ASIO_HAS_EVENTFD)
typedef eventfd_select_interrupter select_interrupter;
#else
typedef pipe_select_interrupter select_interrupter;
#endif

} // namespace detail
} // namespace asio
} // namespace hboost

#endif // !defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#endif // HBOOST_ASIO_DETAIL_SELECT_INTERRUPTER_HPP
