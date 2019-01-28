
//  Copyright 2000 John Maddock (john@johnmaddock.co.uk)
//  Copyright 2002 Aleksey Gurtovoy (agurtovoy@meta-comm.com)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_FUNCTION_HPP_INCLUDED
#define HBOOST_TT_IS_FUNCTION_HPP_INCLUDED

#include <hboost/type_traits/is_reference.hpp>
#include <hboost/type_traits/detail/config.hpp>

#if !defined(HBOOST_TT_TEST_MS_FUNC_SIGS)
#   include <hboost/type_traits/detail/is_function_ptr_helper.hpp>
#else
#   include <hboost/type_traits/detail/is_function_ptr_tester.hpp>
#   include <hboost/type_traits/detail/yes_no_type.hpp>
#endif

// is a type a function?
// Please note that this implementation is unnecessarily complex:
// we could just use !is_convertible<T*, const volatile void*>::value,
// except that some compilers erroneously allow conversions from
// function pointers to void*.

namespace hboost {

#if !defined( __CODEGEARC__ )

namespace detail {

#if !defined(HBOOST_TT_TEST_MS_FUNC_SIGS)
template<bool is_ref = true>
struct is_function_chooser
{
   template< typename T > struct result_
      : public false_type {};
};

template <>
struct is_function_chooser<false>
{
    template< typename T > struct result_
        : public ::hboost::type_traits::is_function_ptr_helper<T*> {};
};

template <typename T>
struct is_function_impl
    : public is_function_chooser< ::hboost::is_reference<T>::value >
        ::HBOOST_NESTED_TEMPLATE result_<T>
{
};

#else

template <typename T>
struct is_function_impl
{
#if HBOOST_WORKAROUND(HBOOST_MSVC_FULL_VER, >= 140050000)
#pragma warning(push)
#pragma warning(disable:6334)
#endif
    static T* t;
    HBOOST_STATIC_CONSTANT(
        bool, value = sizeof(::hboost::type_traits::is_function_ptr_tester(t))
        == sizeof(::hboost::type_traits::yes_type)
        );
#if HBOOST_WORKAROUND(HBOOST_MSVC_FULL_VER, >= 140050000)
#pragma warning(pop)
#endif
};

template <typename T>
struct is_function_impl<T&> : public false_type
{};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
template <typename T>
struct is_function_impl<T&&> : public false_type
{};
#endif

#endif

} // namespace detail

#endif // !defined( __CODEGEARC__ )

#if defined( __CODEGEARC__ )
template <class T> struct is_function : integral_constant<bool, __is_function(T)> {};
#else
template <class T> struct is_function : integral_constant<bool, ::hboost::detail::is_function_impl<T>::value> {};
#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
template <class T> struct is_function<T&&> : public false_type {};
#endif
#endif
} // namespace hboost

#endif // HBOOST_TT_IS_FUNCTION_HPP_INCLUDED
