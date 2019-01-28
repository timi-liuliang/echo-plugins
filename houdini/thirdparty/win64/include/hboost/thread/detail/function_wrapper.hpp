// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/09 Vicente J. Botet Escriba
//    Adapt to boost from CCIA C++11 implementation
//    Make use of Boost.Move

#ifndef HBOOST_THREAD_DETAIL_FUNCTION_WRAPPER_HPP
#define HBOOST_THREAD_DETAIL_FUNCTION_WRAPPER_HPP

#include <hboost/config.hpp>
#include <hboost/thread/detail/memory.hpp>
#include <hboost/thread/detail/move.hpp>

#include <hboost/thread/csbl/memory/unique_ptr.hpp>

#include <memory>
#include <functional>

namespace hboost
{
  namespace detail
  {
    class function_wrapper
    {
      struct impl_base
      {
        virtual void call()=0;
        virtual ~impl_base()
        {
        }
      };
      typedef hboost::csbl::unique_ptr<impl_base> impl_base_type;
      impl_base_type impl;
      template <typename F>
      struct impl_type: impl_base
      {
        F f;
        impl_type(F const &f_)
          : f(f_)
        {}
        impl_type(HBOOST_THREAD_RV_REF(F) f_)
          : f(hboost::move(f_))
        {}

        void call()
        {
          if (impl) f();
        }
      };
    public:
      HBOOST_THREAD_MOVABLE_ONLY(function_wrapper)

//#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
      template<typename F>
      function_wrapper(F const& f):
      impl(new impl_type<F>(f))
      {}
//#endif
      template<typename F>
      function_wrapper(HBOOST_THREAD_RV_REF(F) f):
      impl(new impl_type<F>(hboost::forward<F>(f)))
      {}
      function_wrapper(HBOOST_THREAD_RV_REF(function_wrapper) other) HBOOST_NOEXCEPT :
      impl(other.impl)
      {
        other.impl = 0;
      }
      function_wrapper()
        : impl(0)
      {
      }
      ~function_wrapper()
      {
      }

      function_wrapper& operator=(HBOOST_THREAD_RV_REF(function_wrapper) other) HBOOST_NOEXCEPT
      {
        impl=other.impl;
        other.impl=0;
        return *this;
      }

      void operator()()
      { impl->call();}

    };
  }
}

#endif // header
