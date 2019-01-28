#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/class_member_traits.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/class_member_traits.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    // class_member_traits.hpp
    // Contains specializations of the class_member_traits\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (0, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/class_member_traits.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else

    #define N HBOOST_PP_ITERATION()

    template<typename T, typename U HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct class_member_traits<T (U::*)(HBOOST_PP_ENUM_PARAMS(N, A))>
    {
        typedef U class_type;
        typedef T result_type;
    };

    template<typename T, typename U HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct class_member_traits<T (U::*)(HBOOST_PP_ENUM_PARAMS(N, A)) const>
    {
        typedef U class_type;
        typedef T result_type;
    };

    #undef N

#endif // HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES
