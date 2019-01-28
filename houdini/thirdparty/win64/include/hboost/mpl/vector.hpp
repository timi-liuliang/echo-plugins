
#ifndef HBOOST_MPL_VECTOR_HPP_INCLUDED
#define HBOOST_MPL_VECTOR_HPP_INCLUDED

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
#   include <hboost/mpl/limits/vector.hpp>
#   include <hboost/mpl/aux_/na.hpp>
#   include <hboost/mpl/aux_/config/preprocessor.hpp>

#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/cat.hpp>
#   include <hboost/preprocessor/stringize.hpp>

#if !defined(HBOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING)
#   define AUX778076_VECTOR_HEADER \
    HBOOST_PP_CAT(vector, HBOOST_MPL_LIMIT_VECTOR_SIZE).hpp \
    /**/
#else
#   define AUX778076_VECTOR_HEADER \
    HBOOST_PP_CAT(vector, HBOOST_MPL_LIMIT_VECTOR_SIZE)##.hpp \
    /**/
#endif

#   include HBOOST_PP_STRINGIZE(hboost/mpl/vector/AUX778076_VECTOR_HEADER)
#   undef AUX778076_VECTOR_HEADER
#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER vector.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/limits/vector.hpp>

#   define AUX778076_SEQUENCE_NAME vector
#   define AUX778076_SEQUENCE_LIMIT HBOOST_MPL_LIMIT_VECTOR_SIZE
#   include <hboost/mpl/aux_/sequence_wrapper.hpp>

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_VECTOR_HPP_INCLUDED
