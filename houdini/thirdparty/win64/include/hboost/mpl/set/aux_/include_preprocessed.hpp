
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

// Copyright Aleksey Gurtovoy 2001-2006
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/config/preprocessor.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/stringize.hpp>

#if !defined(HBOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING)
#   define AUX778076_HEADER \
    plain/HBOOST_MPL_PREPROCESSED_HEADER \
/**/
#else
#   define AUX778076_HEADER \
    HBOOST_PP_CAT(plain,/)##HBOOST_MPL_PREPROCESSED_HEADER \
/**/
#endif

#if HBOOST_WORKAROUND(__IBMCPP__, HBOOST_TESTED_AT(700))
#   define AUX778076_INCLUDE_STRING HBOOST_PP_STRINGIZE(hboost/mpl/set/aux_/preprocessed/AUX778076_HEADER)
#   include AUX778076_INCLUDE_STRING
#   undef AUX778076_INCLUDE_STRING
#else
#   include HBOOST_PP_STRINGIZE(hboost/mpl/set/aux_/preprocessed/AUX778076_HEADER)
#endif

#   undef AUX778076_HEADER

#undef HBOOST_MPL_PREPROCESSED_HEADER
