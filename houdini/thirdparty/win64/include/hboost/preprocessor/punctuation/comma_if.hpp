# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
# define HBOOST_PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/facilities/empty.hpp>
# include <hboost/preprocessor/punctuation/comma.hpp>
#
# /* HBOOST_PP_COMMA_IF */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_COMMA_IF(cond) HBOOST_PP_IF(cond, HBOOST_PP_COMMA, HBOOST_PP_EMPTY)()
# else
#    define HBOOST_PP_COMMA_IF(cond) HBOOST_PP_COMMA_IF_I(cond)
#    define HBOOST_PP_COMMA_IF_I(cond) HBOOST_PP_IF(cond, HBOOST_PP_COMMA, HBOOST_PP_EMPTY)()
# endif
#
# endif
