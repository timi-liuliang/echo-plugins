# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_ARRAY_PUSH_BACK_HPP
# define HBOOST_PREPROCESSOR_ARRAY_PUSH_BACK_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/array/data.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/punctuation/comma_if.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
# include <hboost/preprocessor/array/detail/get_data.hpp>
#
# /* HBOOST_PP_ARRAY_PUSH_BACK */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ARRAY_PUSH_BACK(array, elem) HBOOST_PP_ARRAY_PUSH_BACK_I(HBOOST_PP_ARRAY_SIZE(array), HBOOST_PP_ARRAY_DATA(array), elem)
# else
#    define HBOOST_PP_ARRAY_PUSH_BACK(array, elem) HBOOST_PP_ARRAY_PUSH_BACK_D(array, elem)
#    define HBOOST_PP_ARRAY_PUSH_BACK_D(array, elem) HBOOST_PP_ARRAY_PUSH_BACK_I(HBOOST_PP_ARRAY_SIZE(array), HBOOST_PP_ARRAY_DATA(array), elem)
# endif
#
# define HBOOST_PP_ARRAY_PUSH_BACK_I(size, data, elem) (HBOOST_PP_INC(size), (HBOOST_PP_ARRAY_DETAIL_GET_DATA(size,data) HBOOST_PP_COMMA_IF(size) elem))
#
# endif
