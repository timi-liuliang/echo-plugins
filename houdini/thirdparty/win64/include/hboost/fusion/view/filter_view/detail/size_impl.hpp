/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SIZE_IMPL_09232005_1058)
#define HBOOST_FUSION_SIZE_IMPL_09232005_1058

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/distance.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>

namespace hboost { namespace fusion
{
    struct filter_view_tag;

    namespace extension
    {
        template <typename Tag>
        struct size_impl;

        template <>
        struct size_impl<filter_view_tag>
        {
            template <typename Sequence>
            struct apply
                : result_of::distance<
                    typename result_of::begin<Sequence>::type
                  , typename result_of::end<Sequence>::type>
            {};
        };
    }
}}

#endif

