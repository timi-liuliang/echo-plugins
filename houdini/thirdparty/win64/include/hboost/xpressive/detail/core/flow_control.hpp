///////////////////////////////////////////////////////////////////////////////
// flow_control.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_DETAIL_CORE_FLOW_CONTROL_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_DETAIL_CORE_FLOW_CONTROL_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/xpressive/detail/detail_fwd.hpp>
#include <hboost/xpressive/detail/core/regex_impl.hpp>
#include <hboost/xpressive/detail/core/state.hpp>
#include <hboost/xpressive/detail/utility/ignore_unused.hpp>

namespace hboost { namespace xpressive { namespace detail
{

///////////////////////////////////////////////////////////////////////////////
// push_context_match
//
template<typename BidiIter>
inline bool push_context_match
(
    regex_impl<BidiIter> const &impl
  , match_state<BidiIter> &state
  , matchable<BidiIter> const &next
)
{
    // avoid infinite recursion
    // BUGBUG this only catches direct infinite recursion, like sregex::compile("(?R)"), but
    // not indirect infinite recursion where two rules invoke each other recursively.
    if(state.is_active_regex(impl) && state.cur_ == state.sub_match(0).begin_)
    {
        return next.match(state);
    }

    // save state
    match_context<BidiIter> context = state.push_context(impl, next, context);
    detail::ignore_unused(context);

    // match the nested regex and uninitialize the match context
    // (reclaims the sub_match objects if necessary)
    return state.pop_context(impl, impl.xpr_->match(state));
}

///////////////////////////////////////////////////////////////////////////////
// pop_context_match
//
template<typename BidiIter>
inline bool pop_context_match(match_state<BidiIter> &state)
{
    // save state
    // BUGBUG nested regex could have changed state.traits_
    match_context<BidiIter> &context(*state.context_.prev_context_);
    state.swap_context(context);

    // Finished matching the nested regex; now match the rest of the enclosing regex
    bool success = context.next_ptr_->match(state);

    // restore state
    state.swap_context(context);
    return success;
}

}}} // namespace hboost::xpressive::detail

#endif

