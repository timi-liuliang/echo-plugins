
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_SEQ_REMOVE_HPP)
#define HBOOST_VMD_SEQ_REMOVE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/seq/remove.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/vmd/empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_SEQ_REMOVE(seq,index)

    \brief removes an element from a seq.

    seq = seq from which an element is to be removed.
    index = The zero-based position in seq of the element to be removed.

    If index is greater or equal to the seq size the result is undefined.
    If the seq is a single element and the index is 0 the result is an empty seq.
    Otherwise the result is a seq after removing the index element.
*/

#define HBOOST_VMD_SEQ_REMOVE(seq,index) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITAND \
            ( \
            HBOOST_PP_EQUAL(index,0), \
            HBOOST_PP_EQUAL(HBOOST_PP_SEQ_SIZE(seq),1) \
            ), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_SEQ_REMOVE \
        ) \
    (seq,index) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_SEQ_REMOVE_HPP */
