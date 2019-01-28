#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4512) // assignment operator could not be generated
#pragma warning(disable: 4510) // default constructor could not be generated
#pragma warning(disable: 4610) // can never be instantiated - user defined constructor required
#endif
#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)

#ifndef HBOOST_PHOENIX_SUPPORT_VECTOR_HPP
#define HBOOST_PHOENIX_SUPPORT_VECTOR_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/phoenix/support/iterate.hpp>
#include <hboost/preprocessor/repetition/enum_shifted_params.hpp>
#include <hboost/fusion/adapted/struct/adapt_struct.hpp>

#define HBOOST_FUSION_ADAPT_TPL_STRUCT_NO_PARTIAL HBOOST_FUSION_ADAPT_TPL_STRUCT

#include <hboost/phoenix/support/preprocessed/vector.hpp>

#endif

#else

#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_SUPPORT_VECTOR_HPP
#define HBOOST_PHOENIX_SUPPORT_VECTOR_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/phoenix/support/iterate.hpp>
#include <hboost/preprocessor/repetition/enum_shifted_params.hpp>
#include <hboost/fusion/adapted/struct/adapt_struct.hpp>

#define HBOOST_FUSION_ADAPT_TPL_STRUCT_NO_PARTIAL HBOOST_FUSION_ADAPT_TPL_STRUCT

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vector_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
#endif
/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

#define M0(Z, N, D)                                                             \
    typedef HBOOST_PP_CAT(A, N) HBOOST_PP_CAT(member_type, N);                    \
    HBOOST_PP_CAT(A, N) HBOOST_PP_CAT(a, N);                                      \
/**/
#define M1(Z, N, D)                                                             \
    (HBOOST_PP_CAT(A, N))                                                        \
/**/
#define M2(Z, N, D)                                                             \
    (HBOOST_PP_CAT(T, N))                                                        \
/**/
#define M3(Z, N, D)                                                             \
    (HBOOST_PP_CAT(A, N), HBOOST_PP_CAT(a, N))                                    \
/**/

namespace hboost { namespace hboostphoenix
{
    template <typename Dummy = void>
    struct vector0
    {
        typedef mpl::int_<0> size_type;
        static const int size_value = 0;
    };
}}


#define HBOOST_PHOENIX_ITERATION_PARAMS                                          \
        (3, (1, HBOOST_PP_INC(HBOOST_PHOENIX_LIMIT),                              \
        <hboost/phoenix/support/vector.hpp>))
#include HBOOST_PHOENIX_ITERATE()

#undef M0
#undef M1
#undef M2
#undef M3

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif

#else

namespace hboost { namespace hboostphoenix
{
    template <HBOOST_PHOENIX_typename_A>
    struct HBOOST_PP_CAT(vector, HBOOST_PHOENIX_ITERATION)
    {
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M0, _)
        
        typedef mpl::int_<HBOOST_PHOENIX_ITERATION> size_type;
        static const int size_value = HBOOST_PHOENIX_ITERATION;

        typedef
            HBOOST_PP_CAT(vector, HBOOST_PP_DEC(HBOOST_PHOENIX_ITERATION))<HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PHOENIX_ITERATION, A)>
            args_type;

        args_type args() const
        {
            args_type r = {HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PHOENIX_ITERATION, a)};
            return r;
        }
    };
}}

#define HBOOST_PHOENIX_SUPPORT_VECTOR_ADAT_PARAM0                                \
    HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M1, _)                             \
/**/
#define HBOOST_PHOENIX_SUPPORT_VECTOR_ADAT_PARAM1                                \
   (                                                                            \
        HBOOST_PP_CAT(                                                           \
            hboost::hboostphoenix::vector                                              \
          , HBOOST_PHOENIX_ITERATION                                             \
        )                                                                       \
    )                                                                           \
    HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M1, _)                             \
/**/
#define HBOOST_PHOENIX_SUPPORT_VECTOR_ADAT_PARAM2                                \
    HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M3, _)                             \
/**/

HBOOST_FUSION_ADAPT_TPL_STRUCT_NO_PARTIAL(
    HBOOST_PHOENIX_SUPPORT_VECTOR_ADAT_PARAM0
  , HBOOST_PHOENIX_SUPPORT_VECTOR_ADAT_PARAM1
  , HBOOST_PHOENIX_SUPPORT_VECTOR_ADAT_PARAM2
)

#endif

#endif

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif
