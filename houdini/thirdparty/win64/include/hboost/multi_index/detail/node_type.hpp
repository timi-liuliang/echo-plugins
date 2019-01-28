/* Copyright 2003-2013 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_DETAIL_NODE_TYPE_HPP
#define HBOOST_MULTI_INDEX_DETAIL_NODE_TYPE_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <hboost/detail/workaround.hpp>
#include <hboost/mpl/bind.hpp>
#include <hboost/mpl/reverse_iter_fold.hpp>
#include <hboost/mpl/deref.hpp>
#include <hboost/multi_index_container_fwd.hpp>
#include <hboost/multi_index/detail/header_holder.hpp>
#include <hboost/multi_index/detail/index_node_base.hpp>
#include <hboost/multi_index/detail/is_index_list.hpp>
#include <hboost/static_assert.hpp>

namespace hboost{

namespace multi_index{

namespace detail{

/* MPL machinery to construct the internal node type associated to an
 * index list.
 */

struct index_node_applier
{
  template<typename IndexSpecifierIterator,typename Super>
  struct apply
  {
    typedef typename mpl::deref<IndexSpecifierIterator>::type index_specifier;
    typedef typename index_specifier::
      HBOOST_NESTED_TEMPLATE node_class<Super>::type type;
  }; 
};

template<typename Value,typename IndexSpecifierList,typename Allocator>
struct multi_index_node_type
{
  HBOOST_STATIC_ASSERT(detail::is_index_list<IndexSpecifierList>::value);

  typedef typename mpl::reverse_iter_fold<
    IndexSpecifierList,
    index_node_base<Value,Allocator>,
    mpl::bind2<index_node_applier,mpl::_2,mpl::_1>
  >::type type;
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace hboost */

#endif