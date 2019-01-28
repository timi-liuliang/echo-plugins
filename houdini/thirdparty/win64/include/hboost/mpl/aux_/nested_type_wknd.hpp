
#ifndef HBOOST_MPL_AUX_NESTED_TYPE_WKND_HPP_INCLUDED
#define HBOOST_MPL_AUX_NESTED_TYPE_WKND_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, HBOOST_TESTED_AT(0x0302)) \
    || HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x561)) \
    || HBOOST_WORKAROUND(__SUNPRO_CC, HBOOST_TESTED_AT(0x530)) \
    || HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))

namespace hboost { namespace mpl { namespace aux {
template< typename T > struct nested_type_wknd
    : T::type
{
};
}}}

#if HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))
#   define HBOOST_MPL_AUX_NESTED_TYPE_WKND(T) \
    aux::nested_type_wknd<T> \
/**/
#else
#   define HBOOST_MPL_AUX_NESTED_TYPE_WKND(T) \
    ::hboost::mpl::aux::nested_type_wknd<T> \
/**/
#endif

#else // !HBOOST_MPL_CFG_GCC et al.

#   define HBOOST_MPL_AUX_NESTED_TYPE_WKND(T) T::type

#endif 

#endif // HBOOST_MPL_AUX_NESTED_TYPE_WKND_HPP_INCLUDED
