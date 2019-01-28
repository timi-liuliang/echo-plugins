
#if !HBOOST_PHOENIX_IS_ITERATING

#ifndef HBOOST_PHOENIX_OPERATOR_MEMBER_DETAIL_MEM_FUN_PTR_GEN_HPP
#define HBOOST_PHOENIX_OPERATOR_MEMBER_DETAIL_MEM_FUN_PTR_GEN_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/phoenix/support/iterate.hpp>
#include <hboost/phoenix/core/expression.hpp>

#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)

#include <hboost/phoenix/operator/detail/preprocessed/mem_fun_ptr_gen.hpp>

#else

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/mem_fun_ptr_gen_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
#endif

/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

HBOOST_PHOENIX_DEFINE_EXPRESSION_VARARG(
    (hboost)(hboostphoenix)(mem_fun_ptr)
  , (meta_grammar)
    (meta_grammar)
  , HBOOST_PHOENIX_LIMIT
)

namespace hboost { namespace hboostphoenix
{
    namespace detail {
        template <typename Object, typename MemPtr>
        struct mem_fun_ptr_gen
        {
            mem_fun_ptr_gen(Object const& obj_, MemPtr ptr_)
              : obj(obj_)
              , ptr(ptr_)
            {}

            typename hboostphoenix::expression::mem_fun_ptr<Object, MemPtr>::type const
            operator()() const
            {
                return hboostphoenix::expression::mem_fun_ptr<Object, MemPtr>::make(obj, ptr);
            }

#define HBOOST_PHOENIX_ITERATION_PARAMS                                          \
        (3, (1, HBOOST_PHOENIX_MEMBER_LIMIT,                                     \
        <hboost/phoenix/operator/detail/mem_fun_ptr_gen.hpp>))                   \

#include HBOOST_PHOENIX_ITERATE()

            Object const& obj;
            MemPtr ptr;

        };

        struct make_mem_fun_ptr_gen
            : proto::callable
        {
            template<typename Sig>
            struct result;

            template<typename This, typename Object, typename MemPtr>
            struct result<This(Object, MemPtr)>
            {
                typedef
                    mem_fun_ptr_gen<
                        typename remove_const<typename remove_reference<Object>::type>::type
                      , typename remove_const<typename remove_reference<MemPtr>::type>::type
                    >
                type;
            };

            template<typename Object, typename MemPtr>
            mem_fun_ptr_gen<Object, MemPtr> operator()(Object const & obj, MemPtr ptr) const
            {
                return mem_fun_ptr_gen<Object, MemPtr>(obj, ptr);
            }
        };
    }
}}

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES

#endif

#else
            template <HBOOST_PHOENIX_typename_A>
            typename hboostphoenix::expression::mem_fun_ptr<
                Object
              , MemPtr
              , HBOOST_PHOENIX_A
            >::type const
            operator()(HBOOST_PHOENIX_A_const_ref_a) const
            {
                return hboostphoenix::expression::mem_fun_ptr<
                    Object
                  , MemPtr
                  , HBOOST_PHOENIX_A
                >::make(obj, ptr, HBOOST_PHOENIX_a);
            }
#endif
