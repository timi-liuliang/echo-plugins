//
// windows/object_handle_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2011 Boris Schaeling (boris@highscore.de)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_WINDOWS_OBJECT_HANDLE_SERVICE_HPP
#define HBOOST_ASIO_WINDOWS_OBJECT_HANDLE_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_WINDOWS_OBJECT_HANDLE) \
  || defined(GENERATING_DOCUMENTATION)

#include <hboost/asio/async_result.hpp>
#include <hboost/asio/detail/win_object_handle_service.hpp>
#include <hboost/asio/error.hpp>
#include <hboost/asio/io_service.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace windows {

/// Default service implementation for an object handle.
class object_handle_service
#if defined(GENERATING_DOCUMENTATION)
  : public hboost::asio::io_service::service
#else
  : public hboost::asio::detail::service_base<object_handle_service>
#endif
{
public:
#if defined(GENERATING_DOCUMENTATION)
  /// The unique service identifier.
  static hboost::asio::io_service::id id;
#endif

private:
  // The type of the platform-specific implementation.
  typedef detail::win_object_handle_service service_impl_type;

public:
  /// The type of an object handle implementation.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined implementation_type;
#else
  typedef service_impl_type::implementation_type implementation_type;
#endif

  /// The native handle type.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_handle_type;
#else
  typedef service_impl_type::native_handle_type native_handle_type;
#endif

  /// Construct a new object handle service for the specified io_service.
  explicit object_handle_service(hboost::asio::io_service& io_service)
    : hboost::asio::detail::service_base<object_handle_service>(io_service),
      service_impl_(io_service)
  {
  }

  /// Construct a new object handle implementation.
  void construct(implementation_type& impl)
  {
    service_impl_.construct(impl);
  }

#if defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Move-construct a new object handle implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    service_impl_.move_construct(impl, other_impl);
  }

  /// Move-assign from another object handle implementation.
  void move_assign(implementation_type& impl,
      object_handle_service& other_service,
      implementation_type& other_impl)
  {
    service_impl_.move_assign(impl, other_service.service_impl_, other_impl);
  }
#endif // defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Destroy an object handle implementation.
  void destroy(implementation_type& impl)
  {
    service_impl_.destroy(impl);
  }

  /// Assign an existing native handle to an object handle.
  hboost::system::error_code assign(implementation_type& impl,
      const native_handle_type& handle, hboost::system::error_code& ec)
  {
    return service_impl_.assign(impl, handle, ec);
  }

  /// Determine whether the handle is open.
  bool is_open(const implementation_type& impl) const
  {
    return service_impl_.is_open(impl);
  }

  /// Close an object handle implementation.
  hboost::system::error_code close(implementation_type& impl,
      hboost::system::error_code& ec)
  {
    return service_impl_.close(impl, ec);
  }

  /// Get the native handle implementation.
  native_handle_type native_handle(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
  }

  /// Cancel all asynchronous operations associated with the handle.
  hboost::system::error_code cancel(implementation_type& impl,
      hboost::system::error_code& ec)
  {
    return service_impl_.cancel(impl, ec);
  }

  // Wait for a signaled state.
  void wait(implementation_type& impl, hboost::system::error_code& ec)
  {
    service_impl_.wait(impl, ec);
  }

  /// Start an asynchronous wait.
  template <typename WaitHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(WaitHandler,
      void (hboost::system::error_code))
  async_wait(implementation_type& impl,
      HBOOST_ASIO_MOVE_ARG(WaitHandler) handler)
  {
    hboost::asio::detail::async_result_init<
      WaitHandler, void (hboost::system::error_code)> init(
        HBOOST_ASIO_MOVE_CAST(WaitHandler)(handler));

    service_impl_.async_wait(impl, init.handler);

    return init.result.get();
  }

private:
  // Destroy all user-defined handler objects owned by the service.
  void shutdown_service()
  {
    service_impl_.shutdown_service();
  }

  // The platform-specific implementation.
  service_impl_type service_impl_;
};

} // namespace windows
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_HAS_WINDOWS_OBJECT_HANDLE)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // HBOOST_ASIO_WINDOWS_OBJECT_HANDLE_SERVICE_HPP
