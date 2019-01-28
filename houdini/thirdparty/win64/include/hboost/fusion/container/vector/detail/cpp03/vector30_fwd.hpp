#ifndef HBOOST_PP_IS_ITERATING
/*=============================================================================
    Copyright (c) 2011 Eric Niebler
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VECTOR30_FWD_HPP_INCLUDED)
#define HBOOST_FUSION_VECTOR30_FWD_HPP_INCLUDED

#include <hboost/fusion/support/config.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/container/vector/detail/cpp03/preprocessed/vector30_fwd.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vector30_fwd.hpp")
#endif

/*=============================================================================
    Copyright (c) 2011 Eric Niebler
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
    // expand vector21 to vector30
    #define HBOOST_PP_FILENAME_1 <hboost/fusion/container/vector/detail/cpp03/vector30_fwd.hpp>
    #define HBOOST_PP_ITERATION_LIMITS (21, 30)
    #include HBOOST_PP_ITERATE()
}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif

#else

    template <HBOOST_PP_ENUM_PARAMS(HBOOST_PP_ITERATION(), typename T)>
    struct HBOOST_PP_CAT(vector, HBOOST_PP_ITERATION());

#endif
