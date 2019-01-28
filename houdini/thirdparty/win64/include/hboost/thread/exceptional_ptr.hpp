// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2014 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_EXCEPTIONAL_PTR_HPP
#define HBOOST_THREAD_EXCEPTIONAL_PTR_HPP

#include <hboost/thread/detail/move.hpp>
#include <hboost/exception_ptr.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
  struct exceptional_ptr {
    exception_ptr ptr_;

    exceptional_ptr() : ptr_() {}
    explicit exceptional_ptr(exception_ptr ex) : ptr_(ex) {}
    template <class E>
    explicit exceptional_ptr(HBOOST_FWD_REF(E) ex) : ptr_(hboost::copy_exception(hboost::forward<E>(ex))) {}
  };

  template <class E>
  inline exceptional_ptr make_exceptional(HBOOST_FWD_REF(E) ex) {
    return exceptional_ptr(hboost::forward<E>(ex));
  }

  inline exceptional_ptr make_exceptional(exception_ptr ex)
  {
    return exceptional_ptr(ex);
  }

  inline exceptional_ptr make_exceptional()
  {
    return exceptional_ptr();
  }

} // namespace hboost

#include <hboost/config/abi_suffix.hpp>

#endif
