/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_SUBRULE_FWD_HPP)
#define HBOOST_SPIRIT_SUBRULE_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/core/non_terminal/parser_context.hpp>

namespace hboost { namespace spirit  {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    template <int ID, typename ContextT = parser_context<> >
    struct subrule; 

    template <int ID, typename DefT, typename ContextT = parser_context<> >
    struct subrule_parser;

    template <typename ScannerT, typename ListT>
    struct subrules_scanner;

    template <typename FirstT, typename RestT>
    struct subrule_list; 

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif

