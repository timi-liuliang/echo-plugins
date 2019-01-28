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
# ifndef HBOOST_PREPROCESSOR_STRINGIZE_HPP
# define HBOOST_PREPROCESSOR_STRINGIZE_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_STRINGIZE */
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_STRINGIZE(text) HBOOST_PP_STRINGIZE_A((text))
#    define HBOOST_PP_STRINGIZE_A(arg) HBOOST_PP_STRINGIZE_I arg
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_STRINGIZE(text) HBOOST_PP_STRINGIZE_OO((text))
#    define HBOOST_PP_STRINGIZE_OO(par) HBOOST_PP_STRINGIZE_I ## par
# else
#    define HBOOST_PP_STRINGIZE(text) HBOOST_PP_STRINGIZE_I(text)
# endif
#
# define HBOOST_PP_STRINGIZE_I(text) #text
#
# endif
