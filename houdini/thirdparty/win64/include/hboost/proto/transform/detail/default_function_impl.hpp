#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/transform/detail/preprocessed/default_function_impl.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_DEF_FUN_INVOKE_ARG(Z, M, DATA)                                              \
        HBOOST_PROTO_DEFAULT_EVAL(Z, HBOOST_PP_ADD(M, 2), DATA)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/default_function_impl.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file default_function_impl.hpp
    /// Contains definition of the default_function_impl, the implementation of the
    /// _default transform for function-like nodes.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (3, HBOOST_PROTO_MAX_ARITY, <hboost/proto/transform/detail/default_function_impl.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_DEF_FUN_INVOKE_ARG

#else

    #define N HBOOST_PP_ITERATION()

    template<typename Grammar, typename Expr, typename State, typename Data>
    struct default_function_impl<Grammar, Expr, State, Data, N>
      : transform_impl<Expr, State, Data>
    {
        HBOOST_PP_REPEAT(N, HBOOST_PROTO_DEFAULT_EVAL_TYPE, Expr)

        typedef
            typename proto::detail::result_of_fixup<r0>::type
        function_type;

        typedef
            typename HBOOST_PROTO_RESULT_OF<
                function_type(HBOOST_PP_ENUM_SHIFTED_PARAMS(N, r))
            >::type
        result_type;

        result_type operator ()(
            typename default_function_impl::expr_param e
          , typename default_function_impl::state_param s
          , typename default_function_impl::data_param d
        ) const
        {
            return this->invoke(e, s, d, is_member_function_pointer<function_type>());
        }

    private:
        result_type invoke(
            typename default_function_impl::expr_param e
          , typename default_function_impl::state_param s
          , typename default_function_impl::data_param d
          , mpl::false_
        ) const
        {
            return HBOOST_PROTO_DEFAULT_EVAL(~, 0, e)(
                HBOOST_PP_ENUM_SHIFTED(N, HBOOST_PROTO_DEFAULT_EVAL, e)
            );
        }

        result_type invoke(
            typename default_function_impl::expr_param e
          , typename default_function_impl::state_param s
          , typename default_function_impl::data_param d
          , mpl::true_
        ) const
        {
            HBOOST_PROTO_USE_GET_POINTER();
            typedef typename detail::class_member_traits<function_type>::class_type class_type;
            return (
                HBOOST_PROTO_GET_POINTER(class_type, (HBOOST_PROTO_DEFAULT_EVAL(~, 1, e))) ->* 
                HBOOST_PROTO_DEFAULT_EVAL(~, 0, e)
            )(HBOOST_PP_ENUM(HBOOST_PP_SUB(N, 2), HBOOST_PROTO_DEF_FUN_INVOKE_ARG, e));
        }
    };

    #undef N

#endif
