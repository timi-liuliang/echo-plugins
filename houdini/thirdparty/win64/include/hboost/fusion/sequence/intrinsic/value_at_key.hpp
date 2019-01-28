/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VALUE_AT_KEY_05052005_0229)
#define HBOOST_FUSION_VALUE_AT_KEY_05052005_0229

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/empty_base.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/fusion/sequence/intrinsic_fwd.hpp>
#include <hboost/fusion/sequence/intrinsic/has_key.hpp>
#include <hboost/fusion/iterator/value_of_data.hpp>
#include <hboost/fusion/algorithm/query/find.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/support/category_of.hpp>

namespace hboost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag;
    struct hboost_array_tag; // hboost::array tag
    struct mpl_sequence_tag; // mpl sequence tag
    struct std_pair_tag; // std::pair tag

    namespace extension
    {
        template <typename Tag>
        struct value_at_key_impl
        {
            template <typename Seq, typename Key>
            struct apply
              : result_of::value_of_data<
                    typename result_of::find<Seq, Key>::type
                >
            {};
        };

        template <>
        struct value_at_key_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename Key>
            struct apply : Sequence::template value_at_key<Sequence, Key> {};
        };

        template <>
        struct value_at_key_impl<hboost_array_tag>;

        template <>
        struct value_at_key_impl<mpl_sequence_tag>;

        template <>
        struct value_at_key_impl<std_pair_tag>;
    }

    namespace detail
    {
        template <typename Sequence, typename N, typename Tag>
        struct value_at_key_impl
            : mpl::if_<
                  mpl::or_<
                      typename extension::has_key_impl<Tag>::template apply<Sequence, N>
                    , traits::is_unbounded<Sequence>
                  >
                , typename extension::value_at_key_impl<Tag>::template apply<Sequence, N>
                , mpl::empty_base
              >::type
        {};
    }

    namespace result_of
    {
        template <typename Sequence, typename N>
        struct value_at_key
            : detail::value_at_key_impl<Sequence, N, typename detail::tag_of<Sequence>::type>
        {};
    }
}}

#endif

