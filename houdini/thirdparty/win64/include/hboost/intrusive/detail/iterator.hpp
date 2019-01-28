/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2014-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_ITERATOR_HPP
#define HBOOST_INTRUSIVE_DETAIL_ITERATOR_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <cstddef>
#include <hboost/intrusive/detail/std_fwd.hpp>
#include <hboost/intrusive/detail/workaround.hpp>
#include <hboost/move/detail/iterator_traits.hpp>
#include <hboost/move/detail/meta_utils_core.hpp>

namespace hboost {
namespace intrusive {

using hboost::movelib::iterator_traits;

////////////////////
//    iterator
////////////////////
template<class Category, class T, class Difference, class Pointer, class Reference>
struct iterator
{
   typedef Category     iterator_category;
   typedef T            value_type;
   typedef Difference   difference_type;
   typedef Pointer      pointer;
   typedef Reference    reference;
};

////////////////////////////////////////
//    iterator_[dis|en]able_if_tag
////////////////////////////////////////
template<class I, class Tag, class R = void>
struct iterator_enable_if_tag
   : ::hboost::move_detail::enable_if_c
      < ::hboost::move_detail::is_same
         < typename hboost::intrusive::iterator_traits<I>::iterator_category 
         , Tag
         >::value
         , R>
{};

template<class I, class Tag, class R = void>
struct iterator_disable_if_tag
   : ::hboost::move_detail::enable_if_c
      < !::hboost::move_detail::is_same
         < typename hboost::intrusive::iterator_traits<I>::iterator_category 
         , Tag
         >::value
         , R>
{};

////////////////////////////////////////
//    iterator_[dis|en]able_if_tag_difference_type
////////////////////////////////////////
template<class I, class Tag>
struct iterator_enable_if_tag_difference_type
   : iterator_enable_if_tag<I, Tag, typename hboost::intrusive::iterator_traits<I>::difference_type>
{};

template<class I, class Tag>
struct iterator_disable_if_tag_difference_type
   : iterator_disable_if_tag<I, Tag, typename hboost::intrusive::iterator_traits<I>::difference_type>
{};

////////////////////
//    advance
////////////////////
template<class InputIt, class Distance>
typename iterator_enable_if_tag<InputIt, std::input_iterator_tag>::type
   iterator_advance(InputIt& it, Distance n)
{
   while(n--)
	   ++it;
}

template<class InputIt, class Distance>
typename iterator_enable_if_tag<InputIt, std::forward_iterator_tag>::type
   iterator_advance(InputIt& it, Distance n)
{
   while(n--)
	   ++it;
}

template<class InputIt, class Distance>
typename iterator_enable_if_tag<InputIt, std::bidirectional_iterator_tag>::type
   iterator_advance(InputIt& it, Distance n)
{
   for (; 0 < n; --n)
	   ++it;
   for (; n < 0; ++n)
	   --it;
}

template<class InputIt, class Distance>
HBOOST_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag<InputIt, std::random_access_iterator_tag>::type
   iterator_advance(InputIt& it, Distance n)
{
   it += n;
}

////////////////////
//    distance
////////////////////
template<class InputIt> inline
typename iterator_disable_if_tag_difference_type
   <InputIt, std::random_access_iterator_tag>::type
      iterator_distance(InputIt first, InputIt last)
{
   typename iterator_traits<InputIt>::difference_type off = 0;
   while(first != last){
	   ++off;
      ++first;
   }
   return off;
}

template<class InputIt>
HBOOST_INTRUSIVE_FORCEINLINE typename iterator_enable_if_tag_difference_type
   <InputIt, std::random_access_iterator_tag>::type
      iterator_distance(InputIt first, InputIt last)
{
   typename iterator_traits<InputIt>::difference_type off = last - first;
   return off;
}

template<class I>
HBOOST_INTRUSIVE_FORCEINLINE typename iterator_traits<I>::pointer iterator_arrow_result(const I &i)
{  return i.operator->();  }

template<class T>
HBOOST_INTRUSIVE_FORCEINLINE T * iterator_arrow_result(T *p)
{  return p;   }

} //namespace intrusive
} //namespace hboost

#endif //HBOOST_INTRUSIVE_DETAIL_ITERATOR_HPP
