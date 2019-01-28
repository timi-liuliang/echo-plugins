
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_TUPLE_POP_FRONT_HPP)
#define HBOOST_VMD_TUPLE_POP_FRONT_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/pop_front.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_TUPLE_POP_FRONT(tuple)

    \brief pops an element from the front of a tuple. 

    tuple = tuple to pop an element from.

    If the tuple is an empty tuple the result is undefined.
    If the tuple is a single element the result is an empty tuple.
    Otherwise the result is a tuple after removing the first element.
*/

#define HBOOST_VMD_TUPLE_POP_FRONT(tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL(HBOOST_PP_TUPLE_SIZE(tuple),1), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_TUPLE_POP_FRONT \
        ) \
    (tuple) \
/**/

/** \def HBOOST_VMD_TUPLE_POP_FRONT_Z(z,tuple)

    \brief pops an element from the front of a tuple. It reenters HBOOST_PP_REPEAT with maximum efficiency.

    z     = the next available HBOOST_PP_REPEAT dimension.
    tuple = tuple to pop an element from.

    If the tuple is an empty tuple the result is undefined.
    If the tuple is a single element the result is an empty tuple.
    Otherwise the result is a tuple after removing the first element.
*/

#define HBOOST_VMD_TUPLE_POP_FRONT_Z(z,tuple) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL(HBOOST_PP_TUPLE_SIZE(tuple),1), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_TUPLE_POP_FRONT_Z \
        ) \
    (z,tuple) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_TUPLE_POP_FRONT_HPP */
