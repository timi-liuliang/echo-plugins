// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_LOCK_GUARD_HPP
#define HBOOST_THREAD_LOCK_GUARD_HPP

#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/detail/delete.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/detail/lockable_wrapper.hpp>
#include <hboost/thread/lock_options.hpp>
#if ! defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
#include <hboost/thread/is_locked_by_this_thread.hpp>
#include <hboost/assert.hpp>
#endif

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{

  template <typename Mutex>
  class lock_guard
  {
  private:
    Mutex& m;

  public:
    typedef Mutex mutex_type;
    HBOOST_THREAD_NO_COPYABLE( lock_guard )

    explicit lock_guard(Mutex& m_) :
      m(m_)
    {
      m.lock();
    }

    lock_guard(Mutex& m_, adopt_lock_t) :
      m(m_)
    {
#if ! defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
      HBOOST_ASSERT(is_locked_by_this_thread(m));
#endif
    }

#if ! defined HBOOST_THREAD_NO_CXX11_HDR_INITIALIZER_LIST
    lock_guard(std::initializer_list<thread_detail::lockable_wrapper<Mutex> > l_) :
      m(*(const_cast<thread_detail::lockable_wrapper<Mutex>*>(l_.begin())->m))
    {
      m.lock();
    }

    lock_guard(std::initializer_list<thread_detail::lockable_adopt_wrapper<Mutex> > l_) :
      m(*(const_cast<thread_detail::lockable_adopt_wrapper<Mutex>*>(l_.begin())->m))
    {
#if ! defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
      HBOOST_ASSERT(is_locked_by_this_thread(m));
#endif
    }

#endif
    ~lock_guard()
    {
      m.unlock();
    }
  };


#if ! defined HBOOST_THREAD_NO_MAKE_LOCK_GUARD
  template <typename Lockable>
  lock_guard<Lockable> make_lock_guard(Lockable& mtx)
  {
    return { thread_detail::lockable_wrapper<Lockable>(mtx) };
  }
  template <typename Lockable>
  lock_guard<Lockable> make_lock_guard(Lockable& mtx, adopt_lock_t)
  {
    return { thread_detail::lockable_adopt_wrapper<Lockable>(mtx) };
  }
#endif
}

#include <hboost/config/abi_suffix.hpp>

#endif
