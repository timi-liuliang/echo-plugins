/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_FRONT_10022005_1618)
#define HBOOST_FUSION_FRONT_10022005_1618

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/front.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/iterator/value_of.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct front_impl;

    template <>
    struct front_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence>
        struct apply : 
            fusion::result_of::value_of<typename fusion::result_of::begin<Sequence>::type> {};
    };
}}

#endif
