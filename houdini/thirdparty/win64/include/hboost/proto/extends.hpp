///////////////////////////////////////////////////////////////////////////////
/// \file extends.hpp
/// Macros and a base class for defining end-user expression types
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_EXTENDS_HPP_EAN_11_1_2006
#define HBOOST_PROTO_EXTENDS_HPP_EAN_11_1_2006

#include <cstddef> // for offsetof
#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/preprocessor/facilities/empty.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/arithmetic/inc.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/iteration/local.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/utility/addressof.hpp>
#include <hboost/utility/result_of.hpp>
#include <hboost/proto/proto_fwd.hpp>
#include <hboost/proto/traits.hpp>
#include <hboost/proto/expr.hpp>
#include <hboost/proto/args.hpp>
#include <hboost/proto/traits.hpp>
#include <hboost/proto/generate.hpp>
#include <hboost/proto/detail/remove_typename.hpp>

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable : 4714) // function 'xxx' marked as __forceinline not inlined
#endif

namespace hboost { namespace proto
{
    #ifdef __GNUC__
    /// INTERNAL ONLY
    ///
    # define HBOOST_PROTO_ADDROF(x) ((char const volatile*)hboost::addressof(x))
    /// INTERNAL ONLY
    ///
    # define HBOOST_PROTO_OFFSETOF(s,m) (HBOOST_PROTO_ADDROF((((s *)this)->m)) - HBOOST_PROTO_ADDROF(*((s *)this)))
    #else
    /// INTERNAL ONLY
    ///
    # define HBOOST_PROTO_OFFSETOF offsetof
    #endif

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_CONST() const

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_TYPENAME() typename

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_TEMPLATE_YES_(Z, N) template<HBOOST_PP_ENUM_PARAMS_Z(Z, N, typename A)>

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_TEMPLATE_NO_(Z, N)

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_DEFINE_FUN_OP_IMPL_(Z, N, DATA, Const)                                      \
        HBOOST_PP_IF(N, HBOOST_PROTO_TEMPLATE_YES_, HBOOST_PROTO_TEMPLATE_NO_)(Z, N)                   \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        typename HBOOST_PROTO_RESULT_OF<                                                             \
            proto_generator(                                                                        \
                typename hboost::proto::result_of::HBOOST_PP_CAT(funop, N)<                           \
                    proto_derived_expr Const()                                                      \
                  , proto_domain                                                                    \
                    HBOOST_PP_ENUM_TRAILING_PARAMS_Z(Z, N, const A)                                  \
                >::type                                                                             \
            )                                                                                       \
        >::type const                                                                               \
        operator ()(HBOOST_PP_ENUM_BINARY_PARAMS_Z(Z, N, A, const &a)) Const()                       \
        {                                                                                           \
            typedef hboost::proto::result_of::HBOOST_PP_CAT(funop, N)<                                \
                proto_derived_expr Const()                                                          \
              , proto_domain                                                                        \
                HBOOST_PP_ENUM_TRAILING_PARAMS_Z(Z, N, const A)                                      \
            > funop;                                                                                \
            return proto_generator()(                                                               \
                funop::call(                                                                        \
                    *static_cast<proto_derived_expr Const() *>(this)                                \
                    HBOOST_PP_ENUM_TRAILING_PARAMS_Z(Z, N, a)                                        \
                )                                                                                   \
            );                                                                                      \
        }                                                                                           \
        /**/

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_DEFINE_FUN_OP_VARIADIC_IMPL_(Const)                                         \
        template<typename... A>                                                                     \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        typename HBOOST_PROTO_RESULT_OF<                                                             \
            proto_generator(                                                                        \
                typename hboost::proto::result_of::funop<                                            \
                    proto_derived_expr Const()(A const &...)                                        \
                  , proto_derived_expr                                                              \
                  , proto_domain                                                                    \
                >::type                                                                             \
            )                                                                                       \
        >::type const                                                                               \
        operator ()(A const &...a) Const()                                                          \
        {                                                                                           \
            typedef hboost::proto::result_of::funop<                                                 \
                proto_derived_expr Const()(A const &...)                                            \
              , proto_derived_expr                                                                  \
              , proto_domain                                                                        \
            > funop;                                                                                \
            return proto_generator()(                                                               \
                funop::call(                                                                        \
                    *static_cast<proto_derived_expr Const() *>(this)                                \
                  , a...                                                                            \
                )                                                                                   \
            );                                                                                      \
        }                                                                                           \
        /**/

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_DEFINE_FUN_OP_CONST(Z, N, DATA)                                             \
        HBOOST_PROTO_DEFINE_FUN_OP_IMPL_(Z, N, DATA, HBOOST_PROTO_CONST)                              \
        /**/

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_DEFINE_FUN_OP_NON_CONST(Z, N, DATA)                                         \
        HBOOST_PROTO_DEFINE_FUN_OP_IMPL_(Z, N, DATA, HBOOST_PP_EMPTY)                                 \
        /**/

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_DEFINE_FUN_OP(Z, N, DATA)                                                   \
        HBOOST_PROTO_DEFINE_FUN_OP_CONST(Z, N, DATA)                                                 \
        HBOOST_PROTO_DEFINE_FUN_OP_NON_CONST(Z, N, DATA)                                             \
        /**/

    /// INTERNAL ONLY
    ///
    #define HBOOST_PROTO_EXTENDS_CHILD(Z, N, DATA)                                                   \
        typedef                                                                                     \
            typename proto_base_expr::HBOOST_PP_CAT(proto_child, N)                                  \
        HBOOST_PP_CAT(proto_child, N);                                                               \
        /**/

    #define HBOOST_PROTO_BASIC_EXTENDS_(Expr, Derived, Domain)                                       \
        Expr proto_expr_;                                                                           \
                                                                                                    \
        typedef Expr proto_base_expr_; /**< INTERNAL ONLY */                                        \
        typedef typename proto_base_expr_::proto_base_expr proto_base_expr;                         \
        typedef HBOOST_PROTO_REMOVE_TYPENAME(Domain) proto_domain;                                   \
        typedef Derived proto_derived_expr;                                                         \
        typedef Domain::proto_generator proto_generator;                                            \
        typedef typename proto_base_expr::proto_tag proto_tag;                                      \
        typedef typename proto_base_expr::proto_args proto_args;                                    \
        typedef typename proto_base_expr::proto_arity proto_arity;                                  \
        typedef typename proto_base_expr::proto_grammar proto_grammar;                              \
        typedef typename proto_base_expr::address_of_hack_type_ proto_address_of_hack_type_;        \
        typedef void proto_is_expr_; /**< INTERNAL ONLY */                                          \
        static const long proto_arity_c = proto_base_expr::proto_arity_c;                           \
        typedef hboost::proto::tag::proto_expr<proto_tag, proto_domain> fusion_tag;                  \
        HBOOST_PP_REPEAT(HBOOST_PROTO_MAX_ARITY, HBOOST_PROTO_EXTENDS_CHILD, ~)                        \
                                                                                                    \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        static proto_derived_expr const make(Expr const &e)                                         \
        {                                                                                           \
            proto_derived_expr that = {e};                                                          \
            return that;                                                                            \
        }                                                                                           \
                                                                                                    \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        proto_base_expr &proto_base()                                                               \
        {                                                                                           \
            return this->proto_expr_.proto_base();                                                  \
        }                                                                                           \
                                                                                                    \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        proto_base_expr const &proto_base() const                                                   \
        {                                                                                           \
            return this->proto_expr_.proto_base();                                                  \
        }                                                                                           \
                                                                                                    \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        operator proto_address_of_hack_type_() const                                                \
        {                                                                                           \
            return hboost::addressof(this->proto_base().child0);                                     \
        }                                                                                           \
        /**/

    #define HBOOST_PROTO_BASIC_EXTENDS(Expr, Derived, Domain)                                        \
        HBOOST_PROTO_BASIC_EXTENDS_(Expr, Derived, Domain)                                           \
        typedef void proto_is_aggregate_;                                                           \
        /**< INTERNAL ONLY */

    #define HBOOST_PROTO_EXTENDS_COPY_ASSIGN_IMPL_(This, Const, Typename)                            \
        HBOOST_PROTO_DISABLE_MSVC_C4522                                                              \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        Typename() HBOOST_PROTO_RESULT_OF<                                                           \
            Typename() This::proto_generator(                                                       \
                Typename() hboost::proto::base_expr<                                                 \
                    Typename() This::proto_domain                                                   \
                  , hboost::proto::tag::assign                                                       \
                  , hboost::proto::list2<                                                            \
                        This &                                                                      \
                      , This Const() &                                                              \
                    >                                                                               \
                >::type                                                                             \
            )                                                                                       \
        >::type const                                                                               \
        operator =(This Const() &a)                                                                 \
        {                                                                                           \
            typedef                                                                                 \
                Typename() hboost::proto::base_expr<                                                 \
                    Typename() This::proto_domain                                                   \
                  , hboost::proto::tag::assign                                                       \
                  , hboost::proto::list2<                                                            \
                        This &                                                                      \
                      , This Const() &                                                              \
                    >                                                                               \
                >::type                                                                             \
            that_type;                                                                              \
            that_type const that = {                                                                \
                *this                                                                               \
              , a                                                                                   \
            };                                                                                      \
            return Typename() This::proto_generator()(that);                                        \
        }                                                                                           \
        /**/

        // MSVC 8.0 and higher seem to need copy-assignment operator to be overloaded on *both*
        // const and non-const rhs arguments.
    #if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1600)) && (HBOOST_MSVC > 1310)
        #define HBOOST_PROTO_EXTENDS_COPY_ASSIGN_(This, Typename)                                    \
            HBOOST_PROTO_EXTENDS_COPY_ASSIGN_IMPL_(This, HBOOST_PP_EMPTY, Typename)                   \
            HBOOST_PROTO_EXTENDS_COPY_ASSIGN_IMPL_(This, HBOOST_PROTO_CONST, Typename)                \
            /**/
    #else
        #define HBOOST_PROTO_EXTENDS_COPY_ASSIGN_(This, Typename)                                    \
            HBOOST_PROTO_EXTENDS_COPY_ASSIGN_IMPL_(This, HBOOST_PROTO_CONST, Typename)                \
            /**/
    #endif

        /// INTERNAL ONLY
        ///
    #define HBOOST_PROTO_EXTENDS_ASSIGN_IMPL_(ThisConst, ThatConst)                                  \
        template<typename A>                                                                        \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        typename HBOOST_PROTO_RESULT_OF<                                                             \
            proto_generator(                                                                        \
                typename hboost::proto::base_expr<                                                   \
                    proto_domain                                                                    \
                  , hboost::proto::tag::assign                                                       \
                  , hboost::proto::list2<                                                            \
                        proto_derived_expr ThisConst() &                                            \
                      , typename hboost::proto::result_of::as_child<A ThatConst(), proto_domain>::type \
                    >                                                                               \
                >::type                                                                             \
            )                                                                                       \
        >::type const                                                                               \
        operator =(A ThatConst() &a) ThisConst()                                                    \
        {                                                                                           \
            typedef                                                                                 \
                typename hboost::proto::base_expr<                                                   \
                    proto_domain                                                                    \
                  , hboost::proto::tag::assign                                                       \
                  , hboost::proto::list2<                                                            \
                        proto_derived_expr ThisConst() &                                            \
                      , typename hboost::proto::result_of::as_child<A ThatConst(), proto_domain>::type \
                    >                                                                               \
                >::type                                                                             \
            that_type;                                                                              \
            that_type const that = {                                                                \
                *static_cast<proto_derived_expr ThisConst() *>(this)                                \
              , hboost::proto::as_child<proto_domain>(a)                                             \
            };                                                                                      \
            return proto_generator()(that);                                                         \
        }                                                                                           \
        /**/

    #define HBOOST_PROTO_EXTENDS_ASSIGN_CONST_()                                                     \
        HBOOST_PROTO_EXTENDS_ASSIGN_IMPL_(HBOOST_PROTO_CONST, HBOOST_PP_EMPTY)                         \
        HBOOST_PROTO_EXTENDS_ASSIGN_IMPL_(HBOOST_PROTO_CONST, HBOOST_PROTO_CONST)                      \
        /**/

    #define HBOOST_PROTO_EXTENDS_ASSIGN_NON_CONST_()                                                 \
        HBOOST_PROTO_EXTENDS_ASSIGN_IMPL_(HBOOST_PP_EMPTY, HBOOST_PP_EMPTY)                            \
        HBOOST_PROTO_EXTENDS_ASSIGN_IMPL_(HBOOST_PP_EMPTY, HBOOST_PROTO_CONST)                         \
        /**/

    #define HBOOST_PROTO_EXTENDS_ASSIGN_()                                                           \
        HBOOST_PROTO_EXTENDS_ASSIGN_CONST_()                                                         \
        HBOOST_PROTO_EXTENDS_ASSIGN_NON_CONST_()                                                     \
        /**/

    #define HBOOST_PROTO_EXTENDS_ASSIGN_CONST()                                                      \
        HBOOST_PROTO_EXTENDS_COPY_ASSIGN_(proto_derived_expr, HBOOST_PROTO_TYPENAME)                  \
        HBOOST_PROTO_EXTENDS_ASSIGN_CONST_()                                                         \
        /**/

    #define HBOOST_PROTO_EXTENDS_ASSIGN_NON_CONST()                                                  \
        HBOOST_PROTO_EXTENDS_COPY_ASSIGN_(proto_derived_expr, HBOOST_PROTO_TYPENAME)                  \
        HBOOST_PROTO_EXTENDS_ASSIGN_NON_CONST_()                                                     \
        /**/

    #define HBOOST_PROTO_EXTENDS_ASSIGN()                                                            \
        HBOOST_PROTO_EXTENDS_COPY_ASSIGN_(proto_derived_expr, HBOOST_PROTO_TYPENAME)                  \
        HBOOST_PROTO_EXTENDS_ASSIGN_()                                                               \
        /**/

        /// INTERNAL ONLY
        ///
    #define HBOOST_PROTO_EXTENDS_SUBSCRIPT_IMPL_(ThisConst, ThatConst)                               \
        template<typename A>                                                                        \
        HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                            \
        typename HBOOST_PROTO_RESULT_OF<                                                             \
            proto_generator(                                                                        \
                typename hboost::proto::base_expr<                                                   \
                    proto_domain                                                                    \
                  , hboost::proto::tag::subscript                                                    \
                  , hboost::proto::list2<                                                            \
                        proto_derived_expr ThisConst() &                                            \
                      , typename hboost::proto::result_of::as_child<A ThatConst(), proto_domain>::type \
                    >                                                                               \
                >::type                                                                             \
            )                                                                                       \
        >::type const                                                                               \
        operator [](A ThatConst() &a) ThisConst()                                                   \
        {                                                                                           \
            typedef                                                                                 \
                typename hboost::proto::base_expr<                                                   \
                    proto_domain                                                                    \
                  , hboost::proto::tag::subscript                                                    \
                  , hboost::proto::list2<                                                            \
                        proto_derived_expr ThisConst() &                                            \
                      , typename hboost::proto::result_of::as_child<A ThatConst(), proto_domain>::type \
                    >                                                                               \
                >::type                                                                             \
            that_type;                                                                              \
            that_type const that = {                                                                \
                *static_cast<proto_derived_expr ThisConst() *>(this)                                \
              , hboost::proto::as_child<proto_domain>(a)                                             \
            };                                                                                      \
            return proto_generator()(that);                                                         \
        }                                                                                           \
        /**/

    #define HBOOST_PROTO_EXTENDS_SUBSCRIPT_CONST()                                                   \
        HBOOST_PROTO_EXTENDS_SUBSCRIPT_IMPL_(HBOOST_PROTO_CONST, HBOOST_PP_EMPTY)                      \
        HBOOST_PROTO_EXTENDS_SUBSCRIPT_IMPL_(HBOOST_PROTO_CONST, HBOOST_PROTO_CONST)                   \
        /**/

    #define HBOOST_PROTO_EXTENDS_SUBSCRIPT_NON_CONST()                                               \
        HBOOST_PROTO_EXTENDS_SUBSCRIPT_IMPL_(HBOOST_PP_EMPTY, HBOOST_PP_EMPTY)                         \
        HBOOST_PROTO_EXTENDS_SUBSCRIPT_IMPL_(HBOOST_PP_EMPTY, HBOOST_PROTO_CONST)                      \
        /**/

    #define HBOOST_PROTO_EXTENDS_SUBSCRIPT()                                                         \
        HBOOST_PROTO_EXTENDS_SUBSCRIPT_CONST()                                                       \
        HBOOST_PROTO_EXTENDS_SUBSCRIPT_NON_CONST()                                                   \
        /**/

        /// INTERNAL ONLY
        ///
    #define HBOOST_PROTO_EXTENDS_FUNCTION_()                                                         \
        template<typename Sig>                                                                      \
        struct result                                                                               \
        {                                                                                           \
            typedef                                                                                 \
                typename HBOOST_PROTO_RESULT_OF<                                                     \
                    proto_generator(                                                                \
                        typename hboost::proto::result_of::funop<                                    \
                            Sig                                                                     \
                          , proto_derived_expr                                                      \
                          , proto_domain                                                            \
                        >::type                                                                     \
                    )                                                                               \
                >::type const                                                                       \
            type;                                                                                   \
        };                                                                                          \
        /**/

    #ifndef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
        #define HBOOST_PROTO_EXTENDS_FUNCTION_CONST()                                                \
            HBOOST_PROTO_EXTENDS_FUNCTION_()                                                         \
            HBOOST_PROTO_DEFINE_FUN_OP_VARIADIC_IMPL_(HBOOST_PROTO_CONST)                             \
            /**/

        #define HBOOST_PROTO_EXTENDS_FUNCTION_NON_CONST()                                            \
            HBOOST_PROTO_EXTENDS_FUNCTION_()                                                         \
            HBOOST_PROTO_DEFINE_FUN_OP_VARIADIC_IMPL_(HBOOST_PP_EMPTY)                                \
            /**/

        #define HBOOST_PROTO_EXTENDS_FUNCTION()                                                      \
            HBOOST_PROTO_EXTENDS_FUNCTION_()                                                         \
            HBOOST_PROTO_DEFINE_FUN_OP_VARIADIC_IMPL_(HBOOST_PP_EMPTY)                                \
            HBOOST_PROTO_DEFINE_FUN_OP_VARIADIC_IMPL_(HBOOST_PROTO_CONST)                             \
            /**/
    #else
        #define HBOOST_PROTO_EXTENDS_FUNCTION_CONST()                                                \
            HBOOST_PROTO_EXTENDS_FUNCTION_()                                                         \
            HBOOST_PP_REPEAT_FROM_TO(                                                                \
                0                                                                                   \
              , HBOOST_PROTO_MAX_FUNCTION_CALL_ARITY                                                 \
              , HBOOST_PROTO_DEFINE_FUN_OP_CONST                                                     \
              , ~                                                                                   \
            )                                                                                       \
            /**/

        #define HBOOST_PROTO_EXTENDS_FUNCTION_NON_CONST()                                            \
            HBOOST_PROTO_EXTENDS_FUNCTION_()                                                         \
            HBOOST_PP_REPEAT_FROM_TO(                                                                \
                0                                                                                   \
              , HBOOST_PROTO_MAX_FUNCTION_CALL_ARITY                                                 \
              , HBOOST_PROTO_DEFINE_FUN_OP_NON_CONST                                                 \
              , ~                                                                                   \
            )                                                                                       \
            /**/

        #define HBOOST_PROTO_EXTENDS_FUNCTION()                                                      \
            HBOOST_PROTO_EXTENDS_FUNCTION_()                                                         \
            HBOOST_PP_REPEAT_FROM_TO(                                                                \
                0                                                                                   \
              , HBOOST_PROTO_MAX_FUNCTION_CALL_ARITY                                                 \
              , HBOOST_PROTO_DEFINE_FUN_OP                                                           \
              , ~                                                                                   \
            )                                                                                       \
            /**/
    #endif

    #define HBOOST_PROTO_EXTENDS(Expr, Derived, Domain)                                              \
        HBOOST_PROTO_BASIC_EXTENDS(Expr, Derived, Domain)                                            \
        HBOOST_PROTO_EXTENDS_ASSIGN()                                                                \
        HBOOST_PROTO_EXTENDS_SUBSCRIPT()                                                             \
        HBOOST_PROTO_EXTENDS_FUNCTION()                                                              \
        /**/

    #define HBOOST_PROTO_EXTENDS_USING_ASSIGN(Derived)                                               \
        typedef typename Derived::proto_extends proto_extends;                                      \
        using proto_extends::operator =;                                                            \
        HBOOST_PROTO_EXTENDS_COPY_ASSIGN_(Derived, HBOOST_PROTO_TYPENAME)                             \
        /**/

    #define HBOOST_PROTO_EXTENDS_USING_ASSIGN_NON_DEPENDENT(Derived)                                 \
        typedef Derived::proto_extends proto_extends;                                               \
        using proto_extends::operator =;                                                            \
        HBOOST_PROTO_EXTENDS_COPY_ASSIGN_(Derived, HBOOST_PP_EMPTY)                                   \
        /**/

    namespace exprns_
    {
        /// \brief Empty type to be used as a dummy template parameter of
        ///     POD expression wrappers. It allows argument-dependent lookup
        ///     to find Proto's operator overloads.
        ///
        /// \c proto::is_proto_expr allows argument-dependent lookup
        ///     to find Proto's operator overloads. For example:
        ///
        /// \code
        /// template<typename T, typename Dummy = proto::is_proto_expr>
        /// struct my_terminal
        /// {
        ///     HBOOST_PROTO_BASIC_EXTENDS(
        ///         typename proto::terminal<T>::type
        ///       , my_terminal<T>
        ///       , default_domain
        ///     )
        /// };
        ///
        /// // ...
        /// my_terminal<int> _1, _2;
        /// _1 + _2; // OK, uses proto::operator+
        /// \endcode
        ///
        /// Without the second \c Dummy template parameter, Proto's operator
        /// overloads would not be considered by name lookup.
        struct is_proto_expr
        {};

        /// \brief extends\<\> class template for adding behaviors to a Proto expression template
        ///
        template<
            typename Expr
          , typename Derived
          , typename Domain     // = proto::default_domain
          , long Arity          // = Expr::proto_arity_c
        >
        struct extends
        {
            HBOOST_FORCEINLINE
            extends()
              : proto_expr_()
            {}

            HBOOST_FORCEINLINE
            extends(extends const &that)
              : proto_expr_(that.proto_expr_)
            {}

            HBOOST_FORCEINLINE
            extends(Expr const &expr_)
              : proto_expr_(expr_)
            {}

            typedef extends proto_extends;
            HBOOST_PROTO_BASIC_EXTENDS_(Expr, Derived, typename Domain)
            HBOOST_PROTO_EXTENDS_ASSIGN_CONST_()
            HBOOST_PROTO_EXTENDS_SUBSCRIPT_CONST()

            // Instead of using HBOOST_PROTO_EXTENDS_FUNCTION, which uses
            // nested preprocessor loops, use file iteration here to generate
            // the operator() overloads, which is more efficient.
            #include <hboost/proto/detail/extends_funop_const.hpp>
        };

        /// \brief extends\<\> class template for adding behaviors to a Proto expression template
        ///
        template<typename Expr, typename Derived, typename Domain>
        struct extends<Expr, Derived, Domain, 0>
        {
            HBOOST_FORCEINLINE
            extends()
              : proto_expr_()
            {}

            HBOOST_FORCEINLINE
            extends(extends const &that)
              : proto_expr_(that.proto_expr_)
            {}

            HBOOST_FORCEINLINE
            extends(Expr const &expr_)
              : proto_expr_(expr_)
            {}

            typedef extends proto_extends;
            HBOOST_PROTO_BASIC_EXTENDS_(Expr, Derived, typename Domain)
            HBOOST_PROTO_EXTENDS_ASSIGN_()
            HBOOST_PROTO_EXTENDS_SUBSCRIPT()

            // Instead of using HBOOST_PROTO_EXTENDS_FUNCTION, which uses
            // nested preprocessor loops, use file iteration here to generate
            // the operator() overloads, which is more efficient.
            #include <hboost/proto/detail/extends_funop.hpp>
        };

        /// INTERNAL ONLY
        ///
        template<typename This, typename Fun, typename Domain>
        struct virtual_member
        {
            typedef Domain proto_domain;
            typedef typename Domain::proto_generator proto_generator;
            typedef virtual_member<This, Fun, Domain> proto_derived_expr;
            typedef tag::member proto_tag;
            typedef list2<This &, expr<tag::terminal, term<Fun> > const &> proto_args;
            typedef mpl::long_<2> proto_arity;
            typedef detail::not_a_valid_type proto_address_of_hack_type_;
            typedef void proto_is_expr_; /**< INTERNAL ONLY */
            static const long proto_arity_c = 2;
            typedef hboost::proto::tag::proto_expr<proto_tag, Domain> fusion_tag;
            typedef This &proto_child0;
            typedef expr<tag::terminal, term<Fun> > const &proto_child1;
            typedef expr<proto_tag, proto_args, proto_arity_c> proto_base_expr;
            typedef basic_expr<proto_tag, proto_args, proto_arity_c> proto_grammar;
            typedef void proto_is_aggregate_; /**< INTERNAL ONLY */

            HBOOST_PROTO_EXTENDS_ASSIGN_()
            HBOOST_PROTO_EXTENDS_SUBSCRIPT()

            // Instead of using HBOOST_PROTO_EXTENDS_FUNCTION, which uses
            // nested preprocessor loops, use file iteration here to generate
            // the operator() overloads, which is more efficient.
            #define HBOOST_PROTO_NO_WAVE_OUTPUT
            #include <hboost/proto/detail/extends_funop.hpp>
            #undef HBOOST_PROTO_NO_WAVE_OUTPUT

            HBOOST_FORCEINLINE
            proto_base_expr const proto_base() const
            {
                proto_base_expr that = {this->child0(), this->child1()};
                return that;
            }

            HBOOST_FORCEINLINE
            proto_child0 child0() const
            {
                using std::size_t;
                return *(This *)((char *)this - HBOOST_PROTO_OFFSETOF(This, proto_member_union_start_));
            }

            HBOOST_FORCEINLINE
            proto_child1 child1() const
            {
                static expr<tag::terminal, term<Fun>, 0> const that = {Fun()};
                return that;
            }
        };

        /// INTERNAL ONLY
        ///
        #define HBOOST_PROTO_EXTENDS_MEMBER_(R, DOMAIN, ELEM)                                            \
            hboost::proto::exprns_::virtual_member<                                                      \
                proto_derived_expr                                                                      \
              , HBOOST_PP_TUPLE_ELEM(2, 0, ELEM)                                                         \
              , DOMAIN                                                                                  \
            > HBOOST_PP_TUPLE_ELEM(2, 1, ELEM);                                                          \
            /**/

        /// \brief For declaring virtual data members in an extension class.
        ///
        #define HBOOST_PROTO_EXTENDS_MEMBERS_WITH_DOMAIN(SEQ, DOMAIN)                                    \
            union                                                                                       \
            {                                                                                           \
                char proto_member_union_start_;                                                         \
                HBOOST_PP_SEQ_FOR_EACH(HBOOST_PROTO_EXTENDS_MEMBER_, DOMAIN, SEQ)                         \
            };                                                                                          \
            /**/

        /// \brief For declaring virtual data members in an extension class.
        ///
        #define HBOOST_PROTO_EXTENDS_MEMBERS(SEQ)                                                        \
            HBOOST_PROTO_EXTENDS_MEMBERS_WITH_DOMAIN(SEQ, proto_domain)                                  \
            /**/

    }

}}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif
