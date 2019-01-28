//
// detail/handler_tracking.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_HANDLER_TRACKING_HPP
#define HBOOST_ASIO_DETAIL_HANDLER_TRACKING_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_ENABLE_HANDLER_TRACKING)
# include <hboost/system/error_code.hpp>
# include <hboost/asio/detail/cstdint.hpp>
# include <hboost/asio/detail/static_mutex.hpp>
# include <hboost/asio/detail/tss_ptr.hpp>
#endif // defined(HBOOST_ASIO_ENABLE_HANDLER_TRACKING)

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

#if defined(HBOOST_ASIO_ENABLE_HANDLER_TRACKING)

class handler_tracking
{
public:
  class completion;

  // Base class for objects containing tracked handlers.
  class tracked_handler
  {
  private:
    // Only the handler_tracking class will have access to the id.
    friend class handler_tracking;
    friend class completion;
    uint64_t id_;

  protected:
    // Constructor initialises with no id.
    tracked_handler() : id_(0) {}

    // Prevent deletion through this type.
    ~tracked_handler() {}
  };

  // Initialise the tracking system.
  HBOOST_ASIO_DECL static void init();

  // Record the creation of a tracked handler.
  HBOOST_ASIO_DECL static void creation(tracked_handler* h,
      const char* object_type, void* object, const char* op_name);

  class completion
  {
  public:
    // Constructor records that handler is to be invoked with no arguments.
    HBOOST_ASIO_DECL explicit completion(tracked_handler* h);

    // Destructor records only when an exception is thrown from the handler, or
    // if the memory is being freed without the handler having been invoked.
    HBOOST_ASIO_DECL ~completion();

    // Records that handler is to be invoked with no arguments.
    HBOOST_ASIO_DECL void invocation_begin();

    // Records that handler is to be invoked with one arguments.
    HBOOST_ASIO_DECL void invocation_begin(const hboost::system::error_code& ec);

    // Constructor records that handler is to be invoked with two arguments.
    HBOOST_ASIO_DECL void invocation_begin(
        const hboost::system::error_code& ec, std::size_t bytes_transferred);

    // Constructor records that handler is to be invoked with two arguments.
    HBOOST_ASIO_DECL void invocation_begin(
        const hboost::system::error_code& ec, int signal_number);

    // Constructor records that handler is to be invoked with two arguments.
    HBOOST_ASIO_DECL void invocation_begin(
        const hboost::system::error_code& ec, const char* arg);

    // Record that handler invocation has ended.
    HBOOST_ASIO_DECL void invocation_end();

  private:
    friend class handler_tracking;
    uint64_t id_;
    bool invoked_;
    completion* next_;
  };

  // Record an operation that affects pending handlers.
  HBOOST_ASIO_DECL static void operation(const char* object_type,
      void* object, const char* op_name);

  // Write a line of output.
  HBOOST_ASIO_DECL static void write_line(const char* format, ...);

private:
  struct tracking_state;
  HBOOST_ASIO_DECL static tracking_state* get_state();
};

# define HBOOST_ASIO_INHERIT_TRACKED_HANDLER \
  : public hboost::asio::detail::handler_tracking::tracked_handler

# define HBOOST_ASIO_ALSO_INHERIT_TRACKED_HANDLER \
  , public hboost::asio::detail::handler_tracking::tracked_handler

# define HBOOST_ASIO_HANDLER_TRACKING_INIT \
  hboost::asio::detail::handler_tracking::init()

# define HBOOST_ASIO_HANDLER_CREATION(args) \
  hboost::asio::detail::handler_tracking::creation args

# define HBOOST_ASIO_HANDLER_COMPLETION(args) \
  hboost::asio::detail::handler_tracking::completion tracked_completion args

# define HBOOST_ASIO_HANDLER_INVOCATION_BEGIN(args) \
  tracked_completion.invocation_begin args

# define HBOOST_ASIO_HANDLER_INVOCATION_END \
  tracked_completion.invocation_end()

# define HBOOST_ASIO_HANDLER_OPERATION(args) \
  hboost::asio::detail::handler_tracking::operation args

#else // defined(HBOOST_ASIO_ENABLE_HANDLER_TRACKING)

# define HBOOST_ASIO_INHERIT_TRACKED_HANDLER
# define HBOOST_ASIO_ALSO_INHERIT_TRACKED_HANDLER
# define HBOOST_ASIO_HANDLER_TRACKING_INIT (void)0
# define HBOOST_ASIO_HANDLER_CREATION(args) (void)0
# define HBOOST_ASIO_HANDLER_COMPLETION(args) (void)0
# define HBOOST_ASIO_HANDLER_INVOCATION_BEGIN(args) (void)0
# define HBOOST_ASIO_HANDLER_INVOCATION_END (void)0
# define HBOOST_ASIO_HANDLER_OPERATION(args) (void)0

#endif // defined(HBOOST_ASIO_ENABLE_HANDLER_TRACKING)

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/handler_tracking.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // HBOOST_ASIO_DETAIL_HANDLER_TRACKING_HPP
