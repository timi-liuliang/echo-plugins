/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ENABLE_COMPARISON_09232005_1958)
#define HBOOST_FUSION_ENABLE_COMPARISON_09232005_1958

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/equal_to.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>

namespace hboost { namespace fusion { namespace traits
{
    template <typename Seq1, typename Seq2, typename Enable = void>
    struct enable_equality
        : mpl::or_<traits::is_sequence<Seq1>, traits::is_sequence<Seq2> >
    {};

    template <typename Seq1, typename Seq2, typename Enable = void>
    struct enable_comparison
        : mpl::and_<
            mpl::or_<traits::is_sequence<Seq1>, traits::is_sequence<Seq2> >
          , mpl::equal_to<result_of::size<Seq1>, result_of::size<Seq2> >
        >
    {};
}}}

#endif
