/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_CORE_NOTHING_HPP
#define HBOOST_PHOENIX_CORE_NOTHING_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/mpl/void.hpp>
#include <hboost/phoenix/core/actor.hpp>
#include <hboost/phoenix/core/call.hpp>
#include <hboost/phoenix/core/expression.hpp>
#include <hboost/phoenix/core/value.hpp>

namespace hboost { namespace hboostphoenix
{
    /////////////////////////////////////////////////////////////////////////////
    //
    //  null_actor
    //
    //      An actor that does nothing (a "bum", if you will :-).
    //
    /////////////////////////////////////////////////////////////////////////////
    
    namespace detail
    {
        struct nothing {};
    }
    
    namespace expression
    {
        struct null
            : expression::value<detail::nothing>
        {};
    }
    
    template<typename Dummy>
    struct is_custom_terminal<detail::nothing, Dummy>
      : mpl::true_
    {};

    template<typename Dummy>
    struct custom_terminal<detail::nothing, Dummy>
    {
        typedef void result_type;
        template <typename Context>
        void operator()(detail::nothing, Context &) const
        {
        }
    };

    typedef expression::null::type nothing_type;
#ifndef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
    nothing_type const nothing = {{{}}};
#endif
}}

#endif
