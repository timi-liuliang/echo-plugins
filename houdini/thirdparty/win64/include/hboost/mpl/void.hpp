
#ifndef HBOOST_MPL_VOID_HPP_INCLUDED
#define HBOOST_MPL_VOID_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/void_fwd.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

//  [JDG Feb-4-2003] made void_ a complete type to allow it to be
//  instantiated so that it can be passed in as an object that can be
//  used to select an overloaded function. Possible use includes signaling
//  a zero arity functor evaluation call.
struct void_ { typedef void_ type; };

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

namespace hboost { namespace mpl {

template< typename T >
struct is_void_
    : false_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using false_::value;
#endif
};

template<>
struct is_void_<void_>
    : true_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using true_::value;
#endif
};

template< typename T >
struct is_not_void_
    : true_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using true_::value;
#endif
};

template<>
struct is_not_void_<void_>
    : false_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using false_::value;
#endif
};

HBOOST_MPL_AUX_NA_SPEC(1, is_void_)
HBOOST_MPL_AUX_NA_SPEC(1, is_not_void_)

}}

#endif // HBOOST_MPL_VOID_HPP_INCLUDED
