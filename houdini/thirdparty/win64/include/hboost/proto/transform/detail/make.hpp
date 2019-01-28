#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/transform/detail/preprocessed/make.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_MAKE_IF(Z, M, DATA)                                                         \
        make_if_<HBOOST_PP_CAT(A, M), Expr, State, Data>                                             \
        /**/

    #define HBOOST_PROTO_MAKE_IF_TYPE(Z, M, DATA)                                                    \
        typename HBOOST_PROTO_MAKE_IF(Z, M, DATA) ::type                                             \
        /**/

    #define HBOOST_PROTO_MAKE_IF_APPLIED(Z, M, DATA)                                                 \
        HBOOST_PROTO_MAKE_IF(Z, M, DATA) ::applied ||                                                \
        /**/

    #define HBOOST_PROTO_CONSTRUCT_ARG(Z, M, DATA)                                                   \
        detail::as_lvalue(                                                                          \
            typename when<_, HBOOST_PP_CAT(A, M)>::template impl<Expr, State, Data>()(e, s, d)       \
        )                                                                                           \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/make.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file make.hpp
    /// Contains definition of the make<> transform.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (0, HBOOST_PROTO_MAX_ARITY, <hboost/proto/transform/detail/make.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_CONSTRUCT_ARG
    #undef HBOOST_PROTO_MAKE_IF_APPLIED
    #undef HBOOST_PROTO_MAKE_IF_TYPE
    #undef HBOOST_PROTO_MAKE_IF

#else

    #define N HBOOST_PP_ITERATION()

    namespace detail
    {
        #if N > 0

        template<
            template<HBOOST_PP_ENUM_PARAMS(N, typename HBOOST_PP_INTERCEPT)> class R
            HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)
          , typename Expr, typename State, typename Data
        >
        struct make_<
            R<HBOOST_PP_ENUM_PARAMS(N, A)>
          , Expr, State, Data
            HBOOST_PROTO_TEMPLATE_ARITY_PARAM(N)
        >
          : nested_type_if<
                R<HBOOST_PP_ENUM(N, HBOOST_PROTO_MAKE_IF_TYPE, ~)>
              , (HBOOST_PP_REPEAT(N, HBOOST_PROTO_MAKE_IF_APPLIED, ~) false)
            >
        {};

        template<
            template<HBOOST_PP_ENUM_PARAMS(N, typename HBOOST_PP_INTERCEPT)> class R
            HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)
          , typename Expr, typename State, typename Data
        >
        struct make_<
            noinvoke<R<HBOOST_PP_ENUM_PARAMS(N, A)> >
          , Expr, State, Data
            HBOOST_PROTO_TEMPLATE_ARITY_PARAM(1)
        >
        {
            typedef R<HBOOST_PP_ENUM(N, HBOOST_PROTO_MAKE_IF_TYPE, ~)> type;
            static bool const applied = true;
        };

        #endif

        template<typename R HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct is_applyable<R(HBOOST_PP_ENUM_PARAMS(N, A))>
          : mpl::true_
        {};

        template<typename R HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct is_applyable<R(*)(HBOOST_PP_ENUM_PARAMS(N, A))>
          : mpl::true_
        {};

        template<typename T, typename A>
        struct construct_<proto::expr<T, A, N>, true>
        {
            typedef proto::expr<T, A, N> result_type;

            template<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_MAX(N, 1), typename A)>
            HBOOST_FORCEINLINE
            result_type operator ()(HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PP_MAX(N, 1), A, &a)) const
            {
                return result_type::make(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_MAX(N, 1), a));
            }
        };

        template<typename T, typename A>
        struct construct_<proto::basic_expr<T, A, N>, true>
        {
            typedef proto::basic_expr<T, A, N> result_type;

            template<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_MAX(N, 1), typename A)>
            HBOOST_FORCEINLINE
            result_type operator ()(HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PP_MAX(N, 1), A, &a)) const
            {
                return result_type::make(HBOOST_PP_ENUM_PARAMS(HBOOST_PP_MAX(N, 1), a));
            }
        };

        template<typename Type HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        HBOOST_FORCEINLINE
        Type construct(HBOOST_PP_ENUM_BINARY_PARAMS(N, A, &a))
        {
            return construct_<Type>()(HBOOST_PP_ENUM_PARAMS(N, a));
        }

    } // namespace detail

    /// \brief A PrimitiveTransform which computes a type by evaluating any
    /// nested transforms and then constructs an object of that type with the
    /// current expression, state and data, transformed according
    /// to \c A0 through \c AN.
    template<typename Object HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct make<Object(HBOOST_PP_ENUM_PARAMS(N, A))>
      : transform<make<Object(HBOOST_PP_ENUM_PARAMS(N, A))> >
    {
        template<typename Expr, typename State, typename Data>
        struct impl : transform_impl<Expr, State, Data>
        {
            /// \brief <tt>hboost::result_of\<make\<Object\>(Expr, State, Data)\>::type</tt>
            typedef typename detail::make_if_<Object, Expr, State, Data>::type result_type;

            /// Let \c ax be <tt>when\<_, Ax\>()(e, s, d)</tt>
            /// for each \c x in <tt>[0,N]</tt>.
            /// Return <tt>result_type(a0, a1,... aN)</tt>.
            ///
            /// \param e The current expression
            /// \param s The current state
            /// \param d An arbitrary data
            HBOOST_FORCEINLINE
            result_type operator ()(
                typename impl::expr_param   e
              , typename impl::state_param  s
              , typename impl::data_param   d
            ) const
            {
                proto::detail::ignore_unused(e);
                proto::detail::ignore_unused(s);
                proto::detail::ignore_unused(d);
                return detail::construct<result_type>(HBOOST_PP_ENUM(N, HBOOST_PROTO_CONSTRUCT_ARG, DATA));
            }
        };
    };

    #if N > 0
    /// \brief A PrimitiveTransform which computes a type by evaluating any
    /// nested transforms and then constructs an object of that type with the
    /// current expression, state and data, transformed according
    /// to \c A0 through \c AN.
    template<typename Object HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct make<Object(HBOOST_PP_ENUM_PARAMS(N, A)...)>
      : transform<make<Object(HBOOST_PP_ENUM_PARAMS(N, A)...)> >
    {
        template<typename Expr, typename State, typename Data>
        struct impl
          : make<
                typename detail::expand_pattern<
                    proto::arity_of<Expr>::value
                  , HBOOST_PP_CAT(A, HBOOST_PP_DEC(N))
                  , detail::HBOOST_PP_CAT(expand_pattern_rest_, HBOOST_PP_DEC(N))<
                        Object
                        HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_DEC(N), A)
                    >
                >::type
            >::template impl<Expr, State, Data>
        {};
    };
    #endif
    #undef N

#endif
