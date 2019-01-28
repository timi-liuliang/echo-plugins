
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_SEQ_POP_BACK_HPP)
#define HBOOST_VMD_SEQ_POP_BACK_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/seq/pop_back.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/vmd/empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_SEQ_POP_BACK(seq)

    \brief pops an element from the end of a seq. 

    seq = seq to pop an element from.

    If the seq is an empty seq the result is undefined.
    If the seq is a single element the result is an empty seq.
    Otherwise the result is a seq after removing the last element.
*/

#define HBOOST_VMD_SEQ_POP_BACK(seq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL(HBOOST_PP_SEQ_SIZE(seq),1), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_SEQ_POP_BACK \
        ) \
    (seq) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_SEQ_POP_BACK_HPP */
