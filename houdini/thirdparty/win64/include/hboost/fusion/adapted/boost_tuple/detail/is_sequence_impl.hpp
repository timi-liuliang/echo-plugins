/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_IS_SEQUENCE_IMPL_09272006_0726)
#define HBOOST_FUSION_IS_SEQUENCE_IMPL_09272006_0726

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost { namespace fusion 
{
    struct hboost_tuple_tag;

    namespace extension
    {
        template<typename Tag>
        struct is_sequence_impl;

        template<>
        struct is_sequence_impl<hboost_tuple_tag>
        {
            template<typename Sequence>
            struct apply : mpl::true_ {};
        };
    }
}}

#endif
