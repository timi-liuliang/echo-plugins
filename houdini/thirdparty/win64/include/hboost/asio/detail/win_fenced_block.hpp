//
// detail/win_fenced_block.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_WIN_FENCED_BLOCK_HPP
#define HBOOST_ASIO_DETAIL_WIN_FENCED_BLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_WINDOWS) && !defined(UNDER_CE)

#include <hboost/asio/detail/socket_types.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class win_fenced_block
  : private noncopyable
{
public:
  enum half_t { half };
  enum full_t { full };

  // Constructor for a half fenced block.
  explicit win_fenced_block(half_t)
  {
  }

  // Constructor for a full fenced block.
  explicit win_fenced_block(full_t)
  {
#if defined(__BORLANDC__)
    LONG barrier = 0;
    ::InterlockedExchange(&barrier, 1);
#elif defined(HBOOST_ASIO_MSVC) \
  && ((HBOOST_ASIO_MSVC < 1400) || !defined(MemoryBarrier))
# if defined(_M_IX86)
#  pragma warning(push)
#  pragma warning(disable:4793)
    LONG barrier;
    __asm { xchg barrier, eax }
#  pragma warning(pop)
# endif // defined(_M_IX86)
#else
    MemoryBarrier();
#endif
  }

  // Destructor.
  ~win_fenced_block()
  {
#if defined(__BORLANDC__)
    LONG barrier = 0;
    ::InterlockedExchange(&barrier, 1);
#elif defined(HBOOST_ASIO_MSVC) \
  && ((HBOOST_ASIO_MSVC < 1400) || !defined(MemoryBarrier))
# if defined(_M_IX86)
#  pragma warning(push)
#  pragma warning(disable:4793)
    LONG barrier;
    __asm { xchg barrier, eax }
#  pragma warning(pop)
# endif // defined(_M_IX86)
#else
    MemoryBarrier();
#endif
  }
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_WINDOWS) && !defined(UNDER_CE)

#endif // HBOOST_ASIO_DETAIL_WIN_FENCED_BLOCK_HPP
