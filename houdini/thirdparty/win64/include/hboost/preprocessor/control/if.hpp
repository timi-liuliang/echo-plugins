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
# ifndef HBOOST_PREPROCESSOR_CONTROL_IF_HPP
# define HBOOST_PREPROCESSOR_CONTROL_IF_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/logical/bool.hpp>
#
# /* HBOOST_PP_IF */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_IF(cond, t, f) HBOOST_PP_IIF(HBOOST_PP_BOOL(cond), t, f)
# else
#    define HBOOST_PP_IF(cond, t, f) HBOOST_PP_IF_I(cond, t, f)
#    define HBOOST_PP_IF_I(cond, t, f) HBOOST_PP_IIF(HBOOST_PP_BOOL(cond), t, f)
# endif
#
# endif
