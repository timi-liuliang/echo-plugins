/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_EMPTY_10022005_1619)
#define HBOOST_FUSION_EMPTY_10022005_1619

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/empty.hpp>
#include <hboost/fusion/sequence/intrinsic/empty.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct empty_impl;

    template <>
    struct empty_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence>
        struct apply : fusion::result_of::empty<Sequence> {};
    };
}}

#endif
