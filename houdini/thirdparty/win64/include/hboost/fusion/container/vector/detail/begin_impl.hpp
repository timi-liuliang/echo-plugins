/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_BEGIN_IMPL_05042005_1136)
#define HBOOST_FUSION_BEGIN_IMPL_05042005_1136

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/vector_iterator.hpp>

namespace hboost { namespace fusion
{
    struct vector_tag;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<vector_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef vector_iterator<Sequence, 0> type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif
