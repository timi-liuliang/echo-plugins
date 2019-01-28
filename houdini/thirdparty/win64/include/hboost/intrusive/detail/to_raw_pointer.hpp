/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2014-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_TO_RAW_POINTER_HPP
#define HBOOST_INTRUSIVE_DETAIL_TO_RAW_POINTER_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/config_begin.hpp>
#include <hboost/intrusive/detail/workaround.hpp>
#include <hboost/intrusive/detail/pointer_element.hpp>

namespace hboost {
namespace intrusive {
namespace detail {

template <class T>
HBOOST_INTRUSIVE_FORCEINLINE T* to_raw_pointer(T* p)
{  return p; }

template <class Pointer>
HBOOST_INTRUSIVE_FORCEINLINE typename hboost::intrusive::pointer_element<Pointer>::type*
to_raw_pointer(const Pointer &p)
{  return hboost::intrusive::detail::to_raw_pointer(p.operator->());  }

} //namespace detail
} //namespace intrusive
} //namespace hboost

#include <hboost/intrusive/detail/config_end.hpp>

#endif //HBOOST_INTRUSIVE_DETAIL_UTILITIES_HPP
