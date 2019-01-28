
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_TUPLE_TO_LIST_HPP)
#define HBOOST_VMD_TUPLE_TO_LIST_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/to_list.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_TUPLE_TO_LIST(tuple)

    \brief converts a tuple to a list.

    tuple = tuple to be converted.
    
    If the tuple is an empty tuple it is converted to an empty list (HBOOST_PP_NIL).
    Otherwise the tuple is converted to a list with the same number of elements as the tuple.
*/

#define HBOOST_VMD_TUPLE_TO_LIST(tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(tuple), \
            HBOOST_VMD_IDENTITY(HBOOST_PP_NIL), \
            HBOOST_PP_TUPLE_TO_LIST \
            ) \
        (tuple) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_TUPLE_TO_LIST_HPP */
