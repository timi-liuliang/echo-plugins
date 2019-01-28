// Copyright (C) 2013-2014 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation using interprocess::unique_ptr.
// 2014/09 Vicente J. Botet Escriba
//   Adaptation to movelib::unique_ptr

#ifndef HBOOST_CSBL_MEMORY_UNIQUE_PTR_HPP
#define HBOOST_CSBL_MEMORY_UNIQUE_PTR_HPP

#include <hboost/thread/csbl/memory/config.hpp>

#include <hboost/move/unique_ptr.hpp>
#include <hboost/move/make_unique.hpp>

namespace hboost
{
  namespace csbl
  {
    using ::hboost::movelib::unique_ptr;
    using ::hboost::movelib::make_unique;

  }
}
#endif // header
