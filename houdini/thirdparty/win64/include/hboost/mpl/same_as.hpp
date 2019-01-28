
#ifndef HBOOST_MPL_SAME_AS_HPP_INCLUDED
#define HBOOST_MPL_SAME_AS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/not.hpp>
#include <hboost/mpl/aux_/lambda_spec.hpp>
#include <hboost/mpl/aux_/config/forwarding.hpp>

#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace mpl {

template< typename T1 >
struct same_as
{
    template< typename T2 > struct apply
#if !defined(HBOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : is_same<T1,T2>
    {
#else
    {
        typedef typename is_same<T1,T2>::type type;
#endif
    };
};

template< typename T1 >
struct not_same_as
{
    template< typename T2 > struct apply
#if !defined(HBOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : not_< is_same<T1,T2> >
    {
#else
    {
        typedef typename not_< is_same<T1,T2> >::type type;
#endif
    };
};

}}

#endif // HBOOST_MPL_SAME_AS_HPP_INCLUDED
