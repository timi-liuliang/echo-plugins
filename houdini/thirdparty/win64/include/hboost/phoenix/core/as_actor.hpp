/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_CORE_AS_ACTOR_HPP
#define HBOOST_PHOENIX_CORE_AS_ACTOR_HPP

#include <hboost/phoenix/core/actor.hpp>
#include <hboost/fusion/support/void.hpp>

namespace hboost { namespace hboostphoenix
{
    template <typename T, typename U = typename is_actor<T>::type >
    struct as_actor
    {
        typedef T type;

        static type const &
        convert(T const & t)
        {
            return t;
        }
    };
}}

#endif
