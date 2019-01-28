/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_ESCAPE_CHAR_FWD_HPP)
#define HBOOST_SPIRIT_ESCAPE_CHAR_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    template <unsigned long Flags, typename CharT = char>
    struct escape_char_parser;

    template <
        class ParserT, typename ActionT,
        unsigned long Flags, typename CharT = char>
    struct escape_char_action;

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif

