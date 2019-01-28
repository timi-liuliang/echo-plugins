//
// detail/task_io_service_operation.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_TASK_IO_SERVICE_OPERATION_HPP
#define HBOOST_ASIO_DETAIL_TASK_IO_SERVICE_OPERATION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/system/error_code.hpp>
#include <hboost/asio/detail/handler_tracking.hpp>
#include <hboost/asio/detail/op_queue.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class task_io_service;

// Base class for all operations. A function pointer is used instead of virtual
// functions to avoid the associated overhead.
class task_io_service_operation HBOOST_ASIO_INHERIT_TRACKED_HANDLER
{
public:
  void complete(task_io_service& owner,
      const hboost::system::error_code& ec, std::size_t bytes_transferred)
  {
    func_(&owner, this, ec, bytes_transferred);
  }

  void destroy()
  {
    func_(0, this, hboost::system::error_code(), 0);
  }

protected:
  typedef void (*func_type)(task_io_service*,
      task_io_service_operation*,
      const hboost::system::error_code&, std::size_t);

  task_io_service_operation(func_type func)
    : next_(0),
      func_(func),
      task_result_(0)
  {
  }

  // Prevents deletion through this type.
  ~task_io_service_operation()
  {
  }

private:
  friend class op_queue_access;
  task_io_service_operation* next_;
  func_type func_;
protected:
  friend class task_io_service;
  unsigned int task_result_; // Passed into bytes transferred.
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_DETAIL_TASK_IO_SERVICE_OPERATION_HPP
