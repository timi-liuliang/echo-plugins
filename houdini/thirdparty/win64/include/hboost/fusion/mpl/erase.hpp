/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ERASE_10022005_1835)
#define HBOOST_FUSION_ERASE_10022005_1835

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/erase.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/algorithm/transformation/erase.hpp>
#include <hboost/fusion/sequence/convert.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct erase_impl;

    template <>
    struct erase_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence, typename First, typename Last>
        struct apply
        {
            typedef typename
                fusion::result_of::erase<Sequence, First, Last>::type
            result;

            typedef typename
                fusion::result_of::convert<
                    typename fusion::detail::tag_of<Sequence>::type, result>::type
            type;
        };
    };
}}

#endif

