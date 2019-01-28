/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2009-2010 Hartmut Kaiser
    Copyright (c) 2010-2011 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_NAMED_HPP
#define HBOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_NAMED_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/adapted/struct/adapt_struct.hpp>
#include <hboost/fusion/adapted/struct/detail/proxy_type.hpp>
#include <hboost/preprocessor/config/config.hpp>

#ifdef HBOOST_PP_VARIADICS

#   define HBOOST_FUSION_ADAPT_STRUCT_NAMED_NS(                                  \
        WRAPPED_TYPE, NAMESPACE_SEQ, NAME, ...)                                 \
                                                                                \
        HBOOST_FUSION_ADAPT_STRUCT_DEFINE_PROXY_TYPE_IMPL(                       \
            WRAPPED_TYPE,(0)NAMESPACE_SEQ,NAME)                                 \
                                                                                \
        HBOOST_FUSION_ADAPT_STRUCT_AS_VIEW(                                      \
            HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION(                    \
                (0)NAMESPACE_SEQ)NAME,                                          \
            __VA_ARGS__)

#   define HBOOST_FUSION_ADAPT_STRUCT_NAMED(WRAPPED_TYPE, NAME, ...)             \
        HBOOST_FUSION_ADAPT_STRUCT_NAMED_NS(                                     \
            WRAPPED_TYPE,(hboost)(fusion)(adapted),NAME,__VA_ARGS__)


#else // HBOOST_PP_VARIADICS

#   define HBOOST_FUSION_ADAPT_STRUCT_NAMED_NS(                                  \
        WRAPPED_TYPE, NAMESPACE_SEQ, NAME, ATTRIBUTES)                          \
                                                                                \
        HBOOST_FUSION_ADAPT_STRUCT_DEFINE_PROXY_TYPE_IMPL(                       \
            WRAPPED_TYPE,(0)NAMESPACE_SEQ,NAME)                                 \
                                                                                \
        HBOOST_FUSION_ADAPT_STRUCT_AS_VIEW(                                      \
            HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION(                    \
                (0)NAMESPACE_SEQ)NAME,                                          \
            ATTRIBUTES)

#   define HBOOST_FUSION_ADAPT_STRUCT_NAMED(WRAPPED_TYPE, NAME, ATTRIBUTES)      \
        HBOOST_FUSION_ADAPT_STRUCT_NAMED_NS(                                     \
            WRAPPED_TYPE,(hboost)(fusion)(adapted),NAME,ATTRIBUTES)

#endif

#endif
