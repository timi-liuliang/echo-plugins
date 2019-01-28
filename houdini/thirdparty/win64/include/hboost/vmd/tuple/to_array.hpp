
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_TUPLE_TO_ARRAY_HPP)
#define HBOOST_VMD_TUPLE_TO_ARRAY_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/to_array.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_TUPLE_TO_ARRAY(tuple)

    \brief converts a tuple to an array.

    tuple = tuple to be converted.
    
    If the tuple is an empty tuple it is converted to an array with 0 elements.
    Otherwise the tuple is converted to an array with the same number of elements as the tuple.
*/

#define HBOOST_VMD_TUPLE_TO_ARRAY(tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(tuple), \
            HBOOST_VMD_IDENTITY((0,())), \
            HBOOST_PP_TUPLE_TO_ARRAY \
            ) \
        (tuple) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_TUPLE_TO_ARRAY_HPP */
