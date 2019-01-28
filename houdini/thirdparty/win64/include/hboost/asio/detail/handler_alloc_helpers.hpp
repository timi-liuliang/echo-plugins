//
// detail/handler_alloc_helpers.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_HANDLER_ALLOC_HELPERS_HPP
#define HBOOST_ASIO_DETAIL_HANDLER_ALLOC_HELPERS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/detail/addressof.hpp>
#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/handler_alloc_hook.hpp>

#include <hboost/asio/detail/push_options.hpp>

// Calls to asio_handler_allocate and asio_handler_deallocate must be made from
// a namespace that does not contain any overloads of these functions. The
// hboost_asio_handler_alloc_helpers namespace is defined here for that purpose.
namespace hboost_asio_handler_alloc_helpers {

template <typename Handler>
inline void* allocate(std::size_t s, Handler& h)
{
#if !defined(HBOOST_ASIO_HAS_HANDLER_HOOKS)
  return ::operator new(s);
#else
  using hboost::asio::asio_handler_allocate;
  return asio_handler_allocate(s, hboost::asio::detail::addressof(h));
#endif
}

template <typename Handler>
inline void deallocate(void* p, std::size_t s, Handler& h)
{
#if !defined(HBOOST_ASIO_HAS_HANDLER_HOOKS)
  ::operator delete(p);
#else
  using hboost::asio::asio_handler_deallocate;
  asio_handler_deallocate(p, s, hboost::asio::detail::addressof(h));
#endif
}

} // namespace hboost_asio_handler_alloc_helpers

#define HBOOST_ASIO_DEFINE_HANDLER_PTR(op) \
  struct ptr \
  { \
    Handler* h; \
    void* v; \
    op* p; \
    ~ptr() \
    { \
      reset(); \
    } \
    void reset() \
    { \
      if (p) \
      { \
        p->~op(); \
        p = 0; \
      } \
      if (v) \
      { \
        hboost_asio_handler_alloc_helpers::deallocate(v, sizeof(op), *h); \
        v = 0; \
      } \
    } \
  } \
  /**/

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_DETAIL_HANDLER_ALLOC_HELPERS_HPP
