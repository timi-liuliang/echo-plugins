/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_INSERT_RANGE_009172005_1147)
#define HBOOST_FUSION_INSERT_RANGE_009172005_1147

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/mpl/convert_iterator.hpp>
#include <hboost/fusion/view/joint_view/joint_view.hpp>
#include <hboost/fusion/view/iterator_range/iterator_range.hpp>
#include <hboost/fusion/support/detail/as_fusion_element.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/adapted/mpl/mpl_iterator.hpp>

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename Position, typename Range>
        struct insert_range
        {
            typedef typename convert_iterator<Position>::type pos_type;
            typedef typename result_of::begin<Sequence>::type first_type;
            typedef typename result_of::end<Sequence>::type last_type;

            typedef iterator_range<first_type, pos_type> left_type;
            typedef iterator_range<pos_type, last_type> right_type;
            typedef joint_view<left_type, Range> left_insert_type;
            typedef joint_view<left_insert_type, right_type> type;
        };
    }

    template <typename Sequence, typename Position, typename Range>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::insert_range<Sequence const, Position, Range const>::type
    insert_range(Sequence const& seq, Position const& pos, Range const& range)
    {
        typedef result_of::insert_range<Sequence const, Position, Range const> result_of;
        typedef typename result_of::left_type left_type;
        typedef typename result_of::right_type right_type;
        typedef typename result_of::left_insert_type left_insert_type;
        typedef typename result_of::type result;

        left_type left(fusion::begin(seq), convert_iterator<Position>::call(pos));
        right_type right(convert_iterator<Position>::call(pos), fusion::end(seq));
        left_insert_type left_insert(left, range);
        return result(left_insert, right);
    }
}}

#endif
