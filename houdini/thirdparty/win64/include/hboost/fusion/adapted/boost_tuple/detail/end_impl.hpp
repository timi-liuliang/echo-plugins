/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_END_IMPL_09272006_0721)
#define HBOOST_FUSION_END_IMPL_09272006_0721

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/adapted/hboost_tuple/hboost_tuple_iterator.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/is_const.hpp>

namespace hboost { namespace tuples
{
    struct null_type;
}}
    
namespace hboost { namespace fusion
{
    struct hboost_tuple_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<hboost_tuple_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef 
                    hboost_tuple_iterator<
                        typename mpl::if_<
                            is_const<Sequence>
                          , tuples::null_type const
                          , tuples::null_type
                        >::type
                    > 
                type;

                HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence& seq)
                {
                    return type(seq);
                }
            };
        };
    }
}}

#endif
