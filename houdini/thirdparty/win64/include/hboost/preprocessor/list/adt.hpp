# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  *
#  * See http://www.boost.org for most recent version.
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# ifndef HBOOST_PREPROCESSOR_LIST_ADT_HPP
# define HBOOST_PREPROCESSOR_LIST_ADT_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/detail/is_binary.hpp>
# include <hboost/preprocessor/logical/compl.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
#
# /* HBOOST_PP_LIST_CONS */
#
# define HBOOST_PP_LIST_CONS(head, tail) (head, tail)
#
# /* HBOOST_PP_LIST_NIL */
#
# define HBOOST_PP_LIST_NIL HBOOST_PP_NIL
#
# /* HBOOST_PP_LIST_FIRST */
#
# define HBOOST_PP_LIST_FIRST(list) HBOOST_PP_LIST_FIRST_D(list)
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_LIST_FIRST_D(list) HBOOST_PP_LIST_FIRST_I list
# else
#    define HBOOST_PP_LIST_FIRST_D(list) HBOOST_PP_LIST_FIRST_I ## list
# endif
#
# define HBOOST_PP_LIST_FIRST_I(head, tail) head
#
# /* HBOOST_PP_LIST_REST */
#
# define HBOOST_PP_LIST_REST(list) HBOOST_PP_LIST_REST_D(list)
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_LIST_REST_D(list) HBOOST_PP_LIST_REST_I list
# else
#    define HBOOST_PP_LIST_REST_D(list) HBOOST_PP_LIST_REST_I ## list
# endif
#
# define HBOOST_PP_LIST_REST_I(head, tail) tail
#
# /* HBOOST_PP_LIST_IS_CONS */
#
# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_BCC()
#    define HBOOST_PP_LIST_IS_CONS(list) HBOOST_PP_LIST_IS_CONS_D(list)
#    define HBOOST_PP_LIST_IS_CONS_D(list) HBOOST_PP_LIST_IS_CONS_ ## list
#    define HBOOST_PP_LIST_IS_CONS_(head, tail) 1
#    define HBOOST_PP_LIST_IS_CONS_HBOOST_PP_NIL 0
# else
#    define HBOOST_PP_LIST_IS_CONS(list) HBOOST_PP_IS_BINARY(list)
# endif
#
# /* HBOOST_PP_LIST_IS_NIL */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_BCC()
#    define HBOOST_PP_LIST_IS_NIL(list) HBOOST_PP_COMPL(HBOOST_PP_IS_BINARY(list))
# else
#    define HBOOST_PP_LIST_IS_NIL(list) HBOOST_PP_COMPL(HBOOST_PP_LIST_IS_CONS(list))
# endif
#
# endif
