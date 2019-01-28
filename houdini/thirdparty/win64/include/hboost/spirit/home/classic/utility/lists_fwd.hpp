/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_LISTS_FWD_HPP)
#define HBOOST_SPIRIT_LISTS_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/core/parser.hpp>

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    struct no_list_endtoken;

    template <
        typename ItemT, typename DelimT, typename EndT = no_list_endtoken,
        typename CategoryT = plain_parser_category
    >
    struct list_parser;

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif

