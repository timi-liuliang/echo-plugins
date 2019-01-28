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
#ifndef HBOOST_PREPROCESSOR_DETAIL_IS_EMPTY_HPP
#define HBOOST_PREPROCESSOR_DETAIL_IS_EMPTY_HPP

#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>

#if HBOOST_PP_VARIADICS_MSVC

# pragma warning(once:4002)

#define HBOOST_PP_DETAIL_IS_EMPTY_IIF_0(t, b) b
#define HBOOST_PP_DETAIL_IS_EMPTY_IIF_1(t, b) t

#else

#define HBOOST_PP_DETAIL_IS_EMPTY_IIF_0(t, ...) __VA_ARGS__
#define HBOOST_PP_DETAIL_IS_EMPTY_IIF_1(t, ...) t

#endif

#if HBOOST_PP_VARIADICS_MSVC && _MSC_VER <= 1400

#define HBOOST_PP_DETAIL_IS_EMPTY_PROCESS(param) \
	HBOOST_PP_IS_BEGIN_PARENS \
    	( \
        HBOOST_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C param () \
        ) \
/**/

#else

#define HBOOST_PP_DETAIL_IS_EMPTY_PROCESS(...) \
	HBOOST_PP_IS_BEGIN_PARENS \
        ( \
        HBOOST_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ () \
        ) \
/**/

#endif

#define HBOOST_PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(a, b) a ## b
#define HBOOST_PP_DETAIL_IS_EMPTY_IIF(bit) HBOOST_PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(HBOOST_PP_DETAIL_IS_EMPTY_IIF_,bit)
#define HBOOST_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C(...) ()

#endif /* HBOOST_PREPROCESSOR_DETAIL_IS_EMPTY_HPP */
