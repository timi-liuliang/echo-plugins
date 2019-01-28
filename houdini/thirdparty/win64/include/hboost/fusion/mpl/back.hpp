/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_BACK_10022005_1620)
#define HBOOST_FUSION_BACK_10022005_1620

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/back.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/iterator/prior.hpp>
#include <hboost/fusion/iterator/value_of.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct back_impl;

    template <>
    struct back_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence>
        struct apply : 
            fusion::result_of::value_of<
                typename fusion::result_of::prior<
                    typename fusion::result_of::end<Sequence>::type
                >::type> {};
    };
}}

#endif
