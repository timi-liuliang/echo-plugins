//
// detail/impl/throw_error.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_IMPL_THROW_ERROR_IPP
#define HBOOST_ASIO_DETAIL_IMPL_THROW_ERROR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/detail/throw_error.hpp>
#include <hboost/asio/detail/throw_exception.hpp>
#include <hboost/system/system_error.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

void do_throw_error(const hboost::system::error_code& err)
{
  hboost::system::system_error e(err);
  hboost::asio::detail::throw_exception(e);
}

void do_throw_error(const hboost::system::error_code& err, const char* location)
{
  hboost::system::system_error e(err, location);
  hboost::asio::detail::throw_exception(e);
}

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_DETAIL_IMPL_THROW_ERROR_IPP