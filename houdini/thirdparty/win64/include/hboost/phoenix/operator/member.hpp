
#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_OPERATOR_MEMBER_HPP
#define HBOOST_PHOENIX_OPERATOR_MEMBER_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/get_pointer.hpp>
#include <hboost/phoenix/core/domain.hpp>
#include <hboost/phoenix/core/meta_grammar.hpp>
#include <hboost/phoenix/core/call.hpp>
#include <hboost/phoenix/operator/detail/mem_fun_ptr_gen.hpp>
#include <hboost/phoenix/support/iterate.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/is_member_function_pointer.hpp>
#include <hboost/proto/operators.hpp>

#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)

#include <hboost/phoenix/operator/preprocessed/member.hpp>

#else

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/member_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
#endif

/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

#include <hboost/phoenix/operator/detail/define_operator.hpp>

namespace hboost { namespace hboostphoenix
{
    HBOOST_PHOENIX_BINARY_OPERATORS((mem_ptr))

    template<>
    struct phoenix_generator::case_<proto::tag::mem_ptr>
      : proto::or_<
            proto::when<
                proto::and_<
                    proto::mem_ptr<meta_grammar, proto::terminal<proto::_> >
                  , proto::if_<is_member_function_pointer<hboost::remove_reference<proto::call<proto::_value(proto::_right)> > >()>
                >
              , proto::call<detail::make_mem_fun_ptr_gen(proto::_left, proto::call<proto::_value(proto::_right)>)>
            >
          , proto::otherwise<
                proto::call<proto::pod_generator<actor>(proto::_)>
            >
        >
    {};

    namespace result_of
    {
        template <
            typename Context
          , HBOOST_PHOENIX_typename_A_void(HBOOST_PHOENIX_LIMIT)
          , typename Dummy = void
        >
        struct mem_fun_ptr_eval;

        #include <hboost/phoenix/operator/detail/mem_fun_ptr_eval_result_of.hpp>
    }

    struct mem_fun_ptr_eval
    {
        template<typename Sig>
        struct result;

    #define HBOOST_PHOENIX_MEMBER_EVAL(Z, N, D)                                  \
        HBOOST_PP_COMMA_IF(HBOOST_PP_NOT(HBOOST_PP_EQUAL(N, 2)))                   \
        hboost::hboostphoenix::eval(HBOOST_PP_CAT(a, N), ctx)                           \
    /**/

    #define HBOOST_PHOENIX_ITERATION_PARAMS                                      \
        (3, (2, HBOOST_PHOENIX_LIMIT,                                            \
        <hboost/phoenix/operator/member.hpp>))                                   \
    /**/
        #include HBOOST_PHOENIX_ITERATE()
    #undef HBOOST_PHOENIX_MEMBER_EVAL
    };

    template <typename Dummy>
    struct default_actions::when<rule::mem_fun_ptr, Dummy>
        : call<mem_fun_ptr_eval>
    {};
}}

#include <hboost/phoenix/operator/detail/undef_operator.hpp>

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES

#endif

#else // HBOOST_PHOENIX_IS_ITERATING

        template <typename This, HBOOST_PHOENIX_typename_A, typename Context>
        struct result<This(HBOOST_PHOENIX_A, Context)>
            : result<This(HBOOST_PHOENIX_A_const_ref, Context)>
        {};

        template <typename This, HBOOST_PHOENIX_typename_A, typename Context>
        struct result<This(HBOOST_PHOENIX_A_ref, Context)>
            : result_of::mem_fun_ptr_eval<Context, HBOOST_PHOENIX_A>
        {};

        template <HBOOST_PHOENIX_typename_A, typename Context>
        typename result_of::mem_fun_ptr_eval<Context, HBOOST_PHOENIX_A>::type
        operator()(
            HBOOST_PHOENIX_A_const_ref_a
          , Context const & ctx
        ) const
        {
            return
                (
                    get_pointer(hboost::hboostphoenix::eval(a0, ctx))
                    ->*hboost::hboostphoenix::eval(a1, ctx)
                )(
                    HBOOST_PP_REPEAT_FROM_TO(
                        2
                      , HBOOST_PHOENIX_ITERATION
                      , HBOOST_PHOENIX_MEMBER_EVAL
                      , HBOOST_PHOENIX_ITERATION
                    )
                );
        }
        

#endif
