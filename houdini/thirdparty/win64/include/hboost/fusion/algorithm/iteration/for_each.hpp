/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_FOR_EACH_20070527_0943)
#define HBOOST_FUSION_FOR_EACH_20070527_0943

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/algorithm/iteration/detail/for_each.hpp>
#include <hboost/fusion/algorithm/iteration/detail/segmented_for_each.hpp>
#include <hboost/fusion/support/is_segmented.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/utility/enable_if.hpp>

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename F>
        struct for_each
        {
            typedef void type;
        };
    }

    template <typename Sequence, typename F>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename
        enable_if<
            traits::is_sequence<Sequence>
          , void
        >::type
    for_each(Sequence& seq, F const& f)
    {
        detail::for_each(seq, f, typename traits::is_segmented<Sequence>::type());
    }

    template <typename Sequence, typename F>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename
        enable_if<
            traits::is_sequence<Sequence>
          , void
        >::type
    for_each(Sequence const& seq, F const& f)
    {
        detail::for_each(seq, f, typename traits::is_segmented<Sequence>::type());
    }
}}

#endif