// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_MEMORY_ALLOCATOR_TRAITS_HPP
#define HBOOST_CSBL_MEMORY_ALLOCATOR_TRAITS_HPP

#include <hboost/thread/csbl/memory/config.hpp>

// 20.7.8, allocator traits
#if defined HBOOST_NO_CXX11_ALLOCATOR
#include <hboost/container/allocator_traits.hpp>

namespace hboost
{
  namespace csbl
  {
    using ::hboost::container::allocator_traits;
  }
}
#else
namespace hboost
{
  namespace csbl
  {
    using ::std::allocator_traits;
  }
}
#endif // HBOOST_NO_CXX11_POINTER_TRAITS

#endif // header
