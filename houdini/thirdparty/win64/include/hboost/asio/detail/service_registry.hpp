//
// detail/service_registry.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_SERVICE_REGISTRY_HPP
#define HBOOST_ASIO_DETAIL_SERVICE_REGISTRY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <typeinfo>
#include <hboost/asio/detail/mutex.hpp>
#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/io_service.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

template <typename T>
class typeid_wrapper {};

class service_registry
  : private noncopyable
{
public:
  // Constructor. Adds the initial service.
  template <typename Service, typename Arg>
  service_registry(hboost::asio::io_service& o,
      Service* initial_service, Arg arg);

  // Destructor.
  HBOOST_ASIO_DECL ~service_registry();

  // Notify all services of a fork event.
  HBOOST_ASIO_DECL void notify_fork(hboost::asio::io_service::fork_event fork_ev);

  // Get the first service object cast to the specified type. Called during
  // io_service construction and so performs no locking or type checking.
  template <typename Service>
  Service& first_service();

  // Get the service object corresponding to the specified service type. Will
  // create a new service object automatically if no such object already
  // exists. Ownership of the service object is not transferred to the caller.
  template <typename Service>
  Service& use_service();

  // Add a service object. Throws on error, in which case ownership of the
  // object is retained by the caller.
  template <typename Service>
  void add_service(Service* new_service);

  // Check whether a service object of the specified type already exists.
  template <typename Service>
  bool has_service() const;

private:
  // Initialise a service's key based on its id.
  HBOOST_ASIO_DECL static void init_key(
      hboost::asio::io_service::service::key& key,
      const hboost::asio::io_service::id& id);

#if !defined(HBOOST_ASIO_NO_TYPEID)
  // Initialise a service's key based on its id.
  template <typename Service>
  static void init_key(hboost::asio::io_service::service::key& key,
      const hboost::asio::detail::service_id<Service>& /*id*/);
#endif // !defined(HBOOST_ASIO_NO_TYPEID)

  // Check if a service matches the given id.
  HBOOST_ASIO_DECL static bool keys_match(
      const hboost::asio::io_service::service::key& key1,
      const hboost::asio::io_service::service::key& key2);

  // The type of a factory function used for creating a service instance.
  typedef hboost::asio::io_service::service*
    (*factory_type)(hboost::asio::io_service&);

  // Factory function for creating a service instance.
  template <typename Service>
  static hboost::asio::io_service::service* create(
      hboost::asio::io_service& owner);

  // Destroy a service instance.
  HBOOST_ASIO_DECL static void destroy(
      hboost::asio::io_service::service* service);

  // Helper class to manage service pointers.
  struct auto_service_ptr;
  friend struct auto_service_ptr;
  struct auto_service_ptr
  {
    hboost::asio::io_service::service* ptr_;
    ~auto_service_ptr() { destroy(ptr_); }
  };

  // Get the service object corresponding to the specified service key. Will
  // create a new service object automatically if no such object already
  // exists. Ownership of the service object is not transferred to the caller.
  HBOOST_ASIO_DECL hboost::asio::io_service::service* do_use_service(
      const hboost::asio::io_service::service::key& key,
      factory_type factory);

  // Add a service object. Throws on error, in which case ownership of the
  // object is retained by the caller.
  HBOOST_ASIO_DECL void do_add_service(
      const hboost::asio::io_service::service::key& key,
      hboost::asio::io_service::service* new_service);

  // Check whether a service object with the specified key already exists.
  HBOOST_ASIO_DECL bool do_has_service(
      const hboost::asio::io_service::service::key& key) const;

  // Mutex to protect access to internal data.
  mutable hboost::asio::detail::mutex mutex_;

  // The owner of this service registry and the services it contains.
  hboost::asio::io_service& owner_;

  // The first service in the list of contained services.
  hboost::asio::io_service::service* first_service_;
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#include <hboost/asio/detail/impl/service_registry.hpp>
#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/service_registry.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // HBOOST_ASIO_DETAIL_SERVICE_REGISTRY_HPP
