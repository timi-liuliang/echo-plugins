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
# ifndef HBOOST_PREPROCESSOR_SEQ_FOR_EACH_I_HPP
# define HBOOST_PREPROCESSOR_SEQ_FOR_EACH_I_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/repetition/for.hpp>
# include <hboost/preprocessor/seq/seq.hpp>
# include <hboost/preprocessor/seq/size.hpp>
# include <hboost/preprocessor/seq/detail/is_empty.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
#
# /* HBOOST_PP_SEQ_FOR_EACH_I */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_FOR_EACH_I(macro, data, seq) HBOOST_PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq)
# else
#    define HBOOST_PP_SEQ_FOR_EACH_I(macro, data, seq) HBOOST_PP_SEQ_FOR_EACH_I_I(macro, data, seq)
#    define HBOOST_PP_SEQ_FOR_EACH_I_I(macro, data, seq) HBOOST_PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq)
# endif
#
#    define HBOOST_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EXEC(macro, data, seq) HBOOST_PP_FOR((macro, data, seq, 0, HBOOST_PP_SEQ_SIZE(seq)), HBOOST_PP_SEQ_FOR_EACH_I_P, HBOOST_PP_SEQ_FOR_EACH_I_O, HBOOST_PP_SEQ_FOR_EACH_I_M)
#    define HBOOST_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EMPTY(macro, data, seq)
#
#    define HBOOST_PP_SEQ_FOR_EACH_I_DETAIL_CHECK(macro, data, seq) \
		HBOOST_PP_IIF \
			( \
			HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
			HBOOST_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EXEC, \
			HBOOST_PP_SEQ_FOR_EACH_I_DETAIL_CHECK_EMPTY \
			) \
		(macro, data, seq) \
/**/
#
# define HBOOST_PP_SEQ_FOR_EACH_I_P(r, x) HBOOST_PP_TUPLE_ELEM(5, 4, x)
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_SEQ_FOR_EACH_I_O(r, x) HBOOST_PP_SEQ_FOR_EACH_I_O_I x
# else
#    define HBOOST_PP_SEQ_FOR_EACH_I_O(r, x) HBOOST_PP_SEQ_FOR_EACH_I_O_I(HBOOST_PP_TUPLE_ELEM(5, 0, x), HBOOST_PP_TUPLE_ELEM(5, 1, x), HBOOST_PP_TUPLE_ELEM(5, 2, x), HBOOST_PP_TUPLE_ELEM(5, 3, x), HBOOST_PP_TUPLE_ELEM(5, 4, x))
# endif
#
# define HBOOST_PP_SEQ_FOR_EACH_I_O_I(macro, data, seq, i, sz) \
	HBOOST_PP_SEQ_FOR_EACH_I_O_I_DEC(macro, data, seq, i, HBOOST_PP_DEC(sz)) \
/**/
# define HBOOST_PP_SEQ_FOR_EACH_I_O_I_DEC(macro, data, seq, i, sz) \
	( \
	macro, \
	data, \
	HBOOST_PP_IF \
		( \
		sz, \
		HBOOST_PP_SEQ_FOR_EACH_I_O_I_TAIL, \
		HBOOST_PP_SEQ_FOR_EACH_I_O_I_NIL \
		) \
	(seq), \
	HBOOST_PP_INC(i), \
	sz \
	) \
/**/
# define HBOOST_PP_SEQ_FOR_EACH_I_O_I_TAIL(seq) HBOOST_PP_SEQ_TAIL(seq)
# define HBOOST_PP_SEQ_FOR_EACH_I_O_I_NIL(seq) HBOOST_PP_NIL
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_STRICT()
#    define HBOOST_PP_SEQ_FOR_EACH_I_M(r, x) HBOOST_PP_SEQ_FOR_EACH_I_M_IM(r, HBOOST_PP_TUPLE_REM_5 x)
#    define HBOOST_PP_SEQ_FOR_EACH_I_M_IM(r, im) HBOOST_PP_SEQ_FOR_EACH_I_M_I(r, im)
# else
#    define HBOOST_PP_SEQ_FOR_EACH_I_M(r, x) HBOOST_PP_SEQ_FOR_EACH_I_M_I(r, HBOOST_PP_TUPLE_ELEM(5, 0, x), HBOOST_PP_TUPLE_ELEM(5, 1, x), HBOOST_PP_TUPLE_ELEM(5, 2, x), HBOOST_PP_TUPLE_ELEM(5, 3, x), HBOOST_PP_TUPLE_ELEM(5, 4, x))
# endif
#
# define HBOOST_PP_SEQ_FOR_EACH_I_M_I(r, macro, data, seq, i, sz) macro(r, data, i, HBOOST_PP_SEQ_HEAD(seq))
#
# /* HBOOST_PP_SEQ_FOR_EACH_I_R */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_SEQ_FOR_EACH_I_R(r, macro, data, seq) HBOOST_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq)
# else
#    define HBOOST_PP_SEQ_FOR_EACH_I_R(r, macro, data, seq) HBOOST_PP_SEQ_FOR_EACH_I_R_I(r, macro, data, seq)
#    define HBOOST_PP_SEQ_FOR_EACH_I_R_I(r, macro, data, seq) HBOOST_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq)
# endif
#
#    define HBOOST_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EXEC(r, macro, data, seq) HBOOST_PP_FOR_ ## r((macro, data, seq, 0, HBOOST_PP_SEQ_SIZE(seq)), HBOOST_PP_SEQ_FOR_EACH_I_P, HBOOST_PP_SEQ_FOR_EACH_I_O, HBOOST_PP_SEQ_FOR_EACH_I_M)
#    define HBOOST_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EMPTY(r, macro, data, seq)
#
#    define HBOOST_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK(r, macro, data, seq) \
		HBOOST_PP_IIF \
			( \
			HBOOST_PP_SEQ_DETAIL_IS_NOT_EMPTY(seq), \
			HBOOST_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EXEC, \
			HBOOST_PP_SEQ_FOR_EACH_I_R_DETAIL_CHECK_EMPTY \
			) \
		(r, macro, data, seq) \
/**/
#
# endif
