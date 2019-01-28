#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/args.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_DEFINE_CHILD_N(Z, N, DATA)                                              \
        typedef HBOOST_PP_CAT(Arg, N) HBOOST_PP_CAT(child, N);                                    \
        /**< INTERNAL ONLY */

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_DEFINE_VOID_N(z, n, data)                                               \
        typedef mpl::void_ HBOOST_PP_CAT(child, n);                                              \
        /**< INTERNAL ONLY */

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/args.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// \file args.hpp
    /// Contains definition of \c term\<\>, \c list1\<\>, \c list2\<\>, ...
    /// class templates.
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    /// \brief A type sequence, for use as the 2nd parameter to the \c expr\<\> class template.
    ///
    /// A type sequence, for use as the 2nd parameter to the \c expr\<\> class template.
    /// The types in the sequence correspond to the children of a node in an expression tree.
    template< typename Arg0 >
    struct term
    {
        static const long arity = 0;
        typedef Arg0 child0;
        HBOOST_PP_REPEAT_FROM_TO(1, HBOOST_PROTO_MAX_ARITY, HBOOST_PROTO_DEFINE_VOID_N, ~)

        /// INTERNAL ONLY
        ///
        typedef Arg0 back_;
    };

    #define HBOOST_PP_ITERATION_PARAMS_1                                                         \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/args.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

    #undef HBOOST_PROTO_DEFINE_VOID_N
    #undef HBOOST_PROTO_DEFINE_CHILD_N

#else

    #define N HBOOST_PP_ITERATION()

    /// \brief A type sequence, for use as the 2nd parameter to the \c expr\<\> class template.
    ///
    /// A type sequence, for use as the 2nd parameter to the \c expr\<\> class template.
    /// The types in the sequence correspond to the children of a node in an expression tree.
    template< HBOOST_PP_ENUM_PARAMS(N, typename Arg) >
    struct HBOOST_PP_CAT(list, N)
    {
        static const long arity = N;
        HBOOST_PP_REPEAT(N, HBOOST_PROTO_DEFINE_CHILD_N, ~)
        HBOOST_PP_REPEAT_FROM_TO(N, HBOOST_PROTO_MAX_ARITY, HBOOST_PROTO_DEFINE_VOID_N, ~)

        /// INTERNAL ONLY
        ///
        typedef HBOOST_PP_CAT(Arg, HBOOST_PP_DEC(N)) back_;
    };

    #undef N

#endif
