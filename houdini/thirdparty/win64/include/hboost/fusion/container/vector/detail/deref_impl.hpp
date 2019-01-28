/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_DEREF_IMPL_05042005_1037)
#define HBOOST_FUSION_DEREF_IMPL_05042005_1037

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/fusion/container/vector/detail/value_at_impl.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/mpl/if.hpp>

namespace hboost { namespace fusion
{
    struct vector_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<vector_iterator_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef typename Iterator::vector vector;
                typedef typename Iterator::index index;
                typedef typename value_at_impl<vector_tag>::template apply<vector, index>::type element;

                typedef typename
                    mpl::if_<
                        is_const<vector>
                      , typename fusion::detail::cref_result<element>::type
                      , typename fusion::detail::ref_result<element>::type
                    >::type
                type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Iterator const& i)
                {
                    return i.vec.at_impl(index());
                }
            };
        };
    }
}}

#endif