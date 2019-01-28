/* Copyright 2003-2013 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_INDEXED_BY_HPP
#define HBOOST_MULTI_INDEX_INDEXED_BY_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <hboost/mpl/vector.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/control/expr_if.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp> 

/* An alias to mpl::vector used to hide MPL from the user.
 * indexed_by contains the index specifiers for instantiation
 * of a multi_index_container.
 */

/* This user_definable macro limits the number of elements of an index list;
 * useful for shortening resulting symbol names (MSVC++ 6.0, for instance,
 * has problems coping with very long symbol names.)
 */

#if !defined(HBOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE)
#define HBOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE HBOOST_MPL_LIMIT_VECTOR_SIZE
#endif

#if HBOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE<HBOOST_MPL_LIMIT_VECTOR_SIZE
#define HBOOST_MULTI_INDEX_INDEXED_BY_SIZE \
  HBOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE
#else
#define HBOOST_MULTI_INDEX_INDEXED_BY_SIZE HBOOST_MPL_LIMIT_VECTOR_SIZE
#endif

#define HBOOST_MULTI_INDEX_INDEXED_BY_TEMPLATE_PARM(z,n,var) \
  typename HBOOST_PP_CAT(var,n) HBOOST_PP_EXPR_IF(n,=mpl::na)

namespace hboost{

namespace multi_index{

template<
  HBOOST_PP_ENUM(
    HBOOST_MULTI_INDEX_INDEXED_BY_SIZE,
    HBOOST_MULTI_INDEX_INDEXED_BY_TEMPLATE_PARM,T)
>
struct indexed_by:
  mpl::vector<HBOOST_PP_ENUM_PARAMS(HBOOST_MULTI_INDEX_INDEXED_BY_SIZE,T)>
{
};

} /* namespace multi_index */

} /* namespace hboost */

#undef HBOOST_MULTI_INDEX_INDEXED_BY_TEMPLATE_PARM
#undef HBOOST_MULTI_INDEX_INDEXED_BY_SIZE

#endif
