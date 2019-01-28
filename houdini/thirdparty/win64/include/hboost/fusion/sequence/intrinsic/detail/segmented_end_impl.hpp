/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SEGMENTED_END_IMPL_HPP_INCLUDED)
#define HBOOST_FUSION_SEGMENTED_END_IMPL_HPP_INCLUDED

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/fusion/sequence/intrinsic_fwd.hpp>
#include <hboost/fusion/container/list/cons_fwd.hpp>
#include <hboost/fusion/support/is_segmented.hpp>

namespace hboost { namespace fusion
{
    template <typename First, typename Last>
    struct iterator_range;
}}

namespace hboost { namespace fusion { namespace detail
{
    //auto segmented_end_impl( seq, stack )
    //{
    //    assert(is_segmented(seq));
    //    auto it = end(segments(seq));
    //    return cons(iterator_range(it, it), stack);
    //}

    template <typename Sequence, typename Stack>
    struct segmented_end_impl
    {
        HBOOST_MPL_ASSERT((traits::is_segmented<Sequence>));

        typedef
            typename result_of::end<
                typename remove_reference<
                    typename add_const<
                        typename result_of::segments<Sequence>::type
                    >::type
                >::type
            >::type
        end_type;

        typedef iterator_range<end_type, end_type>  pair_type;
        typedef cons<pair_type, Stack>              type;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static pair_type make_pair(end_type end)
        {
            return pair_type(end, end);
        }

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static type call(Sequence & seq, Stack stack)
        {
            return type(
                make_pair(fusion::end(fusion::segments(seq))),
                stack);
        }
    };

}}}

#endif
