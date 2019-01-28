#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/transform/detail/preprocessed/fold_impl.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_CHILD_N_TYPE(N)                                                             \
        HBOOST_PP_CAT(proto_child, N)                                                                \
        /**/

    #define HBOOST_PROTO_FOLD_STATE_TYPE(Z, N, DATA)                                                 \
        typedef                                                                                     \
            typename when<_, Fun>::template impl<                                                   \
                typename result_of::child_c<Expr, N>::type                                          \
              , HBOOST_PP_CAT(state, N)                                                              \
              , Data                                                                                \
            >::result_type                                                                          \
        HBOOST_PP_CAT(state, HBOOST_PP_INC(N));                                                       \
        /**/

    #define HBOOST_PROTO_FOLD_STATE(Z, N, DATA)                                                      \
        HBOOST_PP_CAT(state, HBOOST_PP_INC(N))                                                        \
        HBOOST_PP_CAT(s, HBOOST_PP_INC(N))                                                            \
          = typename when<_, Fun>::template impl<                                                   \
                typename result_of::child_c<Expr, N>::type                                          \
              , HBOOST_PP_CAT(state, N)                                                              \
              , Data                                                                                \
            >()(                                                                                    \
                proto::child_c<N>(e)                                                                \
              , HBOOST_PP_CAT(s, N)                                                                  \
              , d                                                                                   \
            );                                                                                      \
        /**/

    #define HBOOST_PROTO_REVERSE_FOLD_STATE_TYPE(Z, N, DATA)                                         \
        typedef                                                                                     \
            typename when<_, Fun>::template impl<                                                   \
                typename result_of::child_c<                                                        \
                    Expr                                                                            \
                  , HBOOST_PP_SUB(DATA, HBOOST_PP_INC(N))                                             \
                >::type                                                                             \
              , HBOOST_PP_CAT(state, HBOOST_PP_SUB(DATA, N))                                          \
              , Data                                                                                \
            >::result_type                                                                          \
        HBOOST_PP_CAT(state, HBOOST_PP_SUB(DATA, HBOOST_PP_INC(N)));                                   \
        /**/

    #define HBOOST_PROTO_REVERSE_FOLD_STATE(Z, N, DATA)                                              \
        HBOOST_PP_CAT(state, HBOOST_PP_SUB(DATA, HBOOST_PP_INC(N)))                                    \
        HBOOST_PP_CAT(s, HBOOST_PP_SUB(DATA, HBOOST_PP_INC(N)))                                        \
          = typename when<_, Fun>::template impl<                                                   \
                typename result_of::child_c<                                                        \
                    Expr                                                                            \
                  , HBOOST_PP_SUB(DATA, HBOOST_PP_INC(N))                                             \
                >::type                                                                             \
              , HBOOST_PP_CAT(state, HBOOST_PP_SUB(DATA, N))                                          \
              , Data                                                                                \
            >()(                                                                                    \
                proto::child_c<HBOOST_PP_SUB(DATA, HBOOST_PP_INC(N))>(e)                              \
              , HBOOST_PP_CAT(s, HBOOST_PP_SUB(DATA, N))                                              \
              , d                                                                                   \
            );                                                                                      \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/fold_impl.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file fold_impl.hpp
    /// Contains definition of fold_impl<> and reverse_fold_impl<> templates.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/transform/detail/fold_impl.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_REVERSE_FOLD_STATE
    #undef HBOOST_PROTO_REVERSE_FOLD_STATE_TYPE
    #undef HBOOST_PROTO_FOLD_STATE
    #undef HBOOST_PROTO_FOLD_STATE_TYPE
    #undef HBOOST_PROTO_CHILD_N_TYPE

#else

    #define N HBOOST_PP_ITERATION()

    template<typename State0, typename Fun, typename Expr, typename State, typename Data>
    struct fold_impl<State0, Fun, Expr, State, Data, N>
      : transform_impl<Expr, State, Data>
    {
        typedef typename when<_, State0>::template impl<Expr, State, Data>::result_type state0;
        HBOOST_PP_REPEAT(N, HBOOST_PROTO_FOLD_STATE_TYPE, N)
        typedef HBOOST_PP_CAT(state, N) result_type;

        result_type operator ()(
            typename fold_impl::expr_param e
          , typename fold_impl::state_param s
          , typename fold_impl::data_param d
        ) const
        {
            state0 s0 =
                typename when<_, State0>::template impl<Expr, State, Data>()(e, s, d);
            HBOOST_PP_REPEAT(N, HBOOST_PROTO_FOLD_STATE, N)
            return HBOOST_PP_CAT(s, N);
        }
    };

    template<typename State0, typename Fun, typename Expr, typename State, typename Data>
    struct reverse_fold_impl<State0, Fun, Expr, State, Data, N>
      : transform_impl<Expr, State, Data>
    {
        typedef typename when<_, State0>::template impl<Expr, State, Data>::result_type HBOOST_PP_CAT(state, N);
        HBOOST_PP_REPEAT(N, HBOOST_PROTO_REVERSE_FOLD_STATE_TYPE, N)
        typedef state0 result_type;

        result_type operator ()(
            typename reverse_fold_impl::expr_param e
          , typename reverse_fold_impl::state_param s
          , typename reverse_fold_impl::data_param d
        ) const
        {
            HBOOST_PP_CAT(state, N) HBOOST_PP_CAT(s, N) =
                typename when<_, State0>::template impl<Expr, State, Data>()(e, s, d);
            HBOOST_PP_REPEAT(N, HBOOST_PROTO_REVERSE_FOLD_STATE, N)
            return s0;
        }
    };

    #undef N

#endif
