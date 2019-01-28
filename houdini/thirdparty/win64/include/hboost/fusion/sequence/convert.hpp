/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CONVERT_10022005_1442)
#define HBOOST_FUSION_CONVERT_10022005_1442

#include <hboost/fusion/support/config.hpp>
#if HBOOST_WORKAROUND(HBOOST_GCC, < 30500)
#include <hboost/core/enable_if.hpp>
#include <hboost/type_traits/is_const.hpp>
#define HBOOST_FUSION_WA_GCC34(type1, type2) \
    hboost::lazy_disable_if<hboost::is_const<Sequence>, type1, type2>
#else
#define HBOOST_FUSION_WA_GCC34(type1, type2) type1, type2
#endif

namespace hboost { namespace fusion
{
    namespace extension
    {
        template <typename Tag>
        struct convert_impl;
    }

    namespace result_of
    {
        template <typename Tag, typename Sequence>
        struct convert
        {
            typedef typename
                extension::convert_impl<Tag>::template apply<Sequence>
            gen;

            typedef typename gen::type type;
        };
    }

    template <typename Tag, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename HBOOST_FUSION_WA_GCC34(result_of::convert<Tag, Sequence>)::type
    convert(Sequence& seq)
    {
        typedef typename result_of::convert<Tag, Sequence>::gen gen;
        return gen::call(seq);
    }

    template <typename Tag, typename Sequence>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::convert<Tag, Sequence const>::type
    convert(Sequence const& seq)
    {
        typedef typename result_of::convert<Tag, Sequence const>::gen gen;
        return gen::call(seq);
    }
}}

#undef HBOOST_FUSION_WA_GCC34
#endif
