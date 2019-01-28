
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_HPP)
#define HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_HPP

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/vmd/identity.hpp>
#include <hboost/vmd/detail/equal_type.hpp>
#include <hboost/vmd/detail/is_array_common.hpp>
#include <hboost/vmd/detail/is_list.hpp>
#include <hboost/vmd/detail/type_registration.hpp>

#define HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_ARRAY(data,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX(data), \
        HBOOST_VMD_TYPE_ARRAY, \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_ARRAY_D(d,data,type) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_VMD_DETAIL_IS_ARRAY_SYNTAX_D(d,data), \
        HBOOST_VMD_TYPE_ARRAY, \
        type \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_LIST(data,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_LIST_WLOOP(data), \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_LIST), \
            HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_ARRAY \
            ) \
        (data,type) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_LIST_D(d,data,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_IS_LIST_WLOOP_D(d,data), \
            HBOOST_VMD_IDENTITY(HBOOST_VMD_TYPE_LIST), \
            HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_ARRAY_D \
            ) \
        (d,data,type) \
        ) \
/**/

/*

  Input is any VMD data and a VMD type for that data
  
  If the type is a tuple, checks to see if it is a more specific
  type and, if it is, returns that type,
  otherwise returns the type passed as a parameter

*/

#define HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE(data,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE(HBOOST_VMD_TYPE_TUPLE,type), \
            HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_LIST, \
            HBOOST_VMD_IDENTITY(type) \
            ) \
        (data,type) \
        ) \
/**/

#define HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_D(d,data,type) \
    HBOOST_VMD_IDENTITY_RESULT \
        ( \
        HBOOST_PP_IIF \
            ( \
            HBOOST_VMD_DETAIL_EQUAL_TYPE_D(d,HBOOST_VMD_TYPE_TUPLE,type), \
            HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_CHECK_LIST_D, \
            HBOOST_VMD_IDENTITY(type) \
            ) \
        (d,data,type) \
        ) \
/**/

#endif /* HBOOST_VMD_DETAIL_ADJUST_TUPLE_TYPE_HPP */
