//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_STD_FWD_HPP
#define HBOOST_INTRUSIVE_DETAIL_STD_FWD_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

//////////////////////////////////////////////////////////////////////////////
//                        Standard predeclarations
//////////////////////////////////////////////////////////////////////////////

#include <hboost/move/detail/std_ns_begin.hpp>
HBOOST_MOVE_STD_NS_BEG

template<class T>
struct less;

template<class T>
struct equal_to;

struct input_iterator_tag;
struct forward_iterator_tag;
struct bidirectional_iterator_tag;
struct random_access_iterator_tag;

HBOOST_MOVE_STD_NS_END
#include <hboost/move/detail/std_ns_end.hpp>

#endif //#ifndef HBOOST_INTRUSIVE_DETAIL_STD_FWD_HPP
