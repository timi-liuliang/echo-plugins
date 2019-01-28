
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_SIZE_HPP)
#define HBOOST_VMD_SIZE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/vmd/detail/sequence_size.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_SIZE(sequence)

    \brief Returns the size of a sequence.

    sequence  = A sequence to test.

    returns   = If the sequence is empty returns 0, else returns the number of elements
                in the sequence.
    
*/

#define HBOOST_VMD_SIZE(sequence) \
    HBOOST_VMD_DETAIL_SEQUENCE_SIZE(sequence) \
/**/

/** \def HBOOST_VMD_SIZE_D(d,sequence)

    \brief Returns the size of a sequence. Re-entrant version.

    d         = The next available HBOOST_PP_WHILE iteration. 
    sequence  = A sequence to test.

    returns   = If the sequence is empty returns 0, else returns the number of elements
                in the sequence.
    
*/

#define HBOOST_VMD_SIZE_D(d,sequence) \
    HBOOST_VMD_DETAIL_SEQUENCE_SIZE_D(d,sequence) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_SIZE_HPP */
