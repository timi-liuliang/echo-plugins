//
// detail/fd_set_adapter.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_FD_SET_ADAPTER_HPP
#define HBOOST_ASIO_DETAIL_FD_SET_ADAPTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if !defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#include <hboost/asio/detail/posix_fd_set_adapter.hpp>
#include <hboost/asio/detail/win_fd_set_adapter.hpp>

namespace hboost {
namespace asio {
namespace detail {

#if defined(HBOOST_ASIO_WINDOWS) || defined(__CYGWIN__)
typedef win_fd_set_adapter fd_set_adapter;
#else
typedef posix_fd_set_adapter fd_set_adapter;
#endif

} // namespace detail
} // namespace asio
} // namespace hboost

#endif // !defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#endif // HBOOST_ASIO_DETAIL_FD_SET_ADAPTER_HPP
