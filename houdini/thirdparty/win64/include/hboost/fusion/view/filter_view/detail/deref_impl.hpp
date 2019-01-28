/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_DEREF_IMPL_05062005_0905)
#define HBOOST_FUSION_DEREF_IMPL_05062005_0905

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/detail/adapt_deref_traits.hpp>

namespace hboost { namespace fusion
{
    struct filter_view_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<filter_view_iterator_tag>
            : detail::adapt_deref_traits {};
    }
}}

#endif


