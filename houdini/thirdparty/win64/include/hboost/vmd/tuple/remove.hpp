
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_TUPLE_REMOVE_HPP)
#define HBOOST_VMD_TUPLE_REMOVE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/tuple/remove.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_TUPLE_REMOVE(tuple,index)

    \brief removes an element from a tuple.

    tuple = tuple from which an element is to be removed.
    index = The zero-based position in tuple of the element to be removed.

    If index is greater or equal to the tuple size the result is undefined.
    If the tuple is a single element and the index is 0 the result is an empty tuple.
    Otherwise the result is a tuple after removing the index element.
*/

#define HBOOST_VMD_TUPLE_REMOVE(tuple,index) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITAND \
            ( \
            HBOOST_PP_EQUAL(index,0), \
            HBOOST_PP_EQUAL(HBOOST_PP_TUPLE_SIZE(tuple),1) \
            ), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_TUPLE_REMOVE \
        ) \
    (tuple,index) \
/**/

/** \def HBOOST_VMD_TUPLE_REMOVE_D(d,tuple,index)

    \brief removes an element from a tuple. It reenters HBOOST_PP_WHILE with maximum efficiency.

    d     = The next available HBOOST_PP_WHILE iteration. 
    tuple = tuple from which an element is to be removed.
    index = The zero-based position in tuple of the element to be removed.

    If index is greater or equal to the tuple size the result is undefined.
    If the tuple is a single element and the index is 0 the result is an empty tuple.
    Otherwise the result is a tuple after removing the index element.
*/

#define HBOOST_VMD_TUPLE_REMOVE_D(d,tuple,index) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_BITAND \
            ( \
            HBOOST_PP_EQUAL_D(d,index,0), \
            HBOOST_PP_EQUAL_D(d,HBOOST_PP_TUPLE_SIZE(tuple),1) \
            ), \
        HBOOST_VMD_EMPTY, \
        HBOOST_PP_TUPLE_REMOVE_D \
        ) \
    (d,tuple,index) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_TUPLE_REMOVE_HPP */
