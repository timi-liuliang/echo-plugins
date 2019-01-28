
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_GET_TYPE_HPP)
#define HBOOST_VMD_GET_TYPE_HPP

#include <hboost/vmd/detail/setup.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/vmd/detail/sequence_type.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/** \def HBOOST_VMD_GET_TYPE(...)

    \brief Returns the type of a sequence as a VMD type.

    ...       = variadic parameters.
    
    The first variadic parameter is required and is the sequence whose type we are getting.
    
    The optional variadic parameters are return type parameters.
    
    The macro returns the type of a sequence as a VMD type. The type of an empty sequence
    is always HBOOST_VMD_TYPE_EMPTY and the type of a multi-element is always
    HBOOST_VMD_TYPE_SEQUENCE. The type of a single-element sequence is the
    type of that single element.
    
    The type returned can be modified by specifying an optional return type parameter.
    
    If HBOOST_VMD_RETURN_TYPE, the default, is specified the specific type of the element
    is returned.
    
    If HBOOST_VMD_RETURN_TYPE_ARRAY is specified
    an array type is returned if the element is an array, else a tuple
    type is returned if the element is a tuple, else the actual type
    is returned for non-tuple data. 
    
    If HBOOST_VMD_RETURN_TYPE_LIST is specified
    a list type is returned if the element is a list, else a tuple
    type is returned if the element is a tuple, else the actual type
    is returned for non-tuple data. 
    
    If HBOOST_VMD_RETURN_TYPE_TUPLE is specified
    a tuple type is returned for all tuple-like data, else the actual type
    is returned for non-tuple data. 
    
    If HBOOST_VMD_RETURN_NO_TYPE is specified it is ignored since the macro
    always returns the type of the sequence.
             
    If more than one return type optional parameter is specified the last 
    one specified determines the return type.
    
    returns = the type of the sequence as a VMD type.
    
*/

#define HBOOST_VMD_GET_TYPE(...) \
    HBOOST_VMD_DETAIL_SEQUENCE_TYPE(__VA_ARGS__) \
/**/

/** \def HBOOST_VMD_GET_TYPE_D(d,...)

    \brief Returns the type of a sequence as a VMD type. Re-entrant version.

    d         = The next available HBOOST_PP_WHILE iteration. 
    ...       = variadic parameters.
    
    The first variadic parameter is required and is the sequence whose type we are getting.
    
    The optional variadic parameters are return type parameters.
    
    The macro returns the type of a sequence as a VMD type. The type of an empty sequence
    is always HBOOST_VMD_TYPE_EMPTY and the type of a multi-element is always
    HBOOST_VMD_TYPE_SEQUENCE. The type of a single-element sequence is the
    type of that single element.
    
    The type returned can be modified by specifying an optional return type parameter.
    
    If HBOOST_VMD_RETURN_TYPE, the default, is specified the specific type of the element
    is returned.
    
    If HBOOST_VMD_RETURN_TYPE_ARRAY is specified
    an array type is returned if the element is an array, else a tuple
    type is returned if the element is a tuple, else the actual type
    is returned for non-tuple data. 
    
    If HBOOST_VMD_RETURN_TYPE_LIST is specified
    a list type is returned if the element is a list, else a tuple
    type is returned if the element is a tuple, else the actual type
    is returned for non-tuple data. 
    
    If HBOOST_VMD_RETURN_TYPE_TUPLE is specified
    a tuple type is returned for all tuple-like data, else the actual type
    is returned for non-tuple data. 
    
    If HBOOST_VMD_RETURN_NO_TYPE is specified it is ignored since the macro
    always returns the type of the sequence.
             
    If more than one return type optional parameter is specified the last 
    one specified determines the return type.
    
    returns = the type of the sequence as a VMD type.
    
*/

#define HBOOST_VMD_GET_TYPE_D(d,...) \
    HBOOST_VMD_DETAIL_SEQUENCE_TYPE_D(d,__VA_ARGS__) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_VMD_GET_TYPE_HPP */
