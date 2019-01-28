//-----------------------------------------------------------------------------
// boost variant/detail/over_sequence.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Portions Copyright (C) 2002 David Abrahams
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_OVER_SEQUENCE_HPP
#define HBOOST_VARIANT_DETAIL_OVER_SEQUENCE_HPP

#include "hboost/mpl/aux_/config/ctps.hpp"


namespace hboost {
namespace detail { namespace variant {

///////////////////////////////////////////////////////////////////////////////
// (detail) class over_sequence
//
// Wrapper used to indicate bounded types for variant are from type sequence.
//
template <typename Types>
struct over_sequence
{
    typedef Types type;
};

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction is_over_sequence (modeled on code by David Abrahams)
//
// Indicates whether the specified type is of form over_sequence<...> or not.
//


template <typename T>
struct is_over_sequence
    : mpl::false_
{
};

template <typename Types>
struct is_over_sequence< over_sequence<Types> >
    : mpl::true_
{
};


}} // namespace detail::variant
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_OVER_SEQUENCE_HPP
