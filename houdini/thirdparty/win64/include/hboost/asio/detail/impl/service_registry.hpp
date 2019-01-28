//
// detail/impl/service_registry.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_IMPL_SERVICE_REGISTRY_HPP
#define HBOOST_ASIO_DETAIL_IMPL_SERVICE_REGISTRY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

template <typename Service, typename Arg>
service_registry::service_registry(
    hboost::asio::io_service& o, Service*, Arg arg)
  : owner_(o),
    first_service_(new Service(o, arg))
{
  hboost::asio::io_service::service::key key;
  init_key(key, Service::id);
  first_service_->key_ = key;
  first_service_->next_ = 0;
}

template <typename Service>
Service& service_registry::first_service()
{
  return *static_cast<Service*>(first_service_);
}

template <typename Service>
Service& service_registry::use_service()
{
  hboost::asio::io_service::service::key key;
  init_key(key, Service::id);
  factory_type factory = &service_registry::create<Service>;
  return *static_cast<Service*>(do_use_service(key, factory));
}

template <typename Service>
void service_registry::add_service(Service* new_service)
{
  hboost::asio::io_service::service::key key;
  init_key(key, Service::id);
  return do_add_service(key, new_service);
}

template <typename Service>
bool service_registry::has_service() const
{
  hboost::asio::io_service::service::key key;
  init_key(key, Service::id);
  return do_has_service(key);
}

#if !defined(HBOOST_ASIO_NO_TYPEID)
template <typename Service>
void service_registry::init_key(hboost::asio::io_service::service::key& key,
    const hboost::asio::detail::service_id<Service>& /*id*/)
{
  key.type_info_ = &typeid(typeid_wrapper<Service>);
  key.id_ = 0;
}
#endif // !defined(HBOOST_ASIO_NO_TYPEID)

template <typename Service>
hboost::asio::io_service::service* service_registry::create(
    hboost::asio::io_service& owner)
{
  return new Service(owner);
}

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_DETAIL_IMPL_SERVICE_REGISTRY_HPP