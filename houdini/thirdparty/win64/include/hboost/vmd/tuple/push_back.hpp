
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_TUPLE_PUSH_BACK_HPP)
#define HBOOST_VMD_TUPLE_PUSH_BACK_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/push_back.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_TUPLE_PUSH_BACK(tuple,elem)

    \brief appends an element to the end of a tuple. 

    tuple = tuple to to append an element to.
    elem  = element to append.

    If the tuple is an empty tuple the result is a tuple with the single element.
    Otherwise the result is a tuple after adding the element to the end.
*/

#define HBOOST_VMD_TUPLE_PUSH_BACK(tuple,elem) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(tuple), \
            HBOOST_VMD_IDENTITY((elem)), \
            HBOOST_PP_TUPLE_PUSH_BACK \
            ) \
        (tuple,elem) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_TUPLE_PUSH_BACK_HPP */
