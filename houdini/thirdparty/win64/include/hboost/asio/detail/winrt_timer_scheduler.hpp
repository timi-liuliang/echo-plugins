//
// detail/winrt_timer_scheduler.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_WINRT_TIMER_SCHEDULER_HPP
#define HBOOST_ASIO_DETAIL_WINRT_TIMER_SCHEDULER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#include <cstddef>
#include <hboost/asio/detail/event.hpp>
#include <hboost/asio/detail/limits.hpp>
#include <hboost/asio/detail/mutex.hpp>
#include <hboost/asio/detail/op_queue.hpp>
#include <hboost/asio/detail/thread.hpp>
#include <hboost/asio/detail/timer_queue_base.hpp>
#include <hboost/asio/detail/timer_queue_set.hpp>
#include <hboost/asio/detail/wait_op.hpp>
#include <hboost/asio/io_service.hpp>

#if defined(HBOOST_ASIO_HAS_IOCP)
# include <hboost/asio/detail/thread.hpp>
#endif // defined(HBOOST_ASIO_HAS_IOCP)

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class winrt_timer_scheduler
  : public hboost::asio::detail::service_base<winrt_timer_scheduler>
{
public:
  // Constructor.
  HBOOST_ASIO_DECL winrt_timer_scheduler(hboost::asio::io_service& io_service);

  // Destructor.
  HBOOST_ASIO_DECL ~winrt_timer_scheduler();

  // Destroy all user-defined handler objects owned by the service.
  HBOOST_ASIO_DECL void shutdown_service();

  // Recreate internal descriptors following a fork.
  HBOOST_ASIO_DECL void fork_service(
      hboost::asio::io_service::fork_event fork_ev);

  // Initialise the task. No effect as this class uses its own thread.
  HBOOST_ASIO_DECL void init_task();

  // Add a new timer queue to the reactor.
  template <typename Time_Traits>
  void add_timer_queue(timer_queue<Time_Traits>& queue);

  // Remove a timer queue from the reactor.
  template <typename Time_Traits>
  void remove_timer_queue(timer_queue<Time_Traits>& queue);

  // Schedule a new operation in the given timer queue to expire at the
  // specified absolute time.
  template <typename Time_Traits>
  void schedule_timer(timer_queue<Time_Traits>& queue,
      const typename Time_Traits::time_type& time,
      typename timer_queue<Time_Traits>::per_timer_data& timer, wait_op* op);

  // Cancel the timer operations associated with the given token. Returns the
  // number of operations that have been posted or dispatched.
  template <typename Time_Traits>
  std::size_t cancel_timer(timer_queue<Time_Traits>& queue,
      typename timer_queue<Time_Traits>::per_timer_data& timer,
      std::size_t max_cancelled = (std::numeric_limits<std::size_t>::max)());

private:
  // Run the select loop in the thread.
  HBOOST_ASIO_DECL void run_thread();

  // Entry point for the select loop thread.
  HBOOST_ASIO_DECL static void call_run_thread(winrt_timer_scheduler* reactor);

  // Helper function to add a new timer queue.
  HBOOST_ASIO_DECL void do_add_timer_queue(timer_queue_base& queue);

  // Helper function to remove a timer queue.
  HBOOST_ASIO_DECL void do_remove_timer_queue(timer_queue_base& queue);

  // The io_service implementation used to post completions.
  io_service_impl& io_service_;

  // Mutex used to protect internal variables.
  hboost::asio::detail::mutex mutex_;

  // Event used to wake up background thread.
  hboost::asio::detail::event event_;

  // The timer queues.
  timer_queue_set timer_queues_;

  // The background thread that is waiting for timers to expire.
  hboost::asio::detail::thread* thread_;

  // Does the background thread need to stop.
  bool stop_thread_;

  // Whether the service has been shut down.
  bool shutdown_;
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#include <hboost/asio/detail/impl/winrt_timer_scheduler.hpp>
#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/winrt_timer_scheduler.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#endif // HBOOST_ASIO_DETAIL_WINRT_TIMER_SCHEDULER_HPP
