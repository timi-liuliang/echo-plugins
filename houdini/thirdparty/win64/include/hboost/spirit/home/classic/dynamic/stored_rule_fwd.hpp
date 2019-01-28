/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_STORED_RULE_FWD_HPP)
#define HBOOST_SPIRIT_STORED_RULE_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/core/nil.hpp>

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    template <
        typename T0 = nil_t
      , typename T1 = nil_t
      , typename T2 = nil_t
      , bool EmbedByValue = false
    >
    class stored_rule;

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif

