#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/funop.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_AS_CHILD_TYPE(Z, N, DATA)                                                   \
        typename proto::result_of::as_child<HBOOST_PP_CAT(A, N), Domain>::type                       \
        /**/

    #define HBOOST_PROTO_AS_CHILD(Z, N, DATA)                                                        \
        proto::as_child<Domain>(HBOOST_PP_CAT(a, N))                                                 \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/funop.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    // funop.hpp
    // Contains definition of funop[n]\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (0, HBOOST_PP_DEC(HBOOST_PROTO_MAX_FUNCTION_CALL_ARITY), <hboost/proto/detail/funop.hpp>))
    #include HBOOST_PP_ITERATE()

    #undef HBOOST_PROTO_AS_CHILD
    #undef HBOOST_PROTO_AS_CHILD_TYPE

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else

    /// \brief A helper metafunction for computing the
    /// return type of \c proto::expr\<\>::operator().
    template<typename Expr, typename Domain HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_ITERATION(), typename A)>
    struct HBOOST_PP_CAT(funop, HBOOST_PP_ITERATION())
    {
        typedef typename proto::base_expr<
            Domain
          , tag::function
          , HBOOST_PP_CAT(list, HBOOST_PP_INC(HBOOST_PP_ITERATION()))<
                Expr &
                HBOOST_PP_ENUM_TRAILING(HBOOST_PP_ITERATION(), HBOOST_PROTO_AS_CHILD_TYPE, ~)
            >
        >::type type;

        HBOOST_FORCEINLINE
        static type const call(
            Expr &e
            HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS(HBOOST_PP_ITERATION(), A, &a)
        )
        {
            type that = {
                e
                HBOOST_PP_ENUM_TRAILING(HBOOST_PP_ITERATION(), HBOOST_PROTO_AS_CHILD, ~)
            };
            return that;
        }
    };

    /// \brief A helper metafunction for computing the
    /// return type of \c proto::expr\<\>::operator().
    template<typename Expr HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_ITERATION(), typename A), typename This, typename Domain>
    struct funop<Expr(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_ITERATION(), A)), This, Domain>
      : HBOOST_PP_CAT(funop, HBOOST_PP_ITERATION())<
            typename detail::same_cv<Expr, This>::type
          , Domain
            HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                HBOOST_PP_ITERATION()
              , typename remove_reference<A
              , >::type HBOOST_PP_INTERCEPT
            )
        >
    {};

#endif // HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES
