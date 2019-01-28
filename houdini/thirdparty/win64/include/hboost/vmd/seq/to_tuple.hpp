
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_SEQ_TO_TUPLE_HPP)
#define HBOOST_VMD_SEQ_TO_TUPLE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/seq/to_tuple.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_SEQ_TO_TUPLE(seq)

    \brief converts a seq to a tuple.

    seq = seq to be converted.
    
    If the seq is an empty seq it is converted to an empty tuple.
    Otherwise the seq is converted to a tuple with the same number of elements as the seq.
*/

#define HBOOST_VMD_SEQ_TO_TUPLE(seq) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(seq), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_SEQ_TO_TUPLE \
        ) \
    (seq) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_SEQ_TO_TUPLE_HPP */
