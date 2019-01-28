/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_LESS_EQUAL_05052005_0432)
#define HBOOST_FUSION_LESS_EQUAL_05052005_0432

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>
#include <hboost/fusion/sequence/comparison/enable_comparison.hpp>

#if defined(FUSION_DIRECT_OPERATOR_USAGE)
#include <hboost/fusion/sequence/comparison/detail/less_equal.hpp>
#else
#include <hboost/fusion/sequence/comparison/less.hpp>
#endif

namespace hboost { namespace fusion
{
    template <typename Seq1, typename Seq2>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline bool
    less_equal(Seq1 const& a, Seq2 const& b)
    {
#if defined(FUSION_DIRECT_OPERATOR_USAGE)
        return detail::sequence_less_equal<Seq1 const, Seq2 const>::
            call(fusion::begin(a), fusion::begin(b));
#else
        return !(b < a);
#endif
    }

    namespace operators
    {
        template <typename Seq1, typename Seq2>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        inline typename
            hboost::enable_if<
                traits::enable_comparison<Seq1, Seq2>
              , bool
            >::type
        operator<=(Seq1 const& a, Seq2 const& b)
        {
            return fusion::less_equal(a, b);
        }
    }
    using operators::operator<=;
}}

#endif
