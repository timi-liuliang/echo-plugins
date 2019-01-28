/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_PUSH_BACK_10022005_1647)
#define HBOOST_FUSION_PUSH_BACK_10022005_1647

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/push_back.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/algorithm/transformation/push_back.hpp>
#include <hboost/fusion/sequence/convert.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct push_back_impl;

    template <>
    struct push_back_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence, typename T>
        struct apply
        {
            typedef typename
                fusion::result_of::push_back<Sequence, T>::type
            result;

            typedef typename
                fusion::result_of::convert<
                    typename fusion::detail::tag_of<Sequence>::type, result>::type
            type;
        };
    };
}}

#endif

