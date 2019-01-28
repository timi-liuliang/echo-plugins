/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SET_LIMITS_09162005_1103)
#define HBOOST_FUSION_SET_LIMITS_09162005_1103

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/cpp03/limits.hpp>

#if !defined(FUSION_MAX_SET_SIZE)
# define FUSION_MAX_SET_SIZE FUSION_MAX_VECTOR_SIZE
#else
# if FUSION_MAX_SET_SIZE < 3
#   undef FUSION_MAX_SET_SIZE
#   if (FUSION_MAX_VECTOR_SIZE > 10)
#       define FUSION_MAX_SET_SIZE 10
#   else
#       define FUSION_MAX_SET_SIZE FUSION_MAX_VECTOR_SIZE
#   endif
# endif
#endif

#define FUSION_MAX_SET_SIZE_STR HBOOST_PP_STRINGIZE(HBOOST_FUSION_PP_ROUND_UP(FUSION_MAX_SET_SIZE))

#endif