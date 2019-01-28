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
# ifndef HBOOST_PREPROCESSOR_PUNCTUATION_PAREN_IF_HPP
# define HBOOST_PREPROCESSOR_PUNCTUATION_PAREN_IF_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/facilities/empty.hpp>
# include <hboost/preprocessor/punctuation/paren.hpp>
#
# /* HBOOST_PP_LPAREN_IF */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_LPAREN_IF(cond) HBOOST_PP_IF(cond, HBOOST_PP_LPAREN, HBOOST_PP_EMPTY)()
# else
#    define HBOOST_PP_LPAREN_IF(cond) HBOOST_PP_LPAREN_IF_I(cond)
#    define HBOOST_PP_LPAREN_IF_I(cond) HBOOST_PP_IF(cond, HBOOST_PP_LPAREN, HBOOST_PP_EMPTY)()
# endif
#
# /* HBOOST_PP_RPAREN_IF */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_RPAREN_IF(cond) HBOOST_PP_IF(cond, HBOOST_PP_RPAREN, HBOOST_PP_EMPTY)()
# else
#    define HBOOST_PP_RPAREN_IF(cond) HBOOST_PP_RPAREN_IF_I(cond)
#    define HBOOST_PP_RPAREN_IF_I(cond) HBOOST_PP_IF(cond, HBOOST_PP_RPAREN, HBOOST_PP_EMPTY)()
# endif
#
# endif
