#ifndef HBOOST_THREAD_PTHREAD_ONCE_HPP
#define HBOOST_THREAD_PTHREAD_ONCE_HPP

//  once.hpp
//
//  (C) Copyright 2007-8 Anthony Williams
//  (C) Copyright 2011-2012 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/detail/invoke.hpp>

#include <hboost/thread/pthread/pthread_mutex_scoped_lock.hpp>
#include <hboost/thread/detail/delete.hpp>
#include <hboost/core/no_exceptions_support.hpp>

#include <hboost/bind.hpp>
#include <hboost/assert.hpp>
#include <hboost/config/abi_prefix.hpp>

#include <hboost/cstdint.hpp>
#include <pthread.h>
#include <csignal>

namespace hboost
{

  struct once_flag;

  #define HBOOST_ONCE_INITIAL_FLAG_VALUE 0

  namespace thread_detail
  {
    typedef hboost::uint32_t  uintmax_atomic_t;
    #define HBOOST_THREAD_DETAIL_UINTMAX_ATOMIC_C2(value) value##u
    #define HBOOST_THREAD_DETAIL_UINTMAX_ATOMIC_MAX_C HBOOST_THREAD_DETAIL_UINTMAX_ATOMIC_C2(~0)

  }

#ifdef HBOOST_THREAD_PROVIDES_ONCE_CXX11
#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
    template<typename Function, class ...ArgTypes>
    inline void call_once(once_flag& flag, HBOOST_THREAD_RV_REF(Function) f, HBOOST_THREAD_RV_REF(ArgTypes)... args);
#else
    template<typename Function>
    inline void call_once(once_flag& flag, Function f);
    template<typename Function, typename T1>
    inline void call_once(once_flag& flag, Function f, T1 p1);
    template<typename Function, typename T1, typename T2>
    inline void call_once(once_flag& flag, Function f, T1 p1, T2 p2);
    template<typename Function, typename T1, typename T2, typename T3>
    inline void call_once(once_flag& flag, Function f, T1 p1, T2 p2, T3 p3);
#endif

  struct once_flag
  {
      HBOOST_THREAD_NO_COPYABLE(once_flag)
      HBOOST_CONSTEXPR once_flag() HBOOST_NOEXCEPT
        : epoch(HBOOST_ONCE_INITIAL_FLAG_VALUE)
      {}
  private:
      volatile thread_detail::uintmax_atomic_t epoch;

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
      template<typename Function, class ...ArgTypes>
      friend void call_once(once_flag& flag, HBOOST_THREAD_RV_REF(Function) f, HBOOST_THREAD_RV_REF(ArgTypes)... args);
#else
      template<typename Function>
      friend void call_once(once_flag& flag, Function f);
      template<typename Function, typename T1>
      friend void call_once(once_flag& flag, Function f, T1 p1);
      template<typename Function, typename T1, typename T2>
      friend void call_once(once_flag& flag, Function f, T1 p1, T2 p2);
      template<typename Function, typename T1, typename T2, typename T3>
      friend void call_once(once_flag& flag, Function f, T1 p1, T2 p2, T3 p3);

#endif

  };

#define HBOOST_ONCE_INIT once_flag()

#else // HBOOST_THREAD_PROVIDES_ONCE_CXX11

    struct once_flag
    {
      volatile thread_detail::uintmax_atomic_t epoch;
    };

#define HBOOST_ONCE_INIT {HBOOST_ONCE_INITIAL_FLAG_VALUE}
#endif // HBOOST_THREAD_PROVIDES_ONCE_CXX11


#if defined HBOOST_THREAD_PROVIDES_INVOKE
#define HBOOST_THREAD_INVOKE_RET_VOID detail::invoke
#define HBOOST_THREAD_INVOKE_RET_VOID_CALL
#elif defined HBOOST_THREAD_PROVIDES_INVOKE_RET
#define HBOOST_THREAD_INVOKE_RET_VOID detail::invoke<void>
#define HBOOST_THREAD_INVOKE_RET_VOID_CALL
#else
#define HBOOST_THREAD_INVOKE_RET_VOID hboost::bind
#define HBOOST_THREAD_INVOKE_RET_VOID_CALL ()
#endif

    namespace thread_detail
    {
        HBOOST_THREAD_DECL uintmax_atomic_t& get_once_per_thread_epoch();
        HBOOST_THREAD_DECL extern uintmax_atomic_t once_global_epoch;
        HBOOST_THREAD_DECL extern pthread_mutex_t once_epoch_mutex;
        HBOOST_THREAD_DECL extern pthread_cond_t once_epoch_cv;
    }

    // Based on Mike Burrows fast_pthread_once algorithm as described in
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2444.html


#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)


  template<typename Function, class ...ArgTypes>
  inline void call_once(once_flag& flag, HBOOST_THREAD_RV_REF(Function) f, HBOOST_THREAD_RV_REF(ArgTypes)... args)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    HBOOST_THREAD_INVOKE_RET_VOID(
                        thread_detail::decay_copy(hboost::forward<Function>(f)),
                        thread_detail::decay_copy(hboost::forward<ArgTypes>(args))...
                    ) HBOOST_THREAD_INVOKE_RET_VOID_CALL;
                }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;

    }
  }
#else
  template<typename Function>
  inline void call_once(once_flag& flag, Function f)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    f();
                }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }

  template<typename Function, typename T1>
  inline void call_once(once_flag& flag, Function f, T1 p1)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    HBOOST_THREAD_INVOKE_RET_VOID(f,p1) HBOOST_THREAD_INVOKE_RET_VOID_CALL;
                }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }
  template<typename Function, typename T1, typename T2>
  inline void call_once(once_flag& flag, Function f, T1 p1, T2 p2)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    HBOOST_THREAD_INVOKE_RET_VOID(f,p1, p2) HBOOST_THREAD_INVOKE_RET_VOID_CALL;
        }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }

  template<typename Function, typename T1, typename T2, typename T3>
  inline void call_once(once_flag& flag, Function f, T1 p1, T2 p2, T3 p3)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    HBOOST_THREAD_INVOKE_RET_VOID(f,p1, p2, p3) HBOOST_THREAD_INVOKE_RET_VOID_CALL;
        }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }

  template<typename Function>
  inline void call_once(once_flag& flag, HBOOST_THREAD_RV_REF(Function) f)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    f();
                }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }

  template<typename Function, typename T1>
  inline void call_once(once_flag& flag, HBOOST_THREAD_RV_REF(Function) f, HBOOST_THREAD_RV_REF(T1) p1)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    HBOOST_THREAD_INVOKE_RET_VOID(
                        thread_detail::decay_copy(hboost::forward<Function>(f)),
                        thread_detail::decay_copy(hboost::forward<T1>(p1))
                    ) HBOOST_THREAD_INVOKE_RET_VOID_CALL;
                }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }
  template<typename Function, typename T1, typename T2>
  inline void call_once(once_flag& flag, HBOOST_THREAD_RV_REF(Function) f, HBOOST_THREAD_RV_REF(T1) p1, HBOOST_THREAD_RV_REF(T2) p2)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    HBOOST_THREAD_INVOKE_RET_VOID(
                        thread_detail::decay_copy(hboost::forward<Function>(f)),
                        thread_detail::decay_copy(hboost::forward<T1>(p1)),
                        thread_detail::decay_copy(hboost::forward<T1>(p2))
                    ) HBOOST_THREAD_INVOKE_RET_VOID_CALL;
                }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }

  template<typename Function, typename T1, typename T2, typename T3>
  inline void call_once(once_flag& flag, HBOOST_THREAD_RV_REF(Function) f, HBOOST_THREAD_RV_REF(T1) p1, HBOOST_THREAD_RV_REF(T2) p2, HBOOST_THREAD_RV_REF(T3) p3)
  {
    static thread_detail::uintmax_atomic_t const uninitialized_flag=HBOOST_ONCE_INITIAL_FLAG_VALUE;
    static thread_detail::uintmax_atomic_t const being_initialized=uninitialized_flag+1;
    thread_detail::uintmax_atomic_t const epoch=flag.epoch;
    thread_detail::uintmax_atomic_t& this_thread_epoch=thread_detail::get_once_per_thread_epoch();

    if(epoch<this_thread_epoch)
    {
        pthread::pthread_mutex_scoped_lock lk(&thread_detail::once_epoch_mutex);

        while(flag.epoch<=being_initialized)
        {
            if(flag.epoch==uninitialized_flag)
            {
                flag.epoch=being_initialized;
                HBOOST_TRY
                {
                    pthread::pthread_mutex_scoped_unlock relocker(&thread_detail::once_epoch_mutex);
                    HBOOST_THREAD_INVOKE_RET_VOID(
                        thread_detail::decay_copy(hboost::forward<Function>(f)),
                        thread_detail::decay_copy(hboost::forward<T1>(p1)),
                        thread_detail::decay_copy(hboost::forward<T1>(p2)),
                        thread_detail::decay_copy(hboost::forward<T1>(p3))
                    ) HBOOST_THREAD_INVOKE_RET_VOID_CALL;
                }
                HBOOST_CATCH (...)
                {
                    flag.epoch=uninitialized_flag;
                    HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
                    HBOOST_RETHROW
                }
                HBOOST_CATCH_END
                flag.epoch=--thread_detail::once_global_epoch;
                HBOOST_VERIFY(!pthread_cond_broadcast(&thread_detail::once_epoch_cv));
            }
            else
            {
                while(flag.epoch==being_initialized)
                {
                    HBOOST_VERIFY(!pthread_cond_wait(&thread_detail::once_epoch_cv,&thread_detail::once_epoch_mutex));
                }
            }
        }
        this_thread_epoch=thread_detail::once_global_epoch;
    }
  }

#endif

}

#include <hboost/config/abi_suffix.hpp>

#endif
