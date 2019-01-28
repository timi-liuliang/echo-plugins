// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_MEMORY_ALLOCATOR_ARG_HPP
#define HBOOST_CSBL_MEMORY_ALLOCATOR_ARG_HPP

#include <hboost/thread/csbl/memory/config.hpp>

// 20.7.6, allocator argument tag
#if defined HBOOST_NO_CXX11_ALLOCATOR
#include <hboost/container/scoped_allocator.hpp>

namespace hboost
{
  namespace csbl
  {
    using ::hboost::container::allocator_arg_t;
    using ::hboost::container::allocator_arg;
  }
}
#else
namespace hboost
{
  namespace csbl
  {
    using ::std::allocator_arg_t;
    using ::std::allocator_arg;
  }
}
#endif // HBOOST_NO_CXX11_ALLOCATOR
namespace hboost
{
  using ::hboost::csbl::allocator_arg_t;
  using ::hboost::csbl::allocator_arg;
}
#endif // header
