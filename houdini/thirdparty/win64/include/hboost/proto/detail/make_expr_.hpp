#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/make_expr_.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/make_expr_.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file make_expr_.hpp
    /// Contains definition of make_expr_\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    template<
        typename Tag
      , typename Domain
        HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
            HBOOST_PROTO_MAX_ARITY
          , typename A
          , = void HBOOST_PP_INTERCEPT
        )
      , typename _ = void
    >
    struct make_expr_
    {};

    template<typename Domain, typename A>
    struct make_expr_<tag::terminal, Domain, A
        HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PROTO_MAX_ARITY, void HBOOST_PP_INTERCEPT)>
    {
        typedef typename proto::detail::protoify<A, Domain>::result_type result_type;

        HBOOST_FORCEINLINE
        result_type operator()(typename add_reference<A>::type a) const
        {
            return proto::detail::protoify<A, Domain>()(a);
        }
    };

    template<typename A>
    struct make_expr_<tag::terminal, deduce_domain, A
        HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PROTO_MAX_ARITY, void HBOOST_PP_INTERCEPT)>
      : make_expr_<tag::terminal, default_domain, A>
    {};

    #define HBOOST_PP_ITERATION_PARAMS_1                                                         \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/make_expr_.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else // HBOOST_PP_IS_ITERATING

    #define N HBOOST_PP_ITERATION()
    #define M HBOOST_PP_SUB(HBOOST_PROTO_MAX_ARITY, N)

    template<typename Tag, typename Domain HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct make_expr_<Tag, Domain HBOOST_PP_ENUM_TRAILING_PARAMS(N, A)
        HBOOST_PP_ENUM_TRAILING_PARAMS(M, void HBOOST_PP_INTERCEPT), void>
    {
        typedef
            HBOOST_PP_CAT(list, N)<
                HBOOST_PP_ENUM(N, HBOOST_PROTO_AS_CHILD_TYPE, (A, ~, Domain))
            >
        proto_args;

        typedef typename base_expr<Domain, Tag, proto_args>::type expr_type;
        typedef typename Domain::proto_generator proto_generator;
        typedef typename proto_generator::template result<proto_generator(expr_type)>::type result_type;

        HBOOST_FORCEINLINE
        result_type operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N, typename add_reference<A, >::type a)) const
        {
            expr_type const that = {
                HBOOST_PP_ENUM(N, HBOOST_PROTO_AS_CHILD, (A, a, Domain))
            };
            return proto_generator()(that);
        }
    };

    template<typename Tag HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct make_expr_<Tag, deduce_domain HBOOST_PP_ENUM_TRAILING_PARAMS(N, A)
        HBOOST_PP_ENUM_TRAILING_PARAMS(M, void HBOOST_PP_INTERCEPT), void>
      : make_expr_<
            Tag
          , typename HBOOST_PP_CAT(deduce_domain, N)<HBOOST_PP_ENUM_PARAMS(N, A)>::type
            HBOOST_PP_ENUM_TRAILING_PARAMS(N, A)
        >
    {};

    #undef N
    #undef M

#endif
