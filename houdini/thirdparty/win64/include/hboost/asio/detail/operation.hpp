//
// detail/operation.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_OPERATION_HPP
#define HBOOST_ASIO_DETAIL_OPERATION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_IOCP)
# include <hboost/asio/detail/win_iocp_operation.hpp>
#else
# include <hboost/asio/detail/task_io_service_operation.hpp>
#endif

namespace hboost {
namespace asio {
namespace detail {

#if defined(HBOOST_ASIO_HAS_IOCP)
typedef win_iocp_operation operation;
#else
typedef task_io_service_operation operation;
#endif

} // namespace detail
} // namespace asio
} // namespace hboost

#endif // HBOOST_ASIO_DETAIL_OPERATION_HPP
