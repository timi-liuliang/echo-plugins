//
// impl/io_service.ipp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_IMPL_IO_SERVICE_IPP
#define HBOOST_ASIO_IMPL_IO_SERVICE_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/io_service.hpp>
#include <hboost/asio/detail/limits.hpp>
#include <hboost/asio/detail/scoped_ptr.hpp>
#include <hboost/asio/detail/service_registry.hpp>
#include <hboost/asio/detail/throw_error.hpp>

#if defined(HBOOST_ASIO_HAS_IOCP)
# include <hboost/asio/detail/win_iocp_io_service.hpp>
#else
# include <hboost/asio/detail/task_io_service.hpp>
#endif

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {

io_service::io_service()
  : service_registry_(new hboost::asio::detail::service_registry(
        *this, static_cast<impl_type*>(0),
        (std::numeric_limits<std::size_t>::max)())),
    impl_(service_registry_->first_service<impl_type>())
{
}

io_service::io_service(std::size_t concurrency_hint)
  : service_registry_(new hboost::asio::detail::service_registry(
        *this, static_cast<impl_type*>(0), concurrency_hint)),
    impl_(service_registry_->first_service<impl_type>())
{
}

io_service::~io_service()
{
  delete service_registry_;
}

std::size_t io_service::run()
{
  hboost::system::error_code ec;
  std::size_t s = impl_.run(ec);
  hboost::asio::detail::throw_error(ec);
  return s;
}

std::size_t io_service::run(hboost::system::error_code& ec)
{
  return impl_.run(ec);
}

std::size_t io_service::run_one()
{
  hboost::system::error_code ec;
  std::size_t s = impl_.run_one(ec);
  hboost::asio::detail::throw_error(ec);
  return s;
}

std::size_t io_service::run_one(hboost::system::error_code& ec)
{
  return impl_.run_one(ec);
}

std::size_t io_service::poll()
{
  hboost::system::error_code ec;
  std::size_t s = impl_.poll(ec);
  hboost::asio::detail::throw_error(ec);
  return s;
}

std::size_t io_service::poll(hboost::system::error_code& ec)
{
  return impl_.poll(ec);
}

std::size_t io_service::poll_one()
{
  hboost::system::error_code ec;
  std::size_t s = impl_.poll_one(ec);
  hboost::asio::detail::throw_error(ec);
  return s;
}

std::size_t io_service::poll_one(hboost::system::error_code& ec)
{
  return impl_.poll_one(ec);
}

void io_service::stop()
{
  impl_.stop();
}

bool io_service::stopped() const
{
  return impl_.stopped();
}

void io_service::reset()
{
  impl_.reset();
}

void io_service::notify_fork(hboost::asio::io_service::fork_event event)
{
  service_registry_->notify_fork(event);
}

io_service::service::service(hboost::asio::io_service& owner)
  : owner_(owner),
    next_(0)
{
}

io_service::service::~service()
{
}

void io_service::service::fork_service(hboost::asio::io_service::fork_event)
{
}

service_already_exists::service_already_exists()
  : std::logic_error("Service already exists.")
{
}

invalid_service_owner::invalid_service_owner()
  : std::logic_error("Invalid service owner.")
{
}

} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_IMPL_IO_SERVICE_IPP
