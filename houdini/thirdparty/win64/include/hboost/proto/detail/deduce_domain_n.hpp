#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/deduce_domain_n.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_COMMON_DOMAIN2(Z, N, DATA)                                                  \
        typedef                                                                                     \
            typename common_domain2<common ## N, A ## N>::type                                      \
        HBOOST_PP_CAT(common, HBOOST_PP_INC(N));                                                      \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/deduce_domain_n.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    // deduce_domain_n.hpp
    // Definitions of common_domain[n] and deduce_domain[n] class templates.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (3, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/deduce_domain_n.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_COMMON_DOMAIN2

#else

    #define N HBOOST_PP_ITERATION()

    template<HBOOST_PP_ENUM_PARAMS(N, typename A)>
    struct HBOOST_PP_CAT(common_domain, N)
    {
        typedef A0 common1;
        HBOOST_PP_REPEAT_FROM_TO(1, N, HBOOST_PROTO_COMMON_DOMAIN2, ~)
        typedef HBOOST_PP_CAT(common, N) type;
        HBOOST_PROTO_ASSERT_VALID_DOMAIN(type);
    };

    template<HBOOST_PP_ENUM_PARAMS(N, typename E)>
    struct HBOOST_PP_CAT(deduce_domain, N)
      : HBOOST_PP_CAT(common_domain, N)<
            HBOOST_PP_ENUM_BINARY_PARAMS(
                N
              , typename domain_of<E, >::type HBOOST_PP_INTERCEPT
            )
        >
    {};

    #undef N

#endif // HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES
