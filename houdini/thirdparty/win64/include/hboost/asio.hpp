//
// asio.hpp
// ~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See www.boost.org/libs/asio for documentation.
//

#ifndef HBOOST_ASIO_HPP
#define HBOOST_ASIO_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/async_result.hpp>
#include <hboost/asio/basic_datagram_socket.hpp>
#include <hboost/asio/basic_deadline_timer.hpp>
#include <hboost/asio/basic_io_object.hpp>
#include <hboost/asio/basic_raw_socket.hpp>
#include <hboost/asio/basic_seq_packet_socket.hpp>
#include <hboost/asio/basic_serial_port.hpp>
#include <hboost/asio/basic_signal_set.hpp>
#include <hboost/asio/basic_socket_acceptor.hpp>
#include <hboost/asio/basic_socket_iostream.hpp>
#include <hboost/asio/basic_socket_streambuf.hpp>
#include <hboost/asio/basic_stream_socket.hpp>
#include <hboost/asio/basic_streambuf.hpp>
#include <hboost/asio/basic_waitable_timer.hpp>
#include <hboost/asio/buffer.hpp>
#include <hboost/asio/buffered_read_stream_fwd.hpp>
#include <hboost/asio/buffered_read_stream.hpp>
#include <hboost/asio/buffered_stream_fwd.hpp>
#include <hboost/asio/buffered_stream.hpp>
#include <hboost/asio/buffered_write_stream_fwd.hpp>
#include <hboost/asio/buffered_write_stream.hpp>
#include <hboost/asio/buffers_iterator.hpp>
#include <hboost/asio/completion_condition.hpp>
#include <hboost/asio/connect.hpp>
#include <hboost/asio/coroutine.hpp>
#include <hboost/asio/datagram_socket_service.hpp>
#include <hboost/asio/deadline_timer_service.hpp>
#include <hboost/asio/deadline_timer.hpp>
#include <hboost/asio/error.hpp>
#include <hboost/asio/generic/basic_endpoint.hpp>
#include <hboost/asio/generic/datagram_protocol.hpp>
#include <hboost/asio/generic/raw_protocol.hpp>
#include <hboost/asio/generic/seq_packet_protocol.hpp>
#include <hboost/asio/generic/stream_protocol.hpp>
#include <hboost/asio/handler_alloc_hook.hpp>
#include <hboost/asio/handler_continuation_hook.hpp>
#include <hboost/asio/handler_invoke_hook.hpp>
#include <hboost/asio/handler_type.hpp>
#include <hboost/asio/io_service.hpp>
#include <hboost/asio/ip/address.hpp>
#include <hboost/asio/ip/address_v4.hpp>
#include <hboost/asio/ip/address_v6.hpp>
#include <hboost/asio/ip/basic_endpoint.hpp>
#include <hboost/asio/ip/basic_resolver.hpp>
#include <hboost/asio/ip/basic_resolver_entry.hpp>
#include <hboost/asio/ip/basic_resolver_iterator.hpp>
#include <hboost/asio/ip/basic_resolver_query.hpp>
#include <hboost/asio/ip/host_name.hpp>
#include <hboost/asio/ip/icmp.hpp>
#include <hboost/asio/ip/multicast.hpp>
#include <hboost/asio/ip/resolver_query_base.hpp>
#include <hboost/asio/ip/resolver_service.hpp>
#include <hboost/asio/ip/tcp.hpp>
#include <hboost/asio/ip/udp.hpp>
#include <hboost/asio/ip/unicast.hpp>
#include <hboost/asio/ip/v6_only.hpp>
#include <hboost/asio/is_read_buffered.hpp>
#include <hboost/asio/is_write_buffered.hpp>
#include <hboost/asio/local/basic_endpoint.hpp>
#include <hboost/asio/local/connect_pair.hpp>
#include <hboost/asio/local/datagram_protocol.hpp>
#include <hboost/asio/local/stream_protocol.hpp>
#include <hboost/asio/placeholders.hpp>
#include <hboost/asio/posix/basic_descriptor.hpp>
#include <hboost/asio/posix/basic_stream_descriptor.hpp>
#include <hboost/asio/posix/descriptor_base.hpp>
#include <hboost/asio/posix/stream_descriptor.hpp>
#include <hboost/asio/posix/stream_descriptor_service.hpp>
#include <hboost/asio/raw_socket_service.hpp>
#include <hboost/asio/read.hpp>
#include <hboost/asio/read_at.hpp>
#include <hboost/asio/read_until.hpp>
#include <hboost/asio/seq_packet_socket_service.hpp>
#include <hboost/asio/serial_port.hpp>
#include <hboost/asio/serial_port_base.hpp>
#include <hboost/asio/serial_port_service.hpp>
#include <hboost/asio/signal_set.hpp>
#include <hboost/asio/signal_set_service.hpp>
#include <hboost/asio/socket_acceptor_service.hpp>
#include <hboost/asio/socket_base.hpp>
#include <hboost/asio/strand.hpp>
#include <hboost/asio/stream_socket_service.hpp>
#include <hboost/asio/streambuf.hpp>
#include <hboost/asio/time_traits.hpp>
#include <hboost/asio/version.hpp>
#include <hboost/asio/wait_traits.hpp>
#include <hboost/asio/waitable_timer_service.hpp>
#include <hboost/asio/windows/basic_handle.hpp>
#include <hboost/asio/windows/basic_object_handle.hpp>
#include <hboost/asio/windows/basic_random_access_handle.hpp>
#include <hboost/asio/windows/basic_stream_handle.hpp>
#include <hboost/asio/windows/object_handle.hpp>
#include <hboost/asio/windows/object_handle_service.hpp>
#include <hboost/asio/windows/overlapped_ptr.hpp>
#include <hboost/asio/windows/random_access_handle.hpp>
#include <hboost/asio/windows/random_access_handle_service.hpp>
#include <hboost/asio/windows/stream_handle.hpp>
#include <hboost/asio/windows/stream_handle_service.hpp>
#include <hboost/asio/write.hpp>
#include <hboost/asio/write_at.hpp>

#endif // HBOOST_ASIO_HPP
