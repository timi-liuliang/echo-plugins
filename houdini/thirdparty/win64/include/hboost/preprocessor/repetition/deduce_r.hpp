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
# ifndef HBOOST_PREPROCESSOR_REPETITION_DEDUCE_R_HPP
# define HBOOST_PREPROCESSOR_REPETITION_DEDUCE_R_HPP
#
# include <hboost/preprocessor/detail/auto_rec.hpp>
# include <hboost/preprocessor/repetition/for.hpp>
#
# /* HBOOST_PP_DEDUCE_R */
#
# define HBOOST_PP_DEDUCE_R() HBOOST_PP_AUTO_REC(HBOOST_PP_FOR_P, 256)
#
# endif
