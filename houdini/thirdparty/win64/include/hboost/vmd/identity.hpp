
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_IDENTITY_HPP)
#define HBOOST_VMD_IDENTITY_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#if HBOOST_VMD_MSVC
#include <hboost/preprocessor/cat.hpp>
#endif
#include <hboost/vmd/empty.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_IDENTITY(item)

    \brief Macro which expands to its argument when invoked with any number of parameters.

    item = any single argument
    
    When HBOOST_VMD_IDENTITY(item) is subsequently invoked with any number of parameters it expands
    to 'item'. Subsequently invoking the macro is done as 'HBOOST_VMD_IDENTITY(item)(zero_or_more_arguments)'.
    
    The macro is equivalent to the Boost PP macro HBOOST_PP_IDENTITY(item) with the difference
    being that HBOOST_PP_IDENTITY(item) is always invoked with no arguments, as in
    'HBOOST_VMD_IDENTITY(item)()' whereas HBOOST_VMD_IDENTITY can be invoked with any number of
    arguments.
    
    The macro is meant to be used in HBOOST_PP_IF and HBOOST_PP_IIF statements when only one
    of the clauses needs to be invoked with calling another macro and the other is meant to
    return an 'item'.
    
    returns = the macro as 'HBOOST_VMD_IDENTITY(item)', when invoked with any number of parameters
              as in '(zero_or_more_arguments)', returns 'item'. The macro itself returns
              'item HBOOST_VMD_EMPTY'.
    
*/

#define HBOOST_VMD_IDENTITY(item) item HBOOST_VMD_EMPTY

/** \def HBOOST_VMD_IDENTITY_RESULT(result)

    \brief Macro which wraps any result which can return its value using HBOOST_VMD_IDENTITY or 'item HBOOST_VMD_EMPTY'.

    result = any single result returned when HBOOST_VMD_IDENTITY is used or 'item HBOOST_VMD_EMPTY'.
    
    The reason for this macro is to smooth over a problem when using VC++ with HBOOST_VMD_IDENTITY.
    If your HBOOST_VMD_IDENTITY macro can be used where VC++ is the compiler then you need to
    surround your macro code which could return a result with this macro in order that VC++ handles
    HBOOST_VMD_IDENTITY correctly.
    
    If you are not using VC++ you do not have to use this macro, but doing so does no harm.
    
*/

#if HBOOST_VMD_MSVC
#define HBOOST_VMD_IDENTITY_RESULT(result) HBOOST_PP_CAT(result,)
#else
#define HBOOST_VMD_IDENTITY_RESULT(result) result
#endif

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_IDENTITY_HPP */
