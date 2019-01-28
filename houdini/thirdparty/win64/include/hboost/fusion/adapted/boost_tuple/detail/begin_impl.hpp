/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_BEGIN_IMPL_09272006_0719)
#define HBOOST_FUSION_BEGIN_IMPL_09272006_0719

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/adapted/hboost_tuple/hboost_tuple_iterator.hpp>

namespace hboost { namespace fusion 
{
    struct hboost_tuple_tag;

    namespace extension
    {
        template<typename T>
        struct begin_impl;

        template <>
        struct begin_impl<hboost_tuple_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef hboost_tuple_iterator<Sequence> type;
    
                HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif
