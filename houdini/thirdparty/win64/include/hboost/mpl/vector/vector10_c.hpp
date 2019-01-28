
#ifndef HBOOST_MPL_VECTOR_VECTOR10_C_HPP_INCLUDED
#define HBOOST_MPL_VECTOR_VECTOR10_C_HPP_INCLUDED

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
#   include <hboost/mpl/vector/vector0_c.hpp>
#   include <hboost/mpl/vector/vector10.hpp>
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER vector10_c.hpp
#   include <hboost/mpl/vector/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/aux_/config/typeof.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/preprocessor/iterate.hpp>

namespace hboost { namespace mpl {

#   define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, 10, <hboost/mpl/vector/aux_/numbered_c.hpp>))
#   include HBOOST_PP_ITERATE()

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS

#endif // HBOOST_MPL_VECTOR_VECTOR10_C_HPP_INCLUDED
