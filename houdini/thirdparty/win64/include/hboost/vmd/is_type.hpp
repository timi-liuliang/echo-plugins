
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_IS_TYPE_HPP)
#define HBOOST_VMD_IS_TYPE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/vmd/detail/is_type.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_IS_TYPE(sequence)

    \brief Tests whether a sequence is a VMD type.

    sequence = a possible VMD type
    
    returns = 1 if the sequence is a VMD type, 
              0 if it is not.
    
    If the sequence is not a VMD data type this macro could lead to
    a preprocessor error. This is because the macro
    uses preprocessor concatenation to determine if the sequence
    is an identifier once it is determined that the sequence does not
    start with parentheses. If the data being concatenated would
    lead to an invalid preprocessor token the compiler can issue
    a preprocessor error.
    
*/

#define HBOOST_VMD_IS_TYPE(sequence) \
    HBOOST_VMD_DETAIL_IS_TYPE(sequence) \
/**/

/** \def HBOOST_VMD_IS_TYPE_D(d,sequence)

    \brief Tests whether a sequence is a VMD type. Re-entrant version.

    d        = The next available HBOOST_PP_WHILE iteration. 
    sequence = a possible VMD type
    
    returns = 1 if the sequence is a VMD type, 
              0 if it is not.
    
    If the sequence is not a VMD data type this macro could lead to
    a preprocessor error. This is because the macro
    uses preprocessor concatenation to determine if the sequence
    is an identifier once it is determined that the sequence does not
    start with parentheses. If the data being concatenated would
    lead to an invalid preprocessor token the compiler can issue
    a preprocessor error.
    
*/

#define HBOOST_VMD_IS_TYPE_D(d,sequence) \
    HBOOST_VMD_DETAIL_IS_TYPE_D(d,sequence) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_IS_TYPE_HPP */
