//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_INTRUSIVE_DETAIL_TREE_VALUE_COMPARE_HPP
#define HBOOST_INTRUSIVE_DETAIL_TREE_VALUE_COMPARE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/workaround.hpp>
#include <hboost/intrusive/detail/mpl.hpp>
#include <hboost/intrusive/detail/ebo_functor_holder.hpp>

namespace hboost{
namespace intrusive{

template<class Key, class T, class KeyCompare, class KeyOfValue>
struct tree_value_compare
   :  public hboost::intrusive::detail::ebo_functor_holder<KeyCompare>
{
   typedef hboost::intrusive::detail::ebo_functor_holder<KeyCompare> base_t;
   typedef T            value_type;
   typedef KeyCompare   key_compare;
   typedef KeyOfValue   key_of_value;
   typedef Key          key_type;


   tree_value_compare()
      :  base_t()
   {}

   explicit tree_value_compare(const key_compare &kcomp)
      :  base_t(kcomp)
   {}

   tree_value_compare (const tree_value_compare &x)
      :  base_t(x.base_t::get())
   {}

   tree_value_compare &operator=(const tree_value_compare &x)
   {  this->base_t::get() = x.base_t::get();   return *this;  }

   tree_value_compare &operator=(const key_compare &x)
   {  this->base_t::get() = x;   return *this;  }

   HBOOST_INTRUSIVE_FORCEINLINE const key_compare &key_comp() const
   {  return static_cast<const key_compare &>(*this);  }

   HBOOST_INTRUSIVE_FORCEINLINE key_compare &key_comp()
   {  return static_cast<key_compare &>(*this);  }

   template<class U>
   struct is_key
      : hboost::intrusive::detail::is_same<const U, const key_type>
   {};

   template<class U>
   const key_type & key_forward
      (const U &key, typename hboost::intrusive::detail::enable_if<is_key<U> >::type* = 0) const
   {  return key; }

   template<class U>
   HBOOST_INTRUSIVE_FORCEINLINE const key_type & key_forward
      (const U &key, typename hboost::intrusive::detail::disable_if<is_key<U> >::type* = 0) const
   {  return KeyOfValue()(key);  }

   template<class KeyType, class KeyType2>
   HBOOST_INTRUSIVE_FORCEINLINE bool operator()(const KeyType &key1, const KeyType2 &key2) const
   {  return key_compare::operator()(this->key_forward(key1), this->key_forward(key2));  }

   template<class KeyType, class KeyType2>
   HBOOST_INTRUSIVE_FORCEINLINE bool operator()(const KeyType &key1, const KeyType2 &key2)
   {  return key_compare::operator()(this->key_forward(key1), this->key_forward(key2));  }
};

}  //namespace intrusive{
}  //namespace hboost{

#endif   //#ifdef HBOOST_INTRUSIVE_DETAIL_TREE_VALUE_COMPARE_HPP
