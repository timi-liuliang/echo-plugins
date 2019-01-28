/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VALUE_AT_IMPL_09262006_1926)
#define HBOOST_FUSION_VALUE_AT_IMPL_09262006_1926

#include <hboost/fusion/support/config.hpp>
#include <hboost/tuple/tuple.hpp>

namespace hboost { namespace fusion
{
    struct hboost_tuple_tag;

    namespace extension
    {
        template<typename T>
        struct value_at_impl;

        template <>
        struct value_at_impl<hboost_tuple_tag>
        {
            template <typename Sequence, typename N>
            struct apply : tuples::element<N::value, Sequence> {};
        };
    }
}}

#endif
