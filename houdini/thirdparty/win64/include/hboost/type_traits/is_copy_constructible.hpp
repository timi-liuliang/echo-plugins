//  (C) Copyright Antony Polukhin 2013.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_COPY_CONSTRUCTIBLE_HPP_INCLUDED
#define HBOOST_TT_IS_COPY_CONSTRUCTIBLE_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_DECLTYPE) && !HBOOST_WORKAROUND(HBOOST_MSVC, < 1800) && !HBOOST_WORKAROUND(HBOOST_GCC_VERSION, < 40900)

#include <hboost/type_traits/is_constructible.hpp>

#if !HBOOST_WORKAROUND(HBOOST_MSVC, <= 1800)

namespace hboost {

template <class T> struct is_copy_constructible : public hboost::is_constructible<T, const T&>{};

template <> struct is_copy_constructible<void> : public false_type{};
template <> struct is_copy_constructible<void const> : public false_type{};
template <> struct is_copy_constructible<void const volatile> : public false_type{};
template <> struct is_copy_constructible<void volatile> : public false_type{};

} // namespace hboost

#else
//
// Special version for VC12 which has a problem when a base class (such as non_copyable) has a deleted
// copy constructor.  In this case the compiler thinks there really is a copy-constructor and tries to
// instantiate the deleted member.  std::is_copy_constructible has the same issue (or at least returns
// an incorrect value, which just defers the issue into the users code) as well.  We can at least fix
// hboost::non_copyable as a base class as a special case:
//
#include <hboost/type_traits/is_base_and_derived.hpp>
#include <hboost/noncopyable.hpp>

namespace hboost {

   namespace detail
   {

      template <class T, bool b> struct is_copy_constructible_imp : public hboost::is_constructible<T, const T&>{};
      template <class T> struct is_copy_constructible_imp<T, true> : public false_type{};

   }

   template <class T> struct is_copy_constructible : public detail::is_copy_constructible_imp<T, is_base_and_derived<hboost::noncopyable, T>::value>{};

   template <> struct is_copy_constructible<void> : public false_type{};
   template <> struct is_copy_constructible<void const> : public false_type{};
   template <> struct is_copy_constructible<void const volatile> : public false_type{};
   template <> struct is_copy_constructible<void volatile> : public false_type{};

} // namespace hboost

#endif

#else

#include <hboost/type_traits/detail/yes_no_type.hpp>
#include <hboost/type_traits/is_base_and_derived.hpp>
#include <hboost/type_traits/add_reference.hpp>
#include <hboost/type_traits/is_rvalue_reference.hpp>
#include <hboost/type_traits/declval.hpp>
#include <hboost/type_traits/is_array.hpp>
#include <hboost/type_traits/declval.hpp>
#include <hboost/noncopyable.hpp>

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4181)
#endif

namespace hboost {

   namespace detail{

      template <bool DerivedFromNoncopyable, class T>
      struct is_copy_constructible_impl2 {

         // Intel compiler has problems with SFINAE for copy constructors and deleted functions:
         //
         // error: function *function_name* cannot be referenced -- it is a deleted function
         // static hboost::type_traits::yes_type test(T1&, decltype(T1(hboost::declval<T1&>()))* = 0);
         //                                                        ^ 
         //
         // MSVC 12.0 (Visual 2013) has problems when the copy constructor has been deleted. See:
         // https://connect.microsoft.com/VisualStudio/feedback/details/800328/std-is-copy-constructible-is-broken
#if !defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS) && !defined(HBOOST_INTEL_CXX_VERSION) && !(defined(HBOOST_MSVC) && _MSC_VER == 1800)

#ifdef HBOOST_NO_CXX11_DECLTYPE
         template <class T1>
         static hboost::type_traits::yes_type test(const T1&, hboost::mpl::int_<sizeof(T1(hboost::declval<const T1&>()))>* = 0);
#else
         template <class T1>
         static hboost::type_traits::yes_type test(const T1&, decltype(T1(hboost::declval<const T1&>()))* = 0);
#endif

         static hboost::type_traits::no_type test(...);
#else
         template <class T1>
         static hboost::type_traits::no_type test(const T1&, typename T1::hboost_move_no_copy_constructor_or_assign* = 0);
         static hboost::type_traits::yes_type test(...);
#endif

         // If you see errors like this:
         //
         //      `'T::T(const T&)' is private`
         //      `hboost/type_traits/is_copy_constructible.hpp:68:5: error: within this context`
         //
         // then you are trying to call that macro for a structure defined like that:
         //
         //      struct T {
         //          ...
         //      private:
         //          T(const T &);
         //          ...
         //      };
         //
         // To fix that you must modify your structure:
         //
         //      // C++03 and C++11 version
         //      struct T: private hboost::noncopyable {
         //          ...
         //      private:
         //          T(const T &);
         //          ...
         //      };
         //
         //      // C++11 version
         //      struct T {
         //          ...
         //      private:
         //          T(const T &) = delete;
         //          ...
         //      };
         HBOOST_STATIC_CONSTANT(bool, value = (
            sizeof(test(
            hboost::declval<HBOOST_DEDUCED_TYPENAME hboost::add_reference<T const>::type>()
            )) == sizeof(hboost::type_traits::yes_type)
            &&
            !hboost::is_rvalue_reference<T>::value
            && !hboost::is_array<T>::value
            ));
      };

      template <class T>
      struct is_copy_constructible_impl2<true, T> {
         HBOOST_STATIC_CONSTANT(bool, value = false);
      };

      template <class T>
      struct is_copy_constructible_impl {

         HBOOST_STATIC_CONSTANT(bool, value = (
            hboost::detail::is_copy_constructible_impl2<
            hboost::is_base_and_derived<hboost::noncopyable, T>::value,
            T
            >::value
            ));
      };

   } // namespace detail

   template <class T> struct is_copy_constructible : public integral_constant<bool, ::hboost::detail::is_copy_constructible_impl<T>::value>{};
   template <> struct is_copy_constructible<void> : public false_type{};
#ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
   template <> struct is_copy_constructible<void const> : public false_type{};
   template <> struct is_copy_constructible<void volatile> : public false_type{};
   template <> struct is_copy_constructible<void const volatile> : public false_type{};
#endif

} // namespace hboost

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#endif

#endif // HBOOST_TT_IS_COPY_CONSTRUCTIBLE_HPP_INCLUDED
