/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_LIST_FORWARD_07172005_0224)
#define HBOOST_FUSION_LIST_FORWARD_07172005_0224

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/list/detail/cpp03/limits.hpp>
#include <hboost/preprocessor/repetition/enum_params_with_a_default.hpp>

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/container/list/detail/cpp03/preprocessed/list_fwd.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/list" FUSION_MAX_LIST_SIZE_STR "_fwd.hpp")
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
            FUSION_MAX_LIST_SIZE, typename T, void_)
    >
    struct list;
}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
