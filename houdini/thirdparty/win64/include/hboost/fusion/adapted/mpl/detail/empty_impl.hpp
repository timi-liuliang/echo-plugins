/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_EMPTY_IMPL_31122005_1554)
#define HBOOST_FUSION_EMPTY_IMPL_31122005_1554

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/empty.hpp>

namespace hboost { namespace fusion
{
    struct mpl_sequence_tag;

    namespace extension
    {
        template <typename Sequence>
        struct empty_impl;

        template <>
        struct empty_impl<mpl_sequence_tag>
        {
            template <typename Sequence>
            struct apply : mpl::empty<Sequence> {};
        };
    }
}}

#endif
