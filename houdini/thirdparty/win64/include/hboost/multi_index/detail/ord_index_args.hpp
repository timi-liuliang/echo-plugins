/* Copyright 2003-2013 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_DETAIL_ORD_INDEX_ARGS_HPP
#define HBOOST_MULTI_INDEX_DETAIL_ORD_INDEX_ARGS_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <hboost/mpl/aux_/na.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/multi_index/tag.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <functional>

namespace hboost{

namespace multi_index{

namespace detail{

/* Oredered index specifiers can be instantiated in two forms:
 *
 *   (ordered_unique|ordered_non_unique)<
 *     KeyFromValue,Compare=std::less<KeyFromValue::result_type> >
 *   (ordered_unique|ordered_non_unique)<
 *     TagList,KeyFromValue,Compare=std::less<KeyFromValue::result_type> >
 *
 * index_args implements the machinery to accept this argument-dependent
 * polymorphism.
 */

template<typename KeyFromValue>
struct index_args_default_compare
{
  typedef std::less<typename KeyFromValue::result_type> type;
};

template<typename Arg1,typename Arg2,typename Arg3>
struct ordered_index_args
{
  typedef is_tag<Arg1> full_form;

  typedef typename mpl::if_<
    full_form,
    Arg1,
    tag< > >::type                                   tag_list_type;
  typedef typename mpl::if_<
    full_form,
    Arg2,
    Arg1>::type                                      key_from_value_type;
  typedef typename mpl::if_<
    full_form,
    Arg3,
    Arg2>::type                                      supplied_compare_type;
  typedef typename mpl::eval_if<
    mpl::is_na<supplied_compare_type>,
    index_args_default_compare<key_from_value_type>,
    mpl::identity<supplied_compare_type>
  >::type                                            compare_type;

  HBOOST_STATIC_ASSERT(is_tag<tag_list_type>::value);
  HBOOST_STATIC_ASSERT(!mpl::is_na<key_from_value_type>::value);
  HBOOST_STATIC_ASSERT(!mpl::is_na<compare_type>::value);
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace hboost */

#endif
