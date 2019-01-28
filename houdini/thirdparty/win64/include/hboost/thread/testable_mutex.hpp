// (C) Copyright 2012 Vicente J. Botet Escriba
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef HBOOST_THREAD_TESTABLE_LOCKABLE_HPP
#define HBOOST_THREAD_TESTABLE_LOCKABLE_HPP

#include <hboost/thread/detail/config.hpp>

#include <hboost/thread/thread_only.hpp>

#include <hboost/atomic.hpp>
#include <hboost/assert.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
  /**
   * Based on Associate Mutexes with Data to Prevent Races, By Herb Sutter, May 13, 2010
   * http://www.drdobbs.com/windows/associate-mutexes-with-data-to-prevent-r/224701827?pgno=3
   *
   * Make our mutex testable if it isn't already.
   *
   * Many mutex services (including hboost::mutex) don't provide a way to ask,
   * "Do I already hold a lock on this mutex?"
   * Sometimes it is needed to know if a method like is_locked to be available.
   * This wrapper associates an arbitrary lockable type with a thread id that stores the ID of the thread that
   * currently holds the lockable. The thread id initially holds an invalid value that means no threads own the mutex.
   * When we acquire a lock, we set the thread id; and when we release a lock, we reset it back to its default no id state.
   *
   */
  template <typename Lockable>
  class testable_mutex
  {
    Lockable mtx_;
    atomic<thread::id> id_;
  public:
    /// the type of the wrapped lockable
    typedef Lockable lockable_type;

    /// Non copyable
    HBOOST_THREAD_NO_COPYABLE(testable_mutex)

    testable_mutex() : id_(thread::id()) {}

    void lock()
    {
      HBOOST_ASSERT(! is_locked_by_this_thread());
      mtx_.lock();
      id_ = this_thread::get_id();
    }

    void unlock()
    {
      HBOOST_ASSERT(is_locked_by_this_thread());
      id_ = thread::id();
      mtx_.unlock();
    }

    bool try_lock()
    {
      HBOOST_ASSERT(! is_locked_by_this_thread());
      if (mtx_.try_lock())
      {
        id_ = this_thread::get_id();
        return true;
      }
      else
      {
        return false;
      }
    }
#ifdef HBOOST_THREAD_USES_CHRONO
    template <class Rep, class Period>
    bool try_lock_for(const chrono::duration<Rep, Period>& rel_time)
    {
      HBOOST_ASSERT(! is_locked_by_this_thread());
      if (mtx_.try_lock_for(rel_time))
      {
        id_ = this_thread::get_id();
        return true;
      }
      else
      {
        return false;
      }
    }
    template <class Clock, class Duration>
    bool try_lock_until(const chrono::time_point<Clock, Duration>& abs_time)
    {
      HBOOST_ASSERT(! is_locked_by_this_thread());
      if (mtx_.try_lock_until(abs_time))
      {
        id_ = this_thread::get_id();
        return true;
      }
      else
      {
        return false;
      }
    }
#endif

    bool is_locked_by_this_thread() const
    {
      return this_thread::get_id() == id_;
    }
    bool is_locked() const
    {
      return ! (thread::id() == id_);
    }

    thread::id get_id() const
    {
      return id_;
    }

    // todo add the shared and upgrade mutex functions
  };

  template <typename Lockable>
  struct is_testable_lockable : false_type
  {};

  template <typename Lockable>
  struct is_testable_lockable<testable_mutex<Lockable> > : true_type
  {};

//  /**
//   * Overloaded function used to check if the mutex is locked when it is testable and do nothing otherwise.
//   *
//   * This function is used usually to assert the pre-condition when the function can only be called when the mutex
//   * must be locked by the current thread.
//   */
//  template <typename Lockable>
//  bool is_locked_by_this_thread(testable_mutex<Lockable> const& mtx)
//  {
//    return mtx.is_locked();
//  }
//  template <typename Lockable>
//  bool is_locked_by_this_thread(Lockable const&)
//  {
//    return true;
//  }
}

#include <hboost/config/abi_suffix.hpp>

#endif // header