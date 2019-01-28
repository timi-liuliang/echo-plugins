# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2015.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_SEQ_DETAIL_IS_EMPTY_HPP
# define HBOOST_PREPROCESSOR_SEQ_DETAIL_IS_EMPTY_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/logical/bool.hpp>
# include <hboost/preprocessor/logical/compl.hpp>
# include <hboost/preprocessor/seq/size.hpp>
#
/* An empty seq is one that is just HBOOST_PP_SEQ_NIL */
#
# define HBOOST_PP_SEQ_DETAIL_IS_EMPTY(seq) \
	HBOOST_PP_COMPL \
		( \
		HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq) \
		) \
/**/
#
# define HBOOST_PP_SEQ_DETAIL_IS_EMPTY_SIZE(size) \
	HBOOST_PP_COMPL \
		( \
		HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size) \
		) \
/**/
#
# define HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq) \
	HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(HBOOST_PP_SEQ_DETAIL_EMPTY_SIZE(seq)) \
/**/
#
# define HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY_SIZE(size) \
	HBOOST_PP_BOOL(size) \
/**/
#
# define HBOOST_PP_SEQ_DETAIL_EMPTY_SIZE(seq) \
	HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(seq (nil))) \
/**/
#
# endif
