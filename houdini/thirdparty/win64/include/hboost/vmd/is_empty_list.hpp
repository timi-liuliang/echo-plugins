
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_IS_EMPTY_LIST_HPP)
#define HBOOST_VMD_IS_EMPTY_LIST_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/vmd/detail/is_list.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_IS_EMPTY_LIST(sequence)

    \brief Tests whether a sequence is an empty Boost PP list.

    An empty Boost PP list consists of the single identifier 'HBOOST_PP_NIL'.
    This identifier also serves as a list terminator for a non-empty list.
    
    sequence = a preprocessor parameter

    returns = 1 if the sequence is an empty Boost PP list
              0 if it is not.
              
    The macro will generate a preprocessing error if the input
    as an empty list marker, instead of being an identifier, is 
    a preprocessor token which VMD cannot parse, as in the 
    example '&HBOOST_PP_NIL'.
    
*/

#define HBOOST_VMD_IS_EMPTY_LIST(sequence) \
    HBOOST_VMD_DETAIL_IS_LIST_IS_EMPTY_LIST_PROCESS(sequence) \
/**/

/** \def HBOOST_VMD_IS_EMPTY_LIST_D(d,sequence)

    \brief Tests whether a sequence is an empty Boost PP list. Re-entrant version.

    An empty Boost PP list consists of the single identifier 'HBOOST_PP_NIL'.
    This identifier also serves as a list terminator for a non-empty list.
    
    d        = The next available HBOOST_PP_WHILE iteration
    sequence = a preprocessor parameter

    returns = 1 if the sequence is an empty Boost PP list
              0 if it is not.
              
    The macro will generate a preprocessing error if the input
    as an empty list marker, instead of being an identifier, is 
    a preprocessor token which VMD cannot parse, as in the 
    example '&HBOOST_PP_NIL'.
    
*/

#define HBOOST_VMD_IS_EMPTY_LIST_D(d,sequence) \
    HBOOST_VMD_DETAIL_IS_LIST_IS_EMPTY_LIST_PROCESS_D(d,sequence) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_IS_EMPTY_LIST_HPP */
