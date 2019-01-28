
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_IS_EMPTY_ARRAY_HPP)
#define HBOOST_VMD_IS_EMPTY_ARRAY_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/vmd/is_array.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/detail/is_empty_array.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_IS_EMPTY_ARRAY(sequence)

    \brief Tests whether a sequence is an empty Boost PP array.

    An empty Boost PP array is a two element tuple where the first
    size element is 0 and the second element is a tuple with a single 
    empty element, ie. '(0,())'.
    
    sequence = a possible empty array

    returns = 1 if the sequence is an empty Boost PP array
              0 if it is not.
              
    The macro will generate a preprocessing error if the sequence
    is in the form of an array but its first tuple element, instead
    of being a number, is a preprocessor token which VMD cannot parse,
    as in the example '(&0,())' which is a valid tuple but an invalid
    array.
    
*/

#define HBOOST_VMD_IS_EMPTY_ARRAY(sequence) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_ARRAY(sequence), \
            HBOOST_VMD_DETAIL_IS_EMPTY_ARRAY_SIZE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (sequence) \
        ) \
/**/

/** \def HBOOST_VMD_IS_EMPTY_ARRAY_D(d,sequence)

    \brief Tests whether a sequence is an empty Boost PP array. Re-entrant version.

    An empty Boost PP array is a two element tuple where the first
    size element is 0 and the second element is a tuple with a single 
    empty element, ie. '(0,())'.
    
    d        = The next available HBOOST_PP_WHILE iteration. 
    sequence = a possible empty array

    returns = 1 if the sequence is an empty Boost PP array
              0 if it is not.
              
    The macro will generate a preprocessing error if the sequence
    is in the form of an array but its first tuple element, instead
    of being a number, is a preprocessor token which VMD cannot parse,
    as in the example '(&0,())' which is a valid tuple but an invalid
    array.
    
*/

#define HBOOST_VMD_IS_EMPTY_ARRAY_D(d,sequence) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_ARRAY_D(d,sequence), \
            HBOOST_VMD_DETAIL_IS_EMPTY_ARRAY_SIZE, \
            HBOOST_VMD_IDENTITY(0) \
            ) \
        (sequence) \
        ) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_IS_EMPTY_ARRAY_HPP */
