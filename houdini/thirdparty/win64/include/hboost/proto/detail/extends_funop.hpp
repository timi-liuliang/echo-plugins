#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #ifndef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
        HBOOST_PROTO_EXTENDS_FUNCTION_()
        HBOOST_PROTO_DEFINE_FUN_OP_VARIADIC_IMPL_(HBOOST_PP_EMPTY)
        HBOOST_PROTO_DEFINE_FUN_OP_VARIADIC_IMPL_(HBOOST_PROTO_CONST)
    #else
        #include <hboost/proto/detail/preprocessed/extends_funop.hpp>
    #endif

#else

    #define HBOOST_PP_LOCAL_MACRO(N)                                                                 \
        HBOOST_PROTO_DEFINE_FUN_OP(1, N, ~)                                                          \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES) && !defined(HBOOST_PROTO_NO_WAVE_OUTPUT)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/extends_funop.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file extends_funop.hpp
    /// Definitions for extends\<\>::operator()
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES) && !defined(HBOOST_PROTO_NO_WAVE_OUTPUT)
        #pragma wave option(preserve: 1)
    #endif

    HBOOST_PROTO_EXTENDS_FUNCTION_()

    #define HBOOST_PP_LOCAL_LIMITS                                                                   \
        (0, HBOOST_PP_DEC(HBOOST_PROTO_MAX_FUNCTION_CALL_ARITY))
    #include HBOOST_PP_LOCAL_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES) && !defined(HBOOST_PROTO_NO_WAVE_OUTPUT)
        #pragma wave option(output: null)
    #endif

#endif
