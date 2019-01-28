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
# ifndef HBOOST_PREPROCESSOR_ITERATION_ITERATE_HPP
# define HBOOST_PREPROCESSOR_ITERATION_ITERATE_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/array/elem.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/slot/slot.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_ITERATION_DEPTH */
#
# define HBOOST_PP_ITERATION_DEPTH() 0
#
# /* HBOOST_PP_ITERATION */
#
# define HBOOST_PP_ITERATION() HBOOST_PP_CAT(HBOOST_PP_ITERATION_, HBOOST_PP_ITERATION_DEPTH())
#
# /* HBOOST_PP_ITERATION_START && HBOOST_PP_ITERATION_FINISH */
#
# define HBOOST_PP_ITERATION_START() HBOOST_PP_CAT(HBOOST_PP_ITERATION_START_, HBOOST_PP_ITERATION_DEPTH())
# define HBOOST_PP_ITERATION_FINISH() HBOOST_PP_CAT(HBOOST_PP_ITERATION_FINISH_, HBOOST_PP_ITERATION_DEPTH())
#
# /* HBOOST_PP_ITERATION_FLAGS */
#
# define HBOOST_PP_ITERATION_FLAGS() (HBOOST_PP_CAT(HBOOST_PP_ITERATION_FLAGS_, HBOOST_PP_ITERATION_DEPTH())())
#
# /* HBOOST_PP_FRAME_ITERATION */
#
# define HBOOST_PP_FRAME_ITERATION(i) HBOOST_PP_CAT(HBOOST_PP_ITERATION_, i)
#
# /* HBOOST_PP_FRAME_START && HBOOST_PP_FRAME_FINISH */
#
# define HBOOST_PP_FRAME_START(i) HBOOST_PP_CAT(HBOOST_PP_ITERATION_START_, i)
# define HBOOST_PP_FRAME_FINISH(i) HBOOST_PP_CAT(HBOOST_PP_ITERATION_FINISH_, i)
#
# /* HBOOST_PP_FRAME_FLAGS */
#
# define HBOOST_PP_FRAME_FLAGS(i) (HBOOST_PP_CAT(HBOOST_PP_ITERATION_FLAGS_, i)())
#
# /* HBOOST_PP_RELATIVE_ITERATION */
#
# define HBOOST_PP_RELATIVE_ITERATION(i) HBOOST_PP_CAT(HBOOST_PP_RELATIVE_, i)(HBOOST_PP_ITERATION_)
#
# define HBOOST_PP_RELATIVE_0(m) HBOOST_PP_CAT(m, HBOOST_PP_ITERATION_DEPTH())
# define HBOOST_PP_RELATIVE_1(m) HBOOST_PP_CAT(m, HBOOST_PP_DEC(HBOOST_PP_ITERATION_DEPTH()))
# define HBOOST_PP_RELATIVE_2(m) HBOOST_PP_CAT(m, HBOOST_PP_DEC(HBOOST_PP_DEC(HBOOST_PP_ITERATION_DEPTH())))
# define HBOOST_PP_RELATIVE_3(m) HBOOST_PP_CAT(m, HBOOST_PP_DEC(HBOOST_PP_DEC(HBOOST_PP_DEC(HBOOST_PP_ITERATION_DEPTH()))))
# define HBOOST_PP_RELATIVE_4(m) HBOOST_PP_CAT(m, HBOOST_PP_DEC(HBOOST_PP_DEC(HBOOST_PP_DEC(HBOOST_PP_DEC(HBOOST_PP_ITERATION_DEPTH())))))
#
# /* HBOOST_PP_RELATIVE_START && HBOOST_PP_RELATIVE_FINISH */
#
# define HBOOST_PP_RELATIVE_START(i) HBOOST_PP_CAT(HBOOST_PP_RELATIVE_, i)(HBOOST_PP_ITERATION_START_)
# define HBOOST_PP_RELATIVE_FINISH(i) HBOOST_PP_CAT(HBOOST_PP_RELATIVE_, i)(HBOOST_PP_ITERATION_FINISH_)
#
# /* HBOOST_PP_RELATIVE_FLAGS */
#
# define HBOOST_PP_RELATIVE_FLAGS(i) (HBOOST_PP_CAT(HBOOST_PP_RELATIVE_, i)(HBOOST_PP_ITERATION_FLAGS_)())
#
# /* HBOOST_PP_ITERATE */
#
# define HBOOST_PP_ITERATE() HBOOST_PP_CAT(HBOOST_PP_ITERATE_, HBOOST_PP_INC(HBOOST_PP_ITERATION_DEPTH()))
#
# define HBOOST_PP_ITERATE_1 <hboost/preprocessor/iteration/detail/iter/forward1.hpp>
# define HBOOST_PP_ITERATE_2 <hboost/preprocessor/iteration/detail/iter/forward2.hpp>
# define HBOOST_PP_ITERATE_3 <hboost/preprocessor/iteration/detail/iter/forward3.hpp>
# define HBOOST_PP_ITERATE_4 <hboost/preprocessor/iteration/detail/iter/forward4.hpp>
# define HBOOST_PP_ITERATE_5 <hboost/preprocessor/iteration/detail/iter/forward5.hpp>
#
# endif
