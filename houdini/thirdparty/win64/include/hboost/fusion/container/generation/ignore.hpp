/*=============================================================================
    Copyright (c) 2001 Doug Gregor
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_IGNORE_07192005_0329)
#define HBOOST_FUSION_IGNORE_07192005_0329

#include <hboost/fusion/support/config.hpp>

namespace hboost { namespace fusion
{
    //  Swallows any assignment (by Doug Gregor)
    namespace detail
    {
        struct swallow_assign
        {
            template<typename T>
            HBOOST_FUSION_CONSTEXPR_THIS HBOOST_FUSION_GPU_ENABLED
            swallow_assign const&
            operator=(const T&) const
            {
                return *this;
            }
        };
    }

    //  "ignore" allows tuple positions to be ignored when using "tie".
    HBOOST_CONSTEXPR_OR_CONST detail::swallow_assign ignore = detail::swallow_assign();
}}

#endif
