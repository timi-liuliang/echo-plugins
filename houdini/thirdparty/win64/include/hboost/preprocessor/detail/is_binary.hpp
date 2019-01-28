# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_DETAIL_IS_BINARY_HPP
# define HBOOST_PREPROCESSOR_DETAIL_IS_BINARY_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/detail/check.hpp>
#
# /* HBOOST_PP_IS_BINARY */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_IS_BINARY(x) HBOOST_PP_CHECK(x, HBOOST_PP_IS_BINARY_CHECK)
# else
#    define HBOOST_PP_IS_BINARY(x) HBOOST_PP_IS_BINARY_I(x)
#    define HBOOST_PP_IS_BINARY_I(x) HBOOST_PP_CHECK(x, HBOOST_PP_IS_BINARY_CHECK)
# endif
#
# define HBOOST_PP_IS_BINARY_CHECK(a, b) 1
# define HBOOST_PP_CHECK_RESULT_HBOOST_PP_IS_BINARY_CHECK 0, HBOOST_PP_NIL
#
# endif
