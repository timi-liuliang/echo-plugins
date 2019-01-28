//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_PMR_SMALL_VECTOR_HPP
#define HBOOST_CONTAINER_PMR_SMALL_VECTOR_HPP

#if defined (_MSC_VER)
#  pragma once 
#endif

#include <hboost/container/small_vector.hpp>
#include <hboost/container/pmr/polymorphic_allocator.hpp>

namespace hboost {
namespace container {
namespace pmr {

#if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)

template <class T, std::size_t N>
using small_vector = hboost::container::small_vector<T, N, polymorphic_allocator<T>>;

#endif

template<class T, std::size_t N>
struct small_vector_of
{
   typedef hboost::container::small_vector
      < T, N, polymorphic_allocator<T> > type;
};

}  //namespace pmr {
}  //namespace container {
}  //namespace hboost {

#endif   //HBOOST_CONTAINER_PMR_SMALL_VECTOR_HPP
