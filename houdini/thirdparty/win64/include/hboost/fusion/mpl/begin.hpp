/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_BEGIN_10022005_1620)
#define HBOOST_FUSION_BEGIN_10022005_1620

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/begin_end.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/adapted/mpl/detail/begin_impl.hpp>
#include <hboost/fusion/iterator/mpl/fusion_iterator.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence>
        struct apply
        {
            typedef fusion_iterator<typename fusion::result_of::begin<Sequence>::type> type;
        };
    };
}}

#endif
