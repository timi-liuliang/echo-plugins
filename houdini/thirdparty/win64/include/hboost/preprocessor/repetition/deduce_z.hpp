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
# ifndef HBOOST_PREPROCESSOR_REPETITION_DEDUCE_Z_HPP
# define HBOOST_PREPROCESSOR_REPETITION_DEDUCE_Z_HPP
#
# include <hboost/preprocessor/detail/auto_rec.hpp>
# include <hboost/preprocessor/repetition/repeat.hpp>
#
# /* HBOOST_PP_DEDUCE_Z */
#
# define HBOOST_PP_DEDUCE_Z() HBOOST_PP_AUTO_REC(HBOOST_PP_REPEAT_P, 4)
#
# endif
