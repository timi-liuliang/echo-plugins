#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/make_expr_funop.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/make_expr_funop.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file make_expr_funop.hpp
    /// Contains definition of make_expr\<\>::operator() member functions.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                     \
        (3, (2, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/make_expr_funop.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else // HBOOST_PP_IS_ITERATING

    #define N HBOOST_PP_ITERATION()

    template<typename This HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct result<This(HBOOST_PP_ENUM_PARAMS(N, A))>
    {
        typedef
            typename result_of::make_expr<
                Tag
              , Domain
                HBOOST_PP_ENUM_TRAILING_PARAMS(N, A)
            >::type
        type;
    };

    /// \overload
    ///
    template<HBOOST_PP_ENUM_PARAMS(N, typename A)>
    HBOOST_FORCEINLINE
    typename result_of::make_expr<
        Tag
      , Domain
        HBOOST_PP_ENUM_TRAILING_PARAMS(N, const A)
    >::type const
    operator ()(HBOOST_PP_ENUM_BINARY_PARAMS(N, const A, &a)) const
    {
        return proto::detail::make_expr_<
            Tag
          , Domain
            HBOOST_PP_ENUM_TRAILING_PARAMS(N, const A)
        >()(HBOOST_PP_ENUM_PARAMS(N, a));
    }

    #undef N

#endif
