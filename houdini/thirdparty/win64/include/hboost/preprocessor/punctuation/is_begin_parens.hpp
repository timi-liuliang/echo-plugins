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
# ifndef HBOOST_PREPROCESSOR_IS_BEGIN_PARENS_HPP
# define HBOOST_PREPROCESSOR_IS_BEGIN_PARENS_HPP

# include <hboost/preprocessor/config/config.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/punctuation/detail/is_begin_parens.hpp>

#if HBOOST_PP_VARIADICS_MSVC && _MSC_VER <= 1400

#define HBOOST_PP_IS_BEGIN_PARENS(param) \
    HBOOST_PP_DETAIL_IBP_SPLIT \
      ( \
      0, \
      HBOOST_PP_DETAIL_IBP_CAT \
        ( \
        HBOOST_PP_DETAIL_IBP_IS_VARIADIC_R_, \
        HBOOST_PP_DETAIL_IBP_IS_VARIADIC_C param \
        ) \
      ) \
/**/

#else

#define HBOOST_PP_IS_BEGIN_PARENS(...) \
    HBOOST_PP_DETAIL_IBP_SPLIT \
      ( \
      0, \
      HBOOST_PP_DETAIL_IBP_CAT \
        ( \
        HBOOST_PP_DETAIL_IBP_IS_VARIADIC_R_, \
        HBOOST_PP_DETAIL_IBP_IS_VARIADIC_C __VA_ARGS__ \
        ) \
      ) \
/**/

#endif /* HBOOST_PP_VARIADICS_MSVC && _MSC_VER <= 1400 */
#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_PREPROCESSOR_IS_BEGIN_PARENS_HPP */
