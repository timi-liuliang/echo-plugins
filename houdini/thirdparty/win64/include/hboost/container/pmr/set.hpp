//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_PMR_SET_HPP
#define HBOOST_CONTAINER_PMR_SET_HPP

#if defined (_MSC_VER)
#  pragma once 
#endif

#include <hboost/container/set.hpp>
#include <hboost/container/pmr/polymorphic_allocator.hpp>

namespace hboost {
namespace container {
namespace pmr {

#if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
using set = hboost::container::set<Key, Compare, polymorphic_allocator<Key>, Options>;

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
using multiset = hboost::container::multiset<Key, Compare, polymorphic_allocator<Key>, Options>;

#endif

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
struct set_of
{
   typedef hboost::container::set<Key, Compare, polymorphic_allocator<Key>, Options> type;
};

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
struct multiset_of
{
   typedef hboost::container::multiset<Key, Compare, polymorphic_allocator<Key>, Options> type;
};

}  //namespace pmr {
}  //namespace container {
}  //namespace hboost {

#endif   //HBOOST_CONTAINER_PMR_SET_HPP
