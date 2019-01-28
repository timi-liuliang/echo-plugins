
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_DETAIL_TAGS_HPP_INCLUDED
#define HBOOST_FT_DETAIL_TAGS_HPP_INCLUDED

#include <cstddef>

#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/mpl/bitxor.hpp>


namespace hboost { namespace function_types { 

namespace detail 
{
  typedef long bits_t;

  template<bits_t Value> struct constant 
    : hboost::integral_constant<bits_t,Value> 
  { };

  template<bits_t Bits, bits_t Mask> struct property_tag 
  {
    typedef constant<Bits> bits;
    typedef constant<Mask> mask;
  };

  template<typename T> struct bits : T::bits { };
  template<typename T> struct mask : T::mask { };

  // forward declaration, defined in pp_tags
  template<bits_t Bits, bits_t CCID> struct encode_bits_impl; 

  // forward declaration, defined in pp_tags
  template<bits_t LHS_bits, bits_t LHS_mask, 
           bits_t RHS_bits, bits_t RHS_mask> 
  struct tag_ice;
 
  // forward declaration, defined in retag_default_cc 
  template<class Tag, class RegTag = Tag> struct retag_default_cc; 
 
  template<bits_t Bits, bits_t CCID> struct encode_bits
    : constant< 
        ::hboost::function_types::detail::encode_bits_impl<Bits,CCID>::value 
      >
  { };

  template<class LHS, class RHS> struct compound_tag
  {
    typedef constant<
      ::hboost::function_types::detail::tag_ice
        < ::hboost::function_types::detail::bits<LHS>::value
        , ::hboost::function_types::detail::mask<LHS>::value
        , ::hboost::function_types::detail::bits<RHS>::value
        , ::hboost::function_types::detail::mask<RHS>::value
        >::combined_bits 
    > bits;

    typedef constant< 
      ::hboost::function_types::detail::tag_ice
        < ::hboost::function_types::detail::bits<LHS>::value
        , ::hboost::function_types::detail::mask<LHS>::value
        , ::hboost::function_types::detail::bits<RHS>::value
        , ::hboost::function_types::detail::mask<RHS>::value
        >::combined_mask 
    > mask; 
  };

  template <class Base, class PropOld, class PropNew>
  struct changed_tag
    : Base
  {
    typedef mpl::bitxor_
        <typename Base::bits, typename PropOld::bits, typename PropNew::bits>
    bits;
  };

  template<class Tag, class QueryTag> struct represents_impl
    : hboost::integral_constant<bool,
        ::hboost::function_types::detail::tag_ice
          < ::hboost::function_types::detail::bits<Tag>::value
          , ::hboost::function_types::detail::mask<Tag>::value
          , ::hboost::function_types::detail::bits<QueryTag>::value
          , ::hboost::function_types::detail::mask<QueryTag>::value
          >::match
      >
  { };

} // namespace detail

typedef detail::property_tag<0,0> null_tag;

template<class Tag1, class Tag2, class Tag3 = null_tag, class Tag4 = null_tag>
struct tag
  : detail::compound_tag< detail::compound_tag<Tag1,Tag2>, 
        detail::compound_tag<Tag3,Tag4> >
{ };

template<class Tag1, class Tag2, class Tag3> struct tag<Tag1,Tag2,Tag3,null_tag>
  : detail::compound_tag<detail::compound_tag<Tag1,Tag2>,Tag3>
{ };
template<class Tag1, class Tag2> struct tag<Tag1,Tag2,null_tag,null_tag>
  : detail::compound_tag<Tag1,Tag2>
{ };
template<class Tag1> struct tag<Tag1,null_tag,null_tag,null_tag>
  : Tag1
{ };


template<class Tag, class QueryTag> struct represents
  : detail::represents_impl<Tag, detail::retag_default_cc<QueryTag,Tag> >
{ };


template<class Tag, class QueryTag> struct extract
{ 
  typedef detail::constant<
    ::hboost::function_types::detail::tag_ice
      < ::hboost::function_types::detail::bits<Tag>::value
      , ::hboost::function_types::detail::mask<Tag>::value
      , ::hboost::function_types::detail::bits<QueryTag>::value
      , ::hboost::function_types::detail::mask<QueryTag>::value
      >::extracted_bits 
  > bits;

  typedef detail::constant< 
    ::hboost::function_types::detail::mask<QueryTag>::value
  > mask; 
};

} } // namespace ::hboost::function_types

#include <hboost/function_types/detail/pp_tags/preprocessed.hpp>

namespace hboost { namespace function_types {
#define HBOOST_FT_cc_file <hboost/function_types/detail/pp_tags/cc_tag.hpp>
#include <hboost/function_types/detail/pp_loop.hpp>
} } // namespace hboost::function_types

#endif

