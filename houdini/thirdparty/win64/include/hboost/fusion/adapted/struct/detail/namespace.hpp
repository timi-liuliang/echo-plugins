/*=============================================================================
    Copyright (c) 2009-2010 Hartmut Kaiser
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ADAPTED_DETAIL_STRUCT_NAMESPACE_HPP
#define HBOOST_FUSION_ADAPTED_DETAIL_STRUCT_NAMESPACE_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/seq/seq.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/tuple/eat.hpp>

#define HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_BEGIN_I(R,DATA,ELEM)                \
    namespace ELEM {
#define HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_END_I(Z,I,DATA) }
#define HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION_I(Z,I,ELEM) ELEM::

#define HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_BEGIN(NAMESPACE_SEQ)     \
    HBOOST_PP_IF(                                                                \
        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(NAMESPACE_SEQ)),                         \
        HBOOST_PP_SEQ_FOR_EACH_R,                                                \
        HBOOST_PP_TUPLE_EAT(4))(                                                 \
            1,                                                                  \
            HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_BEGIN_I,                        \
            _,                                                                  \
            HBOOST_PP_SEQ_TAIL(NAMESPACE_SEQ))

#define HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_END(NAMESPACE_SEQ)       \
    HBOOST_PP_REPEAT_1(                                                          \
        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(NAMESPACE_SEQ)),                         \
        HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_END_I,                              \
        _)

#define HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION(NAMESPACE_SEQ)          \
    HBOOST_PP_IF(                                                                \
        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(NAMESPACE_SEQ)),                         \
        HBOOST_PP_SEQ_FOR_EACH_R,                                                \
        HBOOST_PP_TUPLE_EAT(4))(                                                 \
            1,                                                                  \
            HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION_I,                  \
            _,                                                                  \
            HBOOST_PP_SEQ_TAIL(NAMESPACE_SEQ))

#endif
