
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_ASSERT_IS_NUMBER_HPP)
#define HBOOST_VMD_ASSERT_IS_NUMBER_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_ASSERT_IS_NUMBER(sequence)

    \brief Asserts that the sequence is a number.

    The macro checks that the parameter is a number.
    If it is not a number, it forces a compiler error.
    
    The macro normally checks for a number only in 
    debug mode. However an end-user can force the macro 
    to check or not check by defining the macro 
    HBOOST_VMD_ASSERT_DATA to 1 or 0 respectively.

    sequence = a possible number.

    returns  = Normally the macro returns nothing. 
    
               If the sequence is a number, nothing is 
               output.
              
               For VC++, because there is no sure way of forcing  
               a compiler error from within a macro without producing
               output, if the sequence is not a number the 
               macro forces a compiler error by outputting invalid C++.
              
               For all other compilers a compiler error is forced 
               without producing output if the sequence is not a 
               number.
    
*/

#if !HBOOST_VMD_ASSERT_DATA

#define HBOOST_VMD_ASSERT_IS_NUMBER(sequence)

#else

#include <hboost/vmd/assert.hpp>
#include <hboost/vmd/is_number.hpp>

#define HBOOST_VMD_ASSERT_IS_NUMBER(sequence) \
    HBOOST_VMD_ASSERT \
          ( \
          HBOOST_VMD_IS_NUMBER(sequence), \
          HBOOST_VMD_IS_NUMBER_ASSERT_ERROR \
          ) \
/**/

#endif // !HBOOST_VMD_ASSERT_DATA

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_ASSERT_IS_NUMBER_HPP */
