//
// windows/random_access_handle_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_WINDOWS_RANDOM_ACCESS_HANDLE_SERVICE_HPP
#define HBOOST_ASIO_WINDOWS_RANDOM_ACCESS_HANDLE_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_WINDOWS_RANDOM_ACCESS_HANDLE) \
  || defined(GENERATING_DOCUMENTATION)

#include <cstddef>
#include <hboost/asio/async_result.hpp>
#include <hboost/asio/detail/cstdint.hpp>
#include <hboost/asio/detail/win_iocp_handle_service.hpp>
#include <hboost/asio/error.hpp>
#include <hboost/asio/io_service.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace windows {

/// Default service implementation for a random-access handle.
class random_access_handle_service
#if defined(GENERATING_DOCUMENTATION)
  : public hboost::asio::io_service::service
#else
  : public hboost::asio::detail::service_base<random_access_handle_service>
#endif
{
public:
#if defined(GENERATING_DOCUMENTATION)
  /// The unique service identifier.
  static hboost::asio::io_service::id id;
#endif

private:
  // The type of the platform-specific implementation.
  typedef detail::win_iocp_handle_service service_impl_type;

public:
  /// The type of a random-access handle implementation.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined implementation_type;
#else
  typedef service_impl_type::implementation_type implementation_type;
#endif

  /// (Deprecated: Use native_handle_type.) The native handle type.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_type;
#else
  typedef service_impl_type::native_handle_type native_type;
#endif

  /// The native handle type.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_handle_type;
#else
  typedef service_impl_type::native_handle_type native_handle_type;
#endif

  /// Construct a new random-access handle service for the specified io_service.
  explicit random_access_handle_service(hboost::asio::io_service& io_service)
    : hboost::asio::detail::service_base<
        random_access_handle_service>(io_service),
      service_impl_(io_service)
  {
  }

  /// Construct a new random-access handle implementation.
  void construct(implementation_type& impl)
  {
    service_impl_.construct(impl);
  }

#if defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Move-construct a new random-access handle implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    service_impl_.move_construct(impl, other_impl);
  }

  /// Move-assign from another random-access handle implementation.
  void move_assign(implementation_type& impl,
      random_access_handle_service& other_service,
      implementation_type& other_impl)
  {
    service_impl_.move_assign(impl, other_service.service_impl_, other_impl);
  }
#endif // defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Destroy a random-access handle implementation.
  void destroy(implementation_type& impl)
  {
    service_impl_.destroy(impl);
  }

  /// Assign an existing native handle to a random-access handle.
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

  /// Close a random-access handle implementation.
  hboost::system::error_code close(implementation_type& impl,
      hboost::system::error_code& ec)
  {
    return service_impl_.close(impl, ec);
  }

  /// (Deprecated: Use native_handle().) Get the native handle implementation.
  native_type native(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
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

  /// Write the given data at the specified offset.
  template <typename ConstBufferSequence>
  std::size_t write_some_at(implementation_type& impl, uint64_t offset,
      const ConstBufferSequence& buffers, hboost::system::error_code& ec)
  {
    return service_impl_.write_some_at(impl, offset, buffers, ec);
  }

  /// Start an asynchronous write at the specified offset.
  template <typename ConstBufferSequence, typename WriteHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
      void (hboost::system::error_code, std::size_t))
  async_write_some_at(implementation_type& impl,
      uint64_t offset, const ConstBufferSequence& buffers,
      HBOOST_ASIO_MOVE_ARG(WriteHandler) handler)
  {
    hboost::asio::detail::async_result_init<
      WriteHandler, void (hboost::system::error_code, std::size_t)> init(
        HBOOST_ASIO_MOVE_CAST(WriteHandler)(handler));

    service_impl_.async_write_some_at(impl, offset, buffers, init.handler);

    return init.result.get();
  }

  /// Read some data from the specified offset.
  template <typename MutableBufferSequence>
  std::size_t read_some_at(implementation_type& impl, uint64_t offset,
      const MutableBufferSequence& buffers, hboost::system::error_code& ec)
  {
    return service_impl_.read_some_at(impl, offset, buffers, ec);
  }

  /// Start an asynchronous read at the specified offset.
  template <typename MutableBufferSequence, typename ReadHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(ReadHandler,
      void (hboost::system::error_code, std::size_t))
  async_read_some_at(implementation_type& impl,
      uint64_t offset, const MutableBufferSequence& buffers,
      HBOOST_ASIO_MOVE_ARG(ReadHandler) handler)
  {
    hboost::asio::detail::async_result_init<
      ReadHandler, void (hboost::system::error_code, std::size_t)> init(
        HBOOST_ASIO_MOVE_CAST(ReadHandler)(handler));

    service_impl_.async_read_some_at(impl, offset, buffers, init.handler);

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

#endif // defined(HBOOST_ASIO_HAS_WINDOWS_RANDOM_ACCESS_HANDLE)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // HBOOST_ASIO_WINDOWS_RANDOM_ACCESS_HANDLE_SERVICE_HPP
