/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_IS_VIEW_IMPL_03202006_0048)
#define HBOOST_FUSION_IS_VIEW_IMPL_03202006_0048

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost { namespace fusion 
{
    struct mpl_sequence_tag;

    namespace extension
    {
        template<typename Tag>
        struct is_view_impl;

        template<>
        struct is_view_impl<mpl_sequence_tag>
        {
            template<typename T>
            struct apply : mpl::true_
            {};
        };
    }
}}

#endif
