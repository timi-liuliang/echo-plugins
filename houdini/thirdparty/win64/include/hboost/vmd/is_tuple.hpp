
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_IS_TUPLE_HPP)
#define HBOOST_VMD_IS_TUPLE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/vmd/detail/is_tuple.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_IS_TUPLE(sequence)

    \brief Tests whether a sequence is a Boost PP tuple.

    The macro checks to see if a sequence is a Boost PP tuple.
    A Boost PP tuple is preprocessor tokens enclosed by a set of parentheses
    with no preprocessing tokens before or after the parentheses.
    
    sequence = a possible tuple

    returns = 1 if the sequence is a Boost PP tuple.
              0 if it is not.
              
*/

#define HBOOST_VMD_IS_TUPLE(sequence) \
    HBOOST_VMD_DETAIL_IS_TUPLE(sequence) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_IS_TUPLE_HPP */