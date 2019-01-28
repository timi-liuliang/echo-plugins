
#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)
#ifndef HBOOST_PHOENIX_CORE_DETAIL_CALL_HPP
#define HBOOST_PHOENIX_CORE_DETAIL_CALL_HPP

#include <hboost/phoenix/core/detail/preprocessed/call.hpp>

#endif
#else

#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_CORE_DETAIL_CALL_HPP
#define HBOOST_PHOENIX_CORE_DETAIL_CALL_HPP

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/call_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
#endif

/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <hboost/phoenix/support/iterate.hpp>

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

#define M0(Z, N ,D) \
            typedef \
                typename proto::result_of::child_c<Expr, N>::type \
                HBOOST_PP_CAT(A, N); \
/**/
#define M1(Z, N ,D) \
    HBOOST_PP_COMMA_IF(N) proto::child_c<N>(e)
/**/
    

    namespace detail
    {
#define HBOOST_PHOENIX_ITERATION_PARAMS                                          \
    (3, (1, HBOOST_PHOENIX_LIMIT,                                                \
    <hboost/phoenix/core/detail/call.hpp>))
#include HBOOST_PHOENIX_ITERATE()
    }

#undef M0
#undef M1

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif

#else
        
        template <typename Fun, typename Expr, typename State, typename Data>
        struct call_impl<Fun, Expr, State, Data, HBOOST_PHOENIX_ITERATION>
            : proto::transform_impl<Expr, State, Data>
        {
            typedef
                typename hboost::hboostphoenix::result_of::context<State, Data>::type
                context_type;

            HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M0, _)

            typedef
                typename hboost::result_of<
                    Fun(HBOOST_PHOENIX_A, context_type)
                >::type
                result_type;

            result_type operator()(
                typename call_impl::expr_param e
              , typename call_impl::state_param s
              , typename call_impl::data_param d
            ) const
            {
                return
                    Fun()(
                        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M1, _)
                      , hboost::hboostphoenix::context(s, d)
                    );
            }
        };

#endif

#endif // HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES
