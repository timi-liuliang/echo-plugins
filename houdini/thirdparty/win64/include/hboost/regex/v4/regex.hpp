/*
 *
 * Copyright (c) 1998-2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex.cpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: Declares hboost::basic_regex<> and associated
  *                functions and classes. This header is the main
  *                entry point for the template regex code.
  */

#ifndef HBOOST_RE_REGEX_HPP_INCLUDED
#define HBOOST_RE_REGEX_HPP_INCLUDED

#ifdef __cplusplus

// what follows is all C++ don't include in C builds!!

#ifndef HBOOST_REGEX_CONFIG_HPP
#include <hboost/regex/config.hpp>
#endif
#ifndef HBOOST_REGEX_WORKAROUND_HPP
#include <hboost/regex/v4/regex_workaround.hpp>
#endif

#ifndef HBOOST_REGEX_FWD_HPP
#include <hboost/regex_fwd.hpp>
#endif
#ifndef HBOOST_REGEX_TRAITS_HPP
#include <hboost/regex/regex_traits.hpp>
#endif
#ifndef HBOOST_REGEX_RAW_BUFFER_HPP
#include <hboost/regex/v4/error_type.hpp>
#endif
#ifndef HBOOST_REGEX_V4_MATCH_FLAGS
#include <hboost/regex/v4/match_flags.hpp>
#endif
#ifndef HBOOST_REGEX_RAW_BUFFER_HPP
#include <hboost/regex/v4/regex_raw_buffer.hpp>
#endif
#ifndef HBOOST_RE_PAT_EXCEPT_HPP
#include <hboost/regex/pattern_except.hpp>
#endif

#ifndef HBOOST_REGEX_V4_CHAR_REGEX_TRAITS_HPP
#include <hboost/regex/v4/char_regex_traits.hpp>
#endif
#ifndef HBOOST_REGEX_V4_STATES_HPP
#include <hboost/regex/v4/states.hpp>
#endif
#ifndef HBOOST_REGEX_V4_REGBASE_HPP
#include <hboost/regex/v4/regbase.hpp>
#endif
#ifndef HBOOST_REGEX_V4_ITERATOR_TRAITS_HPP
#include <hboost/regex/v4/iterator_traits.hpp>
#endif
#ifndef HBOOST_REGEX_V4_BASIC_REGEX_HPP
#include <hboost/regex/v4/basic_regex.hpp>
#endif
#ifndef HBOOST_REGEX_V4_BASIC_REGEX_CREATOR_HPP
#include <hboost/regex/v4/basic_regex_creator.hpp>
#endif
#ifndef HBOOST_REGEX_V4_BASIC_REGEX_PARSER_HPP
#include <hboost/regex/v4/basic_regex_parser.hpp>
#endif
#ifndef HBOOST_REGEX_V4_SUB_MATCH_HPP
#include <hboost/regex/v4/sub_match.hpp>
#endif
#ifndef HBOOST_REGEX_FORMAT_HPP
#include <hboost/regex/v4/regex_format.hpp>
#endif
#ifndef HBOOST_REGEX_V4_MATCH_RESULTS_HPP
#include <hboost/regex/v4/match_results.hpp>
#endif
#ifndef HBOOST_REGEX_V4_PROTECTED_CALL_HPP
#include <hboost/regex/v4/protected_call.hpp>
#endif
#ifndef HBOOST_REGEX_MATCHER_HPP
#include <hboost/regex/v4/perl_matcher.hpp>
#endif
//
// template instances:
//
#define HBOOST_REGEX_CHAR_T char
#ifdef HBOOST_REGEX_NARROW_INSTANTIATE
#  define HBOOST_REGEX_INSTANTIATE
#endif
#include <hboost/regex/v4/instances.hpp>
#undef HBOOST_REGEX_CHAR_T
#ifdef HBOOST_REGEX_INSTANTIATE
#  undef HBOOST_REGEX_INSTANTIATE
#endif

#ifndef HBOOST_NO_WREGEX
#define HBOOST_REGEX_CHAR_T wchar_t
#ifdef HBOOST_REGEX_WIDE_INSTANTIATE
#  define HBOOST_REGEX_INSTANTIATE
#endif
#include <hboost/regex/v4/instances.hpp>
#undef HBOOST_REGEX_CHAR_T
#ifdef HBOOST_REGEX_INSTANTIATE
#  undef HBOOST_REGEX_INSTANTIATE
#endif
#endif

#if !defined(HBOOST_NO_WREGEX) && defined(HBOOST_REGEX_HAS_OTHER_WCHAR_T)
#define HBOOST_REGEX_CHAR_T unsigned short
#ifdef HBOOST_REGEX_US_INSTANTIATE
#  define HBOOST_REGEX_INSTANTIATE
#endif
#include <hboost/regex/v4/instances.hpp>
#undef HBOOST_REGEX_CHAR_T
#ifdef HBOOST_REGEX_INSTANTIATE
#  undef HBOOST_REGEX_INSTANTIATE
#endif
#endif


namespace hboost{
#ifdef HBOOST_REGEX_NO_FWD
typedef basic_regex<char, regex_traits<char> > regex;
#ifndef HBOOST_NO_WREGEX
typedef basic_regex<wchar_t, regex_traits<wchar_t> > wregex;
#endif
#endif

typedef match_results<const char*> cmatch;
typedef match_results<std::string::const_iterator> smatch;
#ifndef HBOOST_NO_WREGEX
typedef match_results<const wchar_t*> wcmatch;
typedef match_results<std::wstring::const_iterator> wsmatch;
#endif

} // namespace hboost
#ifndef HBOOST_REGEX_MATCH_HPP
#include <hboost/regex/v4/regex_match.hpp>
#endif
#ifndef HBOOST_REGEX_V4_REGEX_SEARCH_HPP
#include <hboost/regex/v4/regex_search.hpp>
#endif
#ifndef HBOOST_REGEX_ITERATOR_HPP
#include <hboost/regex/v4/regex_iterator.hpp>
#endif
#ifndef HBOOST_REGEX_TOKEN_ITERATOR_HPP
#include <hboost/regex/v4/regex_token_iterator.hpp>
#endif
#ifndef HBOOST_REGEX_V4_REGEX_GREP_HPP
#include <hboost/regex/v4/regex_grep.hpp>
#endif
#ifndef HBOOST_REGEX_V4_REGEX_REPLACE_HPP
#include <hboost/regex/v4/regex_replace.hpp>
#endif
#ifndef HBOOST_REGEX_V4_REGEX_MERGE_HPP
#include <hboost/regex/v4/regex_merge.hpp>
#endif
#ifndef HBOOST_REGEX_SPLIT_HPP
#include <hboost/regex/v4/regex_split.hpp>
#endif

#endif  // __cplusplus

#endif  // include































