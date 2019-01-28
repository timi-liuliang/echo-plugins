///////////////////////////////////////////////////////////////////////////////
/// \file null.hpp
/// Definintion of null_context\<\>, an evaluation context for
/// proto::eval() that simply evaluates each child expression, doesn't
/// combine the results at all, and returns void.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_CONTEXT_NULL_HPP_EAN_06_24_2007
#define HBOOST_PROTO_CONTEXT_NULL_HPP_EAN_06_24_2007

#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/proto/proto_fwd.hpp>
#include <hboost/proto/eval.hpp>
#include <hboost/proto/traits.hpp>

namespace hboost { namespace proto { namespace context
{

    template<
        typename Expr
      , typename Context
      , long Arity          // = Expr::proto_arity_c
    >
    struct null_eval
    {};

    template<typename Expr, typename Context>
    struct null_eval<Expr, Context, 0>
    {
        typedef void result_type;
        void operator()(Expr &, Context &) const
        {}
    };

    // Additional specializations generated by the preprocessor
    #include <hboost/proto/context/detail/null_eval.hpp>

    /// null_context
    ///
    struct null_context
    {
        /// null_context::eval
        ///
        template<typename Expr, typename ThisContext = null_context const>
        struct eval
          : null_eval<Expr, ThisContext>
        {};
    };

}}}

#endif
