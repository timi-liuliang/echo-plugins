/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PP_IS_ITERATING
#if !defined(HBOOST_FUSION_TUPLE_EXPAND_10032005_0815)
#define HBOOST_FUSION_TUPLE_EXPAND_10032005_0815

#include <hboost/preprocessor/iterate.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>

#define HBOOST_PP_FILENAME_1 \
    <hboost/fusion/tuple/detail/tuple_expand.hpp>
#define HBOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_VECTOR_SIZE)
#include HBOOST_PP_ITERATE()

#endif
#else // defined(HBOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define N HBOOST_PP_ITERATION()

    HBOOST_FUSION_GPU_ENABLED
#if N == 1
    explicit
#endif
    tuple(HBOOST_PP_ENUM_BINARY_PARAMS(
        N, typename detail::call_param<T, >::type arg))
        : base_type(HBOOST_PP_ENUM_PARAMS(N, arg)) {}

    template <HBOOST_PP_ENUM_PARAMS(N, typename U)>
    HBOOST_FUSION_GPU_ENABLED
    tuple(tuple<HBOOST_PP_ENUM_PARAMS(N, U)> const& rhs)
        : base_type(rhs) {}

    template <HBOOST_PP_ENUM_PARAMS(N, typename U)>
    HBOOST_FUSION_GPU_ENABLED
    tuple& operator=(tuple<HBOOST_PP_ENUM_PARAMS(N, U)> const& rhs)
    {
        base_type::operator=(rhs);
        return *this;
    }

#undef N
#endif // defined(HBOOST_PP_IS_ITERATING)

