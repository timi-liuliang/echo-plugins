/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_INSERT_RANGE_10022005_1838)
#define HBOOST_FUSION_INSERT_RANGE_10022005_1838

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/insert_range.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/algorithm/transformation/insert_range.hpp>
#include <hboost/fusion/sequence/convert.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct insert_range_impl;

    template <>
    struct insert_range_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence, typename Pos, typename Range>
        struct apply
        {
            typedef typename
                fusion::result_of::insert_range<Sequence, Pos, Range>::type
            result;

            typedef typename
                fusion::result_of::convert<
                    typename fusion::detail::tag_of<Sequence>::type, result>::type
            type;
        };
    };
}}

#endif

