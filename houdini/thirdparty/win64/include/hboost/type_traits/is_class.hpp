//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000-2003.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_IS_CLASS_HPP_INCLUDED
#define HBOOST_TT_IS_CLASS_HPP_INCLUDED

#include <hboost/type_traits/detail/config.hpp>
#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>
#ifndef HBOOST_IS_CLASS
#   include <hboost/type_traits/is_union.hpp>

#ifdef HBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION
#   include <hboost/type_traits/detail/yes_no_type.hpp>
#else
#   include <hboost/type_traits/is_scalar.hpp>
#   include <hboost/type_traits/is_array.hpp>
#   include <hboost/type_traits/is_reference.hpp>
#   include <hboost/type_traits/is_void.hpp>
#   include <hboost/type_traits/is_function.hpp>
#endif

#endif // HBOOST_IS_CLASS

namespace hboost {

namespace detail {

#ifndef HBOOST_IS_CLASS
#ifdef HBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION

// This is actually the conforming implementation which works with
// abstract classes.  However, enough compilers have trouble with
// it that most will use the one in
// hboost/type_traits/object_traits.hpp. This implementation
// actually works with VC7.0, but other interactions seem to fail
// when we use it.

// is_class<> metafunction due to Paul Mensonides
// (leavings@attbi.com). For more details:
// http://groups.google.com/groups?hl=en&selm=000001c1cc83%24e154d5e0%247772e50c%40c161550a&rnum=1
#if defined(__GNUC__)  && !defined(__EDG_VERSION__)

template <class U> ::hboost::type_traits::yes_type is_class_tester(void(U::*)(void));
template <class U> ::hboost::type_traits::no_type is_class_tester(...);

template <typename T>
struct is_class_impl
{

    HBOOST_STATIC_CONSTANT(bool, value =
            sizeof(is_class_tester<T>(0)) == sizeof(::hboost::type_traits::yes_type)
            && ! ::hboost::is_union<T>::value
        );
};

#else

template <typename T>
struct is_class_impl
{
    template <class U> static ::hboost::type_traits::yes_type is_class_tester(void(U::*)(void));
    template <class U> static ::hboost::type_traits::no_type is_class_tester(...);

    HBOOST_STATIC_CONSTANT(bool, value =
            sizeof(is_class_tester<T>(0)) == sizeof(::hboost::type_traits::yes_type)
            && ! ::hboost::is_union<T>::value
        );
};

#endif

#else

template <typename T>
struct is_class_impl
{
    HBOOST_STATIC_CONSTANT(bool, value =
        ! ::hboost::is_union<T>::value >::value
        && ! ::hboost::is_scalar<T>::value
        && ! ::hboost::is_array<T>::value
        && ! ::hboost::is_reference<T>::value
        && ! ::hboost::is_void<T>::value
        && ! ::hboost::is_function<T>::value
        );
};

# endif // HBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION
# else // HBOOST_IS_CLASS
template <typename T>
struct is_class_impl
{
    HBOOST_STATIC_CONSTANT(bool, value = HBOOST_IS_CLASS(T));
};
# endif // HBOOST_IS_CLASS

} // namespace detail

template <class T> struct is_class : public integral_constant<bool, ::hboost::detail::is_class_impl<T>::value> {};
# ifdef __EDG_VERSION__
template <class T> struct is_class<const T> : public is_class<T>{};
template <class T> struct is_class<const volatile T> : public is_class<T>{};
template <class T> struct is_class<volatile T> : public is_class<T>{};
# endif
    
} // namespace hboost

#endif // HBOOST_TT_IS_CLASS_HPP_INCLUDED
