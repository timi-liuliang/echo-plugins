
#ifndef HBOOST_MPL_AUX_ITER_FOLD_IMPL_HPP_INCLUDED
#define HBOOST_MPL_AUX_ITER_FOLD_IMPL_HPP_INCLUDED

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

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/next_prior.hpp>
#   include <hboost/mpl/apply.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#       include <hboost/mpl/if.hpp>
#       include <hboost/type_traits/is_same.hpp>
#   endif
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER iter_fold_impl.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   define AUX778076_FOLD_IMPL_OP(iter) iter
#   define AUX778076_FOLD_IMPL_NAME_PREFIX iter_fold
#   include <hboost/mpl/aux_/fold_impl_body.hpp>

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_AUX_ITER_FOLD_IMPL_HPP_INCLUDED
