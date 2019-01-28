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
# ifndef HBOOST_PREPROCESSOR_TUPLE_POP_FRONT_HPP
# define HBOOST_PREPROCESSOR_TUPLE_POP_FRONT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# if HBOOST_PP_VARIADICS
#
# include <hboost/preprocessor/array/pop_front.hpp>
# include <hboost/preprocessor/array/to_tuple.hpp>
# include <hboost/preprocessor/comparison/greater.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/tuple/size.hpp>
# include <hboost/preprocessor/tuple/to_array.hpp>
#
#
# /* HBOOST_PP_TUPLE_POP_FRONT */
#
# define HBOOST_PP_TUPLE_POP_FRONT(tuple) \
	HBOOST_PP_IIF \
		( \
		HBOOST_PP_GREATER(HBOOST_PP_TUPLE_SIZE(tuple),1), \
		HBOOST_PP_TUPLE_POP_FRONT_EXEC, \
		HBOOST_PP_TUPLE_POP_FRONT_RETURN \
		) \
	(tuple) \
/**/
#
# define HBOOST_PP_TUPLE_POP_FRONT_EXEC(tuple) \
	HBOOST_PP_ARRAY_TO_TUPLE(HBOOST_PP_ARRAY_POP_FRONT(HBOOST_PP_TUPLE_TO_ARRAY(tuple))) \
/**/
#
# define HBOOST_PP_TUPLE_POP_FRONT_RETURN(tuple) tuple
#
# /* HBOOST_PP_TUPLE_POP_FRONT_Z */
#
# define HBOOST_PP_TUPLE_POP_FRONT_Z(z, tuple) \
	HBOOST_PP_IIF \
		( \
		HBOOST_PP_GREATER(HBOOST_PP_TUPLE_SIZE(tuple),1), \
		HBOOST_PP_TUPLE_POP_FRONT_Z_EXEC, \
		HBOOST_PP_TUPLE_POP_FRONT_Z_RETURN \
		) \
	(z, tuple) \
/**/
#
# define HBOOST_PP_TUPLE_POP_FRONT_Z_EXEC(z, tuple) \
	HBOOST_PP_ARRAY_TO_TUPLE(HBOOST_PP_ARRAY_POP_FRONT_Z(z, HBOOST_PP_TUPLE_TO_ARRAY(tuple))) \
/**/
#
# define HBOOST_PP_TUPLE_POP_FRONT_Z_RETURN(z, tuple) tuple
#
# endif // HBOOST_PP_VARIADICS
#
# endif // HBOOST_PREPROCESSOR_TUPLE_POP_FRONT_HPP
