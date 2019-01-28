
#ifndef HBOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED
#define HBOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED

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

#include <hboost/mpl/aux_/static_cast.hpp>
#include <hboost/mpl/aux_/config/integral.hpp>
#include <hboost/mpl/aux_/config/eti.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if defined(HBOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS) \
    || defined(HBOOST_MPL_CFG_MSVC_60_ETI_BUG)

#   include <hboost/mpl/int.hpp>

namespace hboost { namespace mpl { namespace aux {
template< typename C_ > struct value_wknd
    : C_
{
};

#if defined(HBOOST_MPL_CFG_MSVC_60_ETI_BUG)
template<> struct value_wknd<int>
    : int_<1>
{
    using int_<1>::value;
};
#endif
}}}


#if !defined(HBOOST_MPL_CFG_MSVC_60_ETI_BUG)
#   define HBOOST_MPL_AUX_VALUE_WKND(C) \
    ::HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::aux::value_wknd< C > \
/**/
#    define HBOOST_MPL_AUX_MSVC_VALUE_WKND(C) HBOOST_MPL_AUX_VALUE_WKND(C)
#else
#   define HBOOST_MPL_AUX_VALUE_WKND(C) C
#   define HBOOST_MPL_AUX_MSVC_VALUE_WKND(C) \
    ::hboost::mpl::aux::value_wknd< C > \
/**/
#endif

#else // HBOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS

#   define HBOOST_MPL_AUX_VALUE_WKND(C) C
#   define HBOOST_MPL_AUX_MSVC_VALUE_WKND(C) C

#endif

#if HBOOST_WORKAROUND(__EDG_VERSION__, <= 238)
#   define HBOOST_MPL_AUX_NESTED_VALUE_WKND(T, C) \
    HBOOST_MPL_AUX_STATIC_CAST(T, C::value) \
/**/
#else
#   define HBOOST_MPL_AUX_NESTED_VALUE_WKND(T, C) \
    HBOOST_MPL_AUX_VALUE_WKND(C)::value \
/**/
#endif


namespace hboost { namespace mpl { namespace aux {

template< typename T > struct value_type_wknd
{
    typedef typename T::value_type type;
};

#if defined(HBOOST_MPL_CFG_MSVC_ETI_BUG)
template<> struct value_type_wknd<int>
{
    typedef int type;
};
#endif

}}}

#endif // HBOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED
