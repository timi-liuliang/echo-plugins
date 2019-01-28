/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SIZE_IMPL_09272006_0724)
#define HBOOST_FUSION_SIZE_IMPL_09272006_0724

#include <hboost/fusion/support/config.hpp>
#include <hboost/tuple/tuple.hpp>
#include <hboost/mpl/int.hpp>

namespace hboost { namespace fusion 
{
    struct hboost_tuple_tag;

    namespace extension
    {
        template<typename T>
        struct size_impl;

        template <>
        struct size_impl<hboost_tuple_tag>
        {
            template <typename Sequence>
            struct apply : mpl::int_<tuples::length<Sequence>::value> {};
        };
    }
}}

#endif
