//
// detail/impl/posix_mutex.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_IMPL_POSIX_MUTEX_IPP
#define HBOOST_ASIO_DETAIL_IMPL_POSIX_MUTEX_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_PTHREADS)

#include <hboost/asio/detail/posix_mutex.hpp>
#include <hboost/asio/detail/throw_error.hpp>
#include <hboost/asio/error.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

posix_mutex::posix_mutex()
{
  int error = ::pthread_mutex_init(&mutex_, 0);
  hboost::system::error_code ec(error,
      hboost::asio::error::get_system_category());
  hboost::asio::detail::throw_error(ec, "mutex");
}

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_HAS_PTHREADS)

#endif // HBOOST_ASIO_DETAIL_IMPL_POSIX_MUTEX_IPP
