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
# ifndef HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
# define HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# if HBOOST_PP_VARIADICS
#
# include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
# include <hboost/preprocessor/facilities/detail/is_empty.hpp>
#
#if HBOOST_PP_VARIADICS_MSVC && _MSC_VER <= 1400
#
#define HBOOST_PP_IS_EMPTY(param) \
    HBOOST_PP_DETAIL_IS_EMPTY_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS \
        ( \
        param \
        ) \
      ) \
      ( \
      HBOOST_PP_IS_EMPTY_ZERO, \
      HBOOST_PP_DETAIL_IS_EMPTY_PROCESS \
      ) \
    (param) \
/**/
#define HBOOST_PP_IS_EMPTY_ZERO(param) 0
# else
#define HBOOST_PP_IS_EMPTY(...) \
    HBOOST_PP_DETAIL_IS_EMPTY_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS \
        ( \
        __VA_ARGS__ \
        ) \
      ) \
      ( \
      HBOOST_PP_IS_EMPTY_ZERO, \
      HBOOST_PP_DETAIL_IS_EMPTY_PROCESS \
      ) \
    (__VA_ARGS__) \
/**/
#define HBOOST_PP_IS_EMPTY_ZERO(...) 0
# endif /* HBOOST_PP_VARIADICS_MSVC && _MSC_VER <= 1400 */
# endif /* HBOOST_PP_VARIADICS */
# endif /* HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_VARIADIC_HPP */
