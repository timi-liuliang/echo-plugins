#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #ifndef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
        #include <hboost/proto/detail/preprocessed/expr_variadic.hpp>
    #else
        #include <hboost/proto/detail/preprocessed/expr.hpp>
    #endif

#elif !defined(HBOOST_PP_IS_ITERATING)

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_CHILD(Z, N, DATA)                                                       \
        typedef HBOOST_PP_CAT(Arg, N) HBOOST_PP_CAT(proto_child, N);                              \
        HBOOST_PP_CAT(proto_child, N) HBOOST_PP_CAT(child, N);                                    \
        /**< INTERNAL ONLY */

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_VOID(Z, N, DATA)                                                        \
        typedef void HBOOST_PP_CAT(proto_child, N);                                              \
        /**< INTERNAL ONLY */

    // Generate variadic versions of expr
    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/expr_variadic.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file expr_variadic.hpp
    /// Contains definition of expr\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    // The expr<> specializations are actually defined here.
    #define HBOOST_PROTO_DEFINE_TERMINAL
    #define HBOOST_PP_ITERATION_PARAMS_1                                                         \
        (3, (0, 0, <hboost/proto/detail/expr.hpp>))
    #include HBOOST_PP_ITERATE()

    #undef HBOOST_PROTO_DEFINE_TERMINAL
    #define HBOOST_PP_ITERATION_PARAMS_1                                                         \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/expr.hpp>))
    #include HBOOST_PP_ITERATE()

    // Generate non-variadic versions of expr
    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #define HBOOST_NO_CXX11_VARIADIC_TEMPLATES
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/expr.hpp")

        ///////////////////////////////////////////////////////////////////////////////
        /// \file expr.hpp
        /// Contains definition of expr\<\> class template.
        //
        //  Copyright 2008 Eric Niebler. Distributed under the Boost
        //  Software License, Version 1.0. (See accompanying file
        //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

        #pragma wave option(preserve: 1)

        // The expr<> specializations are actually defined here.
        #define HBOOST_PROTO_DEFINE_TERMINAL
        #define HBOOST_PP_ITERATION_PARAMS_1 (3, (0, 0, <hboost/proto/detail/expr.hpp>))
        #include HBOOST_PP_ITERATE()

        #undef HBOOST_PROTO_DEFINE_TERMINAL
        #define HBOOST_PP_ITERATION_PARAMS_1 (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/expr.hpp>))
        #include HBOOST_PP_ITERATE()

        #pragma wave option(output: null)
        #undef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
    #endif

    #undef HBOOST_PROTO_CHILD
    #undef HBOOST_PROTO_VOID

#else

    #define ARG_COUNT HBOOST_PP_MAX(1, HBOOST_PP_ITERATION())

    /// \brief Representation of a node in an expression tree.
    ///
    /// \c proto::expr\<\> is a node in an expression template tree. It
    /// is a container for its child sub-trees. It also serves as
    /// the terminal nodes of the tree.
    ///
    /// \c Tag is type that represents the operation encoded by
    ///             this expression. It is typically one of the structs
    ///             in the \c hboost::proto::tag namespace, but it doesn't
    ///             have to be.
    ///
    /// \c Args is a type list representing the type of the children
    ///             of this expression. It is an instantiation of one
    ///             of \c proto::list1\<\>, \c proto::list2\<\>, etc. The
    ///             child types must all themselves be either \c expr\<\>
    ///             or <tt>proto::expr\<\>&</tt>. If \c Args is an
    ///             instantiation of \c proto::term\<\> then this
    ///             \c expr\<\> type represents a terminal expression;
    ///             the parameter to the \c proto::term\<\> template
    ///             represents the terminal's value type.
    ///
    /// \c Arity is an integral constant representing the number of child
    ///             nodes this node contains. If \c Arity is 0, then this
    ///             node is a terminal.
    ///
    /// \c proto::expr\<\> is a valid Fusion random-access sequence, where
    /// the elements of the sequence are the child expressions.
    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
    template<typename Tag, typename Arg0>
    struct expr<Tag, term<Arg0>, 0>
    #else
    template<typename Tag HBOOST_PP_ENUM_TRAILING_PARAMS(ARG_COUNT, typename Arg)>
    struct expr<Tag, HBOOST_PP_CAT(list, HBOOST_PP_ITERATION())<HBOOST_PP_ENUM_PARAMS(ARG_COUNT, Arg)>, HBOOST_PP_ITERATION() >
    #endif
    {
        typedef Tag proto_tag;
        static const long proto_arity_c = HBOOST_PP_ITERATION();
        typedef mpl::long_<HBOOST_PP_ITERATION() > proto_arity;
        typedef expr proto_base_expr;
        #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        typedef term<Arg0> proto_args;
        #else
        typedef HBOOST_PP_CAT(list, HBOOST_PP_ITERATION())<HBOOST_PP_ENUM_PARAMS(ARG_COUNT, Arg)> proto_args;
        #endif
        typedef basic_expr<Tag, proto_args, HBOOST_PP_ITERATION() > proto_grammar;
        typedef default_domain proto_domain;
        typedef default_generator proto_generator;
        typedef proto::tag::proto_expr<Tag, proto_domain> fusion_tag;
        typedef expr proto_derived_expr;
        typedef void proto_is_expr_; /**< INTERNAL ONLY */

        HBOOST_PP_REPEAT(ARG_COUNT, HBOOST_PROTO_CHILD, ~)
        HBOOST_PP_REPEAT_FROM_TO(ARG_COUNT, HBOOST_PROTO_MAX_ARITY, HBOOST_PROTO_VOID, ~)

        /// \return *this
        ///
        HBOOST_FORCEINLINE
        expr const &proto_base() const
        {
            return *this;
        }

        /// \overload
        ///
        HBOOST_FORCEINLINE
        expr &proto_base()
        {
            return *this;
        }

    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        /// \return A new \c expr\<\> object initialized with the specified
        /// arguments.
        ///
        template<typename A0>
        HBOOST_FORCEINLINE
        static expr const make(A0 &a0)
        {
            return detail::make_terminal(a0, static_cast<expr *>(0), static_cast<proto_args *>(0));
        }

        /// \overload
        ///
        template<typename A0>
        HBOOST_FORCEINLINE
        static expr const make(A0 const &a0)
        {
            return detail::make_terminal(a0, static_cast<expr *>(0), static_cast<proto_args *>(0));
        }
    #else
        /// \return A new \c expr\<\> object initialized with the specified
        /// arguments.
        ///
        template<HBOOST_PP_ENUM_PARAMS(ARG_COUNT, typename A)>
        HBOOST_FORCEINLINE
        static expr const make(HBOOST_PP_ENUM_BINARY_PARAMS(ARG_COUNT, A, const &a))
        {
            expr that = {HBOOST_PP_ENUM_PARAMS(ARG_COUNT, a)};
            return that;
        }
    #endif

    #if 1 == HBOOST_PP_ITERATION()
        /// If \c Tag is \c hboost::proto::tag::address_of and \c proto_child0 is
        /// <tt>T&</tt>, then \c address_of_hack_type_ is <tt>T*</tt>.
        /// Otherwise, it is some undefined type.
        typedef typename detail::address_of_hack<Tag, proto_child0>::type address_of_hack_type_;

        /// \return The address of <tt>this->child0</tt> if \c Tag is
        /// \c hboost::proto::tag::address_of. Otherwise, this function will
        /// fail to compile.
        ///
        /// \attention Proto overloads <tt>operator&</tt>, which means that
        /// proto-ified objects cannot have their addresses taken, unless we use
        /// the following hack to make \c &x implicitly convertible to \c X*.
        HBOOST_FORCEINLINE
        operator address_of_hack_type_() const
        {
            return hboost::addressof(this->child0);
        }
    #else
        /// INTERNAL ONLY
        ///
        typedef detail::not_a_valid_type address_of_hack_type_;
    #endif

        /// Assignment
        ///
        /// \param a The rhs.
        /// \return A new \c expr\<\> node representing an assignment of \c that to \c *this.
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::assign
          , list2<expr &, expr const &>
          , 2
        > const
        operator =(expr const &a)
        {
            proto::expr<
                proto::tag::assign
              , list2<expr &, expr const &>
              , 2
            > that = {*this, a};
            return that;
        }

        /// Assignment
        ///
        /// \param a The rhs.
        /// \return A new \c expr\<\> node representing an assignment of \c a to \c *this.
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::assign
          , list2<expr const &, typename result_of::as_child<A>::type>
          , 2
        > const
        operator =(A &a) const
        {
            proto::expr<
                proto::tag::assign
              , list2<expr const &, typename result_of::as_child<A>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }

        /// \overload
        ///
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::assign
          , list2<expr const &, typename result_of::as_child<A const>::type>
          , 2
        > const
        operator =(A const &a) const
        {
            proto::expr<
                proto::tag::assign
              , list2<expr const &, typename result_of::as_child<A const>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }

    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        /// \overload
        ///
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::assign
          , list2<expr &, typename result_of::as_child<A>::type>
          , 2
        > const
        operator =(A &a)
        {
            proto::expr<
                proto::tag::assign
              , list2<expr &, typename result_of::as_child<A>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }

        /// \overload
        ///
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::assign
          , list2<expr &, typename result_of::as_child<A const>::type>
          , 2
        > const
        operator =(A const &a)
        {
            proto::expr<
                proto::tag::assign
              , list2<expr &, typename result_of::as_child<A const>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }
    #endif

        /// Subscript
        ///
        /// \param a The rhs.
        /// \return A new \c expr\<\> node representing \c *this subscripted with \c a.
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::subscript
          , list2<expr const &, typename result_of::as_child<A>::type>
          , 2
        > const
        operator [](A &a) const
        {
            proto::expr<
                proto::tag::subscript
              , list2<expr const &, typename result_of::as_child<A>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }

        /// \overload
        ///
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::subscript
          , list2<expr const &, typename result_of::as_child<A const>::type>
          , 2
        > const
        operator [](A const &a) const
        {
            proto::expr<
                proto::tag::subscript
              , list2<expr const &, typename result_of::as_child<A const>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }

    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        /// \overload
        ///
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::subscript
          , list2<expr &, typename result_of::as_child<A>::type>
          , 2
        > const
        operator [](A &a)
        {
            proto::expr<
                proto::tag::subscript
              , list2<expr &, typename result_of::as_child<A>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }

        /// \overload
        ///
        template<typename A>
        HBOOST_FORCEINLINE
        proto::expr<
            proto::tag::subscript
          , list2<expr &, typename result_of::as_child<A const>::type>
          , 2
        > const
        operator [](A const &a)
        {
            proto::expr<
                proto::tag::subscript
              , list2<expr &, typename result_of::as_child<A const>::type>
              , 2
            > that = {*this, proto::as_child(a)};
            return that;
        }
    #endif

        /// Encodes the return type of \c expr\<\>::operator(), for use with \c hboost::result_of\<\>
        ///
        template<typename Sig>
        struct result
        {
            typedef typename result_of::funop<Sig, expr, default_domain>::type const type;
        };

    #ifndef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
        /// \overload
        ///
        template<typename ...A>
        HBOOST_FORCEINLINE
        typename result_of::funop<
            expr const(A const &...)
          , expr
          , default_domain
        >::type const
        operator ()(A const &... a) const
        {
            return result_of::funop<
                expr const(A const &...)
              , expr
              , default_domain
            >::call(*this, a...);
        }

    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        /// \overload
        ///
        template<typename ...A>
        HBOOST_FORCEINLINE
        typename result_of::funop<
            expr(A const &...)
          , expr
          , default_domain
        >::type const
        operator ()(A const &... a)
        {
            return result_of::funop<
                expr(A const &...)
              , expr
              , default_domain
            >::call(*this, a...);
        }
    #endif

    #else // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

        /// Function call
        ///
        /// \return A new \c expr\<\> node representing the function invocation of \c (*this)().
        HBOOST_FORCEINLINE
        proto::expr<proto::tag::function, list1<expr const &>, 1> const
        operator ()() const
        {
            proto::expr<proto::tag::function, list1<expr const &>, 1> that = {*this};
            return that;
        }

    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        /// \overload
        ///
        HBOOST_FORCEINLINE
        proto::expr<proto::tag::function, list1<expr &>, 1> const
        operator ()()
        {
            proto::expr<proto::tag::function, list1<expr &>, 1> that = {*this};
            return that;
        }
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_2                                                             \
        (3, (1, HBOOST_PP_DEC(HBOOST_PROTO_MAX_FUNCTION_CALL_ARITY), <hboost/proto/detail/expr_funop.hpp>))
    #include HBOOST_PP_ITERATE()

#endif
    };

#undef ARG_COUNT

#endif
