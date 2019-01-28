//-----------------------------------------------------------------------------
// boost variant/detail/move.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
//  Copyright (c) 2002-2003 Eric Friedman
//  Copyright (c) 2002 by Andrei Alexandrescu
//  Copyright (c) 2013-2014 Antony Polukhin
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  This file derivative of MoJO. Much thanks to Andrei for his initial work.
//  See <http://www.cuj.com/experts/2102/alexandr.htm> for information on MOJO.
//  Re-issued here under the Boost Software License, with permission of the original
//  author (Andrei Alexandrescu).


#ifndef HBOOST_VARIANT_DETAIL_MOVE_HPP
#define HBOOST_VARIANT_DETAIL_MOVE_HPP

#include <iterator> // for iterator_traits
#include <new> // for placement new

#include "hboost/config.hpp"
#include "hboost/detail/workaround.hpp"
#include "hboost/move/move.hpp"
#include "hboost/move/adl_move_swap.hpp"

namespace hboost { namespace detail { namespace variant {

using hboost::move;

//////////////////////////////////////////////////////////////////////////
// function template move_swap
//
// Swaps using Koenig lookup but falls back to move-swap for primitive
// types and on non-conforming compilers.
//

template <typename T>
inline void move_swap(T& lhs, T& rhs)
{
    ::hboost::adl_move_swap(lhs, rhs);
}

}}} // namespace hboost::detail::variant

#endif // HBOOST_VARIANT_DETAIL_MOVE_HPP



