//
// detail/timer_scheduler.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_TIMER_SCHEDULER_HPP
#define HBOOST_ASIO_DETAIL_TIMER_SCHEDULER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/detail/timer_scheduler_fwd.hpp>

#if defined(HBOOST_ASIO_WINDOWS_RUNTIME)
# include <hboost/asio/detail/winrt_timer_scheduler.hpp>
#elif defined(HBOOST_ASIO_HAS_IOCP)
# include <hboost/asio/detail/win_iocp_io_service.hpp>
#elif defined(HBOOST_ASIO_HAS_EPOLL)
# include <hboost/asio/detail/epoll_reactor.hpp>
#elif defined(HBOOST_ASIO_HAS_KQUEUE)
# include <hboost/asio/detail/kqueue_reactor.hpp>
#elif defined(HBOOST_ASIO_HAS_DEV_POLL)
# include <hboost/asio/detail/dev_poll_reactor.hpp>
#else
# include <hboost/asio/detail/select_reactor.hpp>
#endif

#endif // HBOOST_ASIO_DETAIL_TIMER_SCHEDULER_HPP
