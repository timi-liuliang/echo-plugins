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
# ifndef HBOOST_PREPROCESSOR_DEBUG_ERROR_HPP
# define HBOOST_PREPROCESSOR_DEBUG_ERROR_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_ERROR */
#
# if HBOOST_PP_CONFIG_ERRORS
#    define HBOOST_PP_ERROR(code) HBOOST_PP_CAT(HBOOST_PP_ERROR_, code)
# endif
#
# define HBOOST_PP_ERROR_0x0000 HBOOST_PP_ERROR(0x0000, HBOOST_PP_INDEX_OUT_OF_BOUNDS)
# define HBOOST_PP_ERROR_0x0001 HBOOST_PP_ERROR(0x0001, HBOOST_PP_WHILE_OVERFLOW)
# define HBOOST_PP_ERROR_0x0002 HBOOST_PP_ERROR(0x0002, HBOOST_PP_FOR_OVERFLOW)
# define HBOOST_PP_ERROR_0x0003 HBOOST_PP_ERROR(0x0003, HBOOST_PP_REPEAT_OVERFLOW)
# define HBOOST_PP_ERROR_0x0004 HBOOST_PP_ERROR(0x0004, HBOOST_PP_LIST_FOLD_OVERFLOW)
# define HBOOST_PP_ERROR_0x0005 HBOOST_PP_ERROR(0x0005, HBOOST_PP_SEQ_FOLD_OVERFLOW)
# define HBOOST_PP_ERROR_0x0006 HBOOST_PP_ERROR(0x0006, HBOOST_PP_ARITHMETIC_OVERFLOW)
# define HBOOST_PP_ERROR_0x0007 HBOOST_PP_ERROR(0x0007, HBOOST_PP_DIVISION_BY_ZERO)
#
# endif
