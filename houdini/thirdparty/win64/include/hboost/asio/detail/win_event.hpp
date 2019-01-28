//
// detail/win_event.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_WIN_EVENT_HPP
#define HBOOST_ASIO_DETAIL_WIN_EVENT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_WINDOWS)

#include <hboost/asio/detail/assert.hpp>
#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/detail/socket_types.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class win_event
  : private noncopyable
{
public:
  // Constructor.
  HBOOST_ASIO_DECL win_event();

  // Destructor.
  HBOOST_ASIO_DECL ~win_event();

  // Signal the event. (Retained for backward compatibility.)
  template <typename Lock>
  void signal(Lock& lock)
  {
    this->signal_all(lock);
  }

  // Signal all waiters.
  template <typename Lock>
  void signal_all(Lock& lock)
  {
    HBOOST_ASIO_ASSERT(lock.locked());
    (void)lock;
    state_ |= 1;
    ::SetEvent(events_[0]);
  }

  // Unlock the mutex and signal one waiter.
  template <typename Lock>
  void unlock_and_signal_one(Lock& lock)
  {
    HBOOST_ASIO_ASSERT(lock.locked());
    state_ |= 1;
    bool have_waiters = (state_ > 1);
    lock.unlock();
    if (have_waiters)
      ::SetEvent(events_[1]);
  }

  // If there's a waiter, unlock the mutex and signal it.
  template <typename Lock>
  bool maybe_unlock_and_signal_one(Lock& lock)
  {
    HBOOST_ASIO_ASSERT(lock.locked());
    state_ |= 1;
    if (state_ > 1)
    {
      lock.unlock();
      ::SetEvent(events_[1]);
      return true;
    }
    return false;
  }

  // Reset the event.
  template <typename Lock>
  void clear(Lock& lock)
  {
    HBOOST_ASIO_ASSERT(lock.locked());
    (void)lock;
    ::ResetEvent(events_[0]);
    state_ &= ~std::size_t(1);
  }

  // Wait for the event to become signalled.
  template <typename Lock>
  void wait(Lock& lock)
  {
    HBOOST_ASIO_ASSERT(lock.locked());
    while ((state_ & 1) == 0)
    {
      state_ += 2;
      lock.unlock();
#if defined(HBOOST_ASIO_WINDOWS_APP)
      ::WaitForMultipleObjectsEx(2, events_, false, INFINITE, false);
#else // defined(HBOOST_ASIO_WINDOWS_APP)
      ::WaitForMultipleObjects(2, events_, false, INFINITE);
#endif // defined(HBOOST_ASIO_WINDOWS_APP)
      lock.lock();
      state_ -= 2;
    }
  }

private:
  HANDLE events_[2];
  std::size_t state_;
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/win_event.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // defined(HBOOST_ASIO_WINDOWS)

#endif // HBOOST_ASIO_DETAIL_WIN_EVENT_HPP