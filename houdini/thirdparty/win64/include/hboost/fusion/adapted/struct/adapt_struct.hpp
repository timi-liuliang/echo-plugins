/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2009-2011 Christopher Schmidt
    Copyright (c) 2013-2014 Damien Buhl

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_HPP
#define HBOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/preprocessor/config/config.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/empty.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/comparison/less.hpp>
#include <hboost/preprocessor/seq/seq.hpp>
#include <hboost/preprocessor/variadic/to_seq.hpp>
#include <hboost/type_traits/add_reference.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/remove_const.hpp>

#include <hboost/fusion/adapted/struct/detail/extension.hpp>
#include <hboost/fusion/adapted/struct/detail/adapt_base.hpp>
#include <hboost/fusion/adapted/struct/detail/adapt_base_attr_filler.hpp>
#include <hboost/fusion/adapted/struct/detail/at_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/is_view_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/is_sequence_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/value_at_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/category_of_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/size_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/begin_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/end_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/value_of_impl.hpp>
#include <hboost/fusion/adapted/struct/detail/deref_impl.hpp>

#define HBOOST_FUSION_ADAPT_STRUCT_C(                                            \
    TEMPLATE_PARAMS_SEQ, NAME_SEQ, IS_VIEW, I, ATTRIBUTE)                       \
        HBOOST_FUSION_ADAPT_STRUCT_C_BASE(                                       \
            TEMPLATE_PARAMS_SEQ,                                                \
            NAME_SEQ,                                                           \
            IS_VIEW,                                                            \
            I,                                                                  \
            HBOOST_PP_IIF(IS_VIEW, HBOOST_FUSION_PROXY_PREFIX, HBOOST_PP_EMPTY),   \
            HBOOST_FUSION_ADAPT_STRUCT_WRAPPEDATTR(ATTRIBUTE),                   \
            HBOOST_FUSION_ADAPT_STRUCT_WRAPPEDATTR_SIZE(ATTRIBUTE),              \
            HBOOST_PP_LESS(                                                      \
                HBOOST_FUSION_ADAPT_STRUCT_WRAPPEDATTR_SIZE(ATTRIBUTE), 2))



#if HBOOST_PP_VARIADICS

#   define HBOOST_FUSION_ADAPT_TPL_STRUCT(TEMPLATE_PARAMS_SEQ,NAME_SEQ, ...)     \
        HBOOST_FUSION_ADAPT_STRUCT_BASE(                                         \
            (1)TEMPLATE_PARAMS_SEQ,                                             \
            (1)NAME_SEQ,                                                        \
            struct_tag,                                                         \
            0,                                                                  \
            HBOOST_FUSION_ADAPT_STRUCT_ATTRIBUTES_FILLER(                        \
                HBOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)),                         \
            HBOOST_FUSION_ADAPT_STRUCT_C)

#   define HBOOST_FUSION_ADAPT_STRUCT(...)                                       \
        HBOOST_FUSION_ADAPT_STRUCT_BASE(                                         \
            (0),                                                                \
            (0)(HBOOST_PP_SEQ_HEAD(HBOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))),      \
            struct_tag,                                                         \
            0,                                                                  \
            HBOOST_FUSION_ADAPT_STRUCT_ATTRIBUTES_FILLER(                        \
              HBOOST_PP_SEQ_TAIL(HBOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))),        \
            HBOOST_FUSION_ADAPT_STRUCT_C)

#   define HBOOST_FUSION_ADAPT_STRUCT_AS_VIEW(...)                               \
        HBOOST_FUSION_ADAPT_STRUCT_BASE(                                         \
            (0),                                                                \
            (0)(HBOOST_PP_SEQ_HEAD(HBOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))),      \
            struct_tag,                                                         \
            1,                                                                  \
            HBOOST_FUSION_ADAPT_STRUCT_ATTRIBUTES_FILLER(                        \
              HBOOST_PP_SEQ_TAIL(HBOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))),        \
            HBOOST_FUSION_ADAPT_STRUCT_C)

#else // HBOOST_PP_VARIADICS

#   define HBOOST_FUSION_ADAPT_TPL_STRUCT(                                       \
        TEMPLATE_PARAMS_SEQ,NAME_SEQ, ATTRIBUTES)                               \
        HBOOST_FUSION_ADAPT_STRUCT_BASE(                                         \
            (1)TEMPLATE_PARAMS_SEQ,                                             \
            (1)NAME_SEQ,                                                        \
            struct_tag,                                                         \
            0,                                                                  \
            HBOOST_PP_CAT(                                                       \
                HBOOST_FUSION_ADAPT_STRUCT_FILLER_0(0,0)ATTRIBUTES,_END),        \
            HBOOST_FUSION_ADAPT_STRUCT_C)

#   define HBOOST_FUSION_ADAPT_STRUCT(NAME, ATTRIBUTES)                          \
        HBOOST_FUSION_ADAPT_STRUCT_BASE(                                         \
            (0),                                                                \
            (0)(NAME),                                                          \
            struct_tag,                                                         \
            0,                                                                  \
            HBOOST_PP_CAT(                                                       \
                HBOOST_FUSION_ADAPT_STRUCT_FILLER_0(0,0)ATTRIBUTES,              \
                _END),                                                          \
            HBOOST_FUSION_ADAPT_STRUCT_C)

#   define HBOOST_FUSION_ADAPT_STRUCT_AS_VIEW(NAME, ATTRIBUTES)                  \
        HBOOST_FUSION_ADAPT_STRUCT_BASE(                                         \
            (0),                                                                \
            (0)(NAME),                                                          \
            struct_tag,                                                         \
            1,                                                                  \
            HBOOST_PP_CAT(                                                       \
                HBOOST_FUSION_ADAPT_STRUCT_FILLER_0(0,0)ATTRIBUTES,              \
                _END),                                                          \
            HBOOST_FUSION_ADAPT_STRUCT_C)


#endif // HBOOST_PP_VARIADICS

#endif
