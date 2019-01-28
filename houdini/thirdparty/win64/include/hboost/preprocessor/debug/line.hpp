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
# ifndef HBOOST_PREPROCESSOR_DEBUG_LINE_HPP
# define HBOOST_PREPROCESSOR_DEBUG_LINE_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/iteration/iterate.hpp>
# include <hboost/preprocessor/stringize.hpp>
#
# /* HBOOST_PP_LINE */
#
# if HBOOST_PP_CONFIG_EXTENDED_LINE_INFO
#    define HBOOST_PP_LINE(line, file) line HBOOST_PP_CAT(HBOOST_PP_LINE_, HBOOST_PP_IS_ITERATING)(file)
#    define HBOOST_PP_LINE_HBOOST_PP_IS_ITERATING(file) #file
#    define HBOOST_PP_LINE_1(file) HBOOST_PP_STRINGIZE(file HBOOST_PP_CAT(HBOOST_PP_LINE_I_, HBOOST_PP_ITERATION_DEPTH())())
#    define HBOOST_PP_LINE_I_1() [HBOOST_PP_FRAME_ITERATION(1)]
#    define HBOOST_PP_LINE_I_2() HBOOST_PP_LINE_I_1()[HBOOST_PP_FRAME_ITERATION(2)]
#    define HBOOST_PP_LINE_I_3() HBOOST_PP_LINE_I_2()[HBOOST_PP_FRAME_ITERATION(3)]
#    define HBOOST_PP_LINE_I_4() HBOOST_PP_LINE_I_3()[HBOOST_PP_FRAME_ITERATION(4)]
#    define HBOOST_PP_LINE_I_5() HBOOST_PP_LINE_I_4()[HBOOST_PP_FRAME_ITERATION(5)]
# else
#    define HBOOST_PP_LINE(line, file) line __FILE__
# endif
#
# endif
