# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_ARRAY_TO_TUPLE_HPP
# define HBOOST_PREPROCESSOR_ARRAY_TO_TUPLE_HPP
#
# include <hboost/preprocessor/array/data.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/control/if.hpp>
#
# /* HBOOST_PP_ARRAY_TO_TUPLE */
#
#    define HBOOST_PP_ARRAY_TO_TUPLE(array) \
		HBOOST_PP_IF \
			( \
			HBOOST_PP_ARRAY_SIZE(array), \
			HBOOST_PP_ARRAY_DATA, \
			HBOOST_PP_ARRAY_TO_TUPLE_EMPTY \
			) \
		(array) \
/**/
#    define HBOOST_PP_ARRAY_TO_TUPLE_EMPTY(array)
#
# endif
