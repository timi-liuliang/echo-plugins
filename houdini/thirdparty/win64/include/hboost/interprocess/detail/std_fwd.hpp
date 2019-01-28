//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_INTERPROCESS_DETAIL_STD_FWD_HPP
#define HBOOST_INTERPROCESS_DETAIL_STD_FWD_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

//////////////////////////////////////////////////////////////////////////////
//                        Standard predeclarations
//////////////////////////////////////////////////////////////////////////////

#include <hboost/move/detail/std_ns_begin.hpp>
HBOOST_MOVE_STD_NS_BEG

struct input_iterator_tag;
struct forward_iterator_tag;
struct bidirectional_iterator_tag;
struct random_access_iterator_tag;

template<class T>
struct char_traits;

#if defined(HBOOST_MSVC) && (HBOOST_MSVC >= 1800) &&defined(HBOOST_DINKUMWARE_STDLIB)
#define HBOOST_INTERPROCESS_STD_FWD_MSVC_IOS_BUG
// Compiler bug workaround. Previous versions (<= VC11)
// used dummy virtual functions
#  pragma vtordisp(push, 2)
#endif

template<class CharT, class Traits>
class basic_ostream;

template<class CharT, class Traits>
class basic_istream;

#ifdef HBOOST_INTERPROCESS_STD_FWD_MSVC_IOS_BUG
#  pragma vtordisp(pop)
#  undef HBOOST_INTERPROCESS_STD_FWD_MSVC_IOS_BUG
#endif

HBOOST_MOVE_STD_NS_END
#include <hboost/move/detail/std_ns_end.hpp>

#endif //#ifndef HBOOST_INTERPROCESS_DETAIL_STD_FWD_HPP
