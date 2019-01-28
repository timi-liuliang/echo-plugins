
//  (C) Copyright John Maddock 2005.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_IS_UNSIGNED_HPP_INCLUDED
#define HBOOST_TT_IS_UNSIGNED_HPP_INCLUDED

#include <hboost/type_traits/is_integral.hpp>
#include <hboost/type_traits/is_enum.hpp>
#include <hboost/type_traits/remove_cv.hpp>

#include <climits>

namespace hboost {

#if !defined( __CODEGEARC__ )

#if !(defined(HBOOST_MSVC) && HBOOST_MSVC <= 1310) &&\
    !(defined(__EDG_VERSION__) && __EDG_VERSION__ <= 238) &&\
    !defined(HBOOST_NO_INCLASS_MEMBER_INITIALIZATION)

namespace detail{

template <class T>
struct is_unsigned_values
{
   //
   // Note that we cannot use HBOOST_STATIC_CONSTANT here, using enum's
   // rather than "real" static constants simply doesn't work or give
   // the correct answer.
   //
   typedef typename remove_cv<T>::type no_cv_t;
   static const no_cv_t minus_one = (static_cast<no_cv_t>(-1));
   static const no_cv_t zero = (static_cast<no_cv_t>(0));
};

template <class T>
struct is_ununsigned_helper
{
   HBOOST_STATIC_CONSTANT(bool, value = (::hboost::detail::is_unsigned_values<T>::minus_one > ::hboost::detail::is_unsigned_values<T>::zero));
};

template <bool integral_type>
struct is_unsigned_select_helper
{
   template <class T>
   struct rebind
   {
      typedef is_ununsigned_helper<T> type;
   };
};

template <>
struct is_unsigned_select_helper<false>
{
   template <class T>
   struct rebind
   {
      typedef false_type type;
   };
};

template <class T>
struct is_unsigned
{
   typedef ::hboost::detail::is_unsigned_select_helper< ::hboost::is_integral<T>::value || ::hboost::is_enum<T>::value > selector;
   typedef typename selector::template rebind<T> binder;
   typedef typename binder::type type;
   HBOOST_STATIC_CONSTANT(bool, value = type::value);
};

} // namespace detail

template <class T> struct is_unsigned : public integral_constant<bool, hboost::detail::is_unsigned<T>::value> {};

#else

template <class T> struct is_unsigned : public false_type{};

#endif

#else // defined( __CODEGEARC__ )
template <class T> struct is_unsigned : public integral_constant<bool, __is_unsigned(T)> {};
#endif

template <> struct is_unsigned<unsigned char> : public true_type{};
template <> struct is_unsigned<const unsigned char> : public true_type{};
template <> struct is_unsigned<volatile unsigned char> : public true_type{};
template <> struct is_unsigned<const volatile unsigned char> : public true_type{};
template <> struct is_unsigned<unsigned short> : public true_type{};
template <> struct is_unsigned<const unsigned short> : public true_type{};
template <> struct is_unsigned<volatile unsigned short> : public true_type{};
template <> struct is_unsigned<const volatile unsigned short> : public true_type{};
template <> struct is_unsigned<unsigned int> : public true_type{};
template <> struct is_unsigned<const unsigned int> : public true_type{};
template <> struct is_unsigned<volatile unsigned int> : public true_type{};
template <> struct is_unsigned<const volatile unsigned int> : public true_type{};
template <> struct is_unsigned<unsigned long> : public true_type{};
template <> struct is_unsigned<const unsigned long> : public true_type{};
template <> struct is_unsigned<volatile unsigned long> : public true_type{};
template <> struct is_unsigned<const volatile unsigned long> : public true_type{};

template <> struct is_unsigned<signed char> : public false_type{};
template <> struct is_unsigned<const signed char> : public false_type{};
template <> struct is_unsigned<volatile signed char> : public false_type{};
template <> struct is_unsigned<const volatile signed char> : public false_type{};
template <> struct is_unsigned< short> : public false_type{};
template <> struct is_unsigned<const  short> : public false_type{};
template <> struct is_unsigned<volatile  short> : public false_type{};
template <> struct is_unsigned<const volatile  short> : public false_type{};
template <> struct is_unsigned< int> : public false_type{};
template <> struct is_unsigned<const  int> : public false_type{};
template <> struct is_unsigned<volatile  int> : public false_type{};
template <> struct is_unsigned<const volatile  int> : public false_type{};
template <> struct is_unsigned< long> : public false_type{};
template <> struct is_unsigned<const  long> : public false_type{};
template <> struct is_unsigned<volatile  long> : public false_type{};
template <> struct is_unsigned<const volatile  long> : public false_type{};
#ifdef HBOOST_HAS_LONG_LONG
template <> struct is_unsigned< ::hboost::ulong_long_type> : public true_type{};
template <> struct is_unsigned<const ::hboost::ulong_long_type> : public true_type{};
template <> struct is_unsigned<volatile ::hboost::ulong_long_type> : public true_type{};
template <> struct is_unsigned<const volatile ::hboost::ulong_long_type> : public true_type{};

template <> struct is_unsigned< ::hboost::long_long_type> : public false_type{};
template <> struct is_unsigned<const ::hboost::long_long_type> : public false_type{};
template <> struct is_unsigned<volatile ::hboost::long_long_type> : public false_type{};
template <> struct is_unsigned<const volatile ::hboost::long_long_type> : public false_type{};
#endif
#if defined(CHAR_MIN) 
#if CHAR_MIN == 0
template <> struct is_unsigned<char> : public true_type{};
template <> struct is_unsigned<const char> : public true_type{};
template <> struct is_unsigned<volatile char> : public true_type{};
template <> struct is_unsigned<const volatile char> : public true_type{};
#else
template <> struct is_unsigned<char> : public false_type{};
template <> struct is_unsigned<const char> : public false_type{};
template <> struct is_unsigned<volatile char> : public false_type{};
template <> struct is_unsigned<const volatile char> : public false_type{};
#endif
#endif
#if !defined(HBOOST_NO_INTRINSIC_WCHAR_T) && defined(WCHAR_MIN)
#if WCHAR_MIN == 0
template <> struct is_unsigned<wchar_t> : public true_type{};
template <> struct is_unsigned<const wchar_t> : public true_type{};
template <> struct is_unsigned<volatile wchar_t> : public true_type{};
template <> struct is_unsigned<const volatile wchar_t> : public true_type{};
#else
template <> struct is_unsigned<wchar_t> : public false_type{};
template <> struct is_unsigned<const wchar_t> : public false_type{};
template <> struct is_unsigned<volatile wchar_t> : public false_type{};
template <> struct is_unsigned<const volatile wchar_t> : public false_type{};
#endif
#endif
} // namespace hboost

#endif // HBOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
