/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PP_IS_ITERATING
#if !defined(HBOOST_FUSION_VECTOR_N_CHOOSER_07072005_1248)
#define HBOOST_FUSION_VECTOR_N_CHOOSER_07072005_1248

#include <hboost/fusion/container/vector/detail/cpp03/limits.hpp>

//  include vector0..N where N is FUSION_MAX_VECTOR_SIZE
#include <hboost/fusion/container/vector/detail/cpp03/vector10.hpp>
#if (FUSION_MAX_VECTOR_SIZE > 10)
#include <hboost/fusion/container/vector/detail/cpp03/vector20.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 20)
#include <hboost/fusion/container/vector/detail/cpp03/vector30.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 30)
#include <hboost/fusion/container/vector/detail/cpp03/vector40.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 40)
#include <hboost/fusion/container/vector/detail/cpp03/vector50.hpp>
#endif

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/arithmetic/sub.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>

namespace hboost { namespace fusion
{
    struct void_;
}}

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/container/vector/detail/cpp03/preprocessed/vector_chooser.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vector_chooser" FUSION_MAX_VECTOR_SIZE_STR ".hpp")
#endif

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace hboost { namespace fusion { namespace detail
{
    template <HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename T)>
    struct vector_n_chooser
    {
        typedef HBOOST_PP_CAT(vector, FUSION_MAX_VECTOR_SIZE)<HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)> type;
    };

    template <>
    struct vector_n_chooser<HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, void_ HBOOST_PP_INTERCEPT)>
    {
        typedef vector0<> type;
    };

#define HBOOST_PP_FILENAME_1 \
    <hboost/fusion/container/vector/detail/cpp03/vector_n_chooser.hpp>
#define HBOOST_PP_ITERATION_LIMITS (1, HBOOST_PP_DEC(FUSION_MAX_VECTOR_SIZE))
#include HBOOST_PP_ITERATE()

}}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(HBOOST_PP_IS_ITERATING)

#define N HBOOST_PP_ITERATION()

    template <HBOOST_PP_ENUM_PARAMS(N, typename T)>
    struct vector_n_chooser<
        HBOOST_PP_ENUM_PARAMS(N, T)
        HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE, N), void_ HBOOST_PP_INTERCEPT)>
    {
        typedef HBOOST_PP_CAT(vector, N)<HBOOST_PP_ENUM_PARAMS(N, T)> type;
    };

#undef N
#endif // defined(HBOOST_PP_IS_ITERATING)
