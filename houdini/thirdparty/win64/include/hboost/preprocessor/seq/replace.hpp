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
# ifndef HBOOST_PREPROCESSOR_SEQ_REPLACE_HPP
# define HBOOST_PREPROCESSOR_SEQ_REPLACE_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/comparison/equal.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/seq/first_n.hpp>
# include <hboost/preprocessor/seq/rest_n.hpp>
# include <hboost/preprocessor/seq/size.hpp>
#
# /* HBOOST_PP_SEQ_REPLACE */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_REPLACE(seq, i, elem) HBOOST_PP_SEQ_FIRST_N(i, seq) (elem) HBOOST_PP_SEQ_REPLACE_DETAIL_REST(seq, i)
# else
#    define HBOOST_PP_SEQ_REPLACE(seq, i, elem) HBOOST_PP_SEQ_REPLACE_I(seq, i, elem)
#    define HBOOST_PP_SEQ_REPLACE_I(seq, i, elem) HBOOST_PP_SEQ_FIRST_N(i, seq) (elem) HBOOST_PP_SEQ_REPLACE_DETAIL_REST(seq, i)
# endif
#
#    define HBOOST_PP_SEQ_REPLACE_DETAIL_REST_EMPTY(seq, i)
#    define HBOOST_PP_SEQ_REPLACE_DETAIL_REST_VALID(seq, i) HBOOST_PP_SEQ_REST_N(HBOOST_PP_INC(i), seq)
#    define HBOOST_PP_SEQ_REPLACE_DETAIL_REST(seq, i) \
		HBOOST_PP_IIF \
			( \
			HBOOST_PP_EQUAL(i,HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(seq))), \
			HBOOST_PP_SEQ_REPLACE_DETAIL_REST_EMPTY, \
			HBOOST_PP_SEQ_REPLACE_DETAIL_REST_VALID \
			) \
		(seq, i) \
/**/
#
# endif
