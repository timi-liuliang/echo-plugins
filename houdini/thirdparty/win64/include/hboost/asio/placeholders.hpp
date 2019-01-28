//
// placeholders.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_PLACEHOLDERS_HPP
#define HBOOST_ASIO_PLACEHOLDERS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_HBOOST_BIND)
# include <hboost/bind/arg.hpp>
#endif // defined(HBOOST_ASIO_HAS_HBOOST_BIND)

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace placeholders {

#if defined(GENERATING_DOCUMENTATION)

/// An argument placeholder, for use with hboost::bind(), that corresponds to
/// the error argument of a handler for any of the asynchronous functions.
unspecified error;

/// An argument placeholder, for use with hboost::bind(), that corresponds to
/// the bytes_transferred argument of a handler for asynchronous functions such
/// as hboost::asio::basic_stream_socket::async_write_some or
/// hboost::asio::async_write.
unspecified bytes_transferred;

/// An argument placeholder, for use with hboost::bind(), that corresponds to
/// the iterator argument of a handler for asynchronous functions such as
/// hboost::asio::basic_resolver::async_resolve.
unspecified iterator;

/// An argument placeholder, for use with hboost::bind(), that corresponds to
/// the signal_number argument of a handler for asynchronous functions such as
/// hboost::asio::signal_set::async_wait.
unspecified signal_number;

#elif defined(HBOOST_ASIO_HAS_HBOOST_BIND)
# if defined(__BORLANDC__) || defined(__GNUC__)

inline hboost::arg<1> error()
{
  return hboost::arg<1>();
}

inline hboost::arg<2> bytes_transferred()
{
  return hboost::arg<2>();
}

inline hboost::arg<2> iterator()
{
  return hboost::arg<2>();
}

inline hboost::arg<2> signal_number()
{
  return hboost::arg<2>();
}

# else

namespace detail
{
  template <int Number>
  struct placeholder
  {
    static hboost::arg<Number>& get()
    {
      static hboost::arg<Number> result;
      return result;
    }
  };
}

#  if defined(HBOOST_ASIO_MSVC) && (HBOOST_ASIO_MSVC < 1400)

static hboost::arg<1>& error
  = hboost::asio::placeholders::detail::placeholder<1>::get();
static hboost::arg<2>& bytes_transferred
  = hboost::asio::placeholders::detail::placeholder<2>::get();
static hboost::arg<2>& iterator
  = hboost::asio::placeholders::detail::placeholder<2>::get();
static hboost::arg<2>& signal_number
  = hboost::asio::placeholders::detail::placeholder<2>::get();

#  else

namespace
{
  hboost::arg<1>& error
    = hboost::asio::placeholders::detail::placeholder<1>::get();
  hboost::arg<2>& bytes_transferred
    = hboost::asio::placeholders::detail::placeholder<2>::get();
  hboost::arg<2>& iterator
    = hboost::asio::placeholders::detail::placeholder<2>::get();
  hboost::arg<2>& signal_number
    = hboost::asio::placeholders::detail::placeholder<2>::get();
} // namespace

#  endif
# endif
#endif

} // namespace placeholders
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_PLACEHOLDERS_HPP
