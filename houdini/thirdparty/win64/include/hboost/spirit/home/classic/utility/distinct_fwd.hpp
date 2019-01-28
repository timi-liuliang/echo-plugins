/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_DISTINCT_FWD_HPP)
#define HBOOST_SPIRIT_DISTINCT_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    template<typename CharT> class chset;

    template <typename CharT = char, typename TailT = chset<CharT> >
    class distinct_parser;

    template <typename CharT = char, typename TailT = chset<CharT> >
    class distinct_directive;

    template <typename ScannerT = scanner<> >
    class dynamic_distinct_parser;

    template <typename ScannerT = scanner<> >
    class dynamic_distinct_directive;

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif


