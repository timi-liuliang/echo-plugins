
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_ASSERT_HPP)
#define HBOOST_VMD_DETAIL_ASSERT_HPP

#include <hboost/preprocessor/debug/assert.hpp>
#include <hboost/preprocessor/variadic/elem.hpp>

#if HBOOST_VMD_MSVC

#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/variadic/size.hpp>
#include <hboost/vmd/empty.hpp>

#define HBOOST_VMD_DETAIL_ASSERT_VC_GEN_ERROR_OUTPUT(errstr) \
    HBOOST_PP_ASSERT(0) \
    typedef char errstr[-1]; \
/**/

#define HBOOST_VMD_DETAIL_ASSERT_VC_GEN_ERROR_DEFAULT(...) \
    HBOOST_VMD_DETAIL_ASSERT_VC_GEN_ERROR_OUTPUT(HBOOST_VMD_ASSERT_ERROR) \
/**/

#define HBOOST_VMD_DETAIL_ASSERT_VC_GEN_ERROR_ERRSTR(...) \
    HBOOST_VMD_DETAIL_ASSERT_VC_GEN_ERROR_OUTPUT(HBOOST_PP_VARIADIC_ELEM(1,__VA_ARGS__)) \
/**/

#define HBOOST_VMD_DETAIL_ASSERT_TRUE(...) \
    HBOOST_PP_IIF \
        ( \
        HBOOST_PP_EQUAL \
            ( \
            HBOOST_PP_VARIADIC_SIZE(__VA_ARGS__), \
            1 \
            ), \
        HBOOST_VMD_DETAIL_ASSERT_VC_GEN_ERROR_DEFAULT, \
        HBOOST_VMD_DETAIL_ASSERT_VC_GEN_ERROR_ERRSTR \
        ) \
    (__VA_ARGS__) \
/**/

#define HBOOST_VMD_DETAIL_ASSERT(...) \
    HBOOST_PP_IF \
      ( \
      HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__), \
      HBOOST_VMD_EMPTY, \
      HBOOST_VMD_DETAIL_ASSERT_TRUE \
      ) \
    (__VA_ARGS__) \
/**/

#else

#define HBOOST_VMD_DETAIL_ASSERT_DO(cond) \
    HBOOST_PP_ASSERT(cond) \
/**/

#define HBOOST_VMD_DETAIL_ASSERT(...) \
    HBOOST_VMD_DETAIL_ASSERT_DO(HBOOST_PP_VARIADIC_ELEM(0,__VA_ARGS__)) \
/**/

#endif /* HBOOST_VMD_MSVC */
#endif /* HBOOST_VMD_DETAIL_ASSERT_HPP */
