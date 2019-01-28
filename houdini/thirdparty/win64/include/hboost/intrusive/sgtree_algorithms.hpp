/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2007-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
//
// Scapegoat tree algorithms are taken from the paper titled:
// "Scapegoat Trees" by Igal Galperin Ronald L. Rivest.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_INTRUSIVE_SGTREE_ALGORITHMS_HPP
#define HBOOST_INTRUSIVE_SGTREE_ALGORITHMS_HPP

#include <hboost/intrusive/detail/config_begin.hpp>
#include <hboost/intrusive/intrusive_fwd.hpp>

#include <cstddef>
#include <hboost/intrusive/detail/algo_type.hpp>
#include <hboost/intrusive/bstree_algorithms.hpp>

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace hboost {
namespace intrusive {

//! sgtree_algorithms is configured with a NodeTraits class, which encapsulates the
//! information about the node to be manipulated. NodeTraits must support the
//! following interface:
//!
//! <b>Typedefs</b>:
//!
//! <tt>node</tt>: The type of the node that forms the binary search tree
//!
//! <tt>node_ptr</tt>: A pointer to a node
//!
//! <tt>const_node_ptr</tt>: A pointer to a const node
//!
//! <b>Static functions</b>:
//!
//! <tt>static node_ptr get_parent(const_node_ptr n);</tt>
//!
//! <tt>static void set_parent(node_ptr n, node_ptr parent);</tt>
//!
//! <tt>static node_ptr get_left(const_node_ptr n);</tt>
//!
//! <tt>static void set_left(node_ptr n, node_ptr left);</tt>
//!
//! <tt>static node_ptr get_right(const_node_ptr n);</tt>
//!
//! <tt>static void set_right(node_ptr n, node_ptr right);</tt>
template<class NodeTraits>
class sgtree_algorithms
   #ifndef HBOOST_INTRUSIVE_DOXYGEN_INVOKED
   : public bstree_algorithms<NodeTraits>
   #endif
{
   public:
   typedef typename NodeTraits::node            node;
   typedef NodeTraits                           node_traits;
   typedef typename NodeTraits::node_ptr        node_ptr;
   typedef typename NodeTraits::const_node_ptr  const_node_ptr;

   /// @cond
   private:

   typedef bstree_algorithms<NodeTraits>  bstree_algo;

   /// @endcond

   public:
   //! This type is the information that will be
   //! filled by insert_unique_check
   struct insert_commit_data
      : bstree_algo::insert_commit_data
   {
      std::size_t depth;
   };

   #ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! @copydoc ::hboost::intrusive::bstree_algorithms::get_header(const const_node_ptr&)
   static node_ptr get_header(const const_node_ptr & n);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::begin_node
   static node_ptr begin_node(const const_node_ptr & header);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::end_node
   static node_ptr end_node(const const_node_ptr & header);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::swap_tree
   static void swap_tree(const node_ptr & header1, const node_ptr & header2);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::swap_nodes(const node_ptr&,const node_ptr&)
   static void swap_nodes(const node_ptr & node1, const node_ptr & node2);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::swap_nodes(const node_ptr&,const node_ptr&,const node_ptr&,const node_ptr&)
   static void swap_nodes(const node_ptr & node1, const node_ptr & header1, const node_ptr & node2, const node_ptr & header2);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::replace_node(const node_ptr&,const node_ptr&)
   static void replace_node(const node_ptr & node_to_be_replaced, const node_ptr & new_node);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::replace_node(const node_ptr&,const node_ptr&,const node_ptr&)
   static void replace_node(const node_ptr & node_to_be_replaced, const node_ptr & header, const node_ptr & new_node);

   //Unlink is not possible since tree metadata is needed to update the tree
   //!static void unlink(const node_ptr & node);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::unlink_leftmost_without_rebalance
   static node_ptr unlink_leftmost_without_rebalance(const node_ptr & header);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::unique(const const_node_ptr&)
   static bool unique(const const_node_ptr & node);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::size(const const_node_ptr&)
   static std::size_t size(const const_node_ptr & header);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::next_node(const node_ptr&)
   static node_ptr next_node(const node_ptr & node);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::prev_node(const node_ptr&)
   static node_ptr prev_node(const node_ptr & node);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::init(const node_ptr&)
   static void init(const node_ptr & node);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::init_header(const node_ptr&)
   static void init_header(const node_ptr & header);
   #endif   //#ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED

   //! @copydoc ::hboost::intrusive::bstree_algorithms::erase(const node_ptr&,const node_ptr&)
   template<class AlphaByMaxSize>
   static node_ptr erase(const node_ptr & header, const node_ptr & z, std::size_t tree_size, std::size_t &max_tree_size, AlphaByMaxSize alpha_by_maxsize)
   {
      //typename bstree_algo::data_for_rebalance info;
      bstree_algo::erase(header, z);
      --tree_size;
      if (tree_size > 0 &&
          tree_size < alpha_by_maxsize(max_tree_size)){
         bstree_algo::rebalance(header);
         max_tree_size = tree_size;
      }
      return z;
   }

   #ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! @copydoc ::hboost::intrusive::bstree_algorithms::clone(const const_node_ptr&,const node_ptr&,Cloner,Disposer)
   template <class Cloner, class Disposer>
   static void clone
      (const const_node_ptr & source_header, const node_ptr & target_header, Cloner cloner, Disposer disposer);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::clear_and_dispose(const node_ptr&,Disposer)
   template<class Disposer>
   static void clear_and_dispose(const node_ptr & header, Disposer disposer);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::lower_bound(const const_node_ptr&,const KeyType&,KeyNodePtrCompare)
   template<class KeyType, class KeyNodePtrCompare>
   static node_ptr lower_bound
      (const const_node_ptr & header, const KeyType &key, KeyNodePtrCompare comp);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::upper_bound(const const_node_ptr&,const KeyType&,KeyNodePtrCompare)
   template<class KeyType, class KeyNodePtrCompare>
   static node_ptr upper_bound
      (const const_node_ptr & header, const KeyType &key, KeyNodePtrCompare comp);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::find(const const_node_ptr&, const KeyType&,KeyNodePtrCompare)
   template<class KeyType, class KeyNodePtrCompare>
   static node_ptr find
      (const const_node_ptr & header, const KeyType &key, KeyNodePtrCompare comp);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::equal_range(const const_node_ptr&,const KeyType&,KeyNodePtrCompare)
   template<class KeyType, class KeyNodePtrCompare>
   static std::pair<node_ptr, node_ptr> equal_range
      (const const_node_ptr & header, const KeyType &key, KeyNodePtrCompare comp);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::bounded_range(const const_node_ptr&,const KeyType&,const KeyType&,KeyNodePtrCompare,bool,bool)
   template<class KeyType, class KeyNodePtrCompare>
   static std::pair<node_ptr, node_ptr> bounded_range
      (const const_node_ptr & header, const KeyType &lower_key, const KeyType &upper_key, KeyNodePtrCompare comp
      , bool left_closed, bool right_closed);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::count(const const_node_ptr&,const KeyType&,KeyNodePtrCompare)
   template<class KeyType, class KeyNodePtrCompare>
   static std::size_t count(const const_node_ptr & header, const KeyType &key, KeyNodePtrCompare comp);

   #endif   //#ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED

   //! @copydoc ::hboost::intrusive::bstree_algorithms::insert_equal_upper_bound(const node_ptr&,const node_ptr&,NodePtrCompare)
   template<class NodePtrCompare, class H_Alpha>
   static node_ptr insert_equal_upper_bound
      (const node_ptr & h, const node_ptr & new_node, NodePtrCompare comp
      ,std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      std::size_t depth;
      bstree_algo::insert_equal_upper_bound(h, new_node, comp, &depth);
      rebalance_after_insertion(new_node, depth, tree_size+1, h_alpha, max_tree_size);
      return new_node;
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::insert_equal_lower_bound(const node_ptr&,const node_ptr&,NodePtrCompare)
   template<class NodePtrCompare, class H_Alpha>
   static node_ptr insert_equal_lower_bound
      (const node_ptr & h, const node_ptr & new_node, NodePtrCompare comp
      ,std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      std::size_t depth;
      bstree_algo::insert_equal_lower_bound(h, new_node, comp, &depth);
      rebalance_after_insertion(new_node, depth, tree_size+1, h_alpha, max_tree_size);
      return new_node;
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::insert_equal(const node_ptr&,const node_ptr&,const node_ptr&,NodePtrCompare)
   template<class NodePtrCompare, class H_Alpha>
   static node_ptr insert_equal
      (const node_ptr & header, const node_ptr & hint, const node_ptr & new_node, NodePtrCompare comp
      ,std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      std::size_t depth;
      bstree_algo::insert_equal(header, hint, new_node, comp, &depth);
      rebalance_after_insertion(new_node, depth, tree_size+1, h_alpha, max_tree_size);
      return new_node;
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::insert_before(const node_ptr&,const node_ptr&,const node_ptr&)
   template<class H_Alpha>
   static node_ptr insert_before
      (const node_ptr & header, const node_ptr & pos, const node_ptr & new_node
      ,std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      std::size_t depth;
      bstree_algo::insert_before(header, pos, new_node, &depth);
      rebalance_after_insertion(new_node, depth, tree_size+1, h_alpha, max_tree_size);
      return new_node;
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::push_back(const node_ptr&,const node_ptr&)
   template<class H_Alpha>
   static void push_back(const node_ptr & header, const node_ptr & new_node
         ,std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      std::size_t depth;
      bstree_algo::push_back(header, new_node, &depth);
      rebalance_after_insertion(new_node, depth, tree_size+1, h_alpha, max_tree_size);
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::push_front(const node_ptr&,const node_ptr&)
   template<class H_Alpha>
   static void push_front(const node_ptr & header, const node_ptr & new_node
         ,std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      std::size_t depth;
      bstree_algo::push_front(header, new_node, &depth);
      rebalance_after_insertion(new_node, depth, tree_size+1, h_alpha, max_tree_size);
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::insert_unique_check(const const_node_ptr&,const KeyType&,KeyNodePtrCompare,insert_commit_data&)
   template<class KeyType, class KeyNodePtrCompare>
   static std::pair<node_ptr, bool> insert_unique_check
      (const const_node_ptr & header,  const KeyType &key
      ,KeyNodePtrCompare comp, insert_commit_data &commit_data)
   {
      std::size_t depth;
      std::pair<node_ptr, bool> ret =
         bstree_algo::insert_unique_check(header, key, comp, commit_data, &depth);
      commit_data.depth = depth;
      return ret;
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::insert_unique_check(const const_node_ptr&,const node_ptr&,const KeyType&,KeyNodePtrCompare,insert_commit_data&)
   template<class KeyType, class KeyNodePtrCompare>
   static std::pair<node_ptr, bool> insert_unique_check
      (const const_node_ptr & header, const node_ptr &hint, const KeyType &key
      ,KeyNodePtrCompare comp, insert_commit_data &commit_data)
   {
      std::size_t depth;
      std::pair<node_ptr, bool> ret =
         bstree_algo::insert_unique_check
            (header, hint, key, comp, commit_data, &depth);
      commit_data.depth = depth;
      return ret;
   }

   //! @copydoc ::hboost::intrusive::bstree_algorithms::insert_unique_commit(const node_ptr&,const node_ptr&,const insert_commit_data&)
   template<class H_Alpha>
   static void insert_unique_commit
      (const node_ptr & header, const node_ptr & new_value, const insert_commit_data &commit_data
      ,std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      bstree_algo::insert_unique_commit(header, new_value, commit_data);
      rebalance_after_insertion(new_value, commit_data.depth, tree_size+1, h_alpha, max_tree_size);
   }

   #ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! @copydoc ::hboost::intrusive::bstree_algorithms::is_header
   static bool is_header(const const_node_ptr & p);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::is_header
   static void rebalance(const node_ptr & header);

   //! @copydoc ::hboost::intrusive::bstree_algorithms::rebalance_subtree
   static node_ptr rebalance_subtree(const node_ptr & old_root)
   #endif   //#ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED

   /// @cond
   private:

   template<class H_Alpha>
   static void rebalance_after_insertion
      (const node_ptr &x, std::size_t depth
      , std::size_t tree_size, H_Alpha h_alpha, std::size_t &max_tree_size)
   {
      if(tree_size > max_tree_size)
         max_tree_size = tree_size;

      if(tree_size > 2 && //Nothing to do with only the root
         //Check if the root node is unbalanced
         //Scapegoat paper depth counts root depth as zero and "depth" counts root as 1,
         //but since "depth" is the depth of the ancestor of x, i == depth
         depth > h_alpha(tree_size)){

         //Find the first non height-balanced node
         //as described in the section 4.2 of the paper.
         //This method is the alternative method described
         //in the paper. Authors claim that this method
         //may tend to yield more balanced trees on the average
         //than the weight balanced method.
         node_ptr s = x;
         std::size_t size = 1;
         for(std::size_t ancestor = 1; ancestor != depth; ++ancestor){
            const node_ptr s_parent = NodeTraits::get_parent(s);
            const node_ptr s_parent_left = NodeTraits::get_left(s_parent);
            //Obtain parent's size (previous size + parent + sibling tree)
            const node_ptr s_sibling = s_parent_left == s ? NodeTraits::get_right(s_parent) : s_parent_left;
            size += 1 + bstree_algo::subtree_size(s_sibling);
            s = s_parent;
            if(ancestor > h_alpha(size)){ //is 's' scapegoat?
               bstree_algo::rebalance_subtree(s);
               return;
            }
         }
         //The whole tree must be rebuilt
         max_tree_size = tree_size;
         bstree_algo::rebalance_subtree(NodeTraits::get_parent(s));
      }
   }
   /// @endcond
};

/// @cond

template<class NodeTraits>
struct get_algo<SgTreeAlgorithms, NodeTraits>
{
   typedef sgtree_algorithms<NodeTraits> type;
};

template <class ValueTraits, class NodePtrCompare, class ExtraChecker>
struct get_node_checker<SgTreeAlgorithms, ValueTraits, NodePtrCompare, ExtraChecker>
{
   typedef detail::bstree_node_checker<ValueTraits, NodePtrCompare, ExtraChecker> type;
};

/// @endcond

} //namespace intrusive
} //namespace hboost

#include <hboost/intrusive/detail/config_end.hpp>

#endif //HBOOST_INTRUSIVE_SGTREE_ALGORITHMS_HPP