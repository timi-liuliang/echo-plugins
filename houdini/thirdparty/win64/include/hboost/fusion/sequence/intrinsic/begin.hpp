/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_BEGIN_04052005_1132)
#define HBOOST_FUSION_BEGIN_04052005_1132

#include <hboost/fusion/support/config.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/mpl/empty_base.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/fusion/sequence/intrinsic_fwd.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/fusion/support/is_segmented.hpp>
#include <hboost/fusion/sequence/intrinsic/detail/segmented_begin.hpp>

namespace hboost { namespace fusion
{
    // Special tags:
    struct sequence_facade_tag; // iterator facade tag
    struct hboost_tuple_tag; // hboost::tuples::tuple tag
    struct hboost_array_tag; // hboost::array tag
    struct mpl_sequence_tag; // mpl sequence tag
    struct std_pair_tag; // std::pair tag

    namespace extension
    {
        template <typename Tag>
        struct begin_impl
        {
            template <typename Sequence>
            struct apply
              : mpl::if_<
                    traits::is_segmented<Sequence>
                  , detail::segmented_begin<Sequence>
                  , mpl::empty_base
                >::type
            {};
        };

        template <>
        struct begin_impl<sequence_facade_tag>
        {
            template <typename Sequence>
            struct apply : Sequence::template begin<Sequence> {};
        };

        template <>
        struct begin_impl<hboost_tuple_tag>;

        template <>
        struct begin_impl<hboost_array_tag>;

        template <>
        struct begin_impl<mpl_sequence_tag>;

        template <>
        struct begin_impl<std_pair_tag>;
    }

    namespace result_of
    {
        template <typename Sequence>
        struct begin
            : extension::begin_impl<typename detail::tag_of<Sequence>::type>::
                template apply<Sequence>
        {};
    }

    template <typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename
        lazy_enable_if<
            traits::is_sequence<Sequence>
          , result_of::begin<Sequence>
        >::type const
    begin(Sequence& seq)
    {
        return result_of::begin<Sequence>::call(seq);
    }

    template <typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename
        lazy_enable_if<
            traits::is_sequence<Sequence>
          , result_of::begin<Sequence const>
        >::type const
    begin(Sequence const& seq)
    {
        return result_of::begin<Sequence const>::call(seq);
    }
}}

#endif
