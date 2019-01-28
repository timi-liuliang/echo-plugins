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
# ifndef HBOOST_PREPROCESSOR_ARRAY_TO_SEQ_HPP
# define HBOOST_PREPROCESSOR_ARRAY_TO_SEQ_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/tuple/to_seq.hpp>
#
# /* HBOOST_PP_ARRAY_TO_SEQ */
#
#    define HBOOST_PP_ARRAY_TO_SEQ(array) \
		HBOOST_PP_IF \
			( \
			HBOOST_PP_ARRAY_SIZE(array), \
			HBOOST_PP_ARRAY_TO_SEQ_DO, \
			HBOOST_PP_ARRAY_TO_SEQ_EMPTY \
			) \
		(array) \
/**/
#    define HBOOST_PP_ARRAY_TO_SEQ_EMPTY(array)
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_ARRAY_TO_SEQ_DO(array) HBOOST_PP_ARRAY_TO_SEQ_I(HBOOST_PP_TUPLE_TO_SEQ, array)
#    define HBOOST_PP_ARRAY_TO_SEQ_I(m, args) HBOOST_PP_ARRAY_TO_SEQ_II(m, args)
#    define HBOOST_PP_ARRAY_TO_SEQ_II(m, args) HBOOST_PP_CAT(m ## args,)
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_ARRAY_TO_SEQ_DO(array) HBOOST_PP_ARRAY_TO_SEQ_I(array)
#    define HBOOST_PP_ARRAY_TO_SEQ_I(array) HBOOST_PP_TUPLE_TO_SEQ ## array
# else
#    define HBOOST_PP_ARRAY_TO_SEQ_DO(array) HBOOST_PP_TUPLE_TO_SEQ array
# endif
#
# endif
