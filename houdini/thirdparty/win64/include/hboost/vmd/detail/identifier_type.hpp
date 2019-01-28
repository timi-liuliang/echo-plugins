
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_HPP)
#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_HPP

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/size.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/is_tuple.hpp>
#include <hboost/vmd/detail/idprefix.hpp>
#include <hboost/vmd/detail/number_registration.hpp>
#include <hboost/vmd/detail/type_registration.hpp>

#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_CONCATENATE(id) \
    HBOOST_PP_CAT \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_SUBTYPE_REGISTRATION_PREFIX, \
        id \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_SIZE(cres) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL \
                ( \
                HBOOST_PP_TUPLE_SIZE(cres), \
                2 \
                ), \
            HBOOST_PP_TUPLE_ELEM, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_IDENTIFIER) \
            ) \
        (0,cres) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_SIZE_D(d,cres) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_PP_EQUAL_D \
                ( \
                d, \
                HBOOST_PP_TUPLE_SIZE(cres), \
                2 \
                ), \
            HBOOST_PP_TUPLE_ELEM, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_IDENTIFIER) \
            ) \
        (0,cres) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_CONCAT(cres) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_TUPLE(cres), \
            HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_SIZE, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_IDENTIFIER) \
            ) \
        (cres) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_CONCAT_D(d,cres) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_IS_TUPLE(cres), \
            HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_SIZE_D, \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_IDENTIFIER) \
            ) \
        (d,cres) \
        ) \
/**/

/*

  Determines the type of an identifier.
  
  The type may be that of an identifier or else
  it may be a subtype.
  
  Assumes the 'id' is a valid identifier id
  
  Expands to the appropriate type

*/

#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE(id) \
    HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_CONCAT \
        ( \
        HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_CONCATENATE(id) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_D(d,id) \
    HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_CONCAT_D \
        ( \
        d, \
        HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_CONCATENATE(id) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_IDENTIFIER_TYPE_HPP */
