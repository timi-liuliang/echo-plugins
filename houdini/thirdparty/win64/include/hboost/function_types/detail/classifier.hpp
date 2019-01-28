
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_DETAIL_CLASSIFIER_HPP_INCLUDED
#define HBOOST_FT_DETAIL_CLASSIFIER_HPP_INCLUDED

#include <hboost/type.hpp>
#include <hboost/config.hpp>
#include <hboost/type_traits/is_reference.hpp>
#include <hboost/type_traits/add_reference.hpp>

#include <hboost/function_types/config/config.hpp>
#include <hboost/function_types/property_tags.hpp>

namespace hboost { namespace function_types { namespace detail {

template<typename T> struct classifier;

template<std::size_t S> struct char_array { typedef char (&type)[S]; };

template<bits_t Flags, bits_t CCID, std::size_t Arity> struct encode_charr
{
  typedef typename char_array<
    ::hboost::function_types::detail::encode_charr_impl<Flags,CCID,Arity>::value 
  >::type type;
};

#if defined(HBOOST_MSVC) || (defined(__BORLANDC__) && !defined(HBOOST_DISABLE_WIN32))
#   define HBOOST_FT_DECL __cdecl
#else
#   define HBOOST_FT_DECL /**/
#endif

char HBOOST_FT_DECL classifier_impl(...);

#define HBOOST_FT_variations HBOOST_FT_function|HBOOST_FT_pointer|\
                            HBOOST_FT_member_pointer

#define HBOOST_FT_type_function(cc,name) HBOOST_FT_SYNTAX( \
    R HBOOST_PP_EMPTY,HBOOST_PP_LPAREN,cc,* HBOOST_PP_EMPTY,name,HBOOST_PP_RPAREN)

#define HBOOST_FT_type_function_pointer(cc,name) HBOOST_FT_SYNTAX( \
    R HBOOST_PP_EMPTY,HBOOST_PP_LPAREN,cc,** HBOOST_PP_EMPTY,name,HBOOST_PP_RPAREN)

#define HBOOST_FT_type_member_function_pointer(cc,name) HBOOST_FT_SYNTAX( \
    R HBOOST_PP_EMPTY,HBOOST_PP_LPAREN,cc,T0::** HBOOST_PP_EMPTY,name,HBOOST_PP_RPAREN)

#define HBOOST_FT_al_path hboost/function_types/detail/classifier_impl
#include <hboost/function_types/detail/pp_loop.hpp>

template<typename T> struct classifier_bits
{
  static typename hboost::add_reference<T>::type tester;

  HBOOST_STATIC_CONSTANT(bits_t,value = (bits_t)sizeof(
    hboost::function_types::detail::classifier_impl(& tester) 
  )-1);
};

template<typename T> struct classifier
{
  typedef detail::constant<
    ::hboost::function_types::detail::decode_bits<
      ::hboost::function_types::detail::classifier_bits<T>::value
    >::tag_bits > 
  bits;

  typedef detail::full_mask mask;
 
  typedef detail::constant<
    ::hboost::function_types::detail::decode_bits<
      ::hboost::function_types::detail::classifier_bits<T>::value
    >::arity > 
  function_arity;
};



} } } // namespace ::hboost::function_types::detail

#endif

