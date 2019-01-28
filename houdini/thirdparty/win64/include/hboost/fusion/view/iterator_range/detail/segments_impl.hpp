/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ITERATOR_RANGE_SEGMENTS_HPP_INCLUDED)
#define HBOOST_FUSION_ITERATOR_RANGE_SEGMENTS_HPP_INCLUDED

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/fusion/sequence/intrinsic/segments.hpp>
#include <hboost/fusion/support/is_segmented.hpp>
#include <hboost/fusion/view/iterator_range/detail/segmented_iterator_range.hpp>

namespace hboost { namespace fusion
{
    struct iterator_range_tag;

    namespace extension
    {
        template <typename Tag>
        struct segments_impl;

        template <>
        struct segments_impl<iterator_range_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef
                    detail::make_segmented_range<
                        typename Sequence::begin_type
                      , typename Sequence::end_type
                    >
                impl;

                HBOOST_MPL_ASSERT((traits::is_segmented<typename impl::type>));

                typedef
                    typename result_of::segments<typename impl::type>::type
                type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type call(Sequence & seq)
                {
                    return fusion::segments(impl::call(seq.first, seq.last));
                }
            };
        };
    }
}}

#endif
