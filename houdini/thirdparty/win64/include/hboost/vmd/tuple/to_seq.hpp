
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_TUPLE_TO_SEQ_HPP)
#define HBOOST_VMD_TUPLE_TO_SEQ_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/to_seq.hpp>
#include <hboost/vmd/empty.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_TUPLE_TO_SEQ(tuple)

    \brief converts a tuple to a seq.

    tuple = tuple to be converted.
    
    If the tuple is an empty tuple it is converted to an empty seq.
    Otherwise the tuple is converted to a seq with the same number of elements as the tuple.
*/

#define HBOOST_VMD_TUPLE_TO_SEQ(tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_IS_EMPTY(tuple), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_TUPLE_TO_SEQ \
        ) \
    (tuple) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_TUPLE_TO_SEQ_HPP */
