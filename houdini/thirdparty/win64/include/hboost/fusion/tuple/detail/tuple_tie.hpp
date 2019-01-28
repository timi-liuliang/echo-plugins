/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PP_IS_ITERATING
#if !defined(HBOOST_FUSION_TUPLE_TIE_10032005_0846)
#define HBOOST_FUSION_TUPLE_TIE_10032005_0846

#include <hboost/preprocessor/iterate.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/fusion/tuple/detail/tuple.hpp>

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/tuple/detail/preprocessed/tuple_tie.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/tuple_tie" FUSION_MAX_VECTOR_SIZE_STR ".hpp")
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

namespace hboost { namespace fusion
{
#define HBOOST_FUSION_REF(z, n, data) HBOOST_PP_CAT(T, n)&

#define HBOOST_PP_FILENAME_1 <hboost/fusion/tuple/detail/tuple_tie.hpp>
#define HBOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_VECTOR_SIZE)
#include HBOOST_PP_ITERATE()

#undef HBOOST_FUSION_REF

}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
#else // defined(HBOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define N HBOOST_PP_ITERATION()

    template <HBOOST_PP_ENUM_PARAMS(N, typename T)>
    HBOOST_FUSION_GPU_ENABLED
    inline tuple<HBOOST_PP_ENUM(N, HBOOST_FUSION_REF, _)>
    tie(HBOOST_PP_ENUM_BINARY_PARAMS(N, T, & arg))
    {
        return tuple<HBOOST_PP_ENUM(N, HBOOST_FUSION_REF, _)>(
            HBOOST_PP_ENUM_PARAMS(N, arg));
    }

#undef N
#endif // defined(HBOOST_PP_IS_ITERATING)

