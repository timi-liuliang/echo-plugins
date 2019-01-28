/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010-2011 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_CORE_ARGUMENT_HPP
#define HBOOST_PHOENIX_CORE_ARGUMENT_HPP

#include <hboost/phoenix/core/limits.hpp>
#include <hboost/phoenix/core/actor.hpp>
#include <hboost/phoenix/core/expression.hpp>
#include <hboost/phoenix/core/terminal.hpp>
#include <hboost/mpl/int.hpp>

namespace hboost { namespace hboostphoenix
{
    ////////////////////////////////////////////////////////////////////////////
    //
    //  argument
    //
    //      function for evaluating argument placeholders like: _1
    //
    ////////////////////////////////////////////////////////////////////////////
    
    template <int I>
    struct argument
        //: mpl::int_<I>
    {
        typedef typename mpl::int_<I>::value_type value_type;
        static const value_type value = mpl::int_<I>::value;
        
        bool operator==(argument) const
        {
            return true;
        }
        
        template <int I2>
        bool operator==(argument<I2>) const
        {
            return false;
        }
    };
}}

namespace hboost {
    template <int I>
    struct is_placeholder<hboostphoenix::argument<I> >
        : mpl::int_<I>
    {};
}

namespace hboost { namespace hboostphoenix
{
    namespace expression
    {
        template <int I>
        struct argument
            : expression::terminal<hboostphoenix::argument<I> >
        {
            typedef typename expression::terminal<hboostphoenix::argument<I> >::type type;
            static const type make()
            {
                type const e = {{{}}};
                return e;
            }
        };
    }

#if !defined(HBOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)

#include <hboost/phoenix/core/preprocessed/argument.hpp>

#else

#if !defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#include <hboost/phoenix/core/detail/argument.hpp>
#else

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/argument_predefined_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
#endif
/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010-2011 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

#ifdef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
#undef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
#define HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS_RESTORE
#endif

#include <hboost/phoenix/core/detail/argument.hpp>

#ifndef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS_RESTORE
#define HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
#undef HBOOST_PHOENIX_NO_PREDEFINED_TERIMINALS_RESTORE
#endif

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/argument_no_predefined_" HBOOST_PHOENIX_LIMIT_STR ".hpp")
#endif
/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010-2011 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

#ifndef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
#define HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
#define HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS_RESTORE
#endif

#include <hboost/phoenix/core/detail/argument.hpp>

#ifndef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS_RESTORE
#undef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
#undef HBOOST_PHOENIX_NO_PREDEFINED_TERIMINALS_RESTORE
#endif

#if defined(__WAVE__) && defined(HBOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif

#endif

}}

#endif
