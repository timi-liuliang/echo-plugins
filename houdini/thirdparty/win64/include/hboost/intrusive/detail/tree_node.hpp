/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2007-2013
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_TREE_NODE_HPP
#define HBOOST_INTRUSIVE_TREE_NODE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/config_begin.hpp>
#include <hboost/intrusive/detail/workaround.hpp>
#include <hboost/intrusive/pointer_rebind.hpp>

namespace hboost {
namespace intrusive {

template<class VoidPointer>
struct tree_node
{
   typedef typename pointer_rebind<VoidPointer, tree_node>::type  node_ptr;

   node_ptr parent_, left_, right_;
};

template<class VoidPointer>
struct tree_node_traits
{
   typedef tree_node<VoidPointer> node;

   typedef typename node::node_ptr   node_ptr;
   typedef typename pointer_rebind<VoidPointer, const node>::type const_node_ptr;

   HBOOST_INTRUSIVE_FORCEINLINE static node_ptr get_parent(const const_node_ptr & n)
   {  return n->parent_;  }

   HBOOST_INTRUSIVE_FORCEINLINE static node_ptr get_parent(const node_ptr & n)
   {  return n->parent_;  }

   HBOOST_INTRUSIVE_FORCEINLINE static void set_parent(const node_ptr & n, const node_ptr & p)
   {  n->parent_ = p;  }

   HBOOST_INTRUSIVE_FORCEINLINE static node_ptr get_left(const const_node_ptr & n)
   {  return n->left_;  }

   HBOOST_INTRUSIVE_FORCEINLINE static node_ptr get_left(const node_ptr & n)
   {  return n->left_;  }

   HBOOST_INTRUSIVE_FORCEINLINE static void set_left(const node_ptr & n, const node_ptr & l)
   {  n->left_ = l;  }

   HBOOST_INTRUSIVE_FORCEINLINE static node_ptr get_right(const const_node_ptr & n)
   {  return n->right_;  }

   HBOOST_INTRUSIVE_FORCEINLINE static node_ptr get_right(const node_ptr & n)
   {  return n->right_;  }

   HBOOST_INTRUSIVE_FORCEINLINE static void set_right(const node_ptr & n, const node_ptr & r)
   {  n->right_ = r;  }
};

} //namespace intrusive
} //namespace hboost

#include <hboost/intrusive/detail/config_end.hpp>

#endif //HBOOST_INTRUSIVE_TREE_NODE_HPP
