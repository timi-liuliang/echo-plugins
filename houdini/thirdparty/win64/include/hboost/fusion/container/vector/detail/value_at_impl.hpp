/*=============================================================================
    Copyright (c) 2014 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_VALUE_AT_IMPL_16122014_1641
#define HBOOST_FUSION_VALUE_AT_IMPL_16122014_1641

#include <hboost/config.hpp>
#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/config.hpp>

///////////////////////////////////////////////////////////////////////////////
// Without variadics, we will use the PP version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
# include <hboost/fusion/container/vector/detail/cpp03/value_at_impl.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
#include <hboost/fusion/container/vector/vector_fwd.hpp>
#include <hboost/type_traits/declval.hpp>
#include <hboost/type_traits/remove_cv.hpp>

namespace hboost { namespace fusion
{
    struct vector_tag;

    namespace vector_detail
    {
        template <typename I, typename ...T>
        struct vector_data;
    }

    namespace extension
    {
        template <typename Tag>
        struct value_at_impl;

        template <>
        struct value_at_impl<vector_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                typedef typename hboost::remove_cv<Sequence>::type seq;
                typedef typename mpl::identity<decltype(seq::template value_at_impl<N::value>(hboost::declval<seq*>()))>::type::type type;
            };
        };
    }
}}

#endif
#endif

