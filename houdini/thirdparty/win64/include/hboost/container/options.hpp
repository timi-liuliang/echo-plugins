/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2013-2013
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_OPTIONS_HPP
#define HBOOST_CONTAINER_OPTIONS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/container_fwd.hpp>
#include <hboost/intrusive/pack_options.hpp>

namespace hboost {
namespace container {

#if !defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

template<tree_type_enum TreeType, bool OptimizeSize>
struct tree_opt
{
   static const hboost::container::tree_type_enum tree_type = TreeType;
   static const bool optimize_size = OptimizeSize;
};

#endif   //!defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

//!This option setter specifies the underlying tree type
//!(red-black, AVL, Scapegoat or Splay) for ordered associative containers
HBOOST_INTRUSIVE_OPTION_CONSTANT(tree_type, tree_type_enum, TreeType, tree_type)

//!This option setter specifies if node size is optimized
//!storing rebalancing data masked into pointers for ordered associative containers
HBOOST_INTRUSIVE_OPTION_CONSTANT(optimize_size, bool, Enabled, optimize_size)

//! Helper metafunction to combine options into a single type to be used
//! by \c hboost::container::set, \c hboost::container::multiset
//! \c hboost::container::map and \c hboost::container::multimap.
//! Supported options are: \c hboost::container::optimize_size and \c hboost::container::tree_type
#if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED) || defined(HBOOST_CONTAINER_VARIADIC_TEMPLATES)
template<class ...Options>
#else
template<class O1 = void, class O2 = void, class O3 = void, class O4 = void>
#endif
struct tree_assoc_options
{
   /// @cond
   typedef typename ::hboost::intrusive::pack_options
      < tree_assoc_defaults,
      #if !defined(HBOOST_CONTAINER_VARIADIC_TEMPLATES)
      O1, O2, O3, O4
      #else
      Options...
      #endif
      >::type packed_options;
   typedef tree_opt<packed_options::tree_type, packed_options::optimize_size> implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};

}  //namespace container {
}  //namespace hboost {

#include <hboost/container/detail/config_end.hpp>

#endif   //#ifndef HBOOST_CONTAINER_OPTIONS_HPP
