/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CONVERT_IMPL_09222005_1104)
#define HBOOST_FUSION_CONVERT_IMPL_09222005_1104

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/as_vector.hpp>
#include <hboost/fusion/container/vector/vector.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>

namespace hboost { namespace fusion
{
    struct vector_tag;

    namespace extension
    {
        template <typename T>
        struct convert_impl;

        template <>
        struct convert_impl<vector_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef typename detail::as_vector<result_of::size<Sequence>::value> gen;
                typedef typename gen::
                    template apply<typename result_of::begin<Sequence>::type>::type
                type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type call(Sequence& seq)
                {
                    return gen::call(fusion::begin(seq));
                }
            };
        };
    }
}}

#endif
