/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SEGMENTED_END_HPP_INCLUDED)
#define HBOOST_FUSION_SEGMENTED_END_HPP_INCLUDED

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/sequence/intrinsic/detail/segmented_end_impl.hpp>
#include <hboost/fusion/iterator/segmented_iterator.hpp>
#include <hboost/fusion/container/list/cons.hpp>

namespace hboost { namespace fusion { namespace detail
{
    //auto segmented_end( seq )
    //{
    //    return make_segmented_iterator( segmented_end_impl( seq ) );
    //}

    template <typename Sequence, typename Nil_ = fusion::nil_>
    struct segmented_end
    {
        typedef
            segmented_iterator<
                typename segmented_end_impl<Sequence, Nil_>::type
            >
        type;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static type call(Sequence & seq)
        {
            return type(
                segmented_end_impl<Sequence, Nil_>::call(seq, Nil_()));
        }
    };

}}}

#endif
