//
// detail/throw_error.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_THROW_ERROR_HPP
#define HBOOST_ASIO_DETAIL_THROW_ERROR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/system/error_code.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

HBOOST_ASIO_DECL void do_throw_error(const hboost::system::error_code& err);

HBOOST_ASIO_DECL void do_throw_error(const hboost::system::error_code& err,
    const char* location);

inline void throw_error(const hboost::system::error_code& err)
{
  if (err)
    do_throw_error(err);
}

inline void throw_error(const hboost::system::error_code& err,
    const char* location)
{
  if (err)
    do_throw_error(err, location);
}

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/throw_error.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // HBOOST_ASIO_DETAIL_THROW_ERROR_HPP
