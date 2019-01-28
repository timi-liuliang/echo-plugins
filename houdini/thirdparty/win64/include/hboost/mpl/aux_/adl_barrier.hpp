
#ifndef HBOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED
#define HBOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/config/adl.hpp>
#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if !defined(HBOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE)

#   define HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE hboost_mpl_
#   define HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN namespace hboost_mpl_ {
#   define HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE }
#   define HBOOST_MPL_AUX_ADL_BARRIER_DECL(type) \
    namespace hboost { namespace mpl { \
    using ::HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::type; \
    } } \
/**/

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
namespace HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE { namespace aux {} }
namespace hboost { namespace mpl { using namespace HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE; 
namespace aux { using namespace HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::aux; }
}}
#endif

#else // HBOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE

#   define HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE hboost::mpl
#   define HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN namespace hboost { namespace mpl {
#   define HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE }}
#   define HBOOST_MPL_AUX_ADL_BARRIER_DECL(type) /**/

#endif

#endif // HBOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED
