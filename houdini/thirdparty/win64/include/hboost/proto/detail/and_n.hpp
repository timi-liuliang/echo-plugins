#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/and_n.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/and_n.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file and_n.hpp
    /// Definitions of and_N, and_impl
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (2, HBOOST_PP_MAX(HBOOST_PROTO_MAX_ARITY, HBOOST_PROTO_MAX_LOGICAL_ARITY), <hboost/proto/detail/and_n.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else // HBOOST_PP_IS_ITERATING

    #define N HBOOST_PP_ITERATION()

    // Assymetry here between the handling of and_N and or_N because
    // and_N is used by lambda_matches up to HBOOST_PROTO_MAX_ARITY,
    // regardless of how low HBOOST_PROTO_MAX_LOGICAL_ARITY is.
    template<bool B, HBOOST_PP_ENUM_PARAMS(HBOOST_PP_DEC(N), typename P)>
    struct HBOOST_PP_CAT(and_, N)
    #if 2 == N
      : mpl::bool_<P0::value>
    {};
    #else
      : HBOOST_PP_CAT(and_, HBOOST_PP_DEC(N))<
            P0::value HBOOST_PP_COMMA_IF(HBOOST_PP_SUB(N,2))
            HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_DEC(N), P)
        >
    {};
    #endif

    template<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_DEC(N), typename P)>
    struct HBOOST_PP_CAT(and_, N)<false, HBOOST_PP_ENUM_PARAMS(HBOOST_PP_DEC(N), P)>
      : mpl::false_
    {};

    #if N <= HBOOST_PROTO_MAX_LOGICAL_ARITY

        template<HBOOST_PP_ENUM_PARAMS(N, typename G), typename Expr, typename State, typename Data>
        struct _and_impl<proto::and_<HBOOST_PP_ENUM_PARAMS(N, G)>, Expr, State, Data>
          : proto::transform_impl<Expr, State, Data>
        {
            #define M0(Z, N, DATA)                                                            \
            typedef                                                                           \
                typename proto::when<proto::_, HBOOST_PP_CAT(G, N)>                            \
                    ::template impl<Expr, State, Data>                                        \
            HBOOST_PP_CAT(Gimpl, N);                                                           \
            /**/
            HBOOST_PP_REPEAT(N, M0, ~)
            #undef M0

            typedef typename HBOOST_PP_CAT(Gimpl, HBOOST_PP_DEC(N))::result_type result_type;

            result_type operator()(
                typename _and_impl::expr_param e
              , typename _and_impl::state_param s
              , typename _and_impl::data_param d
            ) const
            {
                // Fix: jfalcou - 12/29/2010
                // Avoid the use of comma operator here so as not to find Proto's
                // by accident.
                // expands to G0()(e,s,d); G1()(e,s,d); ... G{N-1}()(e,s,d);
                #define M0(Z,N,DATA) HBOOST_PP_CAT(Gimpl,N)()(e,s,d);
                HBOOST_PP_REPEAT(HBOOST_PP_DEC(N),M0,~)
                return HBOOST_PP_CAT(Gimpl,HBOOST_PP_DEC(N))()(e,s,d);
                #undef M0
            }
        };

    #endif

    #undef N

#endif
