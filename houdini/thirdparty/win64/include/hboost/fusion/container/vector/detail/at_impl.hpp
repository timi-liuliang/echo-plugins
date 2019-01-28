/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_AT_IMPL_05042005_0741)
#define HBOOST_FUSION_AT_IMPL_05042005_0741

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/fusion/container/vector/detail/value_at_impl.hpp>
#include <hboost/static_assert.hpp>

namespace hboost { namespace fusion
{
    struct vector_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_impl;

        template <>
        struct at_impl<vector_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                typedef typename value_at_impl<vector_tag>::template apply<Sequence, N>::type element;
                typedef typename detail::ref_result<element>::type type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence& v)
                {
                    HBOOST_STATIC_ASSERT((N::value < Sequence::size::value));
                    return v.at_impl(N());
                }
            };

            template <typename Sequence, typename N>
            struct apply <Sequence const, N>
            {
                typedef typename value_at_impl<vector_tag>::template apply<Sequence, N>::type element;
                typedef typename detail::cref_result<element>::type type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence const& v)
                {
                    HBOOST_STATIC_ASSERT((N::value < Sequence::size::value));
                    return v.at_impl(N());
                }
            };
        };
    }
}}

#endif
