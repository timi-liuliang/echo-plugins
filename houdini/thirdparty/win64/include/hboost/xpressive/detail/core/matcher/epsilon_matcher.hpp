///////////////////////////////////////////////////////////////////////////////
// epsilon_matcher.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_DETAIL_CORE_MATCHER_EPSILON_MATCHER_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_DETAIL_CORE_MATCHER_EPSILON_MATCHER_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/xpressive/detail/detail_fwd.hpp>
#include <hboost/xpressive/detail/core/quant_style.hpp>
#include <hboost/xpressive/detail/core/state.hpp>

namespace hboost { namespace xpressive { namespace detail
{

    ///////////////////////////////////////////////////////////////////////////////
    // epsilon_matcher
    //
    struct epsilon_matcher
    {
        HBOOST_XPR_QUANT_STYLE(quant_none, 0, true)

        template<typename BidiIter, typename Next>
        static bool match(match_state<BidiIter> &state, Next const &next)
        {
            return next.match(state);
        }
    };

}}}

#endif