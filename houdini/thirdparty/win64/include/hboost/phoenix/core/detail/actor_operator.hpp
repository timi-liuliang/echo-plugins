
#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)
#ifndef HBOOST_PHOENIX_CORE_DETAIL_ACTOR_OPERATOR_HPP
#define HBOOST_PHOENIX_CORE_DETAIL_ACTOR_OPERATOR_HPP

#include <hboost/phoenix/support/iterate.hpp>

#include <hboost/phoenix/core/detail/preprocessed/actor_operator.hpp>

#endif
#else

#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_CORE_DETAIL_ACTOR_OPERATOR_HPP
#define HBOOST_PHOENIX_CORE_DETAIL_ACTOR_OPERATOR_HPP

#include <hboost/phoenix/support/iterate.hpp>

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/actor_operator_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
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
    typename detail::result_type_deduction_helper<HBOOST_PP_CAT(A, N)>::type     \
/**/

#define HBOOST_PHOENIX_ITERATION_PARAMS                                          \
        (3, (1, HBOOST_PHOENIX_ACTOR_LIMIT,                                      \
        <hboost/phoenix/core/detail/actor_operator.hpp>))
#include HBOOST_PHOENIX_ITERATE()

#undef M0

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif

#else

#if HBOOST_PHOENIX_ITERATION >= HBOOST_PHOENIX_PERFECT_FORWARD_LIMIT

        template <typename This, HBOOST_PHOENIX_typename_A>
        struct result<This(HBOOST_PHOENIX_A)>
            : result<This(HBOOST_PHOENIX_A_const_ref)>
        {};

        template <typename This, HBOOST_PHOENIX_typename_A>
        struct result<This(HBOOST_PHOENIX_A_ref)>
            : result_of::actor<proto_base_expr, HBOOST_PHOENIX_A_ref>
        {};

        template <HBOOST_PHOENIX_typename_A>
        typename result_of::actor<proto_base_expr, HBOOST_PHOENIX_A_ref>::type
        operator()(HBOOST_PHOENIX_A_ref_a)
        {
            typedef
                HBOOST_PP_CAT(vector, HBOOST_PP_INC(HBOOST_PHOENIX_ITERATION))<
                    const actor<Expr> *, HBOOST_PHOENIX_A_ref
                >
                env_type;
            env_type env = {this, HBOOST_PHOENIX_a};
            
            return hboostphoenix::eval(*this, hboostphoenix::context(env, default_actions()));
        }

        template <HBOOST_PHOENIX_typename_A>
        typename result_of::actor<proto_base_expr, HBOOST_PHOENIX_A_ref>::type
        operator()(HBOOST_PHOENIX_A_ref_a) const
        {
            typedef
                HBOOST_PP_CAT(vector, HBOOST_PP_INC(HBOOST_PHOENIX_ITERATION))<
                    const actor<Expr> *, HBOOST_PHOENIX_A_ref
                >
                env_type;
            env_type env = {this, HBOOST_PHOENIX_a};
            
            return hboostphoenix::eval(*this, hboostphoenix::context(env, default_actions()));
        }

        template <HBOOST_PHOENIX_typename_A>
        typename result_of::actor<proto_base_expr, HBOOST_PHOENIX_A_const_ref>::type
        operator()(HBOOST_PHOENIX_A_const_ref_a)
        {
            typedef
                HBOOST_PP_CAT(vector, HBOOST_PP_INC(HBOOST_PHOENIX_ITERATION))<
                    const actor<Expr> *, HBOOST_PHOENIX_A_const_ref
                >
                env_type;
            env_type env = {this, HBOOST_PHOENIX_a};
            
            return hboostphoenix::eval(*this, hboostphoenix::context(env, default_actions()));
        }

        template <HBOOST_PHOENIX_typename_A>
        typename result_of::actor<proto_base_expr, HBOOST_PHOENIX_A_const_ref>::type
        operator()(HBOOST_PHOENIX_A_const_ref_a) const
        {
            typedef
                HBOOST_PP_CAT(vector, HBOOST_PP_INC(HBOOST_PHOENIX_ITERATION))<
                    const actor<Expr> *, HBOOST_PHOENIX_A_const_ref
                >
                env_type;
            env_type env = {this, HBOOST_PHOENIX_a};
            
            return hboostphoenix::eval(*this, hboostphoenix::context(env, default_actions()));
        }

#else

// We need to define operator() for all permutations of reference types.
// For HBOOST_PHOENIX_ITERATION <= HBOOST_PHOENIX_LIMIT_PREFECT_FORWARD
// 2^HBOOST_PHOENIX_ITERATION overloads are created
// For compile time reasons,
// if HBOOST_PHOENIX_ITERATION > HBOOST_PHOENIX_LIMIT_PERFECT_FORWARD
// only operator()(A const &...a) and operator()(A &...a) are generated
// this is all handled by the PP mumbo jumbo above
#define HBOOST_PHOENIX_ACTOR_OPERATOR(_, I, __)                                  \
        template <typename This, HBOOST_PHOENIX_typename_A>                      \
        struct result<This(HBOOST_PHOENIX_PERM_A(I))>                            \
            : result_of::actor<proto_base_expr, HBOOST_PHOENIX_PERM_A(I)>        \
        {};                                                                     \
                                                                                \
        template <HBOOST_PHOENIX_typename_A>                                     \
        typename result_of::actor<proto_base_expr, HBOOST_PHOENIX_PERM_A(I)>::type\
        operator()(HBOOST_PHOENIX_PERM_A_a(I)) const                             \
        {                                                                       \
            typedef                                                             \
                HBOOST_PP_CAT(vector, HBOOST_PP_INC(HBOOST_PHOENIX_ITERATION))<    \
                    const actor<Expr> *, HBOOST_PHOENIX_PERM_A(I)                \
                >                                                               \
                env_type;                                                       \
            env_type env = {this, HBOOST_PHOENIX_a};                             \
                                                                                \
            return hboostphoenix::eval(*this, hboostphoenix::context(env, default_actions()));\
        }                                                                       \
                                                                                \
        template <HBOOST_PHOENIX_typename_A>                                     \
        typename result_of::actor<proto_base_expr, HBOOST_PHOENIX_PERM_A(I)>::type\
        operator()(HBOOST_PHOENIX_PERM_A_a(I))                                   \
        {                                                                       \
            typedef                                                             \
                HBOOST_PP_CAT(vector, HBOOST_PP_INC(HBOOST_PHOENIX_ITERATION))<    \
                    const actor<Expr> *, HBOOST_PHOENIX_PERM_A(I)                \
                >                                                               \
                env_type;                                                       \
            env_type env = {this, HBOOST_PHOENIX_a};                             \
                                                                                \
            return hboostphoenix::eval(*this, hboostphoenix::context(env, default_actions()));\
        }                                                                       \
        /**/

        template <typename This, HBOOST_PHOENIX_typename_A>
        struct result<This(HBOOST_PHOENIX_A)>
            : result<This(HBOOST_PP_ENUM(HBOOST_PHOENIX_ITERATION, M0, _))>
        {};

        HBOOST_PP_REPEAT(HBOOST_PHOENIX_PERM_SIZE, HBOOST_PHOENIX_ACTOR_OPERATOR, _)

#undef HBOOST_PHOENIX_ACTOR_OPERATOR

#endif

#endif

#endif // HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES
