# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Paul Mensonides (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_LIST_TO_SEQ_HPP
# define HBOOST_PREPROCESSOR_LIST_TO_SEQ_HPP
#
# include <hboost/preprocessor/list/for_each.hpp>
#
# /* HBOOST_PP_LIST_TO_SEQ */
#
# define HBOOST_PP_LIST_TO_SEQ(list) \
    HBOOST_PP_LIST_FOR_EACH(HBOOST_PP_LIST_TO_SEQ_MACRO, ~, list) \
    /**/
# define HBOOST_PP_LIST_TO_SEQ_MACRO(r, data, elem) (elem)
#
# /* HBOOST_PP_LIST_TO_SEQ_R */
#
# define HBOOST_PP_LIST_TO_SEQ_R(r, list) \
    HBOOST_PP_LIST_FOR_EACH_R(r, HBOOST_PP_LIST_TO_SEQ_MACRO, ~, list) \
    /**/
#
# endif /* HBOOST_PREPROCESSOR_LIST_TO_SEQ_HPP */
