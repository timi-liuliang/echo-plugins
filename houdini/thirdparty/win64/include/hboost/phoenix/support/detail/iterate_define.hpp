/*==============================================================================
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !HBOOST_PHOENIX_IS_ITERATING

#define HBOOST_PHOENIX_typename_A(N)                                             \
    HBOOST_PP_ENUM_PARAMS(N, typename A)                                         \
/**/

#define HBOOST_PHOENIX_typename_A_void(N)                                        \
    HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(N, typename A, void)                    \
/**/

#define HBOOST_PHOENIX_A(N)                                                      \
    HBOOST_PP_ENUM_PARAMS(N, A)                                                  \
/**/

#define HBOOST_PHOENIX_A_ref(N)                                                  \
    HBOOST_PP_ENUM_BINARY_PARAMS(N, A, & HBOOST_PP_INTERCEPT)                     \
/**/

#define HBOOST_PHOENIX_A_const_ref(N)                                            \
    HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const& HBOOST_PP_INTERCEPT)                \
/**/

#define HBOOST_PHOENIX_A_a(N)                                                    \
    HBOOST_PP_ENUM_BINARY_PARAMS(N, A, a)                                        \
/**/

#define HBOOST_PHOENIX_A_ref_a(N)                                                \
    HBOOST_PP_ENUM_BINARY_PARAMS(N, A, & a)                                      \
/**/

#define HBOOST_PHOENIX_A_const_ref_a(N)                                          \
    HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a)                                 \
/**/

#define HBOOST_PHOENIX_a(N)                                                      \
    HBOOST_PP_ENUM_PARAMS(N, a)                                                  \
/**/

#else

#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <hboost/preprocessor/seq/elem.hpp>
#include <hboost/preprocessor/seq/enum.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/for_each_product.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>

#define HBOOST_PHOENIX_ITERATION                                                 \
    HBOOST_PP_ITERATION()                                                        \
/**/

#define HBOOST_PHOENIX_typename_A                                                \
    HBOOST_PP_ENUM_PARAMS(HBOOST_PHOENIX_ITERATION, typename A)                   \
/**/

#define HBOOST_PHOENIX_typename_A_void                                           \
    HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(HBOOST_PHOENIX_ITERATION, typename A, void)
/**/

#define HBOOST_PHOENIX_A                                                          \
    HBOOST_PP_ENUM_PARAMS(HBOOST_PHOENIX_ITERATION, A)                             \
/**/

#define HBOOST_PHOENIX_A_ref                                                      \
    HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PHOENIX_ITERATION, A, & HBOOST_PP_INTERCEPT)\
/**/

#define HBOOST_PHOENIX_A_const_ref                                                \
    HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PHOENIX_ITERATION, A, const& HBOOST_PP_INTERCEPT)\
/**/

#define HBOOST_PHOENIX_A_a                                                        \
    HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PHOENIX_ITERATION, A, a)                   \
/**/

#define HBOOST_PHOENIX_A_ref_a                                                    \
    HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PHOENIX_ITERATION, A, & a)                 \
/**/

#define HBOOST_PHOENIX_A_const_ref_a                                              \
    HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PHOENIX_ITERATION, A, const& a)            \
/**/

#define HBOOST_PHOENIX_a                                                          \
    HBOOST_PP_ENUM_PARAMS(HBOOST_PHOENIX_ITERATION, a)                             \
/**/

    /////////////////////////////////////////////////////////////////////////////
    // Begin Perfect Forward argument permutation calculation
    /////////////////////////////////////////////////////////////////////////////
#define HBOOST_PHOENIX_M0_R(_, N, __)                                            \
    (((A ## N)(&))((A ## N)(const&)))                                           \
/**/

#define HBOOST_PHOENIX_M0                                                        \
    HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, HBOOST_PHOENIX_M0_R, _)             \
/**/

#define HBOOST_PHOENIX_M1_R_R(_, N, SEQ)                                         \
    HBOOST_PP_SEQ_ELEM(N, SEQ)                                                   \
/**/

#define HBOOST_PHOENIX_M1_R(R, __, ___, ELEM)                                    \
    (HBOOST_PP_REPEAT(HBOOST_PP_SEQ_SIZE(ELEM), HBOOST_PHOENIX_M1_R_R, ELEM))      \
/**/

#define HBOOST_PHOENIX_M1(R, PRODUCT)                                            \
    ((HBOOST_PP_SEQ_ENUM                                                         \
        (HBOOST_PP_SEQ_FOR_EACH_I_R                                              \
            (R, HBOOST_PHOENIX_M1_R, _, PRODUCT))))                              \
/**/

#define HBOOST_PHOENIX_PERM_SEQ                                                  \
    HBOOST_PP_SEQ_FOR_EACH_PRODUCT(HBOOST_PHOENIX_M1, HBOOST_PHOENIX_M0)           \
/**/
    ////////////////////////////////////////////////////////////////////////////
    // End
    ////////////////////////////////////////////////////////////////////////////

#define HBOOST_PHOENIX_PERM_SIZE                                                 \
    HBOOST_PP_SEQ_SIZE(HBOOST_PHOENIX_PERM_SEQ)                                   \
/**/

#define HBOOST_PHOENIX_M2(_, N, TUPLE)                                           \
    HBOOST_PP_COMMA_IF(N) HBOOST_PP_TUPLE_ELEM(HBOOST_PHOENIX_ITERATION, N, TUPLE) \
/**/
    
#define HBOOST_PHOENIX_M3(_, N, TUPLE)                                           \
    HBOOST_PP_COMMA_IF(N) HBOOST_PP_TUPLE_ELEM(HBOOST_PHOENIX_ITERATION, N, TUPLE) a ## N\
/**/

#define HBOOST_PHOENIX_PERM_ELEM(N)                                              \
    HBOOST_PP_SEQ_ELEM(N, HBOOST_PHOENIX_PERM_SEQ)                                \
/**/

#define HBOOST_PHOENIX_PERM_A(N)                                                 \
    HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, HBOOST_PHOENIX_M2, HBOOST_PHOENIX_PERM_ELEM(N))\
/**/

#define HBOOST_PHOENIX_PERM_A_a(N)                                               \
    HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, HBOOST_PHOENIX_M3, HBOOST_PHOENIX_PERM_ELEM(N))\
/**/

#endif
