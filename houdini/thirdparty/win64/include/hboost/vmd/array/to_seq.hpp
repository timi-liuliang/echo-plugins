
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_ARRAY_TO_SEQ_HPP)
#define HBOOST_VMD_ARRAY_TO_SEQ_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/array/to_seq.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/is_empty_array.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_ARRAY_TO_SEQ(array)

    \brief converts an array to a seq.

    array = array to be converted.
    
    If the array is an array of 0 elements it is converted to an empty seq.
    Otherwise the array is converted to a seq with the same number of elements as the array.
*/

#define HBOOST_VMD_ARRAY_TO_SEQ(array) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY_ARRAY(array), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_ARRAY_TO_SEQ \
        ) \
    (array) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_ARRAY_TO_SEQ_HPP */
