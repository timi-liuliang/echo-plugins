/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CONVERT_09232005_1215)
#define HBOOST_FUSION_CONVERT_09232005_1215

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/list/cons.hpp>
#include <hboost/fusion/container/list/detail/build_cons.hpp>
#include <hboost/fusion/container/list/detail/convert_impl.hpp>
#include <hboost/fusion/sequence/intrinsic/empty.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence>
        struct as_list
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
    }

    template <typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::as_list<Sequence>::type
    as_list(Sequence& seq)
    {
        return result_of::as_list<Sequence>::call(seq);
    }

    template <typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::as_list<Sequence const>::type
    as_list(Sequence const& seq)
    {
        return result_of::as_list<Sequence const>::call(seq);
    }
}}

#endif
