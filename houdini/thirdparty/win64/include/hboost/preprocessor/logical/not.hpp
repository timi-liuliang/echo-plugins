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
# ifndef HBOOST_PREPROCESSOR_LOGICAL_NOT_HPP
# define HBOOST_PREPROCESSOR_LOGICAL_NOT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/logical/bool.hpp>
# include <hboost/preprocessor/logical/compl.hpp>
#
# /* HBOOST_PP_NOT */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_NOT(x) HBOOST_PP_COMPL(HBOOST_PP_BOOL(x))
# else
#    define HBOOST_PP_NOT(x) HBOOST_PP_NOT_I(x)
#    define HBOOST_PP_NOT_I(x) HBOOST_PP_COMPL(HBOOST_PP_BOOL(x))
# endif
#
# endif
