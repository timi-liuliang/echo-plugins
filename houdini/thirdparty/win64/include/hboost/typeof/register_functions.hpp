// Copyright (C) 2004 Arkadiy Vertleyb
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_REGISTER_FUNCTIONS_HPP_INCLUDED
#define HBOOST_TYPEOF_REGISTER_FUNCTIONS_HPP_INCLUDED

#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/inc.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/if.hpp>
#include <hboost/preprocessor/arithmetic/add.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

#ifndef HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY
#define HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY 10
#endif

enum 
{
    FUN_ID                          = HBOOST_TYPEOF_UNIQUE_ID(),
    FUN_PTR_ID                      = FUN_ID +  1 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    FUN_REF_ID                      = FUN_ID +  2 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    MEM_FUN_ID                      = FUN_ID +  3 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    CONST_MEM_FUN_ID                = FUN_ID +  4 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    VOLATILE_MEM_FUN_ID             = FUN_ID +  5 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    VOLATILE_CONST_MEM_FUN_ID       = FUN_ID +  6 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    FUN_VAR_ID                      = FUN_ID +  7 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    FUN_VAR_PTR_ID                  = FUN_ID +  8 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    FUN_VAR_REF_ID                  = FUN_ID +  9 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    MEM_FUN_VAR_ID                  = FUN_ID + 10 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    CONST_MEM_FUN_VAR_ID            = FUN_ID + 11 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    VOLATILE_MEM_FUN_VAR_ID         = FUN_ID + 12 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY),
    VOLATILE_CONST_MEM_FUN_VAR_ID   = FUN_ID + 13 * HBOOST_PP_INC(HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY)
};

HBOOST_TYPEOF_BEGIN_ENCODE_NS

# define HBOOST_PP_ITERATION_LIMITS (0, HBOOST_TYPEOF_LIMIT_FUNCTION_ARITY)
# define HBOOST_PP_FILENAME_1 <hboost/typeof/register_functions_iterate.hpp>
# include HBOOST_PP_ITERATE()

HBOOST_TYPEOF_END_ENCODE_NS

#endif//HBOOST_TYPEOF_REGISTER_FUNCTIONS_HPP_INCLUDED
