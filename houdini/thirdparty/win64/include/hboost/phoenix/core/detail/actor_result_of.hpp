
#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)
#ifndef HBOOST_PHOENIX_CORE_DETAIL_ACTOR_RESULT_OF_HPP
#define HBOOST_PHOENIX_CORE_DETAIL_ACTOR_RESULT_OF_HPP

#include <hboost/phoenix/support/iterate.hpp>

#include <hboost/phoenix/core/detail/preprocessed/actor_result_of.hpp>

#endif
#else

#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_CORE_DETAIL_ACTOR_RESULT_OF_HPP
#define HBOOST_PHOENIX_CORE_DETAIL_ACTOR_RESULT_OF_HPP

#include <hboost/phoenix/support/iterate.hpp>

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/actor_result_of_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
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

    namespace result_of
    {
        template <typename Expr
            , HBOOST_PHOENIX_typename_A_void(HBOOST_PHOENIX_ACTOR_LIMIT)
            , typename Dummy = void>
        struct actor;

        template <typename Expr>
        struct nullary_actor_result
        {
            typedef
                typename hboost::hboostphoenix::evaluator::impl<
                    Expr const&
                  , vector2<
                        vector1<const ::hboost::hboostphoenix::actor<Expr> *> &
                      , default_actions
                    > const &
                  , proto::empty_env
                >::result_type
                type;
        };

        template <typename Expr>
        struct actor<Expr>
        {
            typedef
                // avoid calling result_of::actor when this is false
                typename mpl::eval_if_c<
                    result_of::is_nullary<Expr>::value
                  , nullary_actor_result<Expr>
                  , mpl::identity<detail::error_expecting_arguments>
                >::type
            type;
        };

#define HBOOST_PHOENIX_ITERATION_PARAMS                                          \
    (3, (1, HBOOST_PHOENIX_ACTOR_LIMIT,                                          \
    <hboost/phoenix/core/detail/actor_result_of.hpp>))
#include HBOOST_PHOENIX_ITERATE()

    }

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif

#else

        template <typename Expr, HBOOST_PHOENIX_typename_A>
        struct actor<Expr, HBOOST_PHOENIX_A>
        {
            typedef
                typename hboostphoenix::evaluator::
                    impl<
                        Expr const&
                      , vector2<
                            HBOOST_PP_CAT(
                                vector
                              , HBOOST_PP_INC(HBOOST_PHOENIX_ITERATION)
                            )<const ::hboost::hboostphoenix::actor<Expr> *, HBOOST_PHOENIX_A> &
                          , default_actions
                        > const &
                      , proto::empty_env
                    >::result_type
                type;
        };

#endif

#endif // HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES
