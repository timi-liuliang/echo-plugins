// Copyright (C) 2014 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2014/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_MEMORY_SHARED_PTR_HPP
#define HBOOST_CSBL_MEMORY_SHARED_PTR_HPP

#include <hboost/thread/csbl/memory/config.hpp>

#if defined HBOOST_NO_CXX11_SMART_PTR

#include <hboost/smart_ptr/shared_ptr.hpp>
#include <hboost/smart_ptr/make_shared.hpp>

namespace hboost
{
  namespace csbl
  {
    using ::hboost::shared_ptr;
    using ::hboost::make_shared;
  }
}

#else

#include <hboost/shared_ptr.hpp>

namespace hboost
{
  namespace csbl
  {
    using std::shared_ptr;
    using std::make_shared;
  }
}

#endif
#endif // header
