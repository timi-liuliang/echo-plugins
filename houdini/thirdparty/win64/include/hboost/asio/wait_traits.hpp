//
// wait_traits.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_WAIT_TRAITS_HPP
#define HBOOST_ASIO_WAIT_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {

/// Wait traits suitable for use with the basic_waitable_timer class template.
template <typename Clock>
struct wait_traits
{
  /// Convert a clock duration into a duration used for waiting.
  /** 
   * @returns @c d.
   */
  static typename Clock::duration to_wait_duration(
      const typename Clock::duration& d)
  {
    return d;
  }
};

} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_WAIT_TRAITS_HPP
