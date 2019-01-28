/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_AT_IMPL_20061029_1946)
#define HBOOST_FUSION_AT_IMPL_20061029_1946

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/apply.hpp>
#include <hboost/fusion/view/transform_view/detail/apply_transform_result.hpp>
#include <hboost/fusion/sequence/intrinsic/at.hpp>

namespace hboost { namespace fusion {
    struct transform_view_tag;
    struct transform_view2_tag;

    namespace extension
    {
        template<typename Tag>
        struct at_impl;

        template<>
        struct at_impl<transform_view_tag>
        {
            template<typename Seq, typename N>
            struct apply
            {
                typedef typename Seq::transform_type F;
                typedef detail::apply_transform_result<F> transform_type;
                typedef typename hboost::fusion::result_of::at<typename Seq::sequence_type, N>::type value_type;
                typedef typename mpl::apply<transform_type, value_type>::type type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type call(Seq& seq)
                {
                    return seq.f(hboost::fusion::at<N>(seq.seq));
                }
            };
        };

        template<>
        struct at_impl<transform_view2_tag>
        {
            template<typename Seq, typename N>
            struct apply
            {
                typedef typename Seq::transform_type F;
                typedef detail::apply_transform_result<F> transform_type;
                typedef typename hboost::fusion::result_of::at<typename Seq::sequence1_type, N>::type value1_type;
                typedef typename hboost::fusion::result_of::at<typename Seq::sequence2_type, N>::type value2_type;
                typedef typename mpl::apply<transform_type, value1_type, value2_type>::type type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type call(Seq& seq)
                {
                    return seq.f(hboost::fusion::at<N>(seq.seq1), hboost::fusion::at<N>(seq.seq2));
                }
            };
        };
    }
}}

#endif