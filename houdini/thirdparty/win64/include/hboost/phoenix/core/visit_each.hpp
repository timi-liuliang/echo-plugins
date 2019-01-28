/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_CORE_VISIT_EACH_HPP
#define HBOOST_PHOENIX_CORE_VISIT_EACH_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/fusion/algorithm/iteration/for_each.hpp>
#include <hboost/visit_each.hpp>

namespace hboost { namespace hboostphoenix
{
    template <typename> struct actor;

    namespace detail
    {
        template <typename Visitor>
        struct visit_each_impl
        {
            Visitor& visitor;
            visit_each_impl(Visitor& visitor_ ) : visitor(visitor_) {}

            template <typename T>
            void operator()(T const& t) const
            {
                using hboost::visit_each;
                visit_each(visitor, t);
            }
        };
    }

    template <typename Visitor, typename Expr>
    inline void visit_each(Visitor& visitor, actor<Expr> const& a, long)
    {
        fusion::for_each(a, detail::visit_each_impl<Visitor>(visitor));
    }

    template <typename Visitor, typename Expr>
    inline void visit_each(Visitor& visitor, actor<Expr> const& a)
    {
        fusion::for_each(a, detail::visit_each_impl<Visitor>(visitor));
    }
}}

#endif
