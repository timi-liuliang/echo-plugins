/*=============================================================================
    Copyright (c) 2013-2014 Damien Buhl

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ADAPTED_STRUCT_DETAIL_ADAPT_BASE_ATTR_FILLER_HPP
#define HBOOST_FUSION_ADAPTED_STRUCT_DETAIL_ADAPT_BASE_ATTR_FILLER_HPP

#include <hboost/config.hpp>

#include <hboost/fusion/adapted/struct/detail/adapt_auto.hpp>
#include <hboost/fusion/adapted/struct/detail/preprocessor/is_seq.hpp>

#include <hboost/mpl/aux_/preprocessor/token_equal.hpp>

#include <hboost/preprocessor/config/config.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/control/expr_iif.hpp>
#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/push_front.hpp>
#include <hboost/preprocessor/facilities/expand.hpp>
#include <hboost/preprocessor/facilities/is_empty.hpp>


#define HBOOST_FUSION_ADAPT_STRUCT_FILLER_0(X, Y)                                \
    HBOOST_FUSION_ADAPT_STRUCT_WRAP_ATTR(X,Y)                                    \
    HBOOST_FUSION_ADAPT_STRUCT_FILLER_1

#define HBOOST_FUSION_ADAPT_STRUCT_FILLER_1(X, Y)                                \
    HBOOST_FUSION_ADAPT_STRUCT_WRAP_ATTR(X,Y)                                    \
    HBOOST_FUSION_ADAPT_STRUCT_FILLER_0

#define HBOOST_FUSION_ADAPT_STRUCT_FILLER_0_END
#define HBOOST_FUSION_ADAPT_STRUCT_FILLER_1_END

#define HBOOST_FUSION_ADAPT_STRUCT_WRAP_ATTR(X, Y)                               \
    HBOOST_PP_IIF(HBOOST_MPL_PP_TOKEN_EQUAL(auto, HBOOST_PP_EXPAND(X)),            \
      ((1, (Y))),                                                               \
      ((2, (X,Y)))                                                              \
    )

#define HBOOST_FUSION_ADAPT_STRUCT_WRAPPEDATTR_SIZE(ATTRIBUTE)                   \
  HBOOST_PP_TUPLE_ELEM(2, 0, ATTRIBUTE)

#define HBOOST_FUSION_ADAPT_STRUCT_WRAPPEDATTR(ATTRIBUTE)                        \
  HBOOST_PP_TUPLE_ELEM(2, 1, ATTRIBUTE)


#if HBOOST_PP_VARIADICS

#   define HBOOST_FUSION_ADAPT_STRUCT_ATTRIBUTES_FILLER_OP(r, unused, elem)      \
        HBOOST_PP_IIF(HBOOST_FUSION_PP_IS_SEQ(elem),                              \
            HBOOST_PP_CAT(HBOOST_FUSION_ADAPT_STRUCT_FILLER_0 elem ,_END),        \
            HBOOST_PP_EXPR_IIF(HBOOST_PP_COMPL(HBOOST_PP_IS_EMPTY(elem)),          \
                HBOOST_FUSION_ADAPT_STRUCT_WRAP_ATTR(auto, elem)))

#   define HBOOST_FUSION_ADAPT_STRUCT_ATTRIBUTES_FILLER(VA_ARGS_SEQ)             \
        HBOOST_PP_SEQ_PUSH_FRONT(                                                \
            HBOOST_PP_SEQ_FOR_EACH(                                              \
                HBOOST_FUSION_ADAPT_STRUCT_ATTRIBUTES_FILLER_OP,                 \
                unused, VA_ARGS_SEQ),                                           \
            (0,0))

#endif // HBOOST_PP_VARIADICS

#endif
