//
// detail/timer_queue_set.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_TIMER_QUEUE_SET_HPP
#define HBOOST_ASIO_DETAIL_TIMER_QUEUE_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/detail/timer_queue_base.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class timer_queue_set
{
public:
  // Constructor.
  HBOOST_ASIO_DECL timer_queue_set();

  // Add a timer queue to the set.
  HBOOST_ASIO_DECL void insert(timer_queue_base* q);

  // Remove a timer queue from the set.
  HBOOST_ASIO_DECL void erase(timer_queue_base* q);

  // Determine whether all queues are empty.
  HBOOST_ASIO_DECL bool all_empty() const;

  // Get the wait duration in milliseconds.
  HBOOST_ASIO_DECL long wait_duration_msec(long max_duration) const;

  // Get the wait duration in microseconds.
  HBOOST_ASIO_DECL long wait_duration_usec(long max_duration) const;

  // Dequeue all ready timers.
  HBOOST_ASIO_DECL void get_ready_timers(op_queue<operation>& ops);

  // Dequeue all timers.
  HBOOST_ASIO_DECL void get_all_timers(op_queue<operation>& ops);

private:
  timer_queue_base* first_;
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/timer_queue_set.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // HBOOST_ASIO_DETAIL_TIMER_QUEUE_SET_HPP
