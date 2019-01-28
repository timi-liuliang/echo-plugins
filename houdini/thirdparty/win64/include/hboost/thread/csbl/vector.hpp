// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_VECTOR_HPP
#define HBOOST_CSBL_VECTOR_HPP

#include <hboost/config.hpp>

#if defined HBOOST_THREAD_USES_HBOOST_VECTOR || defined HBOOST_NO_CXX11_RVALUE_REFERENCES || defined HBOOST_MSVC
#ifndef HBOOST_THREAD_USES_HBOOST_VECTOR
#define HBOOST_THREAD_USES_HBOOST_VECTOR
#endif
#include <hboost/container/vector.hpp>
#else
#include <vector>
#endif

namespace hboost
{
  namespace csbl
  {
#if defined HBOOST_THREAD_USES_HBOOST_VECTOR
    using ::hboost::container::vector;
#else
    using ::std::vector;
#endif

  }
}
#endif // header
