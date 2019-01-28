// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007-8 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba

#ifndef HBOOST_THREAD_MOVE_HPP
#define HBOOST_THREAD_MOVE_HPP

#include <hboost/thread/detail/config.hpp>
#ifndef HBOOST_NO_SFINAE
#include <hboost/core/enable_if.hpp>
#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/remove_cv.hpp>
#include <hboost/type_traits/decay.hpp>
#include <hboost/type_traits/conditional.hpp>
#include <hboost/type_traits/remove_extent.hpp>
#include <hboost/type_traits/is_array.hpp>
#include <hboost/type_traits/is_function.hpp>
#include <hboost/type_traits/remove_cv.hpp>
#include <hboost/type_traits/add_pointer.hpp>
#include <hboost/type_traits/decay.hpp>
#endif

#include <hboost/thread/detail/delete.hpp>
#include <hboost/move/utility.hpp>
#include <hboost/move/traits.hpp>
#include <hboost/config/abi_prefix.hpp>
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
#include <type_traits>
#endif
namespace hboost
{

    namespace detail
    {
      template <typename T>
      struct enable_move_utility_emulation_dummy_specialization;
        template<typename T>
        struct thread_move_t
        {
            T& t;
            explicit thread_move_t(T& t_):
                t(t_)
            {}

            T& operator*() const
            {
                return t;
            }

            T* operator->() const
            {
                return &t;
            }
        private:
            void operator=(thread_move_t&);
        };
    }

#if !defined HBOOST_THREAD_USES_MOVE

#ifndef HBOOST_NO_SFINAE
    template<typename T>
    typename enable_if<hboost::is_convertible<T&,hboost::detail::thread_move_t<T> >, hboost::detail::thread_move_t<T> >::type move(T& t)
    {
        return hboost::detail::thread_move_t<T>(t);
    }
#endif

    template<typename T>
    hboost::detail::thread_move_t<T> move(hboost::detail::thread_move_t<T> t)
    {
        return t;
    }

#endif   //#if !defined HBOOST_THREAD_USES_MOVE
}

#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES

#define HBOOST_THREAD_COPY_ASSIGN_REF(TYPE) HBOOST_COPY_ASSIGN_REF(TYPE)
#define HBOOST_THREAD_RV_REF(TYPE) HBOOST_RV_REF(TYPE)
#define HBOOST_THREAD_RV_REF_2_TEMPL_ARGS(TYPE) HBOOST_RV_REF_2_TEMPL_ARGS(TYPE)
#define HBOOST_THREAD_RV_REF_BEG HBOOST_RV_REF_BEG
#define HBOOST_THREAD_RV_REF_END HBOOST_RV_REF_END
#define HBOOST_THREAD_RV(V) V
#define HBOOST_THREAD_MAKE_RV_REF(RVALUE) RVALUE
#define HBOOST_THREAD_FWD_REF(TYPE) HBOOST_FWD_REF(TYPE)
#define HBOOST_THREAD_DCL_MOVABLE(TYPE)
#define HBOOST_THREAD_DCL_MOVABLE_BEG(T) \
  namespace detail { \
    template <typename T> \
    struct enable_move_utility_emulation_dummy_specialization<

#define HBOOST_THREAD_DCL_MOVABLE_BEG2(T1, T2) \
  namespace detail { \
    template <typename T1, typename T2> \
    struct enable_move_utility_emulation_dummy_specialization<

#define HBOOST_THREAD_DCL_MOVABLE_END > \
      : integral_constant<bool, false> \
      {}; \
    }

#elif ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES && defined  HBOOST_MSVC

#define HBOOST_THREAD_COPY_ASSIGN_REF(TYPE) HBOOST_COPY_ASSIGN_REF(TYPE)
#define HBOOST_THREAD_RV_REF(TYPE) HBOOST_RV_REF(TYPE)
#define HBOOST_THREAD_RV_REF_2_TEMPL_ARGS(TYPE) HBOOST_RV_REF_2_TEMPL_ARGS(TYPE)
#define HBOOST_THREAD_RV_REF_BEG HBOOST_RV_REF_BEG
#define HBOOST_THREAD_RV_REF_END HBOOST_RV_REF_END
#define HBOOST_THREAD_RV(V) V
#define HBOOST_THREAD_MAKE_RV_REF(RVALUE) RVALUE
#define HBOOST_THREAD_FWD_REF(TYPE) HBOOST_FWD_REF(TYPE)
#define HBOOST_THREAD_DCL_MOVABLE(TYPE)
#define HBOOST_THREAD_DCL_MOVABLE_BEG(T) \
  namespace detail { \
    template <typename T> \
    struct enable_move_utility_emulation_dummy_specialization<

#define HBOOST_THREAD_DCL_MOVABLE_BEG2(T1, T2) \
  namespace detail { \
    template <typename T1, typename T2> \
    struct enable_move_utility_emulation_dummy_specialization<

#define HBOOST_THREAD_DCL_MOVABLE_END > \
      : integral_constant<bool, false> \
      {}; \
    }

#else

#if defined HBOOST_THREAD_USES_MOVE
#define HBOOST_THREAD_COPY_ASSIGN_REF(TYPE) HBOOST_COPY_ASSIGN_REF(TYPE)
#define HBOOST_THREAD_RV_REF(TYPE) HBOOST_RV_REF(TYPE)
#define HBOOST_THREAD_RV_REF_2_TEMPL_ARGS(TYPE) HBOOST_RV_REF_2_TEMPL_ARGS(TYPE)
#define HBOOST_THREAD_RV_REF_BEG HBOOST_RV_REF_BEG
#define HBOOST_THREAD_RV_REF_END HBOOST_RV_REF_END
#define HBOOST_THREAD_RV(V) V
#define HBOOST_THREAD_FWD_REF(TYPE) HBOOST_FWD_REF(TYPE)
#define HBOOST_THREAD_DCL_MOVABLE(TYPE)
#define HBOOST_THREAD_DCL_MOVABLE_BEG(T) \
  namespace detail { \
    template <typename T> \
    struct enable_move_utility_emulation_dummy_specialization<

#define HBOOST_THREAD_DCL_MOVABLE_BEG2(T1, T2) \
  namespace detail { \
    template <typename T1, typename T2> \
    struct enable_move_utility_emulation_dummy_specialization<

#define HBOOST_THREAD_DCL_MOVABLE_END > \
      : integral_constant<bool, false> \
      {}; \
    }

#else

#define HBOOST_THREAD_COPY_ASSIGN_REF(TYPE) const TYPE&
#define HBOOST_THREAD_RV_REF(TYPE) hboost::detail::thread_move_t< TYPE >
#define HBOOST_THREAD_RV_REF_BEG hboost::detail::thread_move_t<
#define HBOOST_THREAD_RV_REF_END >
#define HBOOST_THREAD_RV(V) (*V)
#define HBOOST_THREAD_FWD_REF(TYPE) HBOOST_FWD_REF(TYPE)

#define HBOOST_THREAD_DCL_MOVABLE(TYPE) \
template <> \
struct enable_move_utility_emulation< TYPE > \
{ \
   static const bool value = false; \
};

#define HBOOST_THREAD_DCL_MOVABLE_BEG(T) \
template <typename T> \
struct enable_move_utility_emulation<

#define HBOOST_THREAD_DCL_MOVABLE_BEG2(T1, T2) \
template <typename T1, typename T2> \
struct enable_move_utility_emulation<

#define HBOOST_THREAD_DCL_MOVABLE_END > \
{ \
   static const bool value = false; \
};

#endif

namespace hboost
{
namespace detail
{
  template <typename T>
  HBOOST_THREAD_RV_REF(typename ::hboost::remove_cv<typename ::hboost::remove_reference<T>::type>::type)
  make_rv_ref(T v)  HBOOST_NOEXCEPT
  {
    return (HBOOST_THREAD_RV_REF(typename ::hboost::remove_cv<typename ::hboost::remove_reference<T>::type>::type))(v);
  }
//  template <typename T>
//  HBOOST_THREAD_RV_REF(typename ::hboost::remove_cv<typename ::hboost::remove_reference<T>::type>::type)
//  make_rv_ref(T &v)  HBOOST_NOEXCEPT
//  {
//    return (HBOOST_THREAD_RV_REF(typename ::hboost::remove_cv<typename ::hboost::remove_reference<T>::type>::type))(v);
//  }
//  template <typename T>
//  const HBOOST_THREAD_RV_REF(typename ::hboost::remove_cv<typename ::hboost::remove_reference<T>::type>::type)
//  make_rv_ref(T const&v)  HBOOST_NOEXCEPT
//  {
//    return (const HBOOST_THREAD_RV_REF(typename ::hboost::remove_cv<typename ::hboost::remove_reference<T>::type>::type))(v);
//  }
}
}

#define HBOOST_THREAD_MAKE_RV_REF(RVALUE) RVALUE.move()
//#define HBOOST_THREAD_MAKE_RV_REF(RVALUE) hboost::detail::make_rv_ref(RVALUE)
#endif


#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES

#define HBOOST_THREAD_MOVABLE(TYPE)

#define HBOOST_THREAD_COPYABLE(TYPE)

#else

#if defined HBOOST_THREAD_USES_MOVE

#define HBOOST_THREAD_MOVABLE(TYPE) \
    ::hboost::rv<TYPE>& move()  HBOOST_NOEXCEPT \
    { \
      return *static_cast< ::hboost::rv<TYPE>* >(this); \
    } \
    const ::hboost::rv<TYPE>& move() const HBOOST_NOEXCEPT \
    { \
      return *static_cast<const ::hboost::rv<TYPE>* >(this); \
    } \
    operator ::hboost::rv<TYPE>&() \
    { \
      return *static_cast< ::hboost::rv<TYPE>* >(this); \
    } \
    operator const ::hboost::rv<TYPE>&() const \
    { \
      return *static_cast<const ::hboost::rv<TYPE>* >(this); \
    }\

#define HBOOST_THREAD_COPYABLE(TYPE) \
  TYPE& operator=(TYPE &t)\
  {  this->operator=(static_cast<const ::hboost::rv<TYPE> &>(const_cast<const TYPE &>(t))); return *this;}


#else

#define HBOOST_THREAD_MOVABLE(TYPE) \
    operator ::hboost::detail::thread_move_t<TYPE>() HBOOST_NOEXCEPT \
    { \
        return move(); \
    } \
    ::hboost::detail::thread_move_t<TYPE> move() HBOOST_NOEXCEPT \
    { \
      ::hboost::detail::thread_move_t<TYPE> x(*this); \
        return x; \
    } \

#define HBOOST_THREAD_COPYABLE(TYPE)

#endif
#endif

#define HBOOST_THREAD_MOVABLE_ONLY(TYPE) \
  HBOOST_THREAD_NO_COPYABLE(TYPE) \
  HBOOST_THREAD_MOVABLE(TYPE) \
  typedef int hboost_move_no_copy_constructor_or_assign; \


#define HBOOST_THREAD_COPYABLE_AND_MOVABLE(TYPE) \
    HBOOST_THREAD_COPYABLE(TYPE) \
    HBOOST_THREAD_MOVABLE(TYPE) \



namespace hboost
{
  namespace thread_detail
  {

#if ! defined  HBOOST_NO_CXX11_RVALUE_REFERENCES
#elif defined HBOOST_THREAD_USES_MOVE
    template <class T>
    struct is_rv
       : ::hboost::move_detail::is_rv<T>
    {};

#else
    template <class T>
    struct is_rv
       : ::hboost::integral_constant<bool, false>
    {};

    template <class T>
    struct is_rv< ::hboost::detail::thread_move_t<T> >
       : ::hboost::integral_constant<bool, true>
    {};

    template <class T>
    struct is_rv< const ::hboost::detail::thread_move_t<T> >
       : ::hboost::integral_constant<bool, true>
    {};
#endif

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
    template <class Tp>
    struct remove_reference : hboost::remove_reference<Tp> {};
    template <class Tp>
    struct  decay : hboost::decay<Tp> {};
#else
  template <class Tp>
  struct remove_reference
  {
    typedef Tp type;
  };
  template <class Tp>
  struct remove_reference<Tp&>
  {
    typedef Tp type;
  };
  template <class Tp>
  struct remove_reference< rv<Tp> > {
    typedef Tp type;
  };

  template <class Tp>
  struct  decay
  {
  private:
    typedef typename hboost::move_detail::remove_rvalue_reference<Tp>::type Up0;
    typedef typename hboost::remove_reference<Up0>::type Up;
  public:
      typedef typename conditional
                       <
                           is_array<Up>::value,
                           typename remove_extent<Up>::type*,
                           typename conditional
                           <
                                is_function<Up>::value,
                                typename add_pointer<Up>::type,
                                typename remove_cv<Up>::type
                           >::type
                       >::type type;
  };
#endif

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
      template <class T>
      typename decay<T>::type
      decay_copy(T&& t)
      {
          return hboost::forward<T>(t);
      }
#else
  template <class T>
  typename decay<T>::type
  decay_copy(HBOOST_THREAD_FWD_REF(T) t)
  {
      return hboost::forward<T>(t);
  }
#endif
  }
}

#include <hboost/config/abi_suffix.hpp>

#endif
