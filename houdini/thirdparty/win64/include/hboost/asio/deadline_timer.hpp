//
// deadline_timer.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DEADLINE_TIMER_HPP
#define HBOOST_ASIO_DEADLINE_TIMER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_HBOOST_DATE_TIME) \
  || defined(GENERATING_DOCUMENTATION)

#include <hboost/asio/detail/socket_types.hpp> // Must come before posix_time.
#include <hboost/asio/basic_deadline_timer.hpp>

#include <hboost/date_time/posix_time/posix_time_types.hpp>

namespace hboost {
namespace asio {

/// Typedef for the typical usage of timer. Uses a UTC clock.
typedef basic_deadline_timer<hboost::posix_time::ptime> deadline_timer;

} // namespace asio
} // namespace hboost

#endif // defined(HBOOST_ASIO_HAS_HBOOST_DATE_TIME)
       // || defined(GENERATING_DOCUMENTATION)

#endif // HBOOST_ASIO_DEADLINE_TIMER_HPP
