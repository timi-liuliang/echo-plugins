#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/generate_by_value.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/generate_by_value.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file generate_by_value.hpp
    /// Contains definition of by_value_generator_\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/generate_by_value.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else // HBOOST_PP_IS_ITERATING

    #define N HBOOST_PP_ITERATION()

    template<typename Tag HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename Arg) >
    struct by_value_generator_<
        proto::expr<Tag, HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, Arg)>, N>
    >
    {
        typedef
            HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, Arg)>
        src_args;

        typedef
            HBOOST_PP_CAT(list, N)<
                HBOOST_PP_ENUM_BINARY_PARAMS(N, typename uncvref<Arg, >::type HBOOST_PP_INTERCEPT)
            >
        dst_args;

        typedef proto::expr<Tag, src_args, N> src_type;
        typedef proto::expr<Tag, dst_args, N> type;

        HBOOST_FORCEINLINE
        static type const call(src_type const &e)
        {
            type that = {
                HBOOST_PP_ENUM_PARAMS(N, e.child)
            };
            return that;
        }
    };

    template<typename Tag HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename Arg) >
    struct by_value_generator_<
        proto::basic_expr<Tag, HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, Arg)>, N>
    >
    {
        typedef
            HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, Arg)>
        src_args;

        typedef
            HBOOST_PP_CAT(list, N)<
                HBOOST_PP_ENUM_BINARY_PARAMS(N, typename uncvref<Arg, >::type HBOOST_PP_INTERCEPT)
            >
        dst_args;

        typedef proto::basic_expr<Tag, src_args, N> src_type;
        typedef proto::basic_expr<Tag, dst_args, N> type;

        HBOOST_FORCEINLINE
        static type const call(src_type const &e)
        {
            type that = {
                HBOOST_PP_ENUM_PARAMS(N, e.child)
            };
            return that;
        }
    };

    #undef N

#endif
