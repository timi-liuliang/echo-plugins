// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_LIST_HPP
#define HBOOST_CSBL_LIST_HPP

#include <hboost/config.hpp>

#if defined HBOOST_THREAD_USES_HBOOST_LIST || defined HBOOST_NO_CXX11_RVALUE_REFERENCES
#ifndef HBOOST_THREAD_USES_HBOOST_LIST
#define HBOOST_THREAD_USES_HBOOST_LIST
#endif
#include <hboost/container/list.hpp>
#else
#include <list>
#endif

namespace hboost
{
  namespace csbl
  {
#if defined HBOOST_THREAD_USES_HBOOST_LIST
    using ::hboost::container::list;
#else
    using ::std::list;
#endif

  }
}
#endif // header
