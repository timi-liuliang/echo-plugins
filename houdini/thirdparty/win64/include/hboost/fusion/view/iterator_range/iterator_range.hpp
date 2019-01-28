/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ITERATOR_RANGE_05062005_1224)
#define HBOOST_FUSION_ITERATOR_RANGE_05062005_1224

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/fusion/support/category_of.hpp>
#include <hboost/fusion/iterator/distance.hpp>
#include <hboost/fusion/iterator/mpl/convert_iterator.hpp>
#include <hboost/fusion/view/iterator_range/detail/begin_impl.hpp>
#include <hboost/fusion/view/iterator_range/detail/end_impl.hpp>
#include <hboost/fusion/view/iterator_range/detail/at_impl.hpp>
#include <hboost/fusion/view/iterator_range/detail/size_impl.hpp>
#include <hboost/fusion/view/iterator_range/detail/value_at_impl.hpp>
#include <hboost/fusion/view/iterator_range/detail/is_segmented_impl.hpp>
#include <hboost/fusion/view/iterator_range/detail/segments_impl.hpp>
#include <hboost/fusion/adapted/mpl/mpl_iterator.hpp>
#include <hboost/config.hpp>

#if defined (HBOOST_MSVC)
#  pragma warning(push)
#  pragma warning (disable: 4512) // assignment operator could not be generated.
#endif

namespace hboost { namespace fusion
{
    struct iterator_range_tag;
    struct fusion_sequence_tag;

    template <typename First, typename Last>
    struct iterator_range : sequence_base<iterator_range<First, Last> >
    {
        typedef typename convert_iterator<First>::type begin_type;
        typedef typename convert_iterator<Last>::type end_type;
        typedef iterator_range_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;

        typedef typename traits::category_of<begin_type>::type category;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        iterator_range(First const& in_first, Last const& in_last)
            : first(convert_iterator<First>::call(in_first))
            , last(convert_iterator<Last>::call(in_last)) {}

        begin_type first;
        end_type last;
    };
}}

#if defined (HBOOST_MSVC)
#  pragma warning(pop)
#endif

#endif

