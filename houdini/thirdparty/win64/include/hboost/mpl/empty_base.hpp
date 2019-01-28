
#ifndef HBOOST_MPL_EMPTY_BASE_HPP_INCLUDED
#define HBOOST_MPL_EMPTY_BASE_HPP_INCLUDED

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
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/type_traits/is_empty.hpp>

namespace hboost { namespace mpl {

// empty base class, guaranteed to have no members; inheritance from
// 'empty_base' through the 'inherit' metafunction is a no-op - see 
// "mpl/inherit.hpp> header for the details
struct empty_base {};

template< typename T >
struct is_empty_base
    : false_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using false_::value;
#endif
};

template<>
struct is_empty_base<empty_base>
    : true_
{
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
    using true_::value;
#endif
};

}}

namespace hboost {

template<> struct is_empty< mpl::empty_base >
    : public ::hboost::integral_constant<bool,true>
{
public:
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(1,is_empty,(mpl::empty_base))
};

}

#endif // HBOOST_MPL_EMPTY_BASE_HPP_INCLUDED
