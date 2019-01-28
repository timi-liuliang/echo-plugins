#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/transform/detail/preprocessed/construct_pod_funop.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/construct_pod_funop.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file construct_pod_funop.hpp
    /// Overloads of construct_\<\>::operator().
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                         \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/transform/detail/construct_pod_funop.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else

    #define N HBOOST_PP_ITERATION()

    template<HBOOST_PP_ENUM_PARAMS(N, typename A)>
    HBOOST_FORCEINLINE
    Type operator ()(HBOOST_PP_ENUM_BINARY_PARAMS(N, A, &a)) const
    {
        Type that = {HBOOST_PP_ENUM_PARAMS(N, a)};
        return that;
    }

    #undef N

#endif
