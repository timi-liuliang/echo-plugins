
#ifndef HBOOST_MPL_INTEGRAL_C_HPP_INCLUDED
#define HBOOST_MPL_INTEGRAL_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2006
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/integral_c_fwd.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if HBOOST_WORKAROUND(__HP_aCC, <= 53800)
// the type of non-type template arguments may not depend on template arguments
#   define AUX_WRAPPER_PARAMS(N) typename T, long N
#else
#   define AUX_WRAPPER_PARAMS(N) typename T, T N
#endif

#define AUX_WRAPPER_NAME integral_c
#define AUX_WRAPPER_VALUE_TYPE T
#define AUX_WRAPPER_INST(value) AUX_WRAPPER_NAME< T, value >
#include <hboost/mpl/aux_/integral_wrapper.hpp>


#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
 && !HBOOST_WORKAROUND(__BORLANDC__, <= 0x551)
HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
// 'bool' constant doesn't have 'next'/'prior' members
template< bool C >
struct integral_c<bool, C>
{
    HBOOST_STATIC_CONSTANT(bool, value = C);
    typedef integral_c_tag tag;
    typedef integral_c type;
    typedef bool value_type;
    operator bool() const { return this->value; }
};
HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
#endif

#endif // HBOOST_MPL_INTEGRAL_C_HPP_INCLUDED
