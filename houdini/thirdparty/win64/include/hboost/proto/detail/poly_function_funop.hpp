#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/poly_function_funop.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_NORMALIZE_ARG(Z, N, DATA)                                                   \
        static_cast<typename normalize_arg<HBOOST_PP_CAT(A, N) const &>                              \
            ::reference>(HBOOST_PP_CAT(a, N))                                                        \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/poly_function_funop.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    // poly_function_funop.hpp
    // Contains overloads of poly_function\<\>::operator()
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/poly_function_funop.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_NORMALIZE_ARG

#else

    #define N HBOOST_PP_ITERATION()

    template<typename This HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct result<This(HBOOST_PP_ENUM_PARAMS(N, A))>
      : Derived::template impl<
            HBOOST_PP_ENUM_BINARY_PARAMS(
                N
              , typename normalize_arg<A
              , >::type HBOOST_PP_INTERCEPT
            )
        >
    {
        typedef typename result::result_type type;
    };

    template<HBOOST_PP_ENUM_PARAMS(N, typename A)>
    typename result<
        Derived const(
            HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const & HBOOST_PP_INTERCEPT)
        )
    >::type
    operator ()(HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const &a)) const
    {
        result<
            Derived const(
                HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const & HBOOST_PP_INTERCEPT)
            )
        > impl;

        return impl(HBOOST_PP_ENUM(N, HBOOST_PROTO_NORMALIZE_ARG, ~));
    }

    #undef N

#endif // HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES
