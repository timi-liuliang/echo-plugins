/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VECTOR_LIMITS_07072005_1246)
#define HBOOST_FUSION_VECTOR_LIMITS_07072005_1246

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/detail/pp_round.hpp>
#include <hboost/preprocessor/stringize.hpp>

#if !defined(FUSION_MAX_VECTOR_SIZE)
# define FUSION_MAX_VECTOR_SIZE 10
#else
# if FUSION_MAX_VECTOR_SIZE < 3
#   undef FUSION_MAX_VECTOR_SIZE
#   define FUSION_MAX_VECTOR_SIZE 10
# endif
#endif

#define FUSION_MAX_VECTOR_SIZE_STR HBOOST_PP_STRINGIZE(HBOOST_FUSION_PP_ROUND_UP(FUSION_MAX_VECTOR_SIZE))

#endif
