/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PP_IS_ITERATING
#if !defined(HBOOST_FUSION_VECTOR_FORWARD_CTOR_07122005_1123)
#define HBOOST_FUSION_VECTOR_FORWARD_CTOR_07122005_1123

#define FUSION_FORWARD_CTOR_FORWARD(z, n, _)    HBOOST_FUSION_FWD_ELEM(U##n, _##n)

#define HBOOST_PP_FILENAME_1 \
    <hboost/fusion/container/vector/detail/cpp03/vector_forward_ctor.hpp>
#define HBOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_VECTOR_SIZE)
#include HBOOST_PP_ITERATE()

#undef FUSION_FORWARD_CTOR_FORWARD
#endif
#else // defined(HBOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define M HBOOST_PP_ITERATION()

    // XXX:
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if !defined(HBOOST_CLANG)
    HBOOST_CONSTEXPR
FUSION_HASH endif
#else
#if !defined(HBOOST_CLANG)
    HBOOST_CONSTEXPR
#endif
#endif
    HBOOST_FUSION_GPU_ENABLED
#if M == 1
    explicit
#endif
    vector(HBOOST_PP_ENUM_BINARY_PARAMS(
        M, typename detail::call_param<T, >::type arg))
        : vec(HBOOST_PP_ENUM_PARAMS(M, arg)) {}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
#endif
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) || \
    (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
    template <HBOOST_PP_ENUM_PARAMS(M, typename U)>
    // XXX:
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if !defined(HBOOST_CLANG)
    HBOOST_CXX14_CONSTEXPR
FUSION_HASH endif
#else
#if !defined(HBOOST_CLANG)
    HBOOST_CXX14_CONSTEXPR
#endif
#endif
    HBOOST_FUSION_GPU_ENABLED
#if M == 1
    explicit
#endif
    vector(HBOOST_PP_ENUM_BINARY_PARAMS(M, U, && arg))
        : vec(HBOOST_PP_ENUM(M, FUSION_FORWARD_CTOR_FORWARD, arg)) {}
#endif
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH endif
#endif

#undef M
#endif // defined(HBOOST_PP_IS_ITERATING)
