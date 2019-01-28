//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_PMR_MAP_HPP
#define HBOOST_CONTAINER_PMR_MAP_HPP

#if defined (_MSC_VER)
#  pragma once 
#endif

#include <hboost/container/map.hpp>
#include <hboost/container/pmr/polymorphic_allocator.hpp>

namespace hboost {
namespace container {
namespace pmr {

#if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)

template <class Key
         ,class T
         ,class Compare  = std::less<Key>
         ,class Options = tree_assoc_defaults >
using map = hboost::container::map<Key, T, Compare, polymorphic_allocator<std::pair<const Key, T> >, Options>;

template <class Key
         ,class T
         ,class Compare  = std::less<Key>
         ,class Options = tree_assoc_defaults >
using multimap = hboost::container::multimap<Key, T, Compare, polymorphic_allocator<std::pair<const Key, T> >, Options>;

#endif

template <class Key
         ,class T
         ,class Compare  = std::less<Key>
         ,class Options = tree_assoc_defaults >
struct map_of
{
   typedef hboost::container::map<Key, T, Compare, polymorphic_allocator<std::pair<const Key, T> >, Options> type;
};

template <class Key
         ,class T
         ,class Compare  = std::less<Key>
         ,class Options = tree_assoc_defaults >
struct multimap_of
{
   typedef hboost::container::multimap<Key, T, Compare, polymorphic_allocator<std::pair<const Key, T> >, Options> type;
};

}  //namespace pmr {
}  //namespace container {
}  //namespace hboost {

#endif   //HBOOST_CONTAINER_PMR_MAP_HPP
