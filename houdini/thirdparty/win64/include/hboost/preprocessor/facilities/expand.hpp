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
# ifndef HBOOST_PREPROCESSOR_FACILITIES_EXPAND_HPP
# define HBOOST_PREPROCESSOR_FACILITIES_EXPAND_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC() && ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_DMC()
#    define HBOOST_PP_EXPAND(x) HBOOST_PP_EXPAND_I(x)
# else
#    define HBOOST_PP_EXPAND(x) HBOOST_PP_EXPAND_OO((x))
#    define HBOOST_PP_EXPAND_OO(par) HBOOST_PP_EXPAND_I ## par
# endif
#
# define HBOOST_PP_EXPAND_I(x) x
#
# endif
