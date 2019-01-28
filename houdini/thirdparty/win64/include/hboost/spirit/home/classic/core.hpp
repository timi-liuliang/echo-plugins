/*=============================================================================
    Copyright (c) 1998-2003 Joel de Guzman
    Copyright (c) 2001-2003 Daniel Nuffer
    Copyright (c) 2001-2003 Hartmut Kaiser
    Copyright (c) 2002-2003 Martin Wille
    Copyright (c) 2002 Raghavendra Satish
    Copyright (c) 2001 Bruce Florman
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_CORE_MAIN_HPP)
#define HBOOST_SPIRIT_CORE_MAIN_HPP

#include <hboost/spirit/home/classic/version.hpp>
#include <hboost/spirit/home/classic/debug.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.Core includes
//
///////////////////////////////////////////////////////////////////////////////

//  Spirit.Core.Kernel
#include <hboost/spirit/home/classic/core/config.hpp>
#include <hboost/spirit/home/classic/core/nil.hpp>
#include <hboost/spirit/home/classic/core/match.hpp>
#include <hboost/spirit/home/classic/core/parser.hpp>

//  Spirit.Core.Primitives
#include <hboost/spirit/home/classic/core/primitives/primitives.hpp>
#include <hboost/spirit/home/classic/core/primitives/numerics.hpp>

//  Spirit.Core.Scanner
#include <hboost/spirit/home/classic/core/scanner/scanner.hpp>
#include <hboost/spirit/home/classic/core/scanner/skipper.hpp>

//  Spirit.Core.NonTerminal
#include <hboost/spirit/home/classic/core/non_terminal/subrule.hpp>
#include <hboost/spirit/home/classic/core/non_terminal/rule.hpp>
#include <hboost/spirit/home/classic/core/non_terminal/grammar.hpp>

//  Spirit.Core.Composite
#include <hboost/spirit/home/classic/core/composite/actions.hpp>
#include <hboost/spirit/home/classic/core/composite/composite.hpp>
#include <hboost/spirit/home/classic/core/composite/directives.hpp>
#include <hboost/spirit/home/classic/core/composite/epsilon.hpp>
#include <hboost/spirit/home/classic/core/composite/sequence.hpp>
#include <hboost/spirit/home/classic/core/composite/sequential_and.hpp>
#include <hboost/spirit/home/classic/core/composite/sequential_or.hpp>
#include <hboost/spirit/home/classic/core/composite/alternative.hpp>
#include <hboost/spirit/home/classic/core/composite/difference.hpp>
#include <hboost/spirit/home/classic/core/composite/intersection.hpp>
#include <hboost/spirit/home/classic/core/composite/exclusive_or.hpp>
#include <hboost/spirit/home/classic/core/composite/kleene_star.hpp>
#include <hboost/spirit/home/classic/core/composite/positive.hpp>
#include <hboost/spirit/home/classic/core/composite/optional.hpp>
#include <hboost/spirit/home/classic/core/composite/list.hpp>
#include <hboost/spirit/home/classic/core/composite/no_actions.hpp>

//  Deprecated interface includes
#include <hboost/spirit/home/classic/actor/assign_actor.hpp>
#include <hboost/spirit/home/classic/actor/push_back_actor.hpp>

#if defined(HBOOST_SPIRIT_DEBUG)
    //////////////////////////////////
    #include <hboost/spirit/home/classic/debug/parser_names.hpp>

#endif // HBOOST_SPIRIT_DEBUG

#endif // HBOOST_SPIRIT_CORE_MAIN_HPP

