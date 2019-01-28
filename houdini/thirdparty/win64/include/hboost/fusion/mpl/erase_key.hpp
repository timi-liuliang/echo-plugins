/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ERASE_KEY_10022005_1907)
#define HBOOST_FUSION_ERASE_KEY_10022005_1907

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/erase_key.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/algorithm/transformation/erase_key.hpp>
#include <hboost/fusion/sequence/convert.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct erase_key_impl;

    template <>
    struct erase_key_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence, typename Key>
        struct apply
        {
            typedef typename
                fusion::result_of::erase_key<Sequence, Key>::type
            result;

            typedef typename
                fusion::result_of::convert<
                    typename fusion::detail::tag_of<Sequence>::type, result>::type
            type;
        };
    };
}}

#endif

