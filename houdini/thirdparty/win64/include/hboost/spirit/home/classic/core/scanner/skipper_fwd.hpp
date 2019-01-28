/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_SKIPPER_FWD_HPP)
#define HBOOST_SPIRIT_SKIPPER_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/core/scanner/scanner_fwd.hpp>

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    template <typename BaseT = iteration_policy>
    struct skipper_iteration_policy;

    template <typename BaseT = iteration_policy>
    struct no_skipper_iteration_policy; 

    template <typename ParserT, typename BaseT = iteration_policy>
    class skip_parser_iteration_policy;

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif

