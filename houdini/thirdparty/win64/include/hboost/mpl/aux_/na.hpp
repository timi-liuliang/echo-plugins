
#ifndef HBOOST_MPL_AUX_NA_HPP_INCLUDED
#define HBOOST_MPL_AUX_NA_HPP_INCLUDED

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

#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/aux_/na_fwd.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>

namespace hboost { namespace mpl {

template< typename T >
struct is_na
    : false_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using false_::value;
#endif
};

template<>
struct is_na<na>
    : true_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using true_::value;
#endif
};

template< typename T >
struct is_not_na
    : true_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using true_::value;
#endif
};

template<>
struct is_not_na<na>
    : false_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using false_::value;
#endif
};

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
template< typename T, typename U > struct if_na
{
    typedef T type;
};

template< typename U > struct if_na<na,U>
{
    typedef U type;
};
#else
template< typename T > struct if_na_impl
{
    template< typename U > struct apply
    {
        typedef T type;
    };
};

template<> struct if_na_impl<na>
{
    template< typename U > struct apply
    {
        typedef U type;
    };
};

template< typename T, typename U > struct if_na
    : if_na_impl<T>::template apply<U>
{
};
#endif

}}

#endif // HBOOST_MPL_AUX_NA_HPP_INCLUDED
