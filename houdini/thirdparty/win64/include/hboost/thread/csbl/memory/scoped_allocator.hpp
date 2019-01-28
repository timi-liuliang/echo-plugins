// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_MEMORY_SCOPED_ALLOCATOR_HPP
#define HBOOST_CSBL_MEMORY_SCOPED_ALLOCATOR_HPP

#include <hboost/thread/csbl/memory/config.hpp>

// 20.7.7, uses_allocator
#if defined HBOOST_NO_CXX11_ALLOCATOR
#include <hboost/container/scoped_allocator.hpp>

namespace hboost
{
  namespace csbl
  {
    using ::hboost::container::uses_allocator;
  }
}
#else
namespace hboost
{
  namespace csbl
  {
    using ::std::uses_allocator;
  }
}
#endif // HBOOST_NO_CXX11_POINTER_TRAITS

#endif // header
