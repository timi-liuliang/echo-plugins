// Copyright (C) 2012 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_DETAIL_LOG_HPP
#define HBOOST_THREAD_DETAIL_LOG_HPP

#include <hboost/thread/detail/config.hpp>
#if defined HBOOST_THREAD_USES_LOG
#include <hboost/thread/recursive_mutex.hpp>
#include <hboost/thread/lock_guard.hpp>
#if defined HBOOST_THREAD_USES_LOG_THREAD_ID
#include <hboost/thread/thread.hpp>
#endif
#include <iostream>

namespace hboost
{
  namespace thread_detail
  {
    inline hboost::recursive_mutex& terminal_mutex()
    {
      static hboost::recursive_mutex mtx;
      return mtx;
    }

  }
}
#if defined HBOOST_THREAD_USES_LOG_THREAD_ID

#define HBOOST_THREAD_LOG \
  { \
    hboost::lock_guard<hboost::recursive_mutex> _lk_(hboost::thread_detail::terminal_mutex()); \
    std::cout << hboost::this_thread::get_id() << " - "<<__FILE__<<"["<<__LINE__<<"] " <<std::dec
#else

#define HBOOST_THREAD_LOG \
{ \
  hboost::lock_guard<hboost::recursive_mutex> _lk_(hboost::thread_detail::terminal_mutex()); \
  std::cout << __FILE__<<"["<<__LINE__<<"] " <<std::dec

#endif
#define HBOOST_THREAD_END_LOG \
    std::dec << std::endl; \
  }

#else

namespace hboost
{
  namespace thread_detail
  {
    struct dummy_stream_t
    {
    };

    template <typename T>
    inline dummy_stream_t const& operator<<(dummy_stream_t const& os, T)
    {
      return os;
    }

    inline dummy_stream_t const& operator<<(dummy_stream_t const& os, dummy_stream_t const&)
    {
      return os;
    }


    HBOOST_CONSTEXPR_OR_CONST dummy_stream_t dummy_stream = {};

  }
}

#define HBOOST_THREAD_LOG if (true) {} else hboost::thread_detail::dummy_stream
#define HBOOST_THREAD_END_LOG hboost::thread_detail::dummy_stream

#endif

#define HBOOST_THREAD_TRACE HBOOST_THREAD_LOG << HBOOST_THREAD_END_LOG


#endif // header
