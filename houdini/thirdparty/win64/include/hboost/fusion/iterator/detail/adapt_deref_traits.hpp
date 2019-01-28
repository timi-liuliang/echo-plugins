/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ADAPT_DEREF_TRAITS_05062005_0900)
#define HBOOST_FUSION_ADAPT_DEREF_TRAITS_05062005_0900

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/deref.hpp>

namespace hboost { namespace fusion { namespace detail
{
    struct adapt_deref_traits
    {
        template <typename Iterator>
        struct apply
        {
            typedef typename
                result_of::deref<typename Iterator::first_type>::type
            type;

            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            static type
            call(Iterator const& i)
            {
                return *i.first;
            }
        };
    };
}}}

#endif

