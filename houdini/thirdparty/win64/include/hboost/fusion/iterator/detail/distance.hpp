/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_DISTANCE_09172005_0730)
#define HBOOST_FUSION_DISTANCE_09172005_0730

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/next.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/fusion/iterator/next.hpp>
#include <hboost/fusion/iterator/equal_to.hpp>

namespace hboost { namespace fusion { namespace distance_detail
{
    // Default distance implementation, linear
    // search for the Last iterator.

    template <typename First, typename Last>
    struct linear_distance;

    template <typename First, typename Last>
    struct next_distance
    {
        typedef typename 
            mpl::next<
                typename linear_distance<
                    typename result_of::next<First>::type
                  , Last
                >::type
            >::type 
        type;
    };

    template <typename First, typename Last>
    struct linear_distance
        : mpl::eval_if<
            result_of::equal_to<First, Last>
          , mpl::identity<mpl::int_<0> >
          , next_distance<First, Last>
        >::type
    {
        typedef typename
            mpl::eval_if<
                result_of::equal_to<First, Last>
              , mpl::identity<mpl::int_<0> >
              , next_distance<First, Last>
            >::type
        type;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static type
        call(First const&, Last const&)
        {
            return type();
        }
    };

}}}

#endif
