#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/basic_expr.hpp>

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

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/basic_expr.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file basic_expr.hpp
    /// Contains definition of basic_expr\<\> class template.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    // The expr<> specializations are actually defined here.
    #define HBOOST_PROTO_DEFINE_TERMINAL
    #define HBOOST_PP_ITERATION_PARAMS_1 (3, (0, 0, <hboost/proto/detail/basic_expr.hpp>))
    #include HBOOST_PP_ITERATE()

    #undef HBOOST_PROTO_DEFINE_TERMINAL
    #define HBOOST_PP_ITERATION_PARAMS_1 (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/basic_expr.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_CHILD
    #undef HBOOST_PROTO_VOID

#else

    #define ARG_COUNT HBOOST_PP_MAX(1, HBOOST_PP_ITERATION())

    /// \brief Simplified representation of a node in an expression tree.
    ///
    /// \c proto::basic_expr\<\> is a node in an expression template tree. It
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
    /// \c proto::basic_expr\<\> is a valid Fusion random-access sequence, where
    /// the elements of the sequence are the child expressions.
    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
    template<typename Tag, typename Arg0>
    struct basic_expr<Tag, term<Arg0>, 0>
    #else
    template<typename Tag HBOOST_PP_ENUM_TRAILING_PARAMS(ARG_COUNT, typename Arg)>
    struct basic_expr<Tag, HBOOST_PP_CAT(list, HBOOST_PP_ITERATION())<HBOOST_PP_ENUM_PARAMS(ARG_COUNT, Arg)>, HBOOST_PP_ITERATION() >
    #endif
    {
        typedef Tag proto_tag;
        static const long proto_arity_c = HBOOST_PP_ITERATION();
        typedef mpl::long_<HBOOST_PP_ITERATION() > proto_arity;
        typedef basic_expr proto_base_expr;
        #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        typedef term<Arg0> proto_args;
        #else
        typedef HBOOST_PP_CAT(list, HBOOST_PP_ITERATION())<HBOOST_PP_ENUM_PARAMS(ARG_COUNT, Arg)> proto_args;
        #endif
        typedef basic_expr proto_grammar;
        typedef basic_default_domain proto_domain;
        typedef default_generator proto_generator;
        typedef proto::tag::proto_expr<Tag, proto_domain> fusion_tag;
        typedef basic_expr proto_derived_expr;
        typedef void proto_is_expr_; /**< INTERNAL ONLY */

        HBOOST_PP_REPEAT(ARG_COUNT, HBOOST_PROTO_CHILD, ~)
        HBOOST_PP_REPEAT_FROM_TO(ARG_COUNT, HBOOST_PROTO_MAX_ARITY, HBOOST_PROTO_VOID, ~)

        /// \return *this
        ///
        HBOOST_FORCEINLINE
        basic_expr const &proto_base() const
        {
            return *this;
        }

        /// \overload
        ///
        HBOOST_FORCEINLINE
        basic_expr &proto_base()
        {
            return *this;
        }

    #ifdef HBOOST_PROTO_DEFINE_TERMINAL
        /// \return A new \c expr\<\> object initialized with the specified
        /// arguments.
        ///
        template<typename A0>
        HBOOST_FORCEINLINE
        static basic_expr const make(A0 &a0)
        {
            return detail::make_terminal(a0, static_cast<basic_expr *>(0), static_cast<proto_args *>(0));
        }

        /// \overload
        ///
        template<typename A0>
        HBOOST_FORCEINLINE
        static basic_expr const make(A0 const &a0)
        {
            return detail::make_terminal(a0, static_cast<basic_expr *>(0), static_cast<proto_args *>(0));
        }
    #else
        /// \return A new \c expr\<\> object initialized with the specified
        /// arguments.
        ///
        template<HBOOST_PP_ENUM_PARAMS(ARG_COUNT, typename A)>
        HBOOST_FORCEINLINE
        static basic_expr const make(HBOOST_PP_ENUM_BINARY_PARAMS(ARG_COUNT, A, const &a))
        {
            basic_expr that = {HBOOST_PP_ENUM_PARAMS(ARG_COUNT, a)};
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
    };

#undef ARG_COUNT

#endif
