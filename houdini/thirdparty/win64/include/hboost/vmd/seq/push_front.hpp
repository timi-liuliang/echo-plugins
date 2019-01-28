
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_SEQ_PUSH_FRONT_HPP)
#define HBOOST_VMD_SEQ_PUSH_FRONT_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/seq/push_front.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_SEQ_PUSH_FRONT(seq,elem)

    \brief inserts an element at the beginning of a seq. 

    seq = seq to insert an element at.
    elem  = element to insert.

    If the seq is an empty seq the result is a seq with the single element.
    Otherwise the result is a seq after inserting the element at the beginning.
*/

#define HBOOST_VMD_SEQ_PUSH_FRONT(seq,elem) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(seq), \
            HBOOST_VMD_IDENTITY((elem)), \
            HBOOST_PP_SEQ_PUSH_FRONT \
            ) \
        (seq,elem) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_SEQ_PUSH_FRONT_HPP */
