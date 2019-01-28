/*=============================================================================
    Copyright (c) 2014-2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_TUPLE_14122014_0102
#define HBOOST_FUSION_TUPLE_14122014_0102

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/tuple/tuple_fwd.hpp>

///////////////////////////////////////////////////////////////////////////////
// With no variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_TUPLE)
# include <hboost/fusion/tuple/detail/tuple.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
#include <hboost/fusion/container/vector/vector.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>
#include <hboost/fusion/sequence/intrinsic/value_at.hpp>
#include <hboost/fusion/sequence/intrinsic/at.hpp>
#include <hboost/fusion/sequence/comparison.hpp>
#include <hboost/fusion/sequence/io.hpp>
#include <utility>

namespace hboost { namespace fusion
{
    template <typename ...T>
    struct tuple : vector<T...>
    {
        typedef vector<T...> base_type;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        tuple()
            : base_type() {}

        template <typename ...U>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        tuple(tuple<U...> const& other)
            : base_type(other) {}

        template <typename ...U>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        tuple(tuple<U...>&& other)
            : base_type(std::move(other)) {}

        template <typename ...U>
        /*HBOOST_CONSTEXPR*/ HBOOST_FUSION_GPU_ENABLED
        explicit
        tuple(U&&... args)
            : base_type(std::forward<U>(args)...) {}

        template <typename U>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        tuple& operator=(U&& rhs)
        {
            base_type::operator=(std::forward<U>(rhs));
            return *this;
        }
    };

    template <typename Tuple>
    struct tuple_size : result_of::size<Tuple> {};

    template <int N, typename Tuple>
    struct tuple_element : result_of::value_at_c<Tuple, N> {};

    template <int N, typename Tuple>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::at_c<Tuple, N>::type
    get(Tuple& tup)
    {
        return at_c<N>(tup);
    }

    template <int N, typename Tuple>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::at_c<Tuple const, N>::type
    get(Tuple const& tup)
    {
        return at_c<N>(tup);
    }
}}

#endif
#endif

