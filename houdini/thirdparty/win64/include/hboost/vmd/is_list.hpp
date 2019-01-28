
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_IS_LIST_HPP)
#define HBOOST_VMD_IS_LIST_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/vmd/detail/is_list.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_IS_LIST(sequence)

    \brief Determines if a sequence is a Boost pplib list.

    The macro checks that the sequence is a pplib list.
    It returns 1 if it is a list, else if returns 0.
    
    sequence = input as a possible Boost PP list.
    
    returns = 1 if it a list, else returns 0.
    
    The macro will generate a preprocessing error if the input
    is in the form of a list but its end-of-list marker, instead
    of being an identifier, is a preprocessor token which VMD cannot parse,
    as in the example '(anything,&HBOOST_PP_NIL)' which is a valid tuple but
    an invalid list.
    
*/

#define HBOOST_VMD_IS_LIST(sequence) \
    HBOOST_VMD_DETAIL_IS_LIST(sequence) \
/**/

/** \def HBOOST_VMD_IS_LIST_D(d,sequence)

    \brief Determines if a sequence is a Boost pplib list. Re-entrant version.

    The macro checks that the sequence is a pplib list.
    It returns 1 if it is a list, else if returns 0.
    
    d        = The next available HBOOST_PP_WHILE iteration. 
    sequence = input as a possible Boost PP list.
    
    returns = 1 if it a list, else returns 0.
    
    The macro will generate a preprocessing error if the input
    is in the form of a list but its end-of-list marker, instead
    of being an identifier, is a preprocessor token which VMD cannot parse,
    as in the example '(anything,&HBOOST_PP_NIL)' which is a valid tuple but
    an invalid list.
    
*/

#define HBOOST_VMD_IS_LIST_D(d,sequence) \
    HBOOST_VMD_DETAIL_IS_LIST_D(d,sequence) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_IS_LIST_HPP */
