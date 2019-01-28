/*=============================================================================
    Copyright (c) 2011 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_CORE_V2_EVAL_HPP
#define HBOOST_PHOENIX_CORE_V2_EVAL_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/phoenix/core/environment.hpp>
#include <hboost/phoenix/core/is_actor.hpp>
#include <hboost/phoenix/core/meta_grammar.hpp>
#include <hboost/phoenix/core/terminal_fwd.hpp>
#include <hboost/phoenix/support/vector.hpp>
#include <hboost/proto/transform/fold.hpp>
#include <hboost/proto/transform/lazy.hpp>

namespace hboost { namespace hboostphoenix
{
    struct v2_eval
        : proto::callable
    {
        template <typename Sig>
        struct result;

        template <typename This, typename Eval, typename Env>
        struct result<This(Eval, Env)>
            : Eval::template result<typename proto::detail::uncvref<Env>::type>
        {};

        template <typename This, typename Eval, typename Env>
        struct result<This(Eval &, Env)>
            : Eval::template result<typename proto::detail::uncvref<Env>::type>
        {};

        template <typename This, typename Eval, typename Env>
        struct result<This(Eval const &, Env)>
            : Eval::template result<typename proto::detail::uncvref<Env>::type>
        {};

        template <typename Eval, typename Env>
        typename result<v2_eval(Eval const&, Env)>::type
        operator()(Eval const & e, Env const & env) const
        {
            return e.eval(env);
        }
    };
}}

#endif
