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
# ifndef HBOOST_PREPROCESSOR_ARRAY_REVERSE_HPP
# define HBOOST_PREPROCESSOR_ARRAY_REVERSE_HPP
#
# include <hboost/preprocessor/array/data.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/reverse.hpp>
#
# /* HBOOST_PP_ARRAY_REVERSE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ARRAY_REVERSE(array) (HBOOST_PP_ARRAY_SIZE(array), HBOOST_PP_TUPLE_REVERSE(HBOOST_PP_ARRAY_SIZE(array), HBOOST_PP_ARRAY_DATA(array)))
# else
#    define HBOOST_PP_ARRAY_REVERSE(array) HBOOST_PP_ARRAY_REVERSE_I(array)
#    define HBOOST_PP_ARRAY_REVERSE_I(array) (HBOOST_PP_ARRAY_SIZE(array), HBOOST_PP_TUPLE_REVERSE(HBOOST_PP_ARRAY_SIZE(array), HBOOST_PP_ARRAY_DATA(array)))
# endif
#
# endif
