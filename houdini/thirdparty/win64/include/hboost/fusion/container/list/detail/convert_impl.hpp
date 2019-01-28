/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CONVERT_IMPL_09232005_1215)
#define HBOOST_FUSION_CONVERT_IMPL_09232005_1215

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/list/cons.hpp>
#include <hboost/fusion/container/list/detail/build_cons.hpp>
#include <hboost/fusion/sequence/intrinsic/empty.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>

namespace hboost { namespace fusion
{
    struct cons_tag;

    namespace extension
    {
        template <typename T>
        struct convert_impl;

        template <>
        struct convert_impl<cons_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef typename
                    detail::build_cons<
                        typename result_of::begin<Sequence>::type
                      , typename result_of::end<Sequence>::type
                    >
                build_cons;

                typedef typename build_cons::type type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence& seq)
                {
                    return build_cons::call(fusion::begin(seq), fusion::end(seq));
                }
            };
        };
    }
}}

#endif
