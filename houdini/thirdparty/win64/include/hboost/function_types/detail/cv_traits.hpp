
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_DETAIL_CV_TRAITS_HPP_INCLUDED
#define HBOOST_FT_DETAIL_CV_TRAITS_HPP_INCLUDED

#include <cstddef>
#include <hboost/detail/workaround.hpp>

#if HBOOST_WORKAROUND(__BORLANDC__, <= 0x582)
#   include <hboost/type_traits/remove_cv.hpp>
#   include <hboost/type_traits/remove_pointer.hpp>
#   include <hboost/type_traits/remove_reference.hpp>
#endif

#include <hboost/function_types/property_tags.hpp>

namespace hboost { namespace function_types { namespace detail {

#if !HBOOST_WORKAROUND(__BORLANDC__, <= 0x582)

template<typename T> struct cv_traits 
{ typedef non_cv tag; typedef T type; };
template<typename T> struct cv_traits<T &>     
{ typedef non_cv tag; typedef T type; };
template<typename T> struct cv_traits<T *>    
{ typedef non_cv tag; typedef T type; };
template<typename T> struct cv_traits<T * const> 
{ typedef non_cv tag; typedef T type; };
template<typename T> struct cv_traits<T * volatile> 
{ typedef non_cv tag; typedef T type; };
template<typename T> struct cv_traits<T * const volatile> 
{ typedef non_cv tag; typedef T type; };

template<typename T> struct cv_traits<T const> 
{ typedef const_non_volatile tag; typedef T type; };
template<typename T> struct cv_traits<T const &>
{ typedef const_non_volatile tag; typedef T type; };
template<typename T> struct cv_traits<T const *> 
{ typedef const_non_volatile tag; typedef T type; };
template<typename T> struct cv_traits<T const * const> 
{ typedef const_non_volatile tag; typedef T type; };
template<typename T> struct cv_traits<T const * volatile> 
{ typedef const_non_volatile tag; typedef T type; };
template<typename T> struct cv_traits<T const * const volatile> 
{ typedef const_non_volatile tag; typedef T type; };

template<typename T> struct cv_traits<T volatile>  
{ typedef volatile_non_const tag; typedef T type; };
template<typename T> struct cv_traits<T volatile &>  
{ typedef volatile_non_const tag; typedef T type; };
template<typename T> struct cv_traits<T volatile *> 
{ typedef volatile_non_const tag; typedef T type; };
template<typename T> struct cv_traits<T volatile * const> 
{ typedef volatile_non_const tag; typedef T type; };
template<typename T> struct cv_traits<T volatile * volatile> 
{ typedef volatile_non_const tag; typedef T type; };
template<typename T> struct cv_traits<T volatile * const volatile> 
{ typedef volatile_non_const tag; typedef T type; };

template<typename T> struct cv_traits<T const volatile>   
{ typedef cv_qualified tag; typedef T type; };
template<typename T> struct cv_traits<T const volatile &>
{ typedef cv_qualified tag; typedef T type; };
template<typename T> struct cv_traits<T const volatile *>
{ typedef cv_qualified tag; typedef T type; };
template<typename T> struct cv_traits<T const volatile * const>
{ typedef cv_qualified tag; typedef T type; };
template<typename T> struct cv_traits<T const volatile * volatile>
{ typedef cv_qualified tag; typedef T type; };
template<typename T> struct cv_traits<T const volatile * const volatile>
{ typedef cv_qualified tag; typedef T type; };

#else
template<std::size_t> struct cv_tag_impl;

template<> struct cv_tag_impl<1> { typedef non_cv type;};
template<> struct cv_tag_impl<2> { typedef const_non_volatile type; };
template<> struct cv_tag_impl<3> { typedef volatile_non_const type; };
template<> struct cv_tag_impl<4> { typedef cv_qualified type; };

typedef char (& case_1)[1];
typedef char (& case_2)[2];
typedef char (& case_3)[3];
typedef char (& case_4)[4];

template<typename T> case_1 switch_cv(T *);
template<typename T> case_2 switch_cv(T const *);
template<typename T> case_3 switch_cv(T volatile *);
template<typename T> case_4 switch_cv(T const volatile *);

template<typename T> T                * ref_to_ptr(T &);
template<typename T> T const          * ref_to_ptr(T const &);
template<typename T> T volatile       * ref_to_ptr(T volatile &);
template<typename T> T const volatile * ref_to_ptr(T const volatile &);

template<typename T> T                * ref_to_ptr(T * const volatile &);

template<typename T>
struct cv_code
{
  static T _t;
  HBOOST_STATIC_CONSTANT(std::size_t, value = 
      sizeof(::hboost::function_types::detail::switch_cv(
         ::hboost::function_types::detail::ref_to_ptr(_t) ) ));
};

template<typename T> struct cv_traits 
{
  typedef typename hboost::function_types::detail::cv_tag_impl< 
    ::hboost::function_types::detail::cv_code<T>::value >::type
  tag;

  // may require Boost.TypeTraits broken compiler specializations
  // to work
  typedef typename hboost::remove_cv<
              typename hboost::remove_pointer<
                  typename hboost::remove_reference<T>::type 
              >::type 
          >::type type; 
};
#endif

} } } // namespace hboost::function_types::detail

#endif
 
