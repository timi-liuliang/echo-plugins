/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VALUE_AT_05052005_0229)
#define HBOOST_FUSION_VALUE_AT_05052005_0229

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/mpl/less.hpp>
#include <hboost/mpl/empty_base.hpp>
#include <hboost/fusion/sequence/intrinsic_fwd.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/support/category_of.hpp>

namespace hboost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag;
    struct hboost_tuple_tag; // hboost::tuples::tuple tag
    struct hboost_array_tag; // hboost::array tag
    struct mpl_sequence_tag; // mpl sequence tag
    struct std_pair_tag; // std::pair tag

    namespace extension
    {
        template <typename Tag>
        struct value_at_impl
        {
            template <typename Sequence, typename N>
            struct apply;
        };

        template <>
        struct value_at_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename N>
            struct apply : Sequence::template value_at<Sequence, N> {};
        };

        template <>
        struct value_at_impl<hboost_tuple_tag>;

        template <>
        struct value_at_impl<hboost_array_tag>;

        template <>
        struct value_at_impl<mpl_sequence_tag>;

        template <>
        struct value_at_impl<std_pair_tag>;
    }

    namespace detail
    {
        template <typename Sequence, typename N, typename Tag>
        struct value_at_impl
            : mpl::if_<
                  mpl::or_<
                      mpl::less<N, typename extension::size_impl<Tag>::template apply<Sequence>::type>
                    , traits::is_unbounded<Sequence>
                  >
                , typename extension::value_at_impl<Tag>::template apply<Sequence, N>
                , mpl::empty_base
              >::type
        {};
    }

    namespace result_of
    {
        template <typename Sequence, typename N>
        struct value_at
            : detail::value_at_impl<Sequence, N, typename detail::tag_of<Sequence>::type>
        {};

        template <typename Sequence, int N>
        struct value_at_c
            : fusion::result_of::value_at<Sequence, mpl::int_<N> >
        {};
    }
}}

#endif

