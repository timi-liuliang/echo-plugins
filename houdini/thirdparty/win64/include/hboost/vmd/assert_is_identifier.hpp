
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_ASSERT_IS_IDENTIFIER_HPP)
#define HBOOST_VMD_ASSERT_IS_IDENTIFIER_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_ASSERT_IS_IDENTIFIER(...)

    \brief Asserts that the sequence is an identifier.

    The macro checks that the sequence is an identifier.
    If it is not an identifier, it forces a compiler error.
    
    The macro normally checks for an identifier only in 
    debug mode. However an end-user can force the macro 
    to check or not check by defining the macro 
    HBOOST_VMD_ASSERT_DATA to 1 or 0 respectively.

    ...       = variadic parameters
    
    The variadic parameters are:
    
    sequence         = A sequence to test as an identifier.
    ids (optional)   = The data may take one of two forms:
                       it is either one or more single identifiers
                       or a single Boost PP tuple of identifiers.

    returns = Normally the macro returns nothing. 
    
              If the sequence is an identifier, nothing is 
              output. If optional ids are specified, for the 
              sequence to be an identifier it must be an
              identifier that matches one of the optional
              ids.
              
              For VC++, because there is no sure way of forcing  
              a compiler error from within a macro without producing
              output, if the sequence is not an identifier the 
              macro forces a compiler error by outputting invalid C++.
              
              For all other compilers a compiler error is forced 
              without producing output if the sequence is not an
              identifier.
    
    Identifiers are registered in VMD with:
    
        #define HBOOST_VMD_REG_XXX (XXX) where XXX is a v-identifier.
    
    The identifier must be registered to be found.
    
    Identifiers are pre-detected in VMD with:
    
        #define HBOOST_VMD_DETECT_XXX_XXX where XXX is an identifier.
    
    If you specify optional ids and have not specified the detection
    of an optional id, that id will never match an identifier.
    
*/

/** \def HBOOST_VMD_ASSERT_IS_IDENTIFIER_D(d,...)

    \brief Asserts that the sequence is an identifier. Re-entrant version.

    The macro checks that the sequence is an identifier.
    If it is not an identifier, it forces a compiler error.
    
    The macro normally checks for an identifier only in 
    debug mode. However an end-user can force the macro 
    to check or not check by defining the macro 
    HBOOST_VMD_ASSERT_DATA to 1 or 0 respectively.

    d         = The next available HBOOST_PP_WHILE iteration. 
    ...       = variadic parameters
    
    The variadic parameters are:
    
    sequence         = A sequence to test as an identifier.
    ids (optional)   = The data may take one of two forms:
                       it is either one or more single identifiers
                       or a single Boost PP tuple of identifiers.

    returns = Normally the macro returns nothing. 
    
              If the sequence is an identifier, nothing is 
              output. If optional ids are specified, for the 
              sequence to be an identifier it must be an
              identifier that matches one of the optional
              ids.
              
              For VC++, because there is no sure way of forcing  
              a compiler error from within a macro without producing
              output, if the sequence is not an identifier the 
              macro forces a compiler error by outputting invalid C++.
              
              For all other compilers a compiler error is forced 
              without producing output if the sequence is not an
              identifier.
    
    Identifiers are registered in VMD with:
    
        #define HBOOST_VMD_REG_XXX (XXX) where XXX is a v-identifier.
    
    The identifier must be registered to be found.
    
    Identifiers are pre-detected in VMD with:
    
        #define HBOOST_VMD_DETECT_XXX_XXX where XXX is an identifier.
    
    If you specify optional ids and have not specified the detection
    of an optional id, that id will never match an identifier.
    
*/

#if !HBOOST_VMD_ASSERT_DATA

#define HBOOST_VMD_ASSERT_IS_IDENTIFIER(...)
#define HBOOST_VMD_ASSERT_IS_IDENTIFIER_D(d,...)

#else

#include <hboost/vmd/assert.hpp>
#include <hboost/vmd/is_identifier.hpp>

#define HBOOST_VMD_ASSERT_IS_IDENTIFIER(...) \
    HBOOST_VMD_ASSERT \
          ( \
          HBOOST_VMD_IS_IDENTIFIER(__VA_ARGS__), \
          HBOOST_VMD_IDENTIFIER_ASSERT_ERROR \
          ) \
/**/

#define HBOOST_VMD_ASSERT_IS_IDENTIFIER_D(d,...) \
    HBOOST_VMD_ASSERT \
          ( \
          HBOOST_VMD_IS_IDENTIFIER_D(d,__VA_ARGS__), \
          HBOOST_VMD_IDENTIFIER_ASSERT_ERROR \
          ) \
/**/

#endif // !HBOOST_VMD_ASSERT_DATA
#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_ASSERT_IS_IDENTIFIER_HPP */
