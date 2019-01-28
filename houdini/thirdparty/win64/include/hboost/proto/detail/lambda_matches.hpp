#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/lambda_matches.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_DEFINE_LAMBDA_MATCHES(Z, N, DATA)                                           \
        lambda_matches<                                                                             \
            HBOOST_PP_CAT(Expr, N)                                                                   \
          , HBOOST_PP_CAT(Grammar, N)                                                                \
        >

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/lambda_matches.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file lambda_matches.hpp
    /// Specializations of the lambda_matches template
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (2, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/lambda_matches.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_DEFINE_LAMBDA_MATCHES

#else // HBOOST_PP_IS_ITERATING

    #define N HBOOST_PP_ITERATION()

    template<
        template<HBOOST_PP_ENUM_PARAMS(N, typename HBOOST_PP_INTERCEPT)> class T
        HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename Expr)
        HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename Grammar)
    >
    struct lambda_matches<
        T<HBOOST_PP_ENUM_PARAMS(N, Expr)>
      , T<HBOOST_PP_ENUM_PARAMS(N, Grammar)>
        HBOOST_PROTO_TEMPLATE_ARITY_PARAM(N)
    >
      : HBOOST_PP_CAT(and_, N)<
            HBOOST_PROTO_DEFINE_LAMBDA_MATCHES(~, 0, ~)::value,
            HBOOST_PP_ENUM_SHIFTED(N, HBOOST_PROTO_DEFINE_LAMBDA_MATCHES, ~)
        >
    {};

    #undef N

#endif
