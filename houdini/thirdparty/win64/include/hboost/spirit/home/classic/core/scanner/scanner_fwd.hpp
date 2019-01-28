/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_SCANNER_FWD_HPP)
#define HBOOST_SPIRIT_SCANNER_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    ///////////////////////////////////////////////////////////////////////////
    //
    //  policy classes
    //
    ///////////////////////////////////////////////////////////////////////////
    struct iteration_policy;
    struct action_policy;
    struct match_policy;

    ///////////////////////////////////////////////////////////////////////////
    //
    //  scanner_policies class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename IterationPolicyT   = iteration_policy,
        typename MatchPolicyT       = match_policy,
        typename ActionPolicyT      = action_policy>
    struct scanner_policies;

    ///////////////////////////////////////////////////////////////////////////
    //
    //  scanner class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename IteratorT  = char const*,
        typename PoliciesT  = scanner_policies<> >
    class scanner;

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif
