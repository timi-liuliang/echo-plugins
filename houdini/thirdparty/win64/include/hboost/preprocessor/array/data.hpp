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
# ifndef HBOOST_PREPROCESSOR_ARRAY_DATA_HPP
# define HBOOST_PREPROCESSOR_ARRAY_DATA_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_ARRAY_DATA */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ARRAY_DATA(array) HBOOST_PP_TUPLE_ELEM(2, 1, array)
# else
#    define HBOOST_PP_ARRAY_DATA(array) HBOOST_PP_ARRAY_DATA_I(array)
#    define HBOOST_PP_ARRAY_DATA_I(array) HBOOST_PP_ARRAY_DATA_II array
#    define HBOOST_PP_ARRAY_DATA_II(size, data) data
# endif
#
# endif
