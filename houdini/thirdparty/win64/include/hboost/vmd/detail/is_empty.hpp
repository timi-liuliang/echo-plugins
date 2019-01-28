
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_IS_EMPTY_HPP)
#define HBOOST_VMD_DETAIL_IS_EMPTY_HPP

#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>

#if HBOOST_VMD_MSVC

# pragma warning(once:4002)

#define HBOOST_VMD_DETAIL_IS_EMPTY_IIF_0(t, b) b
#define HBOOST_VMD_DETAIL_IS_EMPTY_IIF_1(t, b) t

#if HBOOST_VMD_MSVC_V8

#define HBOOST_VMD_DETAIL_IS_EMPTY_PROCESS(param) \
    HBOOST_PP_IS_BEGIN_PARENS \
        ( \
        HBOOST_VMD_DETAIL_IS_EMPTY_NON_FUNCTION_C param () \
        ) \
/**/

#else

#define HBOOST_VMD_DETAIL_IS_EMPTY_PROCESS(...) \
    HBOOST_PP_IS_BEGIN_PARENS \
        ( \
        HBOOST_VMD_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ () \
        ) \
/**/

#endif

#else

#define HBOOST_VMD_DETAIL_IS_EMPTY_IIF_0(t, ...) __VA_ARGS__
#define HBOOST_VMD_DETAIL_IS_EMPTY_IIF_1(t, ...) t

#define HBOOST_VMD_DETAIL_IS_EMPTY_PROCESS(...) \
    HBOOST_PP_IS_BEGIN_PARENS \
        ( \
        HBOOST_VMD_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ () \
        ) \
/**/

#endif /* HBOOST_VMD_MSVC */

#define HBOOST_VMD_DETAIL_IS_EMPTY_PRIMITIVE_CAT(a, b) a ## b
#define HBOOST_VMD_DETAIL_IS_EMPTY_IIF(bit) \
    HBOOST_VMD_DETAIL_IS_EMPTY_PRIMITIVE_CAT(HBOOST_VMD_DETAIL_IS_EMPTY_IIF_,bit) \
/**/

#define HBOOST_VMD_DETAIL_IS_EMPTY_NON_FUNCTION_C(...) ()
#define HBOOST_VMD_DETAIL_IS_EMPTY_GEN_ZERO(...) 0

#endif /* HBOOST_VMD_DETAIL_IS_EMPTY_HPP */
