
// (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_EMPTY_HPP_INCLUDED
#define HBOOST_TT_IS_EMPTY_HPP_INCLUDED

#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/type_traits/detail/config.hpp>
#include <hboost/type_traits/intrinsics.hpp>

#include <hboost/type_traits/remove_cv.hpp>
#include <hboost/type_traits/is_class.hpp>
#include <hboost/type_traits/add_reference.hpp>

#ifndef HBOOST_INTERNAL_IS_EMPTY
#define HBOOST_INTERNAL_IS_EMPTY(T) false
#else
#define HBOOST_INTERNAL_IS_EMPTY(T) HBOOST_IS_EMPTY(T)
#endif

namespace hboost {

namespace detail {


#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4624) // destructor could not be generated
#endif

template <typename T>
struct empty_helper_t1 : public T
{
    empty_helper_t1();  // hh compiler bug workaround
    int i[256];
private:
   // suppress compiler warnings:
   empty_helper_t1(const empty_helper_t1&);
   empty_helper_t1& operator=(const empty_helper_t1&);
};

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

struct empty_helper_t2 { int i[256]; };

#if !HBOOST_WORKAROUND(__BORLANDC__, < 0x600)

template <typename T, bool is_a_class = false>
struct empty_helper
{
    HBOOST_STATIC_CONSTANT(bool, value = false);
};

template <typename T>
struct empty_helper<T, true>
{
    HBOOST_STATIC_CONSTANT(
        bool, value = (sizeof(empty_helper_t1<T>) == sizeof(empty_helper_t2))
        );
};

template <typename T>
struct is_empty_impl
{
    typedef typename remove_cv<T>::type cvt;
    HBOOST_STATIC_CONSTANT(
        bool, 
        value = ( ::hboost::detail::empty_helper<cvt,::hboost::is_class<T>::value>::value || HBOOST_INTERNAL_IS_EMPTY(cvt)));
};

#else // __BORLANDC__

template <typename T, bool is_a_class, bool convertible_to_int>
struct empty_helper
{
    HBOOST_STATIC_CONSTANT(bool, value = false);
};

template <typename T>
struct empty_helper<T, true, false>
{
    HBOOST_STATIC_CONSTANT(bool, value = (
        sizeof(empty_helper_t1<T>) == sizeof(empty_helper_t2)
        ));
};

template <typename T>
struct is_empty_impl
{
   typedef typename remove_cv<T>::type cvt;
   typedef typename add_reference<T>::type r_type;

   HBOOST_STATIC_CONSTANT(
       bool, value = (
              ::hboost::detail::empty_helper<
                  cvt
                , ::hboost::is_class<T>::value
                , ::hboost::is_convertible< r_type,int>::value
              >::value || HBOOST_INTERNAL_IS_EMPTY(cvt));
};

#endif // __BORLANDC__

} // namespace detail

template <class T> struct is_empty : integral_constant<bool, ::hboost::detail::is_empty_impl<T>::value> {};

} // namespace hboost

#undef HBOOST_INTERNAL_IS_EMPTY

#endif // HBOOST_TT_IS_EMPTY_HPP_INCLUDED

