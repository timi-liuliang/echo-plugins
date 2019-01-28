/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CONVERT_09222005_1104)
#define HBOOST_FUSION_CONVERT_09222005_1104

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/as_vector.hpp>
#include <hboost/fusion/container/vector/detail/convert_impl.hpp>
#include <hboost/fusion/container/vector/vector.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence>
        struct as_vector
        {
            typedef typename detail::as_vector<result_of::size<Sequence>::value> gen;
            typedef typename gen::
                template apply<typename result_of::begin<Sequence>::type>::type
            type;
        };
    }

    template <typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::as_vector<Sequence>::type
    as_vector(Sequence& seq)
    {
        typedef typename result_of::as_vector<Sequence>::gen gen;
        return gen::call(fusion::begin(seq));
    }

    template <typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::as_vector<Sequence const>::type
    as_vector(Sequence const& seq)
    {
        typedef typename result_of::as_vector<Sequence const>::gen gen;
        return gen::call(fusion::begin(seq));
    }
}}

#endif
