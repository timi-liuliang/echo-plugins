
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_TUPLE_SIZE_HPP)
#define HBOOST_VMD_TUPLE_SIZE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_TUPLE_SIZE(tuple)

    \brief  expands to the size of the tuple passed to it. 

    tuple = tuple whose size is to be extracted. 
    
    If the tuple is an empty tuple its size is 0.
    Otherwise the result is the number of elements in the tuple.
*/

#define HBOOST_VMD_TUPLE_SIZE(tuple) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(tuple), \
            HBOOST_VMD_IDENTITY(0), \
            HBOOST_PP_TUPLE_SIZE \
            ) \
        (tuple) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_TUPLE_SIZE_HPP */
