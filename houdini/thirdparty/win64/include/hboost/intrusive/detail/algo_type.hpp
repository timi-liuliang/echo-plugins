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

#ifndef HBOOST_INTRUSIVE_DETAIL_ALGO_TYPE_HPP
#define HBOOST_INTRUSIVE_DETAIL_ALGO_TYPE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace hboost {
namespace intrusive {

enum algo_types
{
   CircularListAlgorithms,
   CircularSListAlgorithms,
   LinearSListAlgorithms,
   CommonSListAlgorithms,
   BsTreeAlgorithms,
   RbTreeAlgorithms,
   AvlTreeAlgorithms,
   SgTreeAlgorithms,
   SplayTreeAlgorithms,
   TreapAlgorithms
};

template<algo_types AlgoType, class NodeTraits>
struct get_algo;

template<algo_types AlgoType, class ValueTraits, class NodePtrCompare, class ExtraChecker>
struct get_node_checker;

} //namespace intrusive
} //namespace hboost

#endif //HBOOST_INTRUSIVE_DETAIL_ALGO_TYPE_HPP
