//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2014.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_DETAIL_CONSTRUCT_IN_PLACE_HPP
#define HBOOST_CONTAINER_DETAIL_CONSTRUCT_IN_PLACE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/allocator_traits.hpp>
#include <hboost/container/detail/iterators.hpp>

namespace hboost {
namespace container {

template<class Allocator, class T, class InpIt>
inline void construct_in_place(Allocator &a, T* dest, InpIt source)
{     hboost::container::allocator_traits<Allocator>::construct(a, dest, *source);  }

template<class Allocator, class T, class U, class D>
inline void construct_in_place(Allocator &a, T *dest, value_init_construct_iterator<U, D>)
{
   hboost::container::allocator_traits<Allocator>::construct(a, dest);
}

template <class T, class Difference>
class default_init_construct_iterator;

template<class Allocator, class T, class U, class D>
inline void construct_in_place(Allocator &a, T *dest, default_init_construct_iterator<U, D>)
{
   hboost::container::allocator_traits<Allocator>::construct(a, dest, default_init);
}

template <class T, class EmplaceFunctor, class Difference>
class emplace_iterator;

template<class Allocator, class T, class U, class EF, class D>
inline void construct_in_place(Allocator &a, T *dest, emplace_iterator<U, EF, D> ei)
{
   ei.construct_in_place(a, dest);
}

}  //namespace container {
}  //namespace hboost {

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_CONSTRUCT_IN_PLACE_HPP

