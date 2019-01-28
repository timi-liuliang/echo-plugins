#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/transform/detail/preprocessed/expand_pack.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/expand_pack.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file expand_pack.hpp
    /// Contains helpers for pseudo-pack expansion.
    //
    //  Copyright 2012 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (0, HBOOST_PROTO_MAX_ARITY, <hboost/proto/transform/detail/expand_pack.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else
    #define N HBOOST_PP_ITERATION()
    #define M0(Z, X, DATA) typename expand_pattern_helper<Tfx, HBOOST_PP_CAT(A, X)>::type
    #define M1(Z, X, DATA) expand_pattern_helper<Tfx, HBOOST_PP_CAT(A, X)>::applied::value ||

        template<typename Tfx, typename Ret HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct expand_pattern_helper<Tfx, Ret(HBOOST_PP_ENUM_PARAMS(N, A))>
        {
            typedef Ret (*type)(HBOOST_PP_ENUM(N, M0, ~));
            typedef mpl::bool_<HBOOST_PP_REPEAT(N, M1, ~) false> applied;
        };

    #undef M1
    #undef M0
    #undef N
#endif
