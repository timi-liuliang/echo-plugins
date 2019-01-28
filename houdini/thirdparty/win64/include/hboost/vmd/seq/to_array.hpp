
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_SEQ_TO_ARRAY_HPP)
#define HBOOST_VMD_SEQ_TO_ARRAY_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/seq/to_array.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_SEQ_TO_ARRAY(seq)

    \brief converts a seq to an array.

    seq = seq to be converted.
    
    If the seq is an empty seq it is converted to an array with 0 elements.
    Otherwise the seq is converted to an array with the same number of elements as the seq.
*/

#define HBOOST_VMD_SEQ_TO_ARRAY(seq) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(seq), \
            HBOOST_VMD_IDENTITY((0,())), \
            HBOOST_PP_SEQ_TO_ARRAY \
            ) \
        (seq) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_SEQ_TO_ARRAY_HPP */
