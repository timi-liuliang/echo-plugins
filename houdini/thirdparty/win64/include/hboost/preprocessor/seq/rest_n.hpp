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
# ifndef HBOOST_PREPROCESSOR_SEQ_REST_N_HPP
# define HBOOST_PREPROCESSOR_SEQ_REST_N_HPP
#
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/comparison/not_equal.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/expr_iif.hpp>
# include <hboost/preprocessor/facilities/identity.hpp>
# include <hboost/preprocessor/logical/bitand.hpp>
# include <hboost/preprocessor/seq/detail/is_empty.hpp>
# include <hboost/preprocessor/seq/detail/split.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_SEQ_REST_N */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_REST_N(n, seq) HBOOST_PP_SEQ_REST_N_DETAIL_EXEC(n, seq, HBOOST_PP_SEQ_DETAIL_EMPTY_SIZE(seq))
# else
#    define HBOOST_PP_SEQ_REST_N(n, seq) HBOOST_PP_SEQ_REST_N_I(n, seq)
#    define HBOOST_PP_SEQ_REST_N_I(n, seq) HBOOST_PP_SEQ_REST_N_DETAIL_EXEC(n, seq, HBOOST_PP_SEQ_DETAIL_EMPTY_SIZE(seq))
# endif
#
#    define HBOOST_PP_SEQ_REST_N_DETAIL_EXEC(n, seq, size) \
		HBOOST_PP_EXPR_IIF \
			( \
			HBOOST_PP_BITAND \
				( \
				HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size), \
				HBOOST_PP_NOT_EQUAL(n,size) \
				), \
			HBOOST_PP_TUPLE_ELEM(2, 1, HBOOST_PP_SEQ_SPLIT(HBOOST_PP_INC(n), HBOOST_PP_IDENTITY( (nil) seq )))() \
			) \
/**/
#
# endif
