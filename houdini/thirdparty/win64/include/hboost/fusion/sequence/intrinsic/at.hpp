/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_AT_05042005_0722)
#define HBOOST_FUSION_AT_05042005_0722

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/mpl/less.hpp>
#include <hboost/mpl/empty_base.hpp>
#include <hboost/type_traits/is_const.hpp>
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
    struct std_tuple_tag; // std::tuple tag

    namespace extension
    {
        template <typename Tag>
        struct at_impl
        {
            template <typename Sequence, typename N>
            struct apply;
        };

        template <>
        struct at_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename N>
            struct apply : Sequence::template at<Sequence, N> {};
        };

        template <>
        struct at_impl<hboost_tuple_tag>;

        template <>
        struct at_impl<hboost_array_tag>;

        template <>
        struct at_impl<mpl_sequence_tag>;

        template <>
        struct at_impl<std_pair_tag>;

        template <>
        struct at_impl<std_tuple_tag>;
    }

    namespace detail
    {
        template <typename Sequence, typename N, typename Tag>
        struct at_impl
            : mpl::if_<
                  mpl::or_<
                      mpl::less<N, typename extension::size_impl<Tag>::template apply<Sequence>::type>
                    , traits::is_unbounded<Sequence>
                  >
                , typename extension::at_impl<Tag>::template apply<Sequence, N>
                , mpl::empty_base
              >::type
        {};
    }

    namespace result_of
    {
        template <typename Sequence, typename N>
        struct at
            : detail::at_impl<Sequence, N, typename detail::tag_of<Sequence>::type>
        {};

        template <typename Sequence, int N>
        struct at_c
            : at<Sequence, mpl::int_<N> >
        {};
    }


    template <typename N, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename
        lazy_disable_if<
            is_const<Sequence>
          , result_of::at<Sequence, N>
        >::type
    at(Sequence& seq)
    {
        return result_of::at<Sequence, N>::call(seq);
    }

    template <typename N, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::at<Sequence const, N>::type
    at(Sequence const& seq)
    {
        return result_of::at<Sequence const, N>::call(seq);
    }

    template <int N, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename
        lazy_disable_if<
            is_const<Sequence>
          , result_of::at_c<Sequence, N>
        >::type
    at_c(Sequence& seq)
    {
        return fusion::at<mpl::int_<N> >(seq);
    }

    template <int N, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::at_c<Sequence const, N>::type
    at_c(Sequence const& seq)
    {
        return fusion::at<mpl::int_<N> >(seq);
    }
}}

#endif

