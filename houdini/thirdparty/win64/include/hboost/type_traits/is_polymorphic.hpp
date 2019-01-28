//  (C) Copyright John Maddock 2000. 
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_IS_POLYMORPHIC_HPP
#define HBOOST_TT_IS_POLYMORPHIC_HPP

#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>
#ifndef HBOOST_IS_POLYMORPHIC
#include <hboost/type_traits/is_class.hpp>
#endif
#include <hboost/detail/workaround.hpp>

#if defined(HBOOST_MSVC) && (HBOOST_MSVC >= 1700)
#pragma warning(push)
#pragma warning(disable:4250)
#endif

namespace hboost{

#ifndef HBOOST_IS_POLYMORPHIC

namespace detail{

template <class T>
struct is_polymorphic_imp1
{
# if HBOOST_WORKAROUND(__MWERKS__, <= 0x2407) // CWPro7 should return false always.
    typedef char d1, (&d2)[2];
# else 
   struct d1 : public T
   {
      d1();
#  if !defined(__GNUC__) // this raises warnings with some classes, and buys nothing with GCC
      ~d1()throw();
#  endif 
      char padding[256];
   private:
      // keep some picky compilers happy:
      d1(const d1&);
      d1& operator=(const d1&);
   };
   struct d2 : public T
   {
      d2();
      virtual ~d2()throw();
#  if !defined(HBOOST_MSVC) && !defined(__ICL)
      // for some reason this messes up VC++ when T has virtual bases,
      // probably likewise for compilers that use the same ABI:
      struct unique{};
      virtual void unique_name_to_boost5487629(unique*);
#  endif
      char padding[256];
   private:
      // keep some picky compilers happy:
      d2(const d2&);
      d2& operator=(const d2&);
   };
# endif 
   HBOOST_STATIC_CONSTANT(bool, value = (sizeof(d2) == sizeof(d1)));
};

template <class T> struct is_polymorphic_imp1<T const> : public is_polymorphic_imp1<T>{};
template <class T> struct is_polymorphic_imp1<T const volatile> : public is_polymorphic_imp1<T>{};
template <class T> struct is_polymorphic_imp1<T volatile> : public is_polymorphic_imp1<T>{};

template <class T>
struct is_polymorphic_imp2
{
   HBOOST_STATIC_CONSTANT(bool, value = false);
};

template <bool is_class>
struct is_polymorphic_selector
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp2<T> type;
   };
};

template <>
struct is_polymorphic_selector<true>
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp1<T> type;
   };
};

template <class T>
struct is_polymorphic_imp
{
   typedef is_polymorphic_selector< ::hboost::is_class<T>::value> selector;
   typedef typename selector::template rebind<T> binder;
   typedef typename binder::type imp_type;
   HBOOST_STATIC_CONSTANT(bool, value = imp_type::value);
};

} // namespace detail

template <class T> struct is_polymorphic : public integral_constant<bool, ::hboost::detail::is_polymorphic_imp<T>::value> {};

#else // HBOOST_IS_POLYMORPHIC

template <class T> struct is_polymorphic : public integral_constant<bool, HBOOST_IS_POLYMORPHIC(T)> {};

#endif

} // namespace hboost

#if defined(HBOOST_MSVC) && (HBOOST_MSVC >= 1700)
#pragma warning(pop)
#endif

#endif
