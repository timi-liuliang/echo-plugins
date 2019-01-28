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

#include <hboost/container/flat_set.hpp>
#include <hboost/container/pmr/polymorphic_allocator.hpp>

namespace hboost {
namespace container {
namespace pmr {

#if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
using flat_set = hboost::container::flat_set<Key, Compare, polymorphic_allocator<Key> >;

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
using flat_multiset = hboost::container::flat_multiset<Key, Compare, polymorphic_allocator<Key> >;

#endif

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
struct flat_set_of
{
   typedef hboost::container::flat_set<Key, Compare, polymorphic_allocator<Key> > type;
};

template <class Key
         ,class Compare = std::less<Key>
         ,class Options = tree_assoc_defaults >
struct flat_multiset_of
{
   typedef hboost::container::flat_multiset<Key, Compare, polymorphic_allocator<Key> > type;
};

}  //namespace pmr {
}  //namespace container {
}  //namespace hboost {

#endif   //HBOOST_CONTAINER_PMR_SET_HPP
