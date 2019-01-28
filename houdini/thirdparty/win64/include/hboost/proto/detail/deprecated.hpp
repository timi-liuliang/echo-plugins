///////////////////////////////////////////////////////////////////////////////
/// \file deprecated.hpp
/// Definition of the deprecated HBOOST_PROTO_DEFINE_FUCTION_TEMPLATE and
/// HBOOST_PROTO_DEFINE_VARARG_FUCTION_TEMPLATE macros
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_DETAIL_DEPRECATED_HPP_EAN_11_25_2008
#define HBOOST_PROTO_DETAIL_DEPRECATED_HPP_EAN_11_25_2008

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/arithmetic/sub.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/control/expr_if.hpp>
#include <hboost/preprocessor/comparison/greater.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/to_list.hpp>
#include <hboost/preprocessor/logical/and.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/seq/enum.hpp>
#include <hboost/preprocessor/seq/seq.hpp>
#include <hboost/preprocessor/seq/to_tuple.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/pop_back.hpp>
#include <hboost/preprocessor/seq/push_back.hpp>
#include <hboost/preprocessor/seq/push_front.hpp>
#include <hboost/preprocessor/list/for_each_i.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <hboost/proto/proto_fwd.hpp>

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_TEMPLATE_AUX_(R, DATA, I, ELEM)                                      \
    (ELEM HBOOST_PP_CAT(HBOOST_PP_CAT(X, DATA), HBOOST_PP_CAT(_, I)))                              \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_TEMPLATE_YES_(R, DATA, I, ELEM)                                      \
    HBOOST_PP_LIST_FOR_EACH_I_R(                                                                 \
        R                                                                                       \
      , HBOOST_PROTO_VARARG_TEMPLATE_AUX_                                                        \
      , I                                                                                       \
      , HBOOST_PP_TUPLE_TO_LIST(                                                                 \
            HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ELEM))                                               \
          , HBOOST_PP_SEQ_TO_TUPLE(HBOOST_PP_SEQ_TAIL(ELEM))                                      \
        )                                                                                       \
    )                                                                                           \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_TEMPLATE_NO_(R, DATA, I, ELEM)                                       \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_TEMPLATE_(R, DATA, I, ELEM)                                          \
    HBOOST_PP_IF(                                                                                \
        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ELEM))                                                   \
      , HBOOST_PROTO_VARARG_TEMPLATE_YES_                                                        \
      , HBOOST_PROTO_VARARG_TEMPLATE_NO_                                                         \
    )(R, DATA, I, ELEM)                                                                         \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_TYPE_AUX_(R, DATA, I, ELEM)                                          \
    (HBOOST_PP_CAT(HBOOST_PP_CAT(X, DATA), HBOOST_PP_CAT(_, I)))                                   \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_TEMPLATE_PARAMS_YES_(R, DATA, I, ELEM)                                      \
    <                                                                                           \
        HBOOST_PP_SEQ_ENUM(                                                                      \
            HBOOST_PP_LIST_FOR_EACH_I_R(                                                         \
                R                                                                               \
              , HBOOST_PROTO_VARARG_TYPE_AUX_                                                    \
              , I                                                                               \
              , HBOOST_PP_TUPLE_TO_LIST(                                                         \
                    HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ELEM))                                       \
                  , HBOOST_PP_SEQ_TO_TUPLE(HBOOST_PP_SEQ_TAIL(ELEM))                              \
                )                                                                               \
            )                                                                                   \
        )                                                                                       \
    >                                                                                           \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_TEMPLATE_PARAMS_NO_(R, DATA, I, ELEM)                                       \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_TYPE_(R, DATA, I, ELEM)                                              \
    HBOOST_PP_COMMA_IF(I)                                                                        \
    HBOOST_PP_SEQ_HEAD(ELEM)                                                                     \
    HBOOST_PP_IF(                                                                                \
        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ELEM))                                                   \
      , HBOOST_PROTO_TEMPLATE_PARAMS_YES_                                                        \
      , HBOOST_PROTO_TEMPLATE_PARAMS_NO_                                                         \
    )(R, DATA, I, ELEM) HBOOST_PP_EXPR_IF(HBOOST_PP_GREATER(I, 1), const)                         \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_AS_EXPR_(R, DATA, I, ELEM)                                           \
    HBOOST_PP_EXPR_IF(                                                                           \
        HBOOST_PP_GREATER(I, 1)                                                                  \
      , ((                                                                                      \
            HBOOST_PP_SEQ_HEAD(ELEM)                                                             \
            HBOOST_PP_IF(                                                                        \
                HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ELEM))                                           \
              , HBOOST_PROTO_TEMPLATE_PARAMS_YES_                                                \
              , HBOOST_PROTO_TEMPLATE_PARAMS_NO_                                                 \
            )(R, DATA, I, ELEM)()                                                               \
        ))                                                                                      \
    )                                                                                           \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_AS_CHILD_(Z, N, DATA)                                                \
    (HBOOST_PP_CAT(DATA, N))                                                                     \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_SEQ_PUSH_FRONT(SEQ, ELEM)                                                   \
    HBOOST_PP_SEQ_POP_BACK(HBOOST_PP_SEQ_PUSH_FRONT(HBOOST_PP_SEQ_PUSH_BACK(SEQ, _dummy_), ELEM))  \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_AS_PARAM_(Z, N, DATA)                                                \
    (HBOOST_PP_CAT(DATA, N))                                                                     \
    /**/

/// INTERNAL ONLY
///
#define HBOOST_PROTO_VARARG_FUN_(Z, N, DATA)                                                     \
    template<                                                                                   \
        HBOOST_PP_SEQ_ENUM(                                                                      \
            HBOOST_PP_SEQ_FOR_EACH_I(                                                            \
                HBOOST_PROTO_VARARG_TEMPLATE_, ~                                                 \
              , HBOOST_PP_SEQ_PUSH_FRONT(                                                        \
                    HBOOST_PROTO_SEQ_PUSH_FRONT(                                                 \
                        HBOOST_PP_TUPLE_ELEM(4, 2, DATA)                                         \
                      , (HBOOST_PP_TUPLE_ELEM(4, 3, DATA))                                       \
                    )                                                                           \
                  , HBOOST_PP_TUPLE_ELEM(4, 1, DATA)                                             \
                )                                                                               \
            )                                                                                   \
            HBOOST_PP_REPEAT_ ## Z(N, HBOOST_PROTO_VARARG_AS_PARAM_, typename A)                  \
        )                                                                                       \
    >                                                                                           \
    typename hboost::proto::result_of::make_expr<                                                \
        HBOOST_PP_SEQ_FOR_EACH_I(                                                                \
            HBOOST_PROTO_VARARG_TYPE_, ~                                                         \
          , HBOOST_PP_SEQ_PUSH_FRONT(                                                            \
                HBOOST_PROTO_SEQ_PUSH_FRONT(                                                     \
                    HBOOST_PP_TUPLE_ELEM(4, 2, DATA)                                             \
                  , (HBOOST_PP_TUPLE_ELEM(4, 3, DATA))                                           \
                )                                                                               \
              , HBOOST_PP_TUPLE_ELEM(4, 1, DATA)                                                 \
            )                                                                                   \
        )                                                                                       \
        HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(Z, N, A, const & HBOOST_PP_INTERCEPT)             \
    >::type const                                                                               \
    HBOOST_PP_TUPLE_ELEM(4, 0, DATA)(HBOOST_PP_ENUM_BINARY_PARAMS_Z(Z, N, A, const &a))           \
    {                                                                                           \
        return hboost::proto::detail::make_expr_<                                                \
            HBOOST_PP_SEQ_FOR_EACH_I(                                                            \
                HBOOST_PROTO_VARARG_TYPE_, ~                                                     \
              , HBOOST_PP_SEQ_PUSH_FRONT(                                                        \
                    HBOOST_PROTO_SEQ_PUSH_FRONT(                                                 \
                        HBOOST_PP_TUPLE_ELEM(4, 2, DATA)                                         \
                      , (HBOOST_PP_TUPLE_ELEM(4, 3, DATA))                                       \
                    )                                                                           \
                  , HBOOST_PP_TUPLE_ELEM(4, 1, DATA)                                             \
                )                                                                               \
            )                                                                                   \
            HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(Z, N, A, const & HBOOST_PP_INTERCEPT)         \
        >()(                                                                                    \
            HBOOST_PP_SEQ_ENUM(                                                                  \
                HBOOST_PP_SEQ_FOR_EACH_I(                                                        \
                    HBOOST_PROTO_VARARG_AS_EXPR_, ~                                              \
                  , HBOOST_PP_SEQ_PUSH_FRONT(                                                    \
                        HBOOST_PROTO_SEQ_PUSH_FRONT(                                             \
                            HBOOST_PP_TUPLE_ELEM(4, 2, DATA)                                     \
                          , (HBOOST_PP_TUPLE_ELEM(4, 3, DATA))                                   \
                        )                                                                       \
                      , HBOOST_PP_TUPLE_ELEM(4, 1, DATA)                                         \
                    )                                                                           \
                )                                                                               \
                HBOOST_PP_REPEAT_ ## Z(N, HBOOST_PROTO_VARARG_AS_CHILD_, a)                       \
            )                                                                                   \
        );                                                                                      \
    }                                                                                           \
    /**/

/// \code
/// HBOOST_PROTO_DEFINE_FUNCTION_TEMPLATE(
///     1
///   , construct
///   , hboost::proto::default_domain
///   , (hboost::proto::tag::function)
///   , ((op::construct)(typename)(int))
/// )
/// \endcode
#define HBOOST_PROTO_DEFINE_FUNCTION_TEMPLATE(ARGCOUNT, NAME, DOMAIN, TAG, BOUNDARGS)            \
    HBOOST_PP_REPEAT_FROM_TO(                                                                    \
        ARGCOUNT                                                                                \
      , HBOOST_PP_INC(ARGCOUNT)                                                                  \
      , HBOOST_PROTO_VARARG_FUN_                                                                 \
      , (NAME, TAG, BOUNDARGS, DOMAIN)                                                          \
    )\
    /**/

/// \code
/// HBOOST_PROTO_DEFINE_VARARG_FUNCTION_TEMPLATE(
///     construct
///   , hboost::proto::default_domain
///   , (hboost::proto::tag::function)
///   , ((op::construct)(typename)(int))
/// )
/// \endcode
#define HBOOST_PROTO_DEFINE_VARARG_FUNCTION_TEMPLATE(NAME, DOMAIN, TAG, BOUNDARGS)               \
    HBOOST_PP_REPEAT(                                                                            \
        HBOOST_PP_SUB(HBOOST_PP_INC(HBOOST_PROTO_MAX_ARITY), HBOOST_PP_SEQ_SIZE(BOUNDARGS))         \
      , HBOOST_PROTO_VARARG_FUN_                                                                 \
      , (NAME, TAG, BOUNDARGS, DOMAIN)                                                          \
    )                                                                                           \
    /**/

#endif
