
#ifndef HBOOST_MPL_SET_SET40_HPP_INCLUDED
#define HBOOST_MPL_SET_SET40_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
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
#   include <hboost/mpl/set/set30.hpp>
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER set40.hpp
#   include <hboost/mpl/set/aux_/include_preprocessed.hpp>

#else

#   include <hboost/preprocessor/iterate.hpp>

namespace hboost { namespace mpl {

#   define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(31, 40, <hboost/mpl/set/aux_/numbered.hpp>))
#   include HBOOST_PP_ITERATE()

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS

#endif // HBOOST_MPL_SET_SET40_HPP_INCLUDED
