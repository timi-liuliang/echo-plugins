#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/context/detail/preprocessed/null_eval.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_EVAL_N(Z, N, DATA)                                                          \
        proto::eval(proto::child_c<N>(expr), ctx);                                                  \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/null_eval.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file null_eval.hpp
    /// Contains specializations of the null_eval\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/context/detail/null_eval.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_EVAL_N

#else

    #define N HBOOST_PP_ITERATION()

    template<typename Expr, typename Context>
    struct null_eval<Expr, Context, N>
    {
        typedef void result_type;

        void operator ()(Expr &expr, Context &ctx) const
        {
            HBOOST_PP_REPEAT(N, HBOOST_PROTO_EVAL_N, ~)
        }
    };

    #undef N

#endif
