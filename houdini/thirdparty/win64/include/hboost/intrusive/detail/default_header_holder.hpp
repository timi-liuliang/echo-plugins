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

#ifndef HBOOST_INTRUSIVE_DETAIL_DEFAULT_HEADER_HOLDER_HPP
#define HBOOST_INTRUSIVE_DETAIL_DEFAULT_HEADER_HOLDER_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/workaround.hpp>
#include <hboost/intrusive/pointer_traits.hpp>
#include <hboost/intrusive/detail/to_raw_pointer.hpp>

namespace hboost {
namespace intrusive {
namespace detail {

// trivial header node holder
template < typename NodeTraits >
struct default_header_holder : public NodeTraits::node
{
   typedef NodeTraits node_traits;
   typedef typename node_traits::node node;
   typedef typename node_traits::node_ptr node_ptr;
   typedef typename node_traits::const_node_ptr const_node_ptr;

   default_header_holder() : node() {}

   HBOOST_INTRUSIVE_FORCEINLINE const_node_ptr get_node() const
   { return pointer_traits< const_node_ptr >::pointer_to(*static_cast< const node* >(this)); }

   HBOOST_INTRUSIVE_FORCEINLINE node_ptr get_node()
   { return pointer_traits< node_ptr >::pointer_to(*static_cast< node* >(this)); }

   // (unsafe) downcast used to implement container-from-iterator
   HBOOST_INTRUSIVE_FORCEINLINE static default_header_holder* get_holder(const node_ptr &p)
   { return static_cast< default_header_holder* >(hboost::intrusive::detail::to_raw_pointer(p)); }
};

// type function producing the header node holder
template < typename ValueTraits, typename HeaderHolder >
struct get_header_holder_type
{
   typedef HeaderHolder type;
};
template < typename ValueTraits >
struct get_header_holder_type< ValueTraits, void >
{
   typedef default_header_holder< typename ValueTraits::node_traits > type;
};

} //namespace detail
} //namespace intrusive
} //namespace hboost

#endif //HBOOST_INTRUSIVE_DETAIL_DEFAULT_HEADER_HOLDER_HPP
