//
// posix/stream_descriptor_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_POSIX_STREAM_DESCRIPTOR_SERVICE_HPP
#define HBOOST_ASIO_POSIX_STREAM_DESCRIPTOR_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR) \
  || defined(GENERATING_DOCUMENTATION)

#include <cstddef>
#include <hboost/asio/async_result.hpp>
#include <hboost/asio/error.hpp>
#include <hboost/asio/io_service.hpp>
#include <hboost/asio/detail/reactive_descriptor_service.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace posix {

/// Default service implementation for a stream descriptor.
class stream_descriptor_service
#if defined(GENERATING_DOCUMENTATION)
  : public hboost::asio::io_service::service
#else
  : public hboost::asio::detail::service_base<stream_descriptor_service>
#endif
{
public:
#if defined(GENERATING_DOCUMENTATION)
  /// The unique service identifier.
  static hboost::asio::io_service::id id;
#endif

private:
  // The type of the platform-specific implementation.
  typedef detail::reactive_descriptor_service service_impl_type;

public:
  /// The type of a stream descriptor implementation.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined implementation_type;
#else
  typedef service_impl_type::implementation_type implementation_type;
#endif

  /// (Deprecated: Use native_handle_type.) The native descriptor type.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_type;
#else
  typedef service_impl_type::native_handle_type native_type;
#endif

  /// The native descriptor type.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_handle_type;
#else
  typedef service_impl_type::native_handle_type native_handle_type;
#endif

  /// Construct a new stream descriptor service for the specified io_service.
  explicit stream_descriptor_service(hboost::asio::io_service& io_service)
    : hboost::asio::detail::service_base<stream_descriptor_service>(io_service),
      service_impl_(io_service)
  {
  }

  /// Construct a new stream descriptor implementation.
  void construct(implementation_type& impl)
  {
    service_impl_.construct(impl);
  }

#if defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Move-construct a new stream descriptor implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    service_impl_.move_construct(impl, other_impl);
  }

  /// Move-assign from another stream descriptor implementation.
  void move_assign(implementation_type& impl,
      stream_descriptor_service& other_service,
      implementation_type& other_impl)
  {
    service_impl_.move_assign(impl, other_service.service_impl_, other_impl);
  }
#endif // defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Destroy a stream descriptor implementation.
  void destroy(implementation_type& impl)
  {
    service_impl_.destroy(impl);
  }

  /// Assign an existing native descriptor to a stream descriptor.
  hboost::system::error_code assign(implementation_type& impl,
      const native_handle_type& native_descriptor,
      hboost::system::error_code& ec)
  {
    return service_impl_.assign(impl, native_descriptor, ec);
  }

  /// Determine whether the descriptor is open.
  bool is_open(const implementation_type& impl) const
  {
    return service_impl_.is_open(impl);
  }

  /// Close a stream descriptor implementation.
  hboost::system::error_code close(implementation_type& impl,
      hboost::system::error_code& ec)
  {
    return service_impl_.close(impl, ec);
  }

  /// (Deprecated: Use native_handle().) Get the native descriptor
  /// implementation.
  native_type native(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
  }

  /// Get the native descriptor implementation.
  native_handle_type native_handle(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
  }

  /// Release ownership of the native descriptor implementation.
  native_handle_type release(implementation_type& impl)
  {
    return service_impl_.release(impl);
  }

  /// Cancel all asynchronous operations associated with the descriptor.
  hboost::system::error_code cancel(implementation_type& impl,
      hboost::system::error_code& ec)
  {
    return service_impl_.cancel(impl, ec);
  }

  /// Perform an IO control command on the descriptor.
  template <typename IoControlCommand>
  hboost::system::error_code io_control(implementation_type& impl,
      IoControlCommand& command, hboost::system::error_code& ec)
  {
    return service_impl_.io_control(impl, command, ec);
  }

  /// Gets the non-blocking mode of the descriptor.
  bool non_blocking(const implementation_type& impl) const
  {
    return service_impl_.non_blocking(impl);
  }

  /// Sets the non-blocking mode of the descriptor.
  hboost::system::error_code non_blocking(implementation_type& impl,
      bool mode, hboost::system::error_code& ec)
  {
    return service_impl_.non_blocking(impl, mode, ec);
  }

  /// Gets the non-blocking mode of the native descriptor implementation.
  bool native_non_blocking(const implementation_type& impl) const
  {
    return service_impl_.native_non_blocking(impl);
  }

  /// Sets the non-blocking mode of the native descriptor implementation.
  hboost::system::error_code native_non_blocking(implementation_type& impl,
      bool mode, hboost::system::error_code& ec)
  {
    return service_impl_.native_non_blocking(impl, mode, ec);
  }

  /// Write the given data to the stream.
  template <typename ConstBufferSequence>
  std::size_t write_some(implementation_type& impl,
      const ConstBufferSequence& buffers, hboost::system::error_code& ec)
  {
    return service_impl_.write_some(impl, buffers, ec);
  }

  /// Start an asynchronous write.
  template <typename ConstBufferSequence, typename WriteHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
      void (hboost::system::error_code, std::size_t))
  async_write_some(implementation_type& impl,
      const ConstBufferSequence& buffers,
      HBOOST_ASIO_MOVE_ARG(WriteHandler) handler)
  {
    hboost::asio::detail::async_result_init<
      WriteHandler, void (hboost::system::error_code, std::size_t)> init(
        HBOOST_ASIO_MOVE_CAST(WriteHandler)(handler));

    service_impl_.async_write_some(impl, buffers, init.handler);

    return init.result.get();
  }

  /// Read some data from the stream.
  template <typename MutableBufferSequence>
  std::size_t read_some(implementation_type& impl,
      const MutableBufferSequence& buffers, hboost::system::error_code& ec)
  {
    return service_impl_.read_some(impl, buffers, ec);
  }

  /// Start an asynchronous read.
  template <typename MutableBufferSequence, typename ReadHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(ReadHandler,
      void (hboost::system::error_code, std::size_t))
  async_read_some(implementation_type& impl,
      const MutableBufferSequence& buffers,
      HBOOST_ASIO_MOVE_ARG(ReadHandler) handler)
  {
    hboost::asio::detail::async_result_init<
      ReadHandler, void (hboost::system::error_code, std::size_t)> init(
        HBOOST_ASIO_MOVE_CAST(ReadHandler)(handler));

    service_impl_.async_read_some(impl, buffers, init.handler);

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

} // namespace posix
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // HBOOST_ASIO_POSIX_STREAM_DESCRIPTOR_SERVICE_HPP