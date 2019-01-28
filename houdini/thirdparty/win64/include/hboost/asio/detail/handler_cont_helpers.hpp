//
// detail/handler_cont_helpers.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_HANDLER_CONT_HELPERS_HPP
#define HBOOST_ASIO_DETAIL_HANDLER_CONT_HELPERS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/detail/addressof.hpp>
#include <hboost/asio/handler_continuation_hook.hpp>

#include <hboost/asio/detail/push_options.hpp>

// Calls to asio_handler_is_continuation must be made from a namespace that
// does not contain overloads of this function. This namespace is defined here
// for that purpose.
namespace hboost_asio_handler_cont_helpers {

template <typename Context>
inline bool is_continuation(Context& context)
{
#if !defined(HBOOST_ASIO_HAS_HANDLER_HOOKS)
  return false;
#else
  using hboost::asio::asio_handler_is_continuation;
  return asio_handler_is_continuation(
      hboost::asio::detail::addressof(context));
#endif
}

} // namespace hboost_asio_handler_cont_helpers

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_DETAIL_HANDLER_CONT_HELPERS_HPP
