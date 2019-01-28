
#ifndef HBOOST_MPL_AUX_PREPROCESSOR_RANGE_HPP_INCLUDED
#define HBOOST_MPL_AUX_PREPROCESSOR_RANGE_HPP_INCLUDED

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

#include <hboost/preprocessor/seq/subseq.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/arithmetic/add.hpp>

#define HBOOST_MPL_PP_RANGE_ITEM(z,n,_) (n)

#define HBOOST_MPL_PP_RANGE(first, length) \
    HBOOST_PP_SEQ_SUBSEQ( \
        HBOOST_PP_REPEAT(HBOOST_PP_ADD(first,length), HBOOST_MPL_PP_RANGE_ITEM, _), \
        first, length \
    ) \
/**/

#endif // HBOOST_MPL_AUX_PREPROCESSOR_RANGE_HPP_INCLUDED
