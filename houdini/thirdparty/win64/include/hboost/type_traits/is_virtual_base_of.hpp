//  (C) Copyright Daniel Frey and Robert Ramey 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.
 
#ifndef HBOOST_TT_IS_VIRTUAL_BASE_OF_HPP_INCLUDED
#define HBOOST_TT_IS_VIRTUAL_BASE_OF_HPP_INCLUDED

#include <hboost/type_traits/is_base_of.hpp>
#include <hboost/type_traits/is_same.hpp>

namespace hboost {
namespace detail {


#ifdef HBOOST_MSVC
#pragma warning( push )
#pragma warning( disable : 4584 4250)
#elif defined(__GNUC__) && (__GNUC__ >= 4)
#pragma GCC system_header
#endif

template<typename Base, typename Derived, typename tag>
struct is_virtual_base_of_impl
{
    HBOOST_STATIC_CONSTANT(bool, value = false);
};

template<typename Base, typename Derived>
struct is_virtual_base_of_impl<Base, Derived, true_type>
{
   union max_align
   {
      unsigned u;
      unsigned long ul;
      void* v;
      double d;
      long double ld;
#ifndef HBOOST_NO_LONG_LONG
      long long ll;
#endif
   };
#ifdef __BORLANDC__
    struct hboost_type_traits_internal_struct_X : public virtual Derived, public virtual Base 
    {
       hboost_type_traits_internal_struct_X();
       hboost_type_traits_internal_struct_X(const hboost_type_traits_internal_struct_X&);
       hboost_type_traits_internal_struct_X& operator=(const hboost_type_traits_internal_struct_X&);
       ~hboost_type_traits_internal_struct_X()throw();
       max_align data[4];
    };
    struct hboost_type_traits_internal_struct_Y : public virtual Derived 
    {
       hboost_type_traits_internal_struct_Y();
       hboost_type_traits_internal_struct_Y(const hboost_type_traits_internal_struct_Y&);
       hboost_type_traits_internal_struct_Y& operator=(const hboost_type_traits_internal_struct_Y&);
       ~hboost_type_traits_internal_struct_Y()throw();
       max_align data[4];
    };
#else
    struct hboost_type_traits_internal_struct_X : public Derived, virtual Base 
    {
       hboost_type_traits_internal_struct_X();
       hboost_type_traits_internal_struct_X(const hboost_type_traits_internal_struct_X&);
       hboost_type_traits_internal_struct_X& operator=(const hboost_type_traits_internal_struct_X&);
       ~hboost_type_traits_internal_struct_X()throw();
       max_align data[16];
    };
    struct hboost_type_traits_internal_struct_Y : public Derived 
    {
       hboost_type_traits_internal_struct_Y();
       hboost_type_traits_internal_struct_Y(const hboost_type_traits_internal_struct_Y&);
       hboost_type_traits_internal_struct_Y& operator=(const hboost_type_traits_internal_struct_Y&);
       ~hboost_type_traits_internal_struct_Y()throw();
       max_align data[16];
    };
#endif
    HBOOST_STATIC_CONSTANT(bool, value = (sizeof(hboost_type_traits_internal_struct_X)==sizeof(hboost_type_traits_internal_struct_Y)));
};

template<typename Base, typename Derived>
struct is_virtual_base_of_impl2
{
   typedef hboost::integral_constant<bool, (hboost::is_base_of<Base, Derived>::value && ! hboost::is_same<Base, Derived>::value)> tag_type;
   typedef is_virtual_base_of_impl<Base, Derived, tag_type> imp;
   HBOOST_STATIC_CONSTANT(bool, value = imp::value);
};

#ifdef HBOOST_MSVC
#pragma warning( pop )
#endif

} // namespace detail

template <class Base, class Derived> struct is_virtual_base_of : public integral_constant<bool, (::hboost::detail::is_virtual_base_of_impl2<Base, Derived>::value)>{};

template <class Base, class Derived> struct is_virtual_base_of<Base&, Derived> : public false_type{};
template <class Base, class Derived> struct is_virtual_base_of<Base, Derived&> : public false_type{};
template <class Base, class Derived> struct is_virtual_base_of<Base&, Derived&> : public false_type{};

} // namespace hboost

#endif
