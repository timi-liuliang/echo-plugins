//
// detail/solaris_fenced_block.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_SOLARIS_FENCED_BLOCK_HPP
#define HBOOST_ASIO_DETAIL_SOLARIS_FENCED_BLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(__sun)

#include <atomic.h>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class solaris_fenced_block
  : private noncopyable
{
public:
  enum half_t { half };
  enum full_t { full };

  // Constructor for a half fenced block.
  explicit solaris_fenced_block(half_t)
  {
  }

  // Constructor for a full fenced block.
  explicit solaris_fenced_block(full_t)
  {
    membar_consumer();
  }

  // Destructor.
  ~solaris_fenced_block()
  {
    membar_producer();
  }
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(__sun)

#endif // HBOOST_ASIO_DETAIL_SOLARIS_FENCED_BLOCK_HPP
