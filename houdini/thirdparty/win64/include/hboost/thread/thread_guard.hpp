// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2009-2012 Anthony Williams
// (C) Copyright 2012 Vicente J. Botet Escriba

// Based on the Anthony's idea of thread_joiner in CCiA

#ifndef HBOOST_THREAD_THREAD_GUARD_HPP
#define HBOOST_THREAD_THREAD_GUARD_HPP

#include <hboost/thread/detail/delete.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/thread_functors.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{

  /**
   * Non-copyable RAII scoped thread guard joiner which join the thread if joinable when destroyed.
   */
  template <class CallableThread = join_if_joinable>
  class thread_guard
  {
    thread& t_;
  public:
    HBOOST_THREAD_NO_COPYABLE( thread_guard)

    explicit thread_guard(thread& t) :
    t_(t)
    {
    }
    ~thread_guard()
    {
      CallableThread on_destructor;

      on_destructor(t_);
    }
  };

}
#include <hboost/config/abi_suffix.hpp>

#endif
