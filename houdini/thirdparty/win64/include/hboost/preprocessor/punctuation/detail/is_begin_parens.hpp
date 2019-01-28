# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
#ifndef HBOOST_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP
#define HBOOST_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP

#if HBOOST_PP_VARIADICS_MSVC

#include <hboost/preprocessor/facilities/empty.hpp>

#define HBOOST_PP_DETAIL_VD_IBP_CAT(a, b) HBOOST_PP_DETAIL_VD_IBP_CAT_I(a, b)
#define HBOOST_PP_DETAIL_VD_IBP_CAT_I(a, b) HBOOST_PP_DETAIL_VD_IBP_CAT_II(a ## b)
#define HBOOST_PP_DETAIL_VD_IBP_CAT_II(res) res

#define HBOOST_PP_DETAIL_IBP_SPLIT(i, ...) \
    HBOOST_PP_DETAIL_VD_IBP_CAT(HBOOST_PP_DETAIL_IBP_PRIMITIVE_CAT(HBOOST_PP_DETAIL_IBP_SPLIT_,i)(__VA_ARGS__),HBOOST_PP_EMPTY()) \
/**/

#define HBOOST_PP_DETAIL_IBP_IS_VARIADIC_C(...) 1 1

#else

#define HBOOST_PP_DETAIL_IBP_SPLIT(i, ...) \
    HBOOST_PP_DETAIL_IBP_PRIMITIVE_CAT(HBOOST_PP_DETAIL_IBP_SPLIT_,i)(__VA_ARGS__) \
/**/

#define HBOOST_PP_DETAIL_IBP_IS_VARIADIC_C(...) 1

#endif /* HBOOST_PP_VARIADICS_MSVC */

#define HBOOST_PP_DETAIL_IBP_SPLIT_0(a, ...) a
#define HBOOST_PP_DETAIL_IBP_SPLIT_1(a, ...) __VA_ARGS__

#define HBOOST_PP_DETAIL_IBP_CAT(a, ...) HBOOST_PP_DETAIL_IBP_PRIMITIVE_CAT(a,__VA_ARGS__)
#define HBOOST_PP_DETAIL_IBP_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define HBOOST_PP_DETAIL_IBP_IS_VARIADIC_R_1 1,
#define HBOOST_PP_DETAIL_IBP_IS_VARIADIC_R_HBOOST_PP_DETAIL_IBP_IS_VARIADIC_C 0,

#endif /* HBOOST_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP */
