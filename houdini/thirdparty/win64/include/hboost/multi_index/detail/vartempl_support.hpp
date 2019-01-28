/* Copyright 2003-2013 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_DETAIL_VARTEMPL_SUPPORT_HPP
#define HBOOST_MULTI_INDEX_DETAIL_VARTEMPL_SUPPORT_HPP

#if defined(_MSC_VER)
#pragma once
#endif

/* Utilities for emulation of variadic template functions. Variadic packs are
 * replaced by lists of HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS parameters:
 *
 *   - typename... Args            --> HBOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK
 *   - Args&&... args              --> HBOOST_MULTI_INDEX_FUNCTION_PARAM_PACK
 *   - std::forward<Args>(args)... --> HBOOST_MULTI_INDEX_FORWARD_PARAM_PACK
 *
 * Forwarding emulated with Boost.Move. A template functions foo_imp
 * defined in such way accepts *exactly* HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS
 * arguments: variable number of arguments is emulated by providing a set of
 * overloads foo forwarding to foo_impl with
 * 
 *   HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL
 *   HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_EXTRA_ARG (initial extra arg)
 *
 * which fill the extra args with hboost::multi_index::detail::noarg's.
 * hboost::multi_index::detail::vartempl_placement_new works the opposite
 * way: it acceps a full a pointer x to Value and a
 * HBOOST_MULTI_INDEX_FUNCTION_PARAM_PACK and forwards to
 * new(x) Value(args) where args is the argument pack after discarding
 * noarg's.
 *
 * Emulation decays to the real thing when the compiler supports variadic
 * templates and move semantics natively.
 */

#include <hboost/config.hpp>

#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)||\
    defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#include <hboost/move/core.hpp>
#include <hboost/move/utility.hpp>
#include <hboost/preprocessor/arithmetic/add.hpp>
#include <hboost/preprocessor/arithmetic/sub.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/facilities/empty.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/preprocessor/logical/and.hpp>
#include <hboost/preprocessor/punctuation/comma.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>
#include <hboost/preprocessor/seq/elem.hpp>

#if !defined(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS)
#define HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS 5
#endif

#define HBOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK                        \
HBOOST_PP_ENUM_PARAMS(                                                \
  HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,typename T)

#define HBOOST_MULTI_INDEX_VARTEMPL_ARG(z,n,_)                        \
HBOOST_FWD_REF(HBOOST_PP_CAT(T,n)) HBOOST_PP_CAT(t,n)

#define HBOOST_MULTI_INDEX_FUNCTION_PARAM_PACK                        \
HBOOST_PP_ENUM(                                                       \
  HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,                             \
  HBOOST_MULTI_INDEX_VARTEMPL_ARG,~)

#define HBOOST_MULTI_INDEX_VARTEMPL_FORWARD_ARG(z,n,_)                \
hboost::forward<HBOOST_PP_CAT(T,n)>(HBOOST_PP_CAT(t,n))

#define HBOOST_MULTI_INDEX_FORWARD_PARAM_PACK                         \
HBOOST_PP_ENUM(                                                       \
  HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,                             \
  HBOOST_MULTI_INDEX_VARTEMPL_FORWARD_ARG,~)

namespace hboost{namespace multi_index{namespace detail{
struct noarg{};
}}}

/* call vartempl function without args */

#define HBOOST_MULTI_INDEX_NULL_PARAM_PACK                            \
HBOOST_PP_ENUM_PARAMS(                                                \
  HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,                             \
  hboost::multi_index::detail::noarg() HBOOST_PP_INTERCEPT)

#define HBOOST_MULTI_INDEX_TEMPLATE_N(n)                              \
template<HBOOST_PP_ENUM_PARAMS(n,typename T)>

#define HBOOST_MULTI_INDEX_TEMPLATE_0(n)

#define HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_AUX(z,n,data)        \
HBOOST_PP_IF(n,                                                       \
  HBOOST_MULTI_INDEX_TEMPLATE_N,                                      \
  HBOOST_MULTI_INDEX_TEMPLATE_0)(n)                                   \
HBOOST_PP_SEQ_ELEM(0,data) /* ret */                                  \
HBOOST_PP_SEQ_ELEM(1,data) /* name_from */ (                          \
  HBOOST_PP_ENUM(n,HBOOST_MULTI_INDEX_VARTEMPL_ARG,~))                 \
{                                                                    \
  return HBOOST_PP_SEQ_ELEM(2,data) /* name_to */ (                   \
    HBOOST_PP_ENUM(n,HBOOST_MULTI_INDEX_VARTEMPL_FORWARD_ARG,~)        \
    HBOOST_PP_COMMA_IF(                                               \
      HBOOST_PP_AND(                                                  \
        n,HBOOST_PP_SUB(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,n)))    \
    HBOOST_PP_ENUM_PARAMS(                                            \
      HBOOST_PP_SUB(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,n),         \
      hboost::multi_index::detail::noarg() HBOOST_PP_INTERCEPT)        \
  );                                                                 \
}

#define HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL(                     \
  ret,name_from,name_to)                                             \
HBOOST_PP_REPEAT_FROM_TO(                                             \
  0,HBOOST_PP_ADD(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,1),           \
  HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_AUX,                       \
  (ret)(name_from)(name_to))

#define HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_EXTRA_ARG_AUX(       \
  z,n,data)                                                          \
HBOOST_PP_IF(n,                                                       \
  HBOOST_MULTI_INDEX_TEMPLATE_N,                                      \
  HBOOST_MULTI_INDEX_TEMPLATE_0)(n)                                   \
HBOOST_PP_SEQ_ELEM(0,data) /* ret */                                  \
HBOOST_PP_SEQ_ELEM(1,data) /* name_from */ (                          \
  HBOOST_PP_SEQ_ELEM(3,data) HBOOST_PP_SEQ_ELEM(4,data) /* extra arg */\
  HBOOST_PP_COMMA_IF(n)                                               \
  HBOOST_PP_ENUM(n,HBOOST_MULTI_INDEX_VARTEMPL_ARG,~))                 \
{                                                                    \
  return HBOOST_PP_SEQ_ELEM(2,data) /* name_to */ (                   \
    HBOOST_PP_SEQ_ELEM(4,data) /* extra_arg_name */                   \
    HBOOST_PP_COMMA_IF(n)                                             \
    HBOOST_PP_ENUM(n,HBOOST_MULTI_INDEX_VARTEMPL_FORWARD_ARG,~)        \
    HBOOST_PP_COMMA_IF(                                               \
      HBOOST_PP_SUB(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,n))         \
    HBOOST_PP_ENUM_PARAMS(                                            \
      HBOOST_PP_SUB(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,n),         \
      hboost::multi_index::detail::noarg() HBOOST_PP_INTERCEPT)        \
  );                                                                 \
}

#define HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_EXTRA_ARG(           \
  ret,name_from,name_to,extra_arg_type,extra_arg_name)               \
HBOOST_PP_REPEAT_FROM_TO(                                             \
  0,HBOOST_PP_ADD(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,1),           \
  HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_EXTRA_ARG_AUX,             \
  (ret)(name_from)(name_to)(extra_arg_type)(extra_arg_name))

namespace hboost{
  
namespace multi_index{
  
namespace detail{

#define HBOOST_MULTI_INDEX_VARTEMPL_TO_PLACEMENT_NEW_AUX(z,n,name)    \
template<                                                            \
  typename Value                                                     \
  HBOOST_PP_COMMA_IF(n)                                               \
  HBOOST_PP_ENUM_PARAMS(n,typename T)                                 \
>                                                                    \
Value* name(                                                         \
  Value* x                                                           \
  HBOOST_PP_COMMA_IF(n)                                               \
  HBOOST_PP_ENUM(n,HBOOST_MULTI_INDEX_VARTEMPL_ARG,~)                  \
  HBOOST_PP_COMMA_IF(                                                 \
    HBOOST_PP_SUB(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,n))           \
  HBOOST_PP_ENUM_PARAMS(                                              \
    HBOOST_PP_SUB(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,n),           \
    HBOOST_FWD_REF(noarg) HBOOST_PP_INTERCEPT))                        \
{                                                                    \
  return new(x) Value(                                               \
    HBOOST_PP_ENUM(n,HBOOST_MULTI_INDEX_VARTEMPL_FORWARD_ARG,~));      \
}

#define HBOOST_MULTI_INDEX_VARTEMPL_TO_PLACEMENT_NEW(name)            \
HBOOST_PP_REPEAT_FROM_TO(                                             \
  0,HBOOST_PP_ADD(HBOOST_MULTI_INDEX_LIMIT_VARTEMPL_ARGS,1),           \
  HBOOST_MULTI_INDEX_VARTEMPL_TO_PLACEMENT_NEW_AUX,                   \
  name)

HBOOST_MULTI_INDEX_VARTEMPL_TO_PLACEMENT_NEW(vartempl_placement_new)

#undef HBOOST_MULTI_INDEX_VARTEMPL_TO_PLACEMENT_NEW_AUX
#undef HBOOST_MULTI_INDEX_VARTEMPL_TO_PLACEMENT_NEW

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace hboost */

#else

/* native variadic templates support */

#include <utility>

#define HBOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK typename... Args
#define HBOOST_MULTI_INDEX_FUNCTION_PARAM_PACK Args&&... args
#define HBOOST_MULTI_INDEX_FORWARD_PARAM_PACK  std::forward<Args>(args)...
#define HBOOST_MULTI_INDEX_NULL_PARAM_PACK

#define HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL(                     \
  ret,name_from,name_to)                                             \
template<typename... Args> ret name_from(Args&&... args)             \
{                                                                    \
  return name_to(std::forward<Args>(args)...);                       \
}

#define HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_EXTRA_ARG(           \
  ret,name_from,name_to,extra_arg_type,extra_arg_name)               \
template<typename... Args> ret name_from(                            \
  extra_arg_type extra_arg_name,Args&&... args)                      \
{                                                                    \
  return name_to(extra_arg_name,std::forward<Args>(args)...);        \
}

namespace hboost{
  
namespace multi_index{
  
namespace detail{

template<typename Value,typename... Args>
Value* vartempl_placement_new(Value*x,Args&&... args)
{
  return new(x) Value(std::forward<Args>(args)...);
}

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace hboost */

#endif
#endif
