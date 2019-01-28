//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_PMR_LIST_HPP
#define HBOOST_CONTAINER_PMR_LIST_HPP

#if defined (_MSC_VER)
#  pragma once 
#endif

#include <hboost/container/list.hpp>
#include <hboost/container/pmr/polymorphic_allocator.hpp>

namespace hboost {
namespace container {
namespace pmr {

#if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)

template <class T>
using list = hboost::container::list<T, polymorphic_allocator<T>>;

#endif

template<class T>
struct list_of
{
   typedef hboost::container::list
      < T, polymorphic_allocator<T> > type;
};

}  //namespace pmr {
}  //namespace container {
}  //namespace hboost {

#endif   //HBOOST_CONTAINER_PMR_VECTOR_HPP
