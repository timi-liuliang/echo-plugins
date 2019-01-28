
//  (C) Copyright Edward Diener 2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_IS_VMD_TUPLE_HPP)
#define HBOOST_VMD_IS_VMD_TUPLE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_empty.hpp>
#include <hboost/vmd/is_tuple.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_IS_VMD_TUPLE(sequence)

    \brief Determines if a sequence is a VMD tuple.

    The macro checks that the sequence is a VMD tuple.
    A VMD tuple, which may be a Boost PP tuple or emptiness, is a superset of a Boost PP tuple.
    It returns 1 if it is a VMD tuple, else if returns 0.
    
    sequence = a possible Boost PP tuple

    returns = 1 if it a VMD tuple, else returns 0.
    
*/

#define HBOOST_VMD_IS_VMD_TUPLE(sequence) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_EMPTY(sequence), \
            HBOOST_VMD_IDENTITY(1), \
            HBOOST_VMD_IS_TUPLE \
            ) \
        (sequence) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_IS_VMD_TUPLE_HPP */
