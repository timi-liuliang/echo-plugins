// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/09 Vicente J. Botet Escriba
//    Adapt to boost from CCIA C++11 implementation
//    Make use of Boost.Move

#ifndef HBOOST_THREAD_DETAIL_NULLARY_FUNCTION_HPP
#define HBOOST_THREAD_DETAIL_NULLARY_FUNCTION_HPP

#include <hboost/config.hpp>
#include <hboost/thread/detail/memory.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/csbl/memory/shared_ptr.hpp>
#include <hboost/type_traits/decay.hpp>

namespace hboost
{
  namespace detail
  {

    template <typename F>
    class nullary_function;
    template <>
    class nullary_function<void()>
    {
      struct impl_base
      {
        virtual void call()=0;
        virtual ~impl_base()
        {
        }
      };
      csbl::shared_ptr<impl_base> impl;
      template <typename F>
      struct impl_type: impl_base
      {
        F f;
#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES
        impl_type(F &f_)
          : f(f_)
        {}
#endif
        impl_type(HBOOST_THREAD_RV_REF(F) f_)
          : f(hboost::move(f_))
        {}

        void call()
        {
          f();
        }
      };
      struct impl_type_ptr: impl_base
      {
        void (*f)();
        impl_type_ptr(void (*f_)())
          : f(f_)
        {}
        void call()
        {
          f();
        }
      };
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(nullary_function)

      explicit nullary_function(void (*f)()):
      impl(new impl_type_ptr(f))
      {}

#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES
      template<typename F>
      explicit nullary_function(F& f):
      impl(new impl_type<F>(f))
      {}
#endif
      template<typename F>
      nullary_function(HBOOST_THREAD_RV_REF(F) f):
      impl(new impl_type<typename decay<F>::type>(thread_detail::decay_copy(hboost::forward<F>(f))))
      {}

      nullary_function()
        : impl()
      {
      }
      nullary_function(nullary_function const& other) HBOOST_NOEXCEPT :
      impl(other.impl)
      {
      }
      nullary_function(HBOOST_THREAD_RV_REF(nullary_function) other) HBOOST_NOEXCEPT :
#if defined HBOOST_NO_CXX11_SMART_PTR
      impl(HBOOST_THREAD_RV(other).impl)
      {
        HBOOST_THREAD_RV(other).impl.reset();
      }
#else
      impl(hboost::move(other.impl))
      {
      }
#endif
      ~nullary_function()
      {
      }

      nullary_function& operator=(HBOOST_THREAD_COPY_ASSIGN_REF(nullary_function) other) HBOOST_NOEXCEPT
      {
        impl=other.impl;
        return *this;
      }
      nullary_function& operator=(HBOOST_THREAD_RV_REF(nullary_function) other) HBOOST_NOEXCEPT
      {
#if defined HBOOST_NO_CXX11_SMART_PTR
        impl=HBOOST_THREAD_RV(other).impl;
        HBOOST_THREAD_RV(other).impl.reset();
#else
        impl = hboost::move(other.impl);
#endif
        return *this;
      }


      void operator()()
      { if (impl) impl->call();}

    };

    template <typename R>
    class nullary_function<R()>
    {
      struct impl_base
      {
        virtual R call()=0;
        virtual ~impl_base()
        {
        }
      };
      csbl::shared_ptr<impl_base> impl;
      template <typename F>
      struct impl_type: impl_base
      {
        F f;
#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES
        impl_type(F &f_)
          : f(f_)
        {}
#endif
        impl_type(HBOOST_THREAD_RV_REF(F) f_)
          : f(hboost::move(f_))
        {}

        R call()
        {
          return f();
        }
      };
      struct impl_type_ptr: impl_base
      {
        R (*f)();
        impl_type_ptr(R (*f_)())
          : f(f_)
        {}

        R call()
        {
          return f();
        }
      };
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(nullary_function)

      nullary_function(R (*f)()):
      impl(new impl_type_ptr(f))
      {}
#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES
      template<typename F>
      nullary_function(F& f):
      impl(new impl_type<F>(f))
      {}
#endif
      template<typename F>
      nullary_function(HBOOST_THREAD_RV_REF(F) f):
      impl(new impl_type<typename decay<F>::type>(thread_detail::decay_copy(hboost::forward<F>(f))))
      {}

      nullary_function(nullary_function const& other) HBOOST_NOEXCEPT :
      impl(other.impl)
      {
      }
      nullary_function(HBOOST_THREAD_RV_REF(nullary_function) other) HBOOST_NOEXCEPT :
#if defined HBOOST_NO_CXX11_SMART_PTR
      impl(HBOOST_THREAD_RV(other).impl)
      {
        HBOOST_THREAD_RV(other).impl.reset();
      }
#else
      impl(hboost::move(other.impl))
      {
      }
#endif
      nullary_function()
        : impl()
      {
      }
      ~nullary_function()
      {
      }

      nullary_function& operator=(HBOOST_THREAD_COPY_ASSIGN_REF(nullary_function) other) HBOOST_NOEXCEPT
      {
        impl=other.impl;
        return *this;
      }
      nullary_function& operator=(HBOOST_THREAD_RV_REF(nullary_function) other) HBOOST_NOEXCEPT
      {
#if defined HBOOST_NO_CXX11_SMART_PTR
        impl=HBOOST_THREAD_RV(other).impl;
        HBOOST_THREAD_RV(other).impl.reset();
#else
        impl = hboost::move(other.impl);
#endif
        return *this;
      }

      R operator()()
      { if (impl) return impl->call(); else return R();}

    };
  }
  HBOOST_THREAD_DCL_MOVABLE_BEG(F) detail::nullary_function<F> HBOOST_THREAD_DCL_MOVABLE_END
}

#endif // header