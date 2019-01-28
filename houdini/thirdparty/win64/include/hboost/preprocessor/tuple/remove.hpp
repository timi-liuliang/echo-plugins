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
# ifndef HBOOST_PREPROCESSOR_TUPLE_REMOVE_HPP
# define HBOOST_PREPROCESSOR_TUPLE_REMOVE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# if HBOOST_PP_VARIADICS
#
# include <hboost/preprocessor/array/remove.hpp>
# include <hboost/preprocessor/array/to_tuple.hpp>
# include <hboost/preprocessor/comparison/greater.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/tuple/size.hpp>
# include <hboost/preprocessor/tuple/to_array.hpp>
#
# /* HBOOST_PP_TUPLE_REMOVE */
#
# define HBOOST_PP_TUPLE_REMOVE(tuple, i) \
	HBOOST_PP_IIF \
		( \
		HBOOST_PP_GREATER(HBOOST_PP_TUPLE_SIZE(tuple),1), \
		HBOOST_PP_TUPLE_REMOVE_EXEC, \
		HBOOST_PP_TUPLE_REMOVE_RETURN \
		) \
	(tuple, i) \
/**/
#
# define HBOOST_PP_TUPLE_REMOVE_EXEC(tuple, i) \
	HBOOST_PP_ARRAY_TO_TUPLE(HBOOST_PP_ARRAY_REMOVE(HBOOST_PP_TUPLE_TO_ARRAY(tuple), i)) \
/**/
#
# define HBOOST_PP_TUPLE_REMOVE_RETURN(tuple, i) tuple
#
# /* HBOOST_PP_TUPLE_REMOVE_D */
#
# define HBOOST_PP_TUPLE_REMOVE_D(d, tuple, i) \
	HBOOST_PP_IIF \
		( \
		HBOOST_PP_GREATER_D(d, HBOOST_PP_TUPLE_SIZE(tuple), 1), \
		HBOOST_PP_TUPLE_REMOVE_D_EXEC, \
		HBOOST_PP_TUPLE_REMOVE_D_RETURN \
		) \
	(d, tuple, i) \
/**/
#
# define HBOOST_PP_TUPLE_REMOVE_D_EXEC(d, tuple, i) \
	HBOOST_PP_ARRAY_TO_TUPLE(HBOOST_PP_ARRAY_REMOVE_D(d, HBOOST_PP_TUPLE_TO_ARRAY(tuple), i)) \
/**/
#
# define HBOOST_PP_TUPLE_REMOVE_D_RETURN(d, tuple, i) tuple
#
# endif // HBOOST_PP_VARIADICS
#
# endif // HBOOST_PREPROCESSOR_TUPLE_REMOVE_HPP
