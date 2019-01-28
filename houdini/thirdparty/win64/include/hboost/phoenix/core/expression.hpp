
#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_CORE_EXPRESSION_HPP
#define HBOOST_PHOENIX_CORE_EXPRESSION_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/call_traits.hpp>
#include <hboost/fusion/sequence/intrinsic/at.hpp>
#include <hboost/phoenix/core/as_actor.hpp>
#include <hboost/phoenix/core/detail/expression.hpp>
#include <hboost/phoenix/core/domain.hpp>
#include <hboost/phoenix/support/iterate.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/proto/domain.hpp>
#include <hboost/proto/make_expr.hpp>
#include <hboost/proto/transform/pass_through.hpp>

#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)

#include <hboost/phoenix/core/preprocessed/expression.hpp>

#else

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/expression_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
#endif

/*=============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace hboost { namespace hboostphoenix
{
    template <typename Expr> struct actor;
    
    template <
        template <typename> class Actor
      , typename Tag
      , HBOOST_PHOENIX_typename_A_void(HBOOST_PHOENIX_COMPOSITE_LIMIT)
      , typename Dummy = void>
    struct expr_ext;

    template <
        typename Tag
      , HBOOST_PHOENIX_typename_A_void(HBOOST_PHOENIX_COMPOSITE_LIMIT)
      , typename Dummy = void
    >
    struct expr : expr_ext<actor, Tag, HBOOST_PHOENIX_A(HBOOST_PHOENIX_COMPOSITE_LIMIT)> {};

#define M0(Z, N, D)                                                             \
    HBOOST_PP_COMMA_IF(N)                                                        \
    typename proto::detail::uncvref<typename call_traits<HBOOST_PP_CAT(A, N)>::value_type>::type

#define M1(Z, N, D)                                                             \
    HBOOST_PP_COMMA_IF(N) typename call_traits<HBOOST_PP_CAT(A, N)>::param_type HBOOST_PP_CAT(a, N)

#define HBOOST_PHOENIX_ITERATION_PARAMS                                          \
    (3, (1, HBOOST_PHOENIX_COMPOSITE_LIMIT,                                      \
    <hboost/phoenix/core/expression.hpp>))                                       \
/**/
    #include HBOOST_PHOENIX_ITERATE()

#undef M0
#undef M1

}}

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // PHOENIX_DONT_USE_PREPROCESSED_FILES

#endif

#else
    template <template <typename> class Actor, typename Tag, HBOOST_PHOENIX_typename_A>
    struct expr_ext<Actor, Tag, HBOOST_PHOENIX_A>
        : proto::transform<expr_ext<Actor, Tag, HBOOST_PHOENIX_A>, int>
    {
        typedef
            typename proto::result_of::make_expr<
                Tag
              , phoenix_default_domain //proto::basic_default_domain
              , HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M0, _)
            >::type
            base_type;

        typedef Actor<base_type> type;

        typedef
            typename proto::nary_expr<Tag, HBOOST_PHOENIX_A>::proto_grammar
            proto_grammar;
        
        static type make(HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, M1, _))
      { //?? actor or Actor??
        //Actor<base_type> const e =
                actor<base_type> const e =
                {
                    proto::make_expr<
                        Tag
                      , phoenix_default_domain // proto::basic_default_domain
                    >(HBOOST_PHOENIX_a)
                };
            return e;
        }

        template<typename Expr, typename State, typename Data>
        struct impl
          : proto::pass_through<expr_ext>::template impl<Expr, State, Data>
        {};
        
        typedef Tag proto_tag;
    #define HBOOST_PHOENIX_ENUM_CHILDREN(_, N, __)                               \
        typedef HBOOST_PP_CAT(A, N) HBOOST_PP_CAT(proto_child, N);                \
    /**/
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ITERATION, HBOOST_PHOENIX_ENUM_CHILDREN, _)
    #undef HBOOST_PHOENIX_ENUM_CHILDREN
    };

#endif
