// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_TUPLE_HPP
#define HBOOST_CSBL_TUPLE_HPP

#include <hboost/config.hpp>

#if defined HBOOST_THREAD_USES_HBOOST_TUPLE || defined HBOOST_NO_CXX11_HDR_TUPLE || defined HBOOST_NO_CXX11_RVALUE_REFERENCES
#include <hboost/tuple/tuple.hpp>
#ifndef HBOOST_THREAD_USES_HBOOST_TUPLE
#define HBOOST_THREAD_USES_HBOOST_TUPLE
#endif

#else
#include <tuple>
#endif

namespace hboost
{
  namespace csbl
  {
#if defined HBOOST_THREAD_USES_HBOOST_TUPLE
    using ::hboost::tuple;
    using ::hboost::get;
    using ::hboost::make_tuple;
    //using ::hboost::tuple_size;
#else
    // 20.4.2, class template tuple:
    using ::std::tuple;
    using ::std::get;
    using ::std::make_tuple;
    using ::std::tuple_size;
    // 20.4.2.4, tuple creation functions:
    // 20.4.2.5, tuple helper classes:
    // 20.4.2.6, element access:
    // 20.4.2.7, relational operators:
    // 20.4.2.8, allocator-related traits
    // 20.4.2.9, specialized algorithms:
#endif

  }
}
#endif // header
