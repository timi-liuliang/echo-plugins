
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_ASSERT_IS_LIST_HPP)
#define HBOOST_VMD_ASSERT_IS_LIST_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_ASSERT_IS_LIST(sequence)

    \brief Asserts that the sequence is a Boost PP list.

    The macro checks that the sequence is a Boost PP list.
    If it is not a Boost PP list, it forces a compiler error.
    
    The macro normally checks for a Boost PP list only in 
    debug mode. However an end-user can force the macro 
    to check or not check by defining the macro 
    HBOOST_VMD_ASSERT_DATA to 1 or 0 respectively.
    
    sequence = a possible Boost PP list.

    returns  = Normally the macro returns nothing. 
    
               If the sequence is a Boost PP list, nothing is 
               output.
              
               For VC++, because there is no sure way of forcing  
               a compiler error from within a macro without producing
               output, if the sequence is not a Boost PP list the 
               macro forces a compiler error by outputting invalid C++.
              
               For all other compilers a compiler error is forced 
               without producing output if the parameter is not a 
               Boost PP list.
              
*/

/** \def HBOOST_VMD_ASSERT_IS_LIST_D(d,sequence)

    \brief Asserts that the sequence is a Boost PP list. Re-entrant version.

    The macro checks that the sequence is a Boost PP list.
    If it is not a Boost PP list, it forces a compiler error.
    
    The macro normally checks for a Boost PP list only in 
    debug mode. However an end-user can force the macro 
    to check or not check by defining the macro 
    HBOOST_VMD_ASSERT_DATA to 1 or 0 respectively.
    
    d        = The next available HBOOST_PP_WHILE iteration. 
    sequence = a possible Boost PP list.

    returns  = Normally the macro returns nothing. 
    
               If the sequence is a Boost PP list, nothing is 
               output.
              
               For VC++, because there is no sure way of forcing  
               a compiler error from within a macro without producing
               output, if the sequence is not a Boost PP list the 
               macro forces a compiler error by outputting invalid C++.
              
               For all other compilers a compiler error is forced 
               without producing output if the parameter is not a 
               Boost PP list.
              
*/

#if !HBOOST_VMD_ASSERT_DATA

#define HBOOST_VMD_ASSERT_IS_LIST(sequence)
#define HBOOST_VMD_ASSERT_IS_LIST_D(d,sequence)

#else

#include <hboost/vmd/assert.hpp>
#include <hboost/vmd/is_list.hpp>

#define HBOOST_VMD_ASSERT_IS_LIST(sequence) \
    HBOOST_VMD_ASSERT \
      ( \
      HBOOST_VMD_IS_LIST(sequence), \
      HBOOST_VMD_ASSERT_IS_LIST_ERROR \
      ) \
/**/

#define HBOOST_VMD_ASSERT_IS_LIST_D(d,sequence) \
    HBOOST_VMD_ASSERT \
      ( \
      HBOOST_VMD_IS_LIST_D(d,sequence), \
      HBOOST_VMD_ASSERT_IS_LIST_ERROR \
      ) \
/**/

#endif /* HBOOST_VMD_ASSERT_DATA */

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_ASSERT_IS_LIST_HPP */
