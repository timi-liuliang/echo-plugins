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
# ifndef HBOOST_PREPROCESSOR_CAT_HPP
# define HBOOST_PREPROCESSOR_CAT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_CAT */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_CAT(a, b) HBOOST_PP_CAT_I(a, b)
# else
#    define HBOOST_PP_CAT(a, b) HBOOST_PP_CAT_OO((a, b))
#    define HBOOST_PP_CAT_OO(par) HBOOST_PP_CAT_I ## par
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_CAT_I(a, b) a ## b
# else
#    define HBOOST_PP_CAT_I(a, b) HBOOST_PP_CAT_II(~, a ## b)
#    define HBOOST_PP_CAT_II(p, res) res
# endif
#
# endif
