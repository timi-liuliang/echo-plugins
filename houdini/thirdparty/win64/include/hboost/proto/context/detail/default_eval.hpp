#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/context/detail/preprocessed/default_eval.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_DEFAULT_EVAL_SHIFTED(Z, M, DATA)                                            \
        HBOOST_PROTO_DEFAULT_EVAL(Z, HBOOST_PP_ADD(M, 2), DATA)                                       \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/default_eval.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file default_eval.hpp
    /// Contains specializations of the default_eval\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (3, HBOOST_PROTO_MAX_ARITY, <hboost/proto/context/detail/default_eval.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_DEFAULT_EVAL_SHIFTED

#else

    #define N HBOOST_PP_ITERATION()

    template<typename Expr, typename Context>
    struct default_eval<Expr, Context, proto::tag::function, N>
    {
        typedef
            typename proto::detail::result_of_fixup<
                HBOOST_PROTO_DEFAULT_EVAL_TYPE(~, 0, Expr)
            >::type
        function_type;

        typedef
            typename HBOOST_PROTO_RESULT_OF<
                function_type(HBOOST_PP_ENUM_SHIFTED(N, HBOOST_PROTO_DEFAULT_EVAL_TYPE, Expr))
            >::type
        result_type;

        result_type operator ()(Expr &expr, Context &context) const
        {
            return this->invoke(expr, context, is_member_function_pointer<function_type>());
        }

    private:
        result_type invoke(Expr &expr, Context &context, mpl::false_) const
        {
            return HBOOST_PROTO_DEFAULT_EVAL(~, 0, expr)(
                HBOOST_PP_ENUM_SHIFTED(N, HBOOST_PROTO_DEFAULT_EVAL, expr)
            );
        }

        result_type invoke(Expr &expr, Context &context, mpl::true_) const
        {
            HBOOST_PROTO_USE_GET_POINTER();
            typedef typename detail::class_member_traits<function_type>::class_type class_type;
            return (
                HBOOST_PROTO_GET_POINTER(class_type, (HBOOST_PROTO_DEFAULT_EVAL(~, 1, expr))) ->*
                HBOOST_PROTO_DEFAULT_EVAL(~, 0, expr)
            )(HBOOST_PP_ENUM(HBOOST_PP_SUB(N, 2), HBOOST_PROTO_DEFAULT_EVAL_SHIFTED, expr));
        }
    };

    #undef N

#endif
