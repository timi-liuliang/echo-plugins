/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_CORE_TERMINAL_FWD_HPP
#define HBOOST_PHOENIX_CORE_TERMINAL_FWD_HPP

namespace hboost { namespace hboostphoenix
{
    namespace rule
    {
        struct argument;
        struct custom_terminal;
        struct terminal;
    }

    template <typename T, typename Dummy = void>
    struct is_custom_terminal;

    template <typename T, typename Dummy = void>
    struct custom_terminal;
}}

#endif
