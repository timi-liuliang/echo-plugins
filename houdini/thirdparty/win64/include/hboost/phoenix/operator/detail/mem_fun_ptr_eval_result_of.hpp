/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_OPERATOR_DETAIL_MEM_FUN_PTR_EVAL_RESULT_OF_HPP
#define HBOOST_PHOENIX_OPERATOR_DETAIL_MEM_FUN_PTR_EVAL_RESULT_OF_HPP

#define HBOOST_PHOENIX_MEM_FUN_PTR_EVAL_RESULT_OF_CHILD(Z, N, D)                 \
            typedef                                                             \
                typename                                                        \
                evaluator::impl<                                                \
                    HBOOST_PP_CAT(A, N)                                          \
                  , Context                                                     \
                  , proto::empty_env                                            \
                >::result_type                                                  \
                HBOOST_PP_CAT(child, N);                                         \
        /**/

        #define HBOOST_PHOENIX_ITERATION_PARAMS                                  \
            (3, (2, HBOOST_PHOENIX_LIMIT,                                        \
                 <hboost/phoenix/operator/detail/mem_fun_ptr_eval_result_of.hpp>))
        #include HBOOST_PHOENIX_ITERATE()

#undef HBOOST_PHOENIX_MEM_FUN_PTR_EVAL_RESULT_OF_CHILD

#endif

#else

        template <typename Context, HBOOST_PHOENIX_typename_A>
        struct mem_fun_ptr_eval<Context, HBOOST_PHOENIX_A>
        {
            HBOOST_PP_REPEAT(
                HBOOST_PHOENIX_ITERATION
              , HBOOST_PHOENIX_MEM_FUN_PTR_EVAL_RESULT_OF_CHILD
              , _
            )

            typedef
                typename hboost::result_of<
                    child1(
                        HBOOST_PP_ENUM_SHIFTED_PARAMS(
                            HBOOST_PHOENIX_ITERATION
                          , child
                        )
                    )
                >::type
                type;
        };

#endif
