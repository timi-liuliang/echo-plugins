
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_EMPTY_HPP)
#define HBOOST_VMD_EMPTY_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_EMPTY(...)

    \brief Outputs emptiness.

    ... = any variadic parameters. The parameters are ignored.
    
    This macro is used to output emptiness ( nothing ) no matter
    what is passed to it.
    
    If you use this macro to return a result, as in 'result HBOOST_VMD_EMPTY'
    subsequently invoked, you should surround the result with 
    HBOOST_VMD_IDENTITY_RESULT to smooth over a VC++ problem.
    
*/
    
#define HBOOST_VMD_EMPTY(...)

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_EMPTY_HPP */
