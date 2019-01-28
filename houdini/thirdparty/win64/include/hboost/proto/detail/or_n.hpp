#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/or_n.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/or_n.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file or_n.hpp
    /// Definitions of or_N
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (2, HBOOST_PROTO_MAX_LOGICAL_ARITY, <hboost/proto/detail/or_n.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else // HBOOST_PP_IS_ITERATING

    #define N HBOOST_PP_ITERATION()

    template<bool B, typename Expr, typename BasicExpr, HBOOST_PP_ENUM_PARAMS(N, typename G)>
    struct HBOOST_PP_CAT(or_, N)
    #if 2 == N
      : mpl::bool_<matches_<Expr, BasicExpr, typename G1::proto_grammar>::value>
    {
        typedef G1 which;
    };
    #else
      : HBOOST_PP_CAT(or_, HBOOST_PP_DEC(N))<
            matches_<Expr, BasicExpr, typename G1::proto_grammar>::value
          , Expr, BasicExpr, HBOOST_PP_ENUM_SHIFTED_PARAMS(N, G)
        >
    {};
    #endif

    template<typename Expr, typename BasicExpr HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename G)>
    struct HBOOST_PP_CAT(or_, N)<true, Expr, BasicExpr, HBOOST_PP_ENUM_PARAMS(N, G)>
      : mpl::true_
    {
        typedef G0 which;
    };

    #undef N

#endif
