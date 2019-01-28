/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(HBOOST_FUSION_ITERATOR_RANGE_AT_IMPL_HPP_INCLUDED)
#define HBOOST_FUSION_ITERATOR_RANGE_AT_IMPL_HPP_INCLUDED

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/advance.hpp>
#include <hboost/fusion/iterator/deref.hpp>

namespace hboost { namespace fusion
{
    struct iterator_range_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_impl;

        template <>
        struct at_impl<iterator_range_tag>
        {
            template <typename Seq, typename N>
            struct apply
            {
                typedef typename Seq::begin_type begin_type;
                typedef typename result_of::advance<begin_type,N>::type pos;
                typedef typename result_of::deref<pos>::type type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Seq& s)
                {
                    return * fusion::advance<N>(s.first);
                }
            };
        };
    }
}}

#endif

