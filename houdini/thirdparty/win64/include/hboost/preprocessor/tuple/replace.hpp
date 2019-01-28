# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2013.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_TUPLE_REPLACE_HPP
# define HBOOST_PREPROCESSOR_TUPLE_REPLACE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# if HBOOST_PP_VARIADICS
#
# include <hboost/preprocessor/array/replace.hpp>
# include <hboost/preprocessor/array/to_tuple.hpp>
# include <hboost/preprocessor/tuple/to_array.hpp>
#
# /* HBOOST_PP_TUPLE_REPLACE */
#
# define HBOOST_PP_TUPLE_REPLACE(tuple, i, elem) \
	HBOOST_PP_ARRAY_TO_TUPLE(HBOOST_PP_ARRAY_REPLACE(HBOOST_PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# /* HBOOST_PP_TUPLE_REPLACE_D */
#
# define HBOOST_PP_TUPLE_REPLACE_D(d, tuple, i, elem) \
	HBOOST_PP_ARRAY_TO_TUPLE(HBOOST_PP_ARRAY_REPLACE_D(d, HBOOST_PP_TUPLE_TO_ARRAY(tuple), i, elem)) \
/**/
#
# endif // HBOOST_PP_VARIADICS
#
# endif // HBOOST_PREPROCESSOR_TUPLE_REPLACE_HPP
