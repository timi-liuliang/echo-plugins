//
// detail/throw_exception.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_THROW_EXCEPTION_HPP
#define HBOOST_ASIO_DETAIL_THROW_EXCEPTION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_HBOOST_THROW_EXCEPTION)
# include <hboost/throw_exception.hpp>
#endif // defined(HBOOST_ASIO_HBOOST_THROW_EXCEPTION)

namespace hboost {
namespace asio {
namespace detail {

#if defined(HBOOST_ASIO_HAS_HBOOST_THROW_EXCEPTION)
using hboost::throw_exception;
#else // defined(HBOOST_ASIO_HAS_HBOOST_THROW_EXCEPTION)

// Declare the throw_exception function for all targets.
template <typename Exception>
void throw_exception(const Exception& e);

// Only define the throw_exception function when exceptions are enabled.
// Otherwise, it is up to the application to provide a definition of this
// function.
# if !defined(HBOOST_ASIO_NO_EXCEPTIONS)
template <typename Exception>
void throw_exception(const Exception& e)
{
  throw e;
}
# endif // !defined(HBOOST_ASIO_NO_EXCEPTIONS)

#endif // defined(HBOOST_ASIO_HAS_HBOOST_THROW_EXCEPTION)

} // namespace detail
} // namespace asio
} // namespace hboost

#endif // HBOOST_ASIO_DETAIL_THROW_EXCEPTION_HPP
