#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/traits.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #define HBOOST_PROTO_CHILD(Z, N, DATA)                                                           \
        /** INTERNAL ONLY */                                                                        \
        typedef HBOOST_PP_CAT(DATA, N) HBOOST_PP_CAT(proto_child, N);                                 \
        /**/

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/traits.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file traits.hpp
    /// Definitions of proto::function, proto::nary_expr and proto::result_of::child_c
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (0, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/traits.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_CHILD

#else // HBOOST_PP_IS_ITERATING

    #define N HBOOST_PP_ITERATION()

    #if N > 0
        /// \brief A metafunction for generating function-call expression types,
        /// a grammar element for matching function-call expressions, and a
        /// PrimitiveTransform that dispatches to the <tt>pass_through\<\></tt>
        /// transform.
        template<HBOOST_PP_ENUM_PARAMS(N, typename A)>
        struct function
        #if N != HBOOST_PROTO_MAX_ARITY
        <
            HBOOST_PP_ENUM_PARAMS(N, A)
            HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_SUB(HBOOST_PROTO_MAX_ARITY, N), void HBOOST_PP_INTERCEPT)
        >
        #endif
          : proto::transform<
                function<
                    HBOOST_PP_ENUM_PARAMS(N, A)
                    HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_SUB(HBOOST_PROTO_MAX_ARITY, N), void HBOOST_PP_INTERCEPT)
                >
              , int
            >
        {
            typedef proto::expr<proto::tag::function, HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, A)>, N> type;
            typedef proto::basic_expr<proto::tag::function, HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, A)>, N> proto_grammar;

            template<typename Expr, typename State, typename Data>
            struct impl
              : detail::pass_through_impl<function, deduce_domain, Expr, State, Data>
            {};

            /// INTERNAL ONLY
            typedef proto::tag::function proto_tag;
            HBOOST_PP_REPEAT(N, HBOOST_PROTO_CHILD, A)
            HBOOST_PP_REPEAT_FROM_TO(
                N
              , HBOOST_PROTO_MAX_ARITY
              , HBOOST_PROTO_CHILD
              , detail::if_vararg<HBOOST_PP_CAT(A, HBOOST_PP_DEC(N))> HBOOST_PP_INTERCEPT
            )
        };

        /// \brief A metafunction for generating n-ary expression types with a
        /// specified tag type,
        /// a grammar element for matching n-ary expressions, and a
        /// PrimitiveTransform that dispatches to the <tt>pass_through\<\></tt>
        /// transform.
        ///
        /// Use <tt>nary_expr\<_, vararg\<_\> \></tt> as a grammar element to match any
        /// n-ary expression; that is, any non-terminal.
        template<typename Tag HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct nary_expr
        #if N != HBOOST_PROTO_MAX_ARITY
        <
            Tag
            HBOOST_PP_ENUM_TRAILING_PARAMS(N, A)
            HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_SUB(HBOOST_PROTO_MAX_ARITY, N), void HBOOST_PP_INTERCEPT)
        >
        #endif
          : proto::transform<
                nary_expr<
                    Tag
                    HBOOST_PP_ENUM_TRAILING_PARAMS(N, A)
                    HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_PP_SUB(HBOOST_PROTO_MAX_ARITY, N), void HBOOST_PP_INTERCEPT)
                >
              , int
            >
        {
            typedef proto::expr<Tag, HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, A)>, N> type;
            typedef proto::basic_expr<Tag, HBOOST_PP_CAT(list, N)<HBOOST_PP_ENUM_PARAMS(N, A)>, N> proto_grammar;

            template<typename Expr, typename State, typename Data>
            struct impl
              : detail::pass_through_impl<nary_expr, deduce_domain, Expr, State, Data>
            {};

            /// INTERNAL ONLY
            typedef Tag proto_tag;
            HBOOST_PP_REPEAT(N, HBOOST_PROTO_CHILD, A)
            HBOOST_PP_REPEAT_FROM_TO(
                N
              , HBOOST_PROTO_MAX_ARITY
              , HBOOST_PROTO_CHILD
              , detail::if_vararg<HBOOST_PP_CAT(A, HBOOST_PP_DEC(N))> HBOOST_PP_INTERCEPT
            )
        };

        namespace detail
        {
            template<
                template<HBOOST_PP_ENUM_PARAMS(N, typename HBOOST_PP_INTERCEPT)> class T
              , HBOOST_PP_ENUM_PARAMS(N, typename A)
            >
            struct is_callable_<T<HBOOST_PP_ENUM_PARAMS(N, A)> HBOOST_PROTO_TEMPLATE_ARITY_PARAM(N)>
              : is_same<HBOOST_PP_CAT(A, HBOOST_PP_DEC(N)), callable>
            {};
        }

    #endif

        namespace result_of
        {
            /// \brief A metafunction that returns the type of the Nth child
            /// of a Proto expression.
            ///
            /// A metafunction that returns the type of the Nth child
            /// of a Proto expression. \c N must be less than
            /// \c Expr::proto_arity::value.
            template<typename Expr>
            struct child_c<Expr, N>
            {
                /// Verify that we are not operating on a terminal
                HBOOST_STATIC_ASSERT(0 != Expr::proto_arity_c);

                /// The raw type of the Nth child as it is stored within
                /// \c Expr. This may be a value or a reference
                typedef typename Expr::HBOOST_PP_CAT(proto_child, N) value_type;

                /// The "value" type of the child, suitable for return by value,
                /// computed as follows:
                /// \li <tt>T const &</tt> becomes <tt>T</tt>
                /// \li <tt>T &</tt> becomes <tt>T</tt>
                /// \li <tt>T</tt> becomes <tt>T</tt>
                typedef typename detail::expr_traits<typename Expr::HBOOST_PP_CAT(proto_child, N)>::value_type type;
            };

            template<typename Expr>
            struct child_c<Expr &, N>
            {
                /// Verify that we are not operating on a terminal
                HBOOST_STATIC_ASSERT(0 != Expr::proto_arity_c);

                /// The raw type of the Nth child as it is stored within
                /// \c Expr. This may be a value or a reference
                typedef typename Expr::HBOOST_PP_CAT(proto_child, N) value_type;

                /// The "reference" type of the child, suitable for return by
                /// reference, computed as follows:
                /// \li <tt>T const &</tt> becomes <tt>T const &</tt>
                /// \li <tt>T &</tt> becomes <tt>T &</tt>
                /// \li <tt>T</tt> becomes <tt>T &</tt>
                typedef typename detail::expr_traits<typename Expr::HBOOST_PP_CAT(proto_child, N)>::reference type;

                /// INTERNAL ONLY
                ///
                HBOOST_FORCEINLINE
                static type call(Expr &e)
                {
                    return e.proto_base().HBOOST_PP_CAT(child, N);
                }
            };

            template<typename Expr>
            struct child_c<Expr const &, N>
            {
                /// Verify that we are not operating on a terminal
                HBOOST_STATIC_ASSERT(0 != Expr::proto_arity_c);

                /// The raw type of the Nth child as it is stored within
                /// \c Expr. This may be a value or a reference
                typedef typename Expr::HBOOST_PP_CAT(proto_child, N) value_type;

                /// The "const reference" type of the child, suitable for return by
                /// const reference, computed as follows:
                /// \li <tt>T const &</tt> becomes <tt>T const &</tt>
                /// \li <tt>T &</tt> becomes <tt>T &</tt>
                /// \li <tt>T</tt> becomes <tt>T const &</tt>
                typedef typename detail::expr_traits<typename Expr::HBOOST_PP_CAT(proto_child, N)>::const_reference type;

                /// INTERNAL ONLY
                ///
                HBOOST_FORCEINLINE
                static type call(Expr const &e)
                {
                    return e.proto_base().HBOOST_PP_CAT(child, N);
                }
            };
        }

    #undef N

#endif
