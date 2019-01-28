// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_LOCKABLE_TRAITS_HPP
#define HBOOST_THREAD_LOCKABLE_TRAITS_HPP

#include <hboost/thread/detail/config.hpp>

#include <hboost/assert.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/type_traits/is_class.hpp>

#include <hboost/config/abi_prefix.hpp>

// todo make use of integral_constant, true_type and false_type

namespace hboost
{
  namespace sync
  {

#if defined(HBOOST_NO_SFINAE) ||                           \
    HBOOST_WORKAROUND(__IBMCPP__, HBOOST_TESTED_AT(600)) || \
    HBOOST_WORKAROUND(__SUNPRO_CC, HBOOST_TESTED_AT(0x590))
#if ! defined HBOOST_THREAD_NO_AUTO_DETECT_MUTEX_TYPES
#define HBOOST_THREAD_NO_AUTO_DETECT_MUTEX_TYPES
#endif
#endif

#ifndef HBOOST_THREAD_NO_AUTO_DETECT_MUTEX_TYPES
    namespace detail
    {
#define HBOOST_THREAD_DEFINE_HAS_MEMBER_CALLED(member_name)                     \
        template<typename T, bool=hboost::is_class<T>::value>            \
        struct has_member_called_##member_name                          \
        {                                                               \
            HBOOST_STATIC_CONSTANT(bool, value=false);                   \
        };                                                              \
                                                                        \
        template<typename T>                                            \
        struct has_member_called_##member_name<T,true>                  \
        {                                                               \
            typedef char true_type;                                     \
            struct false_type                                           \
            {                                                           \
                true_type dummy[2];                                     \
            };                                                          \
                                                                        \
            struct fallback { int member_name; };                       \
            struct derived:                                             \
                T, fallback                                             \
            {                                                           \
                derived();                                              \
            };                                                          \
                                                                        \
            template<int fallback::*> struct tester;                    \
                                                                        \
            template<typename U>                                        \
                static false_type has_member(tester<&U::member_name>*); \
            template<typename U>                                        \
                static true_type has_member(...);                       \
                                                                        \
            HBOOST_STATIC_CONSTANT(                                      \
                bool, value=sizeof(has_member<derived>(0))==sizeof(true_type)); \
        }

      HBOOST_THREAD_DEFINE_HAS_MEMBER_CALLED(lock)
;      HBOOST_THREAD_DEFINE_HAS_MEMBER_CALLED(unlock);
      HBOOST_THREAD_DEFINE_HAS_MEMBER_CALLED(try_lock);

      template<typename T,bool=has_member_called_lock<T>::value >
      struct has_member_lock
      {
        HBOOST_STATIC_CONSTANT(bool, value=false);
      };

      template<typename T>
      struct has_member_lock<T,true>
      {
        typedef char true_type;
        struct false_type
        {
          true_type dummy[2];
        };

        template<typename U,typename V>
        static true_type has_member(V (U::*)());
        template<typename U>
        static false_type has_member(U);

        HBOOST_STATIC_CONSTANT(
            bool,value=sizeof(has_member_lock<T>::has_member(&T::lock))==sizeof(true_type));
      };

      template<typename T,bool=has_member_called_unlock<T>::value >
      struct has_member_unlock
      {
        HBOOST_STATIC_CONSTANT(bool, value=false);
      };

      template<typename T>
      struct has_member_unlock<T,true>
      {
        typedef char true_type;
        struct false_type
        {
          true_type dummy[2];
        };

        template<typename U,typename V>
        static true_type has_member(V (U::*)());
        template<typename U>
        static false_type has_member(U);

        HBOOST_STATIC_CONSTANT(
            bool,value=sizeof(has_member_unlock<T>::has_member(&T::unlock))==sizeof(true_type));
      };

      template<typename T,bool=has_member_called_try_lock<T>::value >
      struct has_member_try_lock
      {
        HBOOST_STATIC_CONSTANT(bool, value=false);
      };

      template<typename T>
      struct has_member_try_lock<T,true>
      {
        typedef char true_type;
        struct false_type
        {
          true_type dummy[2];
        };

        template<typename U>
        static true_type has_member(bool (U::*)());
        template<typename U>
        static false_type has_member(U);

        HBOOST_STATIC_CONSTANT(
            bool,value=sizeof(has_member_try_lock<T>::has_member(&T::try_lock))==sizeof(true_type));
      };

    }

    template<typename T>
    struct is_basic_lockable
    {
      HBOOST_STATIC_CONSTANT(bool, value = detail::has_member_lock<T>::value &&
          detail::has_member_unlock<T>::value);
    };
    template<typename T>
    struct is_lockable
    {
      HBOOST_STATIC_CONSTANT(bool, value =
          is_basic_lockable<T>::value &&
          detail::has_member_try_lock<T>::value);
    };

#else
    template<typename T>
    struct is_basic_lockable
    {
      HBOOST_STATIC_CONSTANT(bool, value = false);
    };
    template<typename T>
    struct is_lockable
    {
      HBOOST_STATIC_CONSTANT(bool, value = false);
    };
#endif

    template<typename T>
    struct is_recursive_mutex_sur_parole
    {
      HBOOST_STATIC_CONSTANT(bool, value = false);
    };
    template<typename T>
    struct is_recursive_mutex_sur_parolle : is_recursive_mutex_sur_parole<T>
    {
    };

    template<typename T>
    struct is_recursive_basic_lockable
    {
      HBOOST_STATIC_CONSTANT(bool, value = is_basic_lockable<T>::value &&
          is_recursive_mutex_sur_parolle<T>::value);
    };
    template<typename T>
    struct is_recursive_lockable
    {
      HBOOST_STATIC_CONSTANT(bool, value = is_lockable<T>::value &&
          is_recursive_mutex_sur_parolle<T>::value);
    };
  }
  template<typename T>
  struct is_mutex_type
  {
    HBOOST_STATIC_CONSTANT(bool, value = sync::is_lockable<T>::value);
  };

}
#include <hboost/config/abi_suffix.hpp>

#endif
