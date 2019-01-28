///////////////////////////////////////////////////////////////////////////////
/// \file operators.hpp
/// Contains all the overloaded operators that make it possible to build
/// Proto expression trees.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_OPERATORS_HPP_EAN_04_01_2005
#define HBOOST_PROTO_OPERATORS_HPP_EAN_04_01_2005

#include <hboost/config.hpp>
#include <hboost/preprocessor/punctuation/comma.hpp>
#include <hboost/mpl/logical.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/proto/proto_fwd.hpp>
#include <hboost/proto/tags.hpp>
#include <hboost/proto/domain.hpp>
#include <hboost/proto/matches.hpp>
#include <hboost/proto/generate.hpp>
#include <hboost/proto/make_expr.hpp>

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable : 4714) // function 'xxx' marked as __forceinline not inlined
#endif

namespace hboost { namespace proto
{
    namespace detail
    {
        template<typename MakeExpr, typename Grammar>
        struct lazy_matches
          : proto::matches<typename MakeExpr::type, Grammar>
        {};

        template<typename Domain, typename Grammar, typename Trait, typename Tag, typename Arg>
        struct enable_unary
          : hboost::lazy_enable_if_c<
                hboost::mpl::and_<
                    Trait
                  , lazy_matches<result_of::make_expr<Tag, basic_default_domain, Arg>, Grammar>
                >::value
              , result_of::make_expr<Tag, Domain, Arg>
            >
        {};

        template<typename Domain, typename Trait, typename Tag, typename Arg>
        struct enable_unary<Domain, proto::_, Trait, Tag, Arg &>
          : hboost::lazy_enable_if_c<
                Trait::value
              , result_of::make_expr<Tag, Domain, Arg &>
            >
        {};

        template<typename Trait, typename Tag, typename Arg>
        struct enable_unary<deduce_domain, not_a_grammar, Trait, Tag, Arg &>
          : enable_unary<
                typename domain_of<Arg>::type
              , typename domain_of<Arg>::type::proto_grammar
              , Trait
              , Tag
              , Arg &
            >
        {};

        template<typename Domain, typename Grammar, typename Trait, typename Tag, typename Left, typename Right>
        struct enable_binary
          : hboost::lazy_enable_if_c<
                hboost::mpl::and_<
                    Trait
                  , lazy_matches<result_of::make_expr<Tag, basic_default_domain, Left, Right>, Grammar>
                >::value
              , result_of::make_expr<Tag, Domain, Left, Right>
            >
        {};

        template<typename Domain, typename Trait, typename Tag, typename Left, typename Right>
        struct enable_binary<Domain, proto::_, Trait, Tag, Left &, Right &>
          : hboost::lazy_enable_if_c<
                Trait::value
              , result_of::make_expr<Tag, Domain, Left &, Right &>
            >
        {};

        template<typename Trait, typename Tag, typename Left, typename Right>
        struct enable_binary<deduce_domain, not_a_grammar, Trait, Tag, Left &, Right &>
          : enable_binary<
                typename deduce_domain2<Left, Right>::type
              , typename deduce_domain2<Left, Right>::type::proto_grammar
              , Trait
              , Tag
              , Left &
              , Right &
            >
        {};

    } // detail

#define HBOOST_PROTO_UNARY_OP_IS_POSTFIX_0
#define HBOOST_PROTO_UNARY_OP_IS_POSTFIX_1 , int

#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES

#define HBOOST_PROTO_DEFINE_UNARY_OPERATOR(OP, TAG, TRAIT, DOMAIN, POST)                             \
    template<typename Arg>                                                                          \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_unary<                                                    \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_UNARY_(TRAIT, Arg)                                                        \
      , TAG                                                                                         \
      , Arg &                                                                                       \
    >::type const                                                                                   \
    operator OP(Arg &arg HBOOST_PROTO_UNARY_OP_IS_POSTFIX_ ## POST)                                  \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Arg &>()(arg);                         \
    }                                                                                               \
                                                                                                    \
    template<typename Arg>                                                                          \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_unary<                                                    \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_UNARY_(TRAIT, Arg)                                                        \
      , TAG                                                                                         \
      , Arg const &                                                                                 \
    >::type const                                                                                   \
    operator OP(Arg const &arg HBOOST_PROTO_UNARY_OP_IS_POSTFIX_ ## POST)                            \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Arg const &>()(arg);                   \
    }                                                                                               \
    /**/

#define HBOOST_PROTO_DEFINE_BINARY_OPERATOR(OP, TAG, TRAIT, DOMAIN)                                  \
    template<typename Left, typename Right>                                                         \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_binary<                                                   \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right)                                               \
      , TAG                                                                                         \
      , Left &                                                                                      \
      , Right &                                                                                     \
    >::type const                                                                                   \
    operator OP(Left &left, Right &right)                                                           \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Left &, Right &>()(left, right);       \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_binary<                                                   \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right)                                               \
      , TAG                                                                                         \
      , Left &                                                                                      \
      , Right const &                                                                               \
    >::type const                                                                                   \
    operator OP(Left &left, Right const &right)                                                     \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Left &, Right const &>()(left, right); \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_binary<                                                   \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right)                                               \
      , TAG                                                                                         \
      , Left const &                                                                                \
      , Right &                                                                                     \
    >::type const                                                                                   \
    operator OP(Left const &left, Right &right)                                                     \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Left const &, Right &>()(left, right); \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_binary<                                                   \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right)                                               \
      , TAG                                                                                         \
      , Left const &                                                                                \
      , Right const &                                                                               \
    >::type const                                                                                   \
    operator OP(Left const &left, Right const &right)                                               \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Left const &, Right const &>()(left, right);\
    }                                                                                               \
    /**/

#else

#define HBOOST_PROTO_DEFINE_UNARY_OPERATOR(OP, TAG, TRAIT, DOMAIN, POST)                             \
    template<typename Arg>                                                                          \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_unary<                                                    \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_UNARY_(TRAIT, Arg)                                                        \
      , TAG                                                                                         \
      , Arg const &                                                                                 \
    >::type const                                                                                   \
    operator OP(Arg &&arg HBOOST_PROTO_UNARY_OP_IS_POSTFIX_ ## POST)                                 \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Arg const &>()(arg);                   \
    }                                                                                               \
    /**/

#define HBOOST_PROTO_DEFINE_BINARY_OPERATOR(OP, TAG, TRAIT, DOMAIN)                                  \
    template<typename Left, typename Right>                                                         \
    HBOOST_PROTO_DISABLE_MSVC_C4714 HBOOST_FORCEINLINE                                                \
    typename hboost::proto::detail::enable_binary<                                                   \
        DOMAIN                                                                                      \
      , DOMAIN::proto_grammar                                                                       \
      , HBOOST_PROTO_APPLY_BINARY_(TRAIT, Left, Right)                                               \
      , TAG                                                                                         \
      , Left const &                                                                                \
      , Right const &                                                                               \
    >::type const                                                                                   \
    operator OP(Left &&left, Right &&right)                                                         \
    {                                                                                               \
        return hboost::proto::detail::make_expr_<TAG, DOMAIN, Left const &, Right const &>()(left, right);\
    }                                                                                               \
    /**/

#endif

#define HBOOST_PROTO_DEFINE_OPERATORS(TRAIT, DOMAIN)                                                 \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(+, hboost::proto::tag::unary_plus, TRAIT, DOMAIN, 0)           \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(-, hboost::proto::tag::negate, TRAIT, DOMAIN, 0)               \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(*, hboost::proto::tag::dereference, TRAIT, DOMAIN, 0)          \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(~, hboost::proto::tag::complement, TRAIT, DOMAIN, 0)           \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(&, hboost::proto::tag::address_of, TRAIT, DOMAIN, 0)           \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(!, hboost::proto::tag::logical_not, TRAIT, DOMAIN, 0)          \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(++, hboost::proto::tag::pre_inc, TRAIT, DOMAIN, 0)             \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(--, hboost::proto::tag::pre_dec, TRAIT, DOMAIN, 0)             \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(++, hboost::proto::tag::post_inc, TRAIT, DOMAIN, 1)            \
    HBOOST_PROTO_DEFINE_UNARY_OPERATOR(--, hboost::proto::tag::post_dec, TRAIT, DOMAIN, 1)            \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(<<, hboost::proto::tag::shift_left, TRAIT, DOMAIN)            \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(>>, hboost::proto::tag::shift_right, TRAIT, DOMAIN)           \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(*, hboost::proto::tag::multiplies, TRAIT, DOMAIN)             \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(/, hboost::proto::tag::divides, TRAIT, DOMAIN)                \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(%, hboost::proto::tag::modulus, TRAIT, DOMAIN)                \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(+, hboost::proto::tag::plus, TRAIT, DOMAIN)                   \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(-, hboost::proto::tag::minus, TRAIT, DOMAIN)                  \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(<, hboost::proto::tag::less, TRAIT, DOMAIN)                   \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(>, hboost::proto::tag::greater, TRAIT, DOMAIN)                \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(<=, hboost::proto::tag::less_equal, TRAIT, DOMAIN)            \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(>=, hboost::proto::tag::greater_equal, TRAIT, DOMAIN)         \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(==, hboost::proto::tag::equal_to, TRAIT, DOMAIN)              \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(!=, hboost::proto::tag::not_equal_to, TRAIT, DOMAIN)          \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(||, hboost::proto::tag::logical_or, TRAIT, DOMAIN)            \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(&&, hboost::proto::tag::logical_and, TRAIT, DOMAIN)           \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(&, hboost::proto::tag::bitwise_and, TRAIT, DOMAIN)            \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(|, hboost::proto::tag::bitwise_or, TRAIT, DOMAIN)             \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(^, hboost::proto::tag::bitwise_xor, TRAIT, DOMAIN)            \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(HBOOST_PP_COMMA(), hboost::proto::tag::comma, TRAIT, DOMAIN)   \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(->*, hboost::proto::tag::mem_ptr, TRAIT, DOMAIN)              \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(<<=, hboost::proto::tag::shift_left_assign, TRAIT, DOMAIN)    \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(>>=, hboost::proto::tag::shift_right_assign, TRAIT, DOMAIN)   \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(*=, hboost::proto::tag::multiplies_assign, TRAIT, DOMAIN)     \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(/=, hboost::proto::tag::divides_assign, TRAIT, DOMAIN)        \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(%=, hboost::proto::tag::modulus_assign, TRAIT, DOMAIN)        \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(+=, hboost::proto::tag::plus_assign, TRAIT, DOMAIN)           \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(-=, hboost::proto::tag::minus_assign, TRAIT, DOMAIN)          \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(&=, hboost::proto::tag::bitwise_and_assign, TRAIT, DOMAIN)    \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(|=, hboost::proto::tag::bitwise_or_assign, TRAIT, DOMAIN)     \
    HBOOST_PROTO_DEFINE_BINARY_OPERATOR(^=, hboost::proto::tag::bitwise_xor_assign, TRAIT, DOMAIN)    \
    /**/

    // Extensions are a superset of Proto expressions
    template<typename T>
    struct is_extension
      : is_expr<T>
    {};

    template<typename T>
    struct is_extension<T &>
      : is_expr<T>
    {};

    #define HBOOST_PROTO_APPLY_UNARY_(TRAIT, ARG) TRAIT<ARG>
    #define HBOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT) hboost::mpl::or_<TRAIT<LEFT>, TRAIT<RIGHT> >

    namespace exprns_
    {
        // This defines all of Proto's built-in free operator overloads
        HBOOST_PROTO_DEFINE_OPERATORS(is_extension, deduce_domain)

        // if_else, for the non-overloadable ternary conditional operator ?:
        template<typename A0, typename A1, typename A2>
        HBOOST_FORCEINLINE
        typename result_of::make_expr<
            tag::if_else_
          , deduce_domain
          , A0 const &
          , A1 const &
          , A2 const &
        >::type const
        if_else(A0 const &a0, A1 const &a1, A2 const &a2)
        {
            return proto::detail::make_expr_<
                tag::if_else_
              , deduce_domain
              , A0 const &
              , A1 const &
              , A2 const &
            >()(a0, a1, a2);
        }
    }

    using exprns_::if_else;

    #undef HBOOST_PROTO_APPLY_UNARY_
    #undef HBOOST_PROTO_APPLY_BINARY_

    // Redefine HBOOST_PROTO_APPLY_UNARY_ and HBOOST_PROTO_APPLY_BINARY_ so that end users
    // can use HBOOST_PROTO_DEFINE_OPERATORS to define Proto operator overloads that work
    // with their own terminal types.

#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES

    #define HBOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)                                                    \
        hboost::mpl::and_<                                                                           \
            TRAIT<ARG>                                                                              \
          , hboost::mpl::not_<hboost::proto::is_extension<ARG> >                                      \
        >                                                                                           \
        /**/

    #define HBOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT)                                           \
        hboost::mpl::and_<                                                                           \
            hboost::mpl::or_<TRAIT<LEFT>, TRAIT<RIGHT> >                                             \
          , hboost::mpl::not_<                                                                       \
                hboost::mpl::or_<                                                                    \
                    hboost::proto::is_extension<LEFT>                                                \
                  , hboost::proto::is_extension<RIGHT>                                               \
                >                                                                                   \
            >                                                                                       \
        >                                                                                           \
        /**/

#else

    #define HBOOST_PROTO_APPLY_UNARY_(TRAIT, ARG)                                                    \
        hboost::mpl::and_<                                                                           \
            TRAIT<HBOOST_PROTO_UNCVREF(ARG) >                                                        \
          , hboost::mpl::not_<hboost::proto::is_extension<ARG> >                                      \
        >                                                                                           \
        /**/

    #define HBOOST_PROTO_APPLY_BINARY_(TRAIT, LEFT, RIGHT)                                           \
        hboost::mpl::and_<                                                                           \
            hboost::mpl::or_<TRAIT<HBOOST_PROTO_UNCVREF(LEFT) >, TRAIT<HBOOST_PROTO_UNCVREF(RIGHT) > > \
          , hboost::mpl::not_<                                                                       \
                hboost::mpl::or_<                                                                    \
                    hboost::proto::is_extension<LEFT>                                                \
                  , hboost::proto::is_extension<RIGHT>                                               \
                >                                                                                   \
            >                                                                                       \
        >                                                                                           \
        /**/

#endif

}}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif
