/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PP_IS_ITERATING
#if !defined(HBOOST_FUSION_AS_VECTOR_09222005_0950)
#define HBOOST_FUSION_AS_VECTOR_09222005_0950

#include <hboost/preprocessor/iterate.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/inc.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/fusion/container/vector/vector.hpp>
#include <hboost/fusion/iterator/value_of.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/iterator/next.hpp>

namespace hboost { namespace fusion { namespace detail
{
HBOOST_FUSION_BARRIER_BEGIN

    template <int size>
    struct as_vector;

    template <>
    struct as_vector<0>
    {
        template <typename Iterator>
        struct apply
        {
            typedef vector0<> type;
        };

        template <typename Iterator>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static typename apply<Iterator>::type
        call(Iterator)
        {
            return vector0<>();
        }
    };

HBOOST_FUSION_BARRIER_END
}}}

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/container/vector/detail/cpp03/preprocessed/as_vector.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/as_vector" FUSION_MAX_VECTOR_SIZE_STR ".hpp")
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
HBOOST_FUSION_BARRIER_BEGIN

#define HBOOST_FUSION_NEXT_ITERATOR(z, n, data)                                  \
    typedef typename fusion::result_of::next<HBOOST_PP_CAT(I, n)>::type          \
        HBOOST_PP_CAT(I, HBOOST_PP_INC(n));

#define HBOOST_FUSION_NEXT_CALL_ITERATOR(z, n, data)                             \
    typename gen::HBOOST_PP_CAT(I, HBOOST_PP_INC(n))                              \
        HBOOST_PP_CAT(i, HBOOST_PP_INC(n)) = fusion::next(HBOOST_PP_CAT(i, n));

#define HBOOST_FUSION_VALUE_OF_ITERATOR(z, n, data)                              \
    typedef typename fusion::result_of::value_of<HBOOST_PP_CAT(I, n)>::type      \
        HBOOST_PP_CAT(T, n);

#define HBOOST_PP_FILENAME_1 <hboost/fusion/container/vector/detail/cpp03/as_vector.hpp>
#define HBOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_VECTOR_SIZE)
#include HBOOST_PP_ITERATE()

#undef HBOOST_FUSION_NEXT_ITERATOR
#undef HBOOST_FUSION_NEXT_CALL_ITERATOR
#undef HBOOST_FUSION_VALUE_OF_ITERATOR

HBOOST_FUSION_BARRIER_END
}}}

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

    template <>
    struct as_vector<N>
    {
        template <typename I0>
        struct apply
        {
            HBOOST_PP_REPEAT(N, HBOOST_FUSION_NEXT_ITERATOR, _)
            HBOOST_PP_REPEAT(N, HBOOST_FUSION_VALUE_OF_ITERATOR, _)
            typedef HBOOST_PP_CAT(vector, N)<HBOOST_PP_ENUM_PARAMS(N, T)> type;
        };

        template <typename Iterator>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static typename apply<Iterator>::type
        call(Iterator const& i0)
        {
            typedef apply<Iterator> gen;
            typedef typename gen::type result;
            HBOOST_PP_REPEAT(HBOOST_PP_DEC(N), HBOOST_FUSION_NEXT_CALL_ITERATOR, _)
            return result(HBOOST_PP_ENUM_PARAMS(N, *i));
        }
    };

#undef N
#endif // defined(HBOOST_PP_IS_ITERATING)

