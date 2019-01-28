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
# ifndef HBOOST_PREPROCESSOR_CONTROL_DEDUCE_D_HPP
# define HBOOST_PREPROCESSOR_CONTROL_DEDUCE_D_HPP
#
# include <hboost/preprocessor/control/while.hpp>
# include <hboost/preprocessor/detail/auto_rec.hpp>
#
# /* HBOOST_PP_DEDUCE_D */
#
# define HBOOST_PP_DEDUCE_D() HBOOST_PP_AUTO_REC(HBOOST_PP_WHILE_P, 256)
#
# endif
