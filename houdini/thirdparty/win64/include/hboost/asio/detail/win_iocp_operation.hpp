//
// detail/win_iocp_operation.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_WIN_IOCP_OPERATION_HPP
#define HBOOST_ASIO_DETAIL_WIN_IOCP_OPERATION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_IOCP)

#include <hboost/asio/detail/handler_tracking.hpp>
#include <hboost/asio/detail/op_queue.hpp>
#include <hboost/asio/detail/socket_types.hpp>
#include <hboost/system/error_code.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class win_iocp_io_service;

// Base class for all operations. A function pointer is used instead of virtual
// functions to avoid the associated overhead.
class win_iocp_operation
  : public OVERLAPPED
    HBOOST_ASIO_ALSO_INHERIT_TRACKED_HANDLER
{
public:
  void complete(win_iocp_io_service& owner,
      const hboost::system::error_code& ec,
      std::size_t bytes_transferred)
  {
    func_(&owner, this, ec, bytes_transferred);
  }

  void destroy()
  {
    func_(0, this, hboost::system::error_code(), 0);
  }

protected:
  typedef void (*func_type)(
      win_iocp_io_service*, win_iocp_operation*,
      const hboost::system::error_code&, std::size_t);

  win_iocp_operation(func_type func)
    : next_(0),
      func_(func)
  {
    reset();
  }

  // Prevents deletion through this type.
  ~win_iocp_operation()
  {
  }

  void reset()
  {
    Internal = 0;
    InternalHigh = 0;
    Offset = 0;
    OffsetHigh = 0;
    hEvent = 0;
    ready_ = 0;
  }

private:
  friend class op_queue_access;
  friend class win_iocp_io_service;
  win_iocp_operation* next_;
  func_type func_;
  long ready_;
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_HAS_IOCP)

#endif // HBOOST_ASIO_DETAIL_WIN_IOCP_OPERATION_HPP