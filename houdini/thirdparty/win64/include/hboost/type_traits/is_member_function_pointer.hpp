
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
#define HBOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED

#include <hboost/type_traits/detail/config.hpp>
#include <hboost/detail/workaround.hpp>

#if !HBOOST_WORKAROUND(__BORLANDC__, < 0x600) && !defined(HBOOST_TT_TEST_MS_FUNC_SIGS)
   //
   // Note: we use the "workaround" version for MSVC because it works for 
   // __stdcall etc function types, where as the partial specialisation
   // version does not do so.
   //
#   include <hboost/type_traits/detail/is_mem_fun_pointer_impl.hpp>
#   include <hboost/type_traits/remove_cv.hpp>
#   include <hboost/type_traits/integral_constant.hpp>
#else
#   include <hboost/type_traits/is_reference.hpp>
#   include <hboost/type_traits/is_array.hpp>
#   include <hboost/type_traits/detail/yes_no_type.hpp>
#   include <hboost/type_traits/detail/is_mem_fun_pointer_tester.hpp>
#endif

namespace hboost {

#if defined( __CODEGEARC__ )
template <class T> struct is_member_function_pointer : public integral_constant<bool, __is_member_function_pointer( T )> {};
#elif !HBOOST_WORKAROUND(__BORLANDC__, < 0x600) && !defined(HBOOST_TT_TEST_MS_FUNC_SIGS)

template <class T> struct is_member_function_pointer 
   : public ::hboost::integral_constant<bool, ::hboost::type_traits::is_mem_fun_pointer_impl<typename remove_cv<T>::type>::value>{};

#else

namespace detail {

#ifndef __BORLANDC__

template <bool>
struct is_mem_fun_pointer_select
{
   template <class T> struct result_ : public false_type{};
};

template <>
struct is_mem_fun_pointer_select<false>
{
    template <typename T> struct result_
    {
#if HBOOST_WORKAROUND(HBOOST_MSVC_FULL_VER, >= 140050000)
#pragma warning(push)
#pragma warning(disable:6334)
#endif
        static T* make_t;
        typedef result_<T> self_type;

        HBOOST_STATIC_CONSTANT(
            bool, value = (
                1 == sizeof(::hboost::type_traits::is_mem_fun_pointer_tester(self_type::make_t))
            ));
#if HBOOST_WORKAROUND(HBOOST_MSVC_FULL_VER, >= 140050000)
#pragma warning(pop)
#endif
    };
};

template <typename T>
struct is_member_function_pointer_impl
    : public is_mem_fun_pointer_select< 
      ::hboost::is_reference<T>::value || ::hboost::is_array<T>::value>::template result_<T>{};

template <typename T>
struct is_member_function_pointer_impl<T&> : public false_type{};

#else // Borland C++

template <typename T>
struct is_member_function_pointer_impl
{
   static T* m_t;
   HBOOST_STATIC_CONSTANT(
              bool, value =
               (1 == sizeof(type_traits::is_mem_fun_pointer_tester(m_t))) );
};

template <typename T>
struct is_member_function_pointer_impl<T&>
{
   HBOOST_STATIC_CONSTANT(bool, value = false);
};

#endif

template<> struct is_member_function_pointer_impl<void> : public false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template<> struct is_member_function_pointer_impl<void const> : public false_type{};
template<> struct is_member_function_pointer_impl<void const volatile> : public false_type{};
template<> struct is_member_function_pointer_impl<void volatile> : public false_type{};
#endif

} // namespace detail

template <class T>
struct is_member_function_pointer
   : public integral_constant<bool, ::hboost::detail::is_member_function_pointer_impl<T>::value>{};

#endif

} // namespace hboost

#endif // HBOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
