/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_HAS_KEY_09232005_1454)
#define HBOOST_FUSION_HAS_KEY_09232005_1454

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/sequence/intrinsic_fwd.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/iterator/equal_to.hpp>
#include <hboost/fusion/algorithm/query/find.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/mpl/not.hpp>

namespace hboost { namespace fusion
{
    struct void_;

    // Special tags:
    struct sequence_facade_tag;
    struct hboost_array_tag; // hboost::array tag
    struct mpl_sequence_tag; // mpl sequence tag
    struct std_pair_tag; // std::pair tag

    namespace extension
    {
        template <typename Tag>
        struct has_key_impl
        {
            template <typename Seq, typename Key>
            struct apply
              : mpl::not_<
                    typename result_of::equal_to<
                        typename result_of::find<Seq, Key>::type
                      , typename result_of::end<Seq>::type
                    >::type
                >::type
            {};
        };

        template <>
        struct has_key_impl<sequence_facade_tag>
        {
            template <typename Sequence, typename Key>
            struct apply : Sequence::template has_key<Sequence, Key> {};
        };

        template <>
        struct has_key_impl<hboost_array_tag>;

        template <>
        struct has_key_impl<mpl_sequence_tag>;

        template <>
        struct has_key_impl<std_pair_tag>;
    }
    
    namespace result_of
    {
        template <typename Sequence, typename Key>
        struct has_key 
            : extension::has_key_impl<typename detail::tag_of<Sequence>::type>::
                template apply<Sequence, Key>
        {};
    }

    template <typename Key, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::has_key<Sequence, Key>::type
    has_key(Sequence const&)
    {
        typedef typename result_of::has_key<Sequence, Key>::type result;
        return result();
    }
}}

#endif

