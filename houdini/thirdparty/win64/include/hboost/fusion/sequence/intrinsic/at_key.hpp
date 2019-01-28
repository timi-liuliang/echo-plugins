/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_AT_KEY_20060304_1755)
#define HBOOST_FUSION_AT_KEY_20060304_1755

#include <hboost/fusion/support/config.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/fusion/sequence/intrinsic_fwd.hpp>
#include <hboost/fusion/sequence/intrinsic/has_key.hpp>
#include <hboost/fusion/algorithm/query/find.hpp>
#include <hboost/fusion/iterator/deref_data.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/support/category_of.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/mpl/empty_base.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/or.hpp>

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
        struct at_key_impl
        {
            template <typename Seq, typename Key>
            struct apply
            {
                typedef typename
                    result_of::deref_data<
                        typename result_of::find<Seq, Key>::type
                    >::type
                type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Seq& seq)
                {
                    return fusion::deref_data(fusion::find<Key>(seq));
                }
            };
        };

        template <>
        struct at_key_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename Key>
            struct apply : Sequence::template at_key_impl<Sequence, Key> {};
        };

        template <>
        struct at_key_impl<hboost_array_tag>;

        template <>
        struct at_key_impl<mpl_sequence_tag>;

        template <>
        struct at_key_impl<std_pair_tag>;
    }

    namespace detail
    {
        template <typename Sequence, typename Key, typename Tag>
        struct at_key_impl
            : mpl::if_<
                  mpl::or_<
                      typename extension::has_key_impl<Tag>::template apply<Sequence, Key>
                    , traits::is_unbounded<Sequence>
                  >
                , typename extension::at_key_impl<Tag>::template apply<Sequence, Key>
                , mpl::empty_base
              >::type
        {};
    }

    namespace result_of
    {
        template <typename Sequence, typename Key>
        struct at_key
            : detail::at_key_impl<Sequence, Key, typename detail::tag_of<Sequence>::type>
        {};
    }

    template <typename Key, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename 
        lazy_disable_if<
            is_const<Sequence>
          , result_of::at_key<Sequence, Key>
        >::type
    at_key(Sequence& seq)
    {
        return result_of::at_key<Sequence, Key>::call(seq);
    }

    template <typename Key, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::at_key<Sequence const, Key>::type
    at_key(Sequence const& seq)
    {
        return result_of::at_key<Sequence const, Key>::call(seq);
    }
}}

#endif
