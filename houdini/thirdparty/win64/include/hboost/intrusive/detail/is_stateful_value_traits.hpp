/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2009-2013.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_IS_STATEFUL_VALUE_TRAITS_HPP
#define HBOOST_INTRUSIVE_DETAIL_IS_STATEFUL_VALUE_TRAITS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1310)

#include <hboost/intrusive/detail/mpl.hpp>

namespace hboost {
namespace intrusive {
namespace detail {

template<class ValueTraits>
struct is_stateful_value_traits
{
   static const bool value = !detail::is_empty<ValueTraits>::value;
};

}}}

#else

#include <hboost/intrusive/detail/function_detector.hpp>

HBOOST_INTRUSIVE_CREATE_FUNCTION_DETECTOR(to_node_ptr, hboost_intrusive)
HBOOST_INTRUSIVE_CREATE_FUNCTION_DETECTOR(to_value_ptr, hboost_intrusive)

namespace hboost {
namespace intrusive {
namespace detail {

template<class ValueTraits>
struct is_stateful_value_traits
{
   typedef typename ValueTraits::node_ptr       node_ptr;
   typedef typename ValueTraits::pointer        pointer;
   typedef typename ValueTraits::value_type     value_type;
   typedef typename ValueTraits::const_node_ptr const_node_ptr;
   typedef typename ValueTraits::const_pointer  const_pointer;

   typedef ValueTraits value_traits;

   static const bool value =
      (hboost::intrusive::function_detector::NonStaticFunction ==
         (HBOOST_INTRUSIVE_DETECT_FUNCTION(ValueTraits, hboost_intrusive, node_ptr, to_node_ptr, (value_type&) )))
      ||
      (hboost::intrusive::function_detector::NonStaticFunction ==
         (HBOOST_INTRUSIVE_DETECT_FUNCTION(ValueTraits, hboost_intrusive, pointer, to_value_ptr, (node_ptr) )))
      ||
      (hboost::intrusive::function_detector::NonStaticFunction ==
         (HBOOST_INTRUSIVE_DETECT_FUNCTION(ValueTraits, hboost_intrusive, const_node_ptr, to_node_ptr, (const value_type&) )))
      ||
      (hboost::intrusive::function_detector::NonStaticFunction ==
         (HBOOST_INTRUSIVE_DETECT_FUNCTION(ValueTraits, hboost_intrusive, const_pointer, to_value_ptr, (const_node_ptr) )))
      ;
};

}}}

#endif

#endif   //@ifndef HBOOST_INTRUSIVE_DETAIL_IS_STATEFUL_VALUE_TRAITS_HPP
