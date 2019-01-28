//
// seq_packet_socket_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_SEQ_PACKET_SOCKET_SERVICE_HPP
#define HBOOST_ASIO_SEQ_PACKET_SOCKET_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <cstddef>
#include <hboost/asio/async_result.hpp>
#include <hboost/asio/detail/type_traits.hpp>
#include <hboost/asio/error.hpp>
#include <hboost/asio/io_service.hpp>

#if defined(HBOOST_ASIO_WINDOWS_RUNTIME)
# include <hboost/asio/detail/null_socket_service.hpp>
#elif defined(HBOOST_ASIO_HAS_IOCP)
# include <hboost/asio/detail/win_iocp_socket_service.hpp>
#else
# include <hboost/asio/detail/reactive_socket_service.hpp>
#endif

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {

/// Default service implementation for a sequenced packet socket.
template <typename Protocol>
class seq_packet_socket_service
#if defined(GENERATING_DOCUMENTATION)
  : public hboost::asio::io_service::service
#else
  : public hboost::asio::detail::service_base<
      seq_packet_socket_service<Protocol> >
#endif
{
public:
#if defined(GENERATING_DOCUMENTATION)
  /// The unique service identifier.
  static hboost::asio::io_service::id id;
#endif

  /// The protocol type.
  typedef Protocol protocol_type;

  /// The endpoint type.
  typedef typename Protocol::endpoint endpoint_type;

private:
  // The type of the platform-specific implementation.
#if defined(HBOOST_ASIO_WINDOWS_RUNTIME)
  typedef detail::null_socket_service<Protocol> service_impl_type;
#elif defined(HBOOST_ASIO_HAS_IOCP)
  typedef detail::win_iocp_socket_service<Protocol> service_impl_type;
#else
  typedef detail::reactive_socket_service<Protocol> service_impl_type;
#endif

public:
  /// The type of a sequenced packet socket implementation.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined implementation_type;
#else
  typedef typename service_impl_type::implementation_type implementation_type;
#endif

  /// (Deprecated: Use native_handle_type.) The native socket type.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_type;
#else
  typedef typename service_impl_type::native_handle_type native_type;
#endif

  /// The native socket type.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_handle_type;
#else
  typedef typename service_impl_type::native_handle_type native_handle_type;
#endif

  /// Construct a new sequenced packet socket service for the specified
  /// io_service.
  explicit seq_packet_socket_service(hboost::asio::io_service& io_service)
    : hboost::asio::detail::service_base<
        seq_packet_socket_service<Protocol> >(io_service),
      service_impl_(io_service)
  {
  }

  /// Construct a new sequenced packet socket implementation.
  void construct(implementation_type& impl)
  {
    service_impl_.construct(impl);
  }

#if defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Move-construct a new sequenced packet socket implementation.
  void move_construct(implementation_type& impl,
      implementation_type& other_impl)
  {
    service_impl_.move_construct(impl, other_impl);
  }

  /// Move-assign from another sequenced packet socket implementation.
  void move_assign(implementation_type& impl,
      seq_packet_socket_service& other_service,
      implementation_type& other_impl)
  {
    service_impl_.move_assign(impl, other_service.service_impl_, other_impl);
  }

  /// Move-construct a new sequenced packet socket implementation from another
  /// protocol type.
  template <typename Protocol1>
  void converting_move_construct(implementation_type& impl,
      typename seq_packet_socket_service<
        Protocol1>::implementation_type& other_impl,
      typename enable_if<is_convertible<
        Protocol1, Protocol>::value>::type* = 0)
  {
    service_impl_.template converting_move_construct<Protocol1>(
        impl, other_impl);
  }
#endif // defined(HBOOST_ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Destroy a sequenced packet socket implementation.
  void destroy(implementation_type& impl)
  {
    service_impl_.destroy(impl);
  }

  /// Open a sequenced packet socket.
  hboost::system::error_code open(implementation_type& impl,
      const protocol_type& protocol, hboost::system::error_code& ec)
  {
    if (protocol.type() == HBOOST_ASIO_OS_DEF(SOCK_SEQPACKET))
      service_impl_.open(impl, protocol, ec);
    else
      ec = hboost::asio::error::invalid_argument;
    return ec;
  }

  /// Assign an existing native socket to a sequenced packet socket.
  hboost::system::error_code assign(implementation_type& impl,
      const protocol_type& protocol, const native_handle_type& native_socket,
      hboost::system::error_code& ec)
  {
    return service_impl_.assign(impl, protocol, native_socket, ec);
  }

  /// Determine whether the socket is open.
  bool is_open(const implementation_type& impl) const
  {
    return service_impl_.is_open(impl);
  }

  /// Close a sequenced packet socket implementation.
  hboost::system::error_code close(implementation_type& impl,
      hboost::system::error_code& ec)
  {
    return service_impl_.close(impl, ec);
  }

  /// (Deprecated: Use native_handle().) Get the native socket implementation.
  native_type native(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
  }

  /// Get the native socket implementation.
  native_handle_type native_handle(implementation_type& impl)
  {
    return service_impl_.native_handle(impl);
  }

  /// Cancel all asynchronous operations associated with the socket.
  hboost::system::error_code cancel(implementation_type& impl,
      hboost::system::error_code& ec)
  {
    return service_impl_.cancel(impl, ec);
  }

  /// Determine whether the socket is at the out-of-band data mark.
  bool at_mark(const implementation_type& impl,
      hboost::system::error_code& ec) const
  {
    return service_impl_.at_mark(impl, ec);
  }

  /// Determine the number of bytes available for reading.
  std::size_t available(const implementation_type& impl,
      hboost::system::error_code& ec) const
  {
    return service_impl_.available(impl, ec);
  }

  /// Bind the sequenced packet socket to the specified local endpoint.
  hboost::system::error_code bind(implementation_type& impl,
      const endpoint_type& endpoint, hboost::system::error_code& ec)
  {
    return service_impl_.bind(impl, endpoint, ec);
  }

  /// Connect the sequenced packet socket to the specified endpoint.
  hboost::system::error_code connect(implementation_type& impl,
      const endpoint_type& peer_endpoint, hboost::system::error_code& ec)
  {
    return service_impl_.connect(impl, peer_endpoint, ec);
  }

  /// Start an asynchronous connect.
  template <typename ConnectHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(ConnectHandler,
      void (hboost::system::error_code))
  async_connect(implementation_type& impl,
      const endpoint_type& peer_endpoint,
      HBOOST_ASIO_MOVE_ARG(ConnectHandler) handler)
  {
    detail::async_result_init<
      ConnectHandler, void (hboost::system::error_code)> init(
        HBOOST_ASIO_MOVE_CAST(ConnectHandler)(handler));

    service_impl_.async_connect(impl, peer_endpoint, init.handler);

    return init.result.get();
  }

  /// Set a socket option.
  template <typename SettableSocketOption>
  hboost::system::error_code set_option(implementation_type& impl,
      const SettableSocketOption& option, hboost::system::error_code& ec)
  {
    return service_impl_.set_option(impl, option, ec);
  }

  /// Get a socket option.
  template <typename GettableSocketOption>
  hboost::system::error_code get_option(const implementation_type& impl,
      GettableSocketOption& option, hboost::system::error_code& ec) const
  {
    return service_impl_.get_option(impl, option, ec);
  }

  /// Perform an IO control command on the socket.
  template <typename IoControlCommand>
  hboost::system::error_code io_control(implementation_type& impl,
      IoControlCommand& command, hboost::system::error_code& ec)
  {
    return service_impl_.io_control(impl, command, ec);
  }

  /// Gets the non-blocking mode of the socket.
  bool non_blocking(const implementation_type& impl) const
  {
    return service_impl_.non_blocking(impl);
  }

  /// Sets the non-blocking mode of the socket.
  hboost::system::error_code non_blocking(implementation_type& impl,
      bool mode, hboost::system::error_code& ec)
  {
    return service_impl_.non_blocking(impl, mode, ec);
  }

  /// Gets the non-blocking mode of the native socket implementation.
  bool native_non_blocking(const implementation_type& impl) const
  {
    return service_impl_.native_non_blocking(impl);
  }

  /// Sets the non-blocking mode of the native socket implementation.
  hboost::system::error_code native_non_blocking(implementation_type& impl,
      bool mode, hboost::system::error_code& ec)
  {
    return service_impl_.native_non_blocking(impl, mode, ec);
  }

  /// Get the local endpoint.
  endpoint_type local_endpoint(const implementation_type& impl,
      hboost::system::error_code& ec) const
  {
    return service_impl_.local_endpoint(impl, ec);
  }

  /// Get the remote endpoint.
  endpoint_type remote_endpoint(const implementation_type& impl,
      hboost::system::error_code& ec) const
  {
    return service_impl_.remote_endpoint(impl, ec);
  }

  /// Disable sends or receives on the socket.
  hboost::system::error_code shutdown(implementation_type& impl,
      socket_base::shutdown_type what, hboost::system::error_code& ec)
  {
    return service_impl_.shutdown(impl, what, ec);
  }

  /// Send the given data to the peer.
  template <typename ConstBufferSequence>
  std::size_t send(implementation_type& impl,
      const ConstBufferSequence& buffers,
      socket_base::message_flags flags, hboost::system::error_code& ec)
  {
    return service_impl_.send(impl, buffers, flags, ec);
  }

  /// Start an asynchronous send.
  template <typename ConstBufferSequence, typename WriteHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
      void (hboost::system::error_code, std::size_t))
  async_send(implementation_type& impl,
      const ConstBufferSequence& buffers,
      socket_base::message_flags flags,
      HBOOST_ASIO_MOVE_ARG(WriteHandler) handler)
  {
    detail::async_result_init<
      WriteHandler, void (hboost::system::error_code, std::size_t)> init(
        HBOOST_ASIO_MOVE_CAST(WriteHandler)(handler));

    service_impl_.async_send(impl, buffers, flags, init.handler);

    return init.result.get();
  }

  /// Receive some data from the peer.
  template <typename MutableBufferSequence>
  std::size_t receive(implementation_type& impl,
      const MutableBufferSequence& buffers, socket_base::message_flags in_flags,
      socket_base::message_flags& out_flags, hboost::system::error_code& ec)
  {
    return service_impl_.receive_with_flags(impl,
        buffers, in_flags, out_flags, ec);
  }

  /// Start an asynchronous receive.
  template <typename MutableBufferSequence, typename ReadHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(ReadHandler,
      void (hboost::system::error_code, std::size_t))
  async_receive(implementation_type& impl,
      const MutableBufferSequence& buffers, socket_base::message_flags in_flags,
      socket_base::message_flags& out_flags,
      HBOOST_ASIO_MOVE_ARG(ReadHandler) handler)
  {
    detail::async_result_init<
      ReadHandler, void (hboost::system::error_code, std::size_t)> init(
        HBOOST_ASIO_MOVE_CAST(ReadHandler)(handler));

    service_impl_.async_receive_with_flags(impl,
        buffers, in_flags, out_flags, init.handler);

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

} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_SEQ_PACKET_SOCKET_SERVICE_HPP
