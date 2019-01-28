/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_IS_VIEW_IMPL_09272006_0725)
#define HBOOST_FUSION_IS_VIEW_IMPL_09272006_0725

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost { namespace fusion 
{
    struct hboost_tuple_tag;

    namespace extension
    {
        template<typename Tag>
        struct is_view_impl;

        template<>
        struct is_view_impl<hboost_tuple_tag>
        {
            template<typename T>
            struct apply : mpl::false_ {};
        };
    }
}}

#endif
