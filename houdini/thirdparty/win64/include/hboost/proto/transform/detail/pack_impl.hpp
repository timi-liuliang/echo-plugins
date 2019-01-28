#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/transform/detail/preprocessed/pack_impl.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/pack_impl.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file pack_impl.hpp
    /// Contains helpers for pseudo-pack expansion.
    //
    //  Copyright 2012 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (0, HBOOST_PP_DEC(HBOOST_PROTO_MAX_ARITY), <hboost/proto/transform/detail/pack_impl.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else
    #if HBOOST_PP_ITERATION_DEPTH() == 1
        #define N HBOOST_PP_ITERATION()
        #define M HBOOST_PP_SUB(HBOOST_PROTO_MAX_ARITY, N)
        #define M0(Z, X, D)  typename expand_pattern_helper<proto::_child_c<X>, Fun>::type

        template<typename Fun, typename Cont>
        struct expand_pattern<HBOOST_PP_INC(N), Fun, Cont>
          : Cont::template cat<HBOOST_PP_ENUM(HBOOST_PP_INC(N), M0, ~)>
        {
            HBOOST_MPL_ASSERT_MSG(
                (expand_pattern_helper<proto::_child_c<0>, Fun>::applied::value)
              , NO_PACK_EXPRESSION_FOUND_IN_UNPACKING_PATTERN
              , (Fun)
            );
        };

        template<typename Ret HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct HBOOST_PP_CAT(expand_pattern_rest_, N)
        {
            template<HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(HBOOST_PP_INC(M), typename C, void)>
            struct cat;

            #define HBOOST_PP_ITERATION_PARAMS_2                                                     \
                (3, (1, M, <hboost/proto/transform/detail/pack_impl.hpp>))
            #include HBOOST_PP_ITERATE()
        };
        #undef M0
        #undef M
        #undef N
    #else
        #define I HBOOST_PP_ITERATION()
        #define J HBOOST_PP_RELATIVE_ITERATION(1)
            template<HBOOST_PP_ENUM_PARAMS(I, typename C)>
            struct cat<HBOOST_PP_ENUM_PARAMS(I, C)>
            {
                typedef msvc_fun_workaround<Ret(HBOOST_PP_ENUM_PARAMS(J, A) HBOOST_PP_COMMA_IF(J) HBOOST_PP_ENUM_PARAMS(I, C))> type;
            };
        #undef J
        #undef I
    #endif
#endif
