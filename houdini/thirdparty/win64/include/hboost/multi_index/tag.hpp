/* Copyright 2003-2013 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_TAG_HPP
#define HBOOST_MULTI_INDEX_TAG_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <hboost/multi_index/detail/no_duplicate_tags.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/transform.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp> 
#include <hboost/preprocessor/repetition/enum_binary_params.hpp> 
#include <hboost/preprocessor/repetition/enum_params.hpp> 
#include <hboost/static_assert.hpp>
#include <hboost/type_traits/is_base_and_derived.hpp>

/* A wrapper of mpl::vector used to hide MPL from the user.
 * tag contains types used as tag names for indices in get() functions.
 */

/* This user_definable macro limits the number of elements of a tag;
 * useful for shortening resulting symbol names (MSVC++ 6.0, for instance,
 * has problems coping with very long symbol names.)
 */

#if !defined(HBOOST_MULTI_INDEX_LIMIT_TAG_SIZE)
#define HBOOST_MULTI_INDEX_LIMIT_TAG_SIZE HBOOST_MPL_LIMIT_VECTOR_SIZE
#endif

#if HBOOST_MULTI_INDEX_LIMIT_TAG_SIZE<HBOOST_MPL_LIMIT_VECTOR_SIZE
#define HBOOST_MULTI_INDEX_TAG_SIZE HBOOST_MULTI_INDEX_LIMIT_TAG_SIZE
#else
#define HBOOST_MULTI_INDEX_TAG_SIZE HBOOST_MPL_LIMIT_VECTOR_SIZE
#endif

namespace hboost{

namespace multi_index{

namespace detail{

struct tag_marker{};

template<typename T>
struct is_tag
{
  HBOOST_STATIC_CONSTANT(bool,value=(is_base_and_derived<tag_marker,T>::value));
};

} /* namespace multi_index::detail */

template<
  HBOOST_PP_ENUM_BINARY_PARAMS(
    HBOOST_MULTI_INDEX_TAG_SIZE,
    typename T,
    =mpl::na HBOOST_PP_INTERCEPT) 
>
struct tag:private detail::tag_marker
{
  /* The mpl::transform pass produces shorter symbols (without
   * trailing mpl::na's.)
   */

  typedef typename mpl::transform<
    mpl::vector<HBOOST_PP_ENUM_PARAMS(HBOOST_MULTI_INDEX_TAG_SIZE,T)>,
    mpl::identity<mpl::_1>
  >::type type;

  HBOOST_STATIC_ASSERT(detail::no_duplicate_tags<type>::value);
};

} /* namespace multi_index */

} /* namespace hboost */

#undef HBOOST_MULTI_INDEX_TAG_SIZE

#endif
