///////////////////////////////////////////////////////////////////////////////
// regex_matcher.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_DETAIL_CORE_MATCHER_REGEX_MATCHER_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_DETAIL_CORE_MATCHER_REGEX_MATCHER_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/mpl/assert.hpp>
#include <hboost/xpressive/regex_error.hpp>
#include <hboost/xpressive/regex_constants.hpp>
#include <hboost/xpressive/detail/core/regex_impl.hpp>
#include <hboost/xpressive/detail/detail_fwd.hpp>
#include <hboost/xpressive/detail/core/quant_style.hpp>
#include <hboost/xpressive/detail/core/state.hpp>
#include <hboost/xpressive/detail/core/adaptor.hpp>

namespace hboost { namespace xpressive { namespace detail
{

    ///////////////////////////////////////////////////////////////////////////////
    // regex_matcher
    //
    template<typename BidiIter>
    struct regex_matcher
      : quant_style<quant_variable_width, unknown_width::value, false>
    {
        regex_impl<BidiIter> impl_;

        regex_matcher(shared_ptr<regex_impl<BidiIter> > const &impl)
          : impl_()
        {
            this->impl_.xpr_ = impl->xpr_;
            this->impl_.traits_ = impl->traits_;
            this->impl_.mark_count_ = impl->mark_count_;
            this->impl_.hidden_mark_count_ = impl->hidden_mark_count_;

            HBOOST_XPR_ENSURE_(this->impl_.xpr_, regex_constants::error_badref, "bad regex reference");
        }

        template<typename Next>
        bool match(match_state<BidiIter> &state, Next const &next) const
        {
            // regex_matcher is used for embeding a dynamic regex in a static regex. As such,
            // Next will always point to a static regex.
            HBOOST_MPL_ASSERT((is_static_xpression<Next>));

            // wrap the static xpression in a matchable interface
            xpression_adaptor<reference_wrapper<Next const>, matchable<BidiIter> > adaptor(hboost::cref(next));
            return push_context_match(this->impl_, state, adaptor);
        }
    };

}}}

#endif
