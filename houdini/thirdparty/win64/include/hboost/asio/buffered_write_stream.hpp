//
// buffered_write_stream.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_BUFFERED_WRITE_STREAM_HPP
#define HBOOST_ASIO_BUFFERED_WRITE_STREAM_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <cstddef>
#include <hboost/asio/buffered_write_stream_fwd.hpp>
#include <hboost/asio/buffer.hpp>
#include <hboost/asio/completion_condition.hpp>
#include <hboost/asio/detail/bind_handler.hpp>
#include <hboost/asio/detail/buffered_stream_storage.hpp>
#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/detail/type_traits.hpp>
#include <hboost/asio/error.hpp>
#include <hboost/asio/io_service.hpp>
#include <hboost/asio/write.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {

/// Adds buffering to the write-related operations of a stream.
/**
 * The buffered_write_stream class template can be used to add buffering to the
 * synchronous and asynchronous write operations of a stream.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 *
 * @par Concepts:
 * AsyncReadStream, AsyncWriteStream, Stream, SyncReadStream, SyncWriteStream.
 */
template <typename Stream>
class buffered_write_stream
  : private noncopyable
{
public:
  /// The type of the next layer.
  typedef typename remove_reference<Stream>::type next_layer_type;

  /// The type of the lowest layer.
  typedef typename next_layer_type::lowest_layer_type lowest_layer_type;

#if defined(GENERATING_DOCUMENTATION)
  /// The default buffer size.
  static const std::size_t default_buffer_size = implementation_defined;
#else
  HBOOST_ASIO_STATIC_CONSTANT(std::size_t, default_buffer_size = 1024);
#endif

  /// Construct, passing the specified argument to initialise the next layer.
  template <typename Arg>
  explicit buffered_write_stream(Arg& a)
    : next_layer_(a),
      storage_(default_buffer_size)
  {
  }

  /// Construct, passing the specified argument to initialise the next layer.
  template <typename Arg>
  buffered_write_stream(Arg& a, std::size_t buffer_size)
    : next_layer_(a),
      storage_(buffer_size)
  {
  }

  /// Get a reference to the next layer.
  next_layer_type& next_layer()
  {
    return next_layer_;
  }

  /// Get a reference to the lowest layer.
  lowest_layer_type& lowest_layer()
  {
    return next_layer_.lowest_layer();
  }

  /// Get a const reference to the lowest layer.
  const lowest_layer_type& lowest_layer() const
  {
    return next_layer_.lowest_layer();
  }

  /// Get the io_service associated with the object.
  hboost::asio::io_service& get_io_service()
  {
    return next_layer_.get_io_service();
  }

  /// Close the stream.
  void close()
  {
    next_layer_.close();
  }

  /// Close the stream.
  hboost::system::error_code close(hboost::system::error_code& ec)
  {
    return next_layer_.close(ec);
  }

  /// Flush all data from the buffer to the next layer. Returns the number of
  /// bytes written to the next layer on the last write operation. Throws an
  /// exception on failure.
  std::size_t flush();

  /// Flush all data from the buffer to the next layer. Returns the number of
  /// bytes written to the next layer on the last write operation, or 0 if an
  /// error occurred.
  std::size_t flush(hboost::system::error_code& ec);

  /// Start an asynchronous flush.
  template <typename WriteHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
      void (hboost::system::error_code, std::size_t))
  async_flush(HBOOST_ASIO_MOVE_ARG(WriteHandler) handler);

  /// Write the given data to the stream. Returns the number of bytes written.
  /// Throws an exception on failure.
  template <typename ConstBufferSequence>
  std::size_t write_some(const ConstBufferSequence& buffers);

  /// Write the given data to the stream. Returns the number of bytes written,
  /// or 0 if an error occurred and the error handler did not throw.
  template <typename ConstBufferSequence>
  std::size_t write_some(const ConstBufferSequence& buffers,
      hboost::system::error_code& ec);

  /// Start an asynchronous write. The data being written must be valid for the
  /// lifetime of the asynchronous operation.
  template <typename ConstBufferSequence, typename WriteHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(WriteHandler,
      void (hboost::system::error_code, std::size_t))
  async_write_some(const ConstBufferSequence& buffers,
      HBOOST_ASIO_MOVE_ARG(WriteHandler) handler);

  /// Read some data from the stream. Returns the number of bytes read. Throws
  /// an exception on failure.
  template <typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence& buffers)
  {
    return next_layer_.read_some(buffers);
  }

  /// Read some data from the stream. Returns the number of bytes read or 0 if
  /// an error occurred.
  template <typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence& buffers,
      hboost::system::error_code& ec)
  {
    return next_layer_.read_some(buffers, ec);
  }

  /// Start an asynchronous read. The buffer into which the data will be read
  /// must be valid for the lifetime of the asynchronous operation.
  template <typename MutableBufferSequence, typename ReadHandler>
  HBOOST_ASIO_INITFN_RESULT_TYPE(ReadHandler,
      void (hboost::system::error_code, std::size_t))
  async_read_some(const MutableBufferSequence& buffers,
      HBOOST_ASIO_MOVE_ARG(ReadHandler) handler)
  {
    detail::async_result_init<
      ReadHandler, void (hboost::system::error_code, std::size_t)> init(
        HBOOST_ASIO_MOVE_CAST(ReadHandler)(handler));

    next_layer_.async_read_some(buffers,
        HBOOST_ASIO_MOVE_CAST(HBOOST_ASIO_HANDLER_TYPE(ReadHandler,
            void (hboost::system::error_code, std::size_t)))(init.handler));

    return init.result.get();
  }

  /// Peek at the incoming data on the stream. Returns the number of bytes read.
  /// Throws an exception on failure.
  template <typename MutableBufferSequence>
  std::size_t peek(const MutableBufferSequence& buffers)
  {
    return next_layer_.peek(buffers);
  }

  /// Peek at the incoming data on the stream. Returns the number of bytes read,
  /// or 0 if an error occurred.
  template <typename MutableBufferSequence>
  std::size_t peek(const MutableBufferSequence& buffers,
      hboost::system::error_code& ec)
  {
    return next_layer_.peek(buffers, ec);
  }

  /// Determine the amount of data that may be read without blocking.
  std::size_t in_avail()
  {
    return next_layer_.in_avail();
  }

  /// Determine the amount of data that may be read without blocking.
  std::size_t in_avail(hboost::system::error_code& ec)
  {
    return next_layer_.in_avail(ec);
  }

private:
  /// Copy data into the internal buffer from the specified source buffer.
  /// Returns the number of bytes copied.
  template <typename ConstBufferSequence>
  std::size_t copy(const ConstBufferSequence& buffers);

  /// The next layer.
  Stream next_layer_;

  // The data in the buffer.
  detail::buffered_stream_storage storage_;
};

} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#include <hboost/asio/impl/buffered_write_stream.hpp>

#endif // HBOOST_ASIO_BUFFERED_WRITE_STREAM_HPP