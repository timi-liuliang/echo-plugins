/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_LESS_05052005_1141)
#define HBOOST_FUSION_LESS_05052005_1141

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/iterator/next.hpp>
#include <hboost/fusion/iterator/equal_to.hpp>
#include <hboost/fusion/support/as_const.hpp>

namespace hboost { namespace fusion { namespace detail
{
    template <typename Seq1, typename Seq2>
    struct sequence_less
    {
        typedef typename result_of::end<Seq1>::type end1_type;
        typedef typename result_of::end<Seq2>::type end2_type;

        template <typename I1, typename I2>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static bool
        call(I1 const&, I2 const&, mpl::true_)
        {
            return false;
        }

        template <typename I1, typename I2>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static bool
        call(I1 const& a, I2 const& b, mpl::false_)
        {
            return extension::as_const(*a) < extension::as_const(*b) ||
                (!(extension::as_const(*b) < extension::as_const(*a)) && 
                 call(fusion::next(a), fusion::next(b)));
        }

        template <typename I1, typename I2>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static bool
        call(I1 const& a, I2 const& b)
        {
            typename result_of::equal_to<I1, end1_type>::type eq;
            return call(a, b, eq);
        }
    };
}}}

#endif