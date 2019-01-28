/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_TUPLE_FORWARD_10032005_0956)
#define HBOOST_FUSION_TUPLE_FORWARD_10032005_0956

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/cpp03/limits.hpp>
#include <hboost/preprocessor/repetition/enum_params_with_a_default.hpp>

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/tuple/detail/preprocessed/tuple_fwd.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/tuple" FUSION_MAX_VECTOR_SIZE_STR "_fwd.hpp")
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
    struct void_;

    template <
        HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            FUSION_MAX_VECTOR_SIZE, typename T, void_)
    >
    struct tuple;
}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif

