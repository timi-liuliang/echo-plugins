///////////////////////////////////////////////////////////////////////////////
/// \file xpressive_static.hpp
/// Includes everything you need to write static regular expressions and use
/// them.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_STATIC_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_STATIC_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#ifdef _MSC_VER
// inline aggressively
# pragma inline_recursion(on) // turn on inline recursion
# pragma inline_depth(255)    // max inline depth
#endif

#include <hboost/xpressive/regex_primitives.hpp>
#include <hboost/xpressive/basic_regex.hpp>
#include <hboost/xpressive/sub_match.hpp>
#include <hboost/xpressive/match_results.hpp>
#include <hboost/xpressive/regex_algorithms.hpp>
#include <hboost/xpressive/regex_iterator.hpp>
#include <hboost/xpressive/regex_token_iterator.hpp>

#endif