/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_IITERATOR_HPP
#define HBOOST_INTRUSIVE_DETAIL_IITERATOR_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/workaround.hpp>
#include <hboost/intrusive/detail/iterator.hpp>
#include <hboost/intrusive/pointer_traits.hpp>
#include <hboost/intrusive/detail/mpl.hpp>
#include <hboost/intrusive/detail/is_stateful_value_traits.hpp>

namespace hboost {
namespace intrusive {

template<class ValueTraits>
struct value_traits_pointers
{
   typedef HBOOST_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT
      (hboost::intrusive::detail::
      , ValueTraits, value_traits_ptr
      , typename hboost::intrusive::pointer_traits<typename ValueTraits::node_traits::node_ptr>::template
         rebind_pointer<ValueTraits>::type)   value_traits_ptr;

   typedef typename hboost::intrusive::pointer_traits<value_traits_ptr>::template
      rebind_pointer<ValueTraits const>::type const_value_traits_ptr;
};

template<class ValueTraits, bool IsConst, class Category>
struct iiterator
{
   typedef ValueTraits                                         value_traits;
   typedef typename value_traits::node_traits                  node_traits;
   typedef typename node_traits::node                          node;
   typedef typename node_traits::node_ptr                      node_ptr;
   typedef ::hboost::intrusive::pointer_traits<node_ptr>        nodepointer_traits_t;
   typedef typename nodepointer_traits_t::template
      rebind_pointer<void>::type                               void_pointer;
   typedef typename ValueTraits::value_type                    value_type;
   typedef typename ValueTraits::pointer                       nonconst_pointer;
   typedef typename ValueTraits::const_pointer                 yesconst_pointer;
   typedef typename ::hboost::intrusive::pointer_traits
      <nonconst_pointer>::reference                            nonconst_reference;
   typedef typename ::hboost::intrusive::pointer_traits
      <yesconst_pointer>::reference                            yesconst_reference;
   typedef typename nodepointer_traits_t::difference_type      difference_type;
   typedef typename detail::if_c
      <IsConst, yesconst_pointer, nonconst_pointer>::type      pointer;
   typedef typename detail::if_c
      <IsConst, yesconst_reference, nonconst_reference>::type  reference;
   typedef iterator
         < Category
         , value_type
         , difference_type
         , pointer
         , reference
         > iterator_type;
   typedef typename value_traits_pointers
      <ValueTraits>::value_traits_ptr                          value_traits_ptr;
   typedef typename value_traits_pointers
      <ValueTraits>::const_value_traits_ptr                    const_value_traits_ptr;
   static const bool stateful_value_traits =
      detail::is_stateful_value_traits<value_traits>::value;
};

template<class NodePtr, class StoredPointer, bool StatefulValueTraits = true>
struct iiterator_members
{

   HBOOST_INTRUSIVE_FORCEINLINE iiterator_members()
      : nodeptr_()//Value initialization to achieve "null iterators" (N3644)
   {}

   HBOOST_INTRUSIVE_FORCEINLINE iiterator_members(const NodePtr &n_ptr, const StoredPointer &data)
      :  nodeptr_(n_ptr), ptr_(data)
   {}

   HBOOST_INTRUSIVE_FORCEINLINE StoredPointer get_ptr() const
   {  return ptr_;  }

   NodePtr nodeptr_;
   StoredPointer ptr_;
};

template<class NodePtr, class StoredPointer>
struct iiterator_members<NodePtr, StoredPointer, false>
{
   HBOOST_INTRUSIVE_FORCEINLINE iiterator_members()
      : nodeptr_()//Value initialization to achieve "null iterators" (N3644)
   {}

   HBOOST_INTRUSIVE_FORCEINLINE iiterator_members(const NodePtr &n_ptr, const StoredPointer &)
      : nodeptr_(n_ptr)
   {}

   HBOOST_INTRUSIVE_FORCEINLINE StoredPointer get_ptr() const
   {  return StoredPointer();  }

   NodePtr nodeptr_;
};

} //namespace intrusive
} //namespace hboost

#endif //HBOOST_INTRUSIVE_DETAIL_IITERATOR_HPP
