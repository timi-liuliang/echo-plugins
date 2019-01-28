#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/memfun_funop.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/memfun_funop.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    // memfun_funop.hpp
    // Contains overloads of memfun::operator().
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/memfun_funop.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else

    #define N HBOOST_PP_ITERATION()

    template<HBOOST_PP_ENUM_PARAMS(N, typename A)>
    HBOOST_FORCEINLINE
    result_type operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N, A, const &a)) const
    {
        HBOOST_PROTO_USE_GET_POINTER();
        return (HBOOST_PROTO_GET_POINTER(V, obj) ->* pmf)(HBOOST_PP_ENUM_PARAMS(N, a));
    }

    #undef N

#endif // HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES
