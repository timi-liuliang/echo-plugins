//
// detail/macos_fenced_block.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_MACOS_FENCED_BLOCK_HPP
#define HBOOST_ASIO_DETAIL_MACOS_FENCED_BLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(__MACH__) && defined(__APPLE__)

#include <libkern/OSAtomic.h>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class macos_fenced_block
  : private noncopyable
{
public:
  enum half_t { half };
  enum full_t { full };

  // Constructor for a half fenced block.
  explicit macos_fenced_block(half_t)
  {
  }

  // Constructor for a full fenced block.
  explicit macos_fenced_block(full_t)
  {
    OSMemoryBarrier();
  }

  // Destructor.
  ~macos_fenced_block()
  {
    OSMemoryBarrier();
  }
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(__MACH__) && defined(__APPLE__)

#endif // HBOOST_ASIO_DETAIL_MACOS_FENCED_BLOCK_HPP
