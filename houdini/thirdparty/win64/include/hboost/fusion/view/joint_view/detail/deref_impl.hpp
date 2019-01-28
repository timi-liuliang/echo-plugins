/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_DEREF_IMPL_07162005_0137)
#define HBOOST_FUSION_DEREF_IMPL_07162005_0137

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/detail/adapt_deref_traits.hpp>

namespace hboost { namespace fusion
{
    struct joint_view_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<joint_view_iterator_tag>
            : detail::adapt_deref_traits {};
    }
}}

#endif


